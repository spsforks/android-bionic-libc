/*  $OpenBSD: atexit.c,v 1.20 2014/07/11 09:51:37 kettenis Exp $ */
/*
 * Copyright (c) 2002 Daniel Hartmeier
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *    - Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    - Redistributions in binary form must reproduce the above
 *      copyright notice, this list of conditions and the following
 *      disclaimer in the documentation and/or other materials provided
 *      with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include "atexit.h"

#include <errno.h>
#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/prctl.h>

#include <async_safe/log.h>

#include "platform/bionic/page.h"

extern "C" void __libc_stdio_cleanup();
extern "C" void __unregister_atfork(void* dso);

namespace {

struct AtexitEntry {
  void (*fn)(void*);
  void* arg;  // argument for __cxa_atexit callback
  void* dso;  // shared module handle
};

class AtexitArray {
  AtexitEntry* array_;
  size_t size_;
  size_t capacity_;
  bool is_compact_;

  // Compacting or appending a new entry increments the version, but erasing a slot doesn't.
  uint64_t version_;

  static size_t round_up_to_page_bytes(size_t capacity);
  static size_t next_capacity(size_t capacity);
  void set_writable(bool writable);
  bool expand_capacity();

 public:
  size_t size() const { return size_; }
  uint64_t version() const { return version_; }
  const AtexitEntry& operator[](size_t idx) const { return array_[idx]; }

  bool append_entry(const AtexitEntry& entry);
  AtexitEntry extract_entry(size_t idx);
  void recompact();
};

}  // anonymous namespace

size_t AtexitArray::round_up_to_page_bytes(size_t capacity) {
  return PAGE_END(capacity * sizeof(AtexitEntry));
}

size_t AtexitArray::next_capacity(size_t capacity) {
  return round_up_to_page_bytes(capacity + 1 + capacity / 5) / sizeof(AtexitEntry);
}

// Use mprotect to make the array writable or read-only. Returns true on success.
void AtexitArray::set_writable(bool writable) {
  if (array_ == nullptr) return;
  const int prot = PROT_READ | (writable ? PROT_WRITE : 0);
  if (mprotect(array_, round_up_to_page_bytes(capacity_), prot) != 0) {
    async_safe_fatal("mprotect failed on atexit array: %s", strerror(errno));
  }
}

bool AtexitArray::expand_capacity() {
  const size_t new_capacity = next_capacity(capacity_);
  const size_t new_capacity_bytes = round_up_to_page_bytes(new_capacity);

  void* new_pages;
  if (array_ == nullptr) {
    new_pages = mmap(nullptr, new_capacity_bytes, PROT_READ | PROT_WRITE,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  } else {
    new_pages =
        mremap(array_, round_up_to_page_bytes(capacity_), new_capacity_bytes, MREMAP_MAYMOVE);
  }
  if (new_pages == MAP_FAILED) {
    async_safe_format_log(ANDROID_LOG_WARN, "libc",
                          "__cxa_atexit: mmap/mremap failed to allocate %zu bytes: %s",
                          new_capacity_bytes, strerror(errno));
    return false;
  }

  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, new_pages, new_capacity_bytes, "atexit handlers");
  array_ = static_cast<AtexitEntry*>(new_pages);
  capacity_ = new_capacity;
  return true;
}

bool AtexitArray::append_entry(const AtexitEntry& entry) {
  bool result = false;

  set_writable(true);
  if (size_ < capacity_ || expand_capacity()) {
    array_[size_++] = entry;
    ++version_;
    result = true;
  }
  set_writable(false);

  return result;
}

// Extract an entry and return it. This operation doesn't bump the version, because the
// __cxa_finalize loops don't need to restart.
AtexitEntry AtexitArray::extract_entry(size_t idx) {
  AtexitEntry result = array_[idx];

  set_writable(true);
  array_[idx] = {};
  is_compact_ = false;
  set_writable(false);

  return result;
}

void AtexitArray::recompact() {
  if (is_compact_) return;

  set_writable(true);

  // Optimization: quickly skip over the initial non-null entries.
  size_t src = 0, dst = 0;
  while (src < size_ && array_[src].fn != nullptr) {
    ++src;
    ++dst;
  }

  // Shift the non-null entries forward, and zero out the removed entries at the end of the array.
  for (; src < size_; ++src) {
    const AtexitEntry entry = array_[src];
    array_[src] = {};
    if (entry.fn != nullptr) {
      array_[dst++] = entry;
    }
  }

  // If the table uses fewer pages, clean the pages at the end.
  size_t old_bytes = round_up_to_page_bytes(size_);
  size_t new_bytes = round_up_to_page_bytes(dst);
  if (new_bytes < old_bytes) {
    madvise(reinterpret_cast<char*>(array_) + new_bytes, old_bytes - new_bytes, MADV_DONTNEED);
  }

  size_ = dst;
  is_compact_ = true;
  ++version_;

  set_writable(false);
}

static AtexitArray g_array;
static pthread_mutex_t g_atexit_lock = PTHREAD_MUTEX_INITIALIZER;

static inline void atexit_lock() {
  pthread_mutex_lock(&g_atexit_lock);
}

static inline void atexit_unlock() {
  pthread_mutex_unlock(&g_atexit_lock);
}

// Outside the following functions, all pages are mprotect()'ed to prevent unintentional/malicious
// corruption.

// Register a function to be performed at exit or when a shared object with the given DSO handle is
// unloaded dynamically. Also used as the backend for atexit(). For more info on this API, see:
//
//  https://itanium-cxx-abi.github.io/cxx-abi/abi.html#dso-dtor
//
int __cxa_atexit(void (*func)(void*), void* arg, void* dso) {
  int ret = -1;

  atexit_lock();
  if (g_array.append_entry({.fn = func, .arg = arg, .dso = dso})) {
    ret = 0;
  }
  atexit_unlock();

  return ret;
}

void __cxa_finalize(void* dso) {
  atexit_lock();

  static uint32_t call_depth = 0;
  ++call_depth;

restart:
  const uint64_t version = g_array.version();

  for (ssize_t i = g_array.size() - 1; i >= 0; --i) {
    if (g_array[i].fn == nullptr || (dso != nullptr && g_array[i].dso != dso)) continue;

    // Clear the entry in the array because its DSO handle will become invalid, and to avoid calling
    // an entry again if __cxa_finalize is called recursively.
    const AtexitEntry entry = g_array.extract_entry(i);

    atexit_unlock();
    entry.fn(entry.arg);
    atexit_lock();

    if (g_array.version() != version) goto restart;
  }

  // Avoid recompaction on recursive calls because it's unnecessary and would require the earlier
  // calls to restart.
  --call_depth;
  if (call_depth == 0) {
    g_array.recompact();
  }

  atexit_unlock();

  if (dso != nullptr) {
    __unregister_atfork(dso);
  } else {
    // If called via exit(), flush output of all open files.
    __libc_stdio_cleanup();
  }
}
