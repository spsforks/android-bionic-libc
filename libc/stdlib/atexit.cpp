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

#include <pthread.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/prctl.h>

#include "platform/bionic/page.h"

extern "C" void __libc_stdio_cleanup();
extern "C" void __unregister_atfork(void* dso);

namespace {

struct AtexitFn {
  void (*ptr)(void*);
  void* arg;  // argument for __cxa_atexit callback
  void* dso;  // shared module handle
};

class AtexitArray {
  AtexitFn* array_;
  size_t size_;
  size_t capacity_;
  bool is_compact_;

  // Compacting or appending a new entry increments the version, but erasing a slot doesn't.
  uint64_t version_;

 public:
  size_t size() const { return size_; }
  uint64_t version() const { return version_; }
  const AtexitFn& operator[](size_t idx) { return array_[idx]; }

  bool set_writable(bool writable);
  void recompact();
  bool append_entry(const AtexitFn& entry);
  void erase_entry(size_t idx);
};

}  // anonymous namespace

// Use mprotect to make the array writable or read-only. Returns true on success.
bool AtexitArray::set_writable(bool writable) {
  if (array_ == nullptr) return true;
  const int prot = PROT_READ | (writable ? PROT_WRITE : 0);
  return mprotect(array_, capacity_ * sizeof(AtexitFn), prot) == 0;
}

void AtexitArray::recompact() {
  if (is_compact_) return;

  size_t new_size = 0;
  for (size_t i = 0; i < size_; ++i) {
    const AtexitFn fn = array_[i];
    array_[i] = {};
    if (fn.ptr != nullptr) {
      array_[new_size++] = fn;
    }
  }
  size_ = new_size;
  is_compact_ = true;
  ++version_;
}

bool AtexitArray::append_entry(const AtexitFn& entry) {
  // Expand the array if needed.
  if (size_ == capacity_) {
    const size_t new_capacity_bytes = PAGE_END((capacity_ + 1 + capacity_ / 5) * sizeof(AtexitFn));
    const size_t new_capacity = new_capacity_bytes / sizeof(AtexitFn);
    void* const new_pages = mmap(nullptr, new_capacity_bytes, PROT_READ | PROT_WRITE,
                                 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_pages == MAP_FAILED) {
      return false;
    }
    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, new_pages, new_capacity_bytes, "atexit handlers");
    memcpy(new_pages, array_, capacity_ * sizeof(AtexitFn));
    munmap(array_, capacity_ * sizeof(AtexitFn));
    array_ = static_cast<AtexitFn*>(new_pages);
    capacity_ = new_capacity;
  }

  array_[size_++] = entry;
  ++version_;
  return true;
}

void AtexitArray::erase_entry(size_t idx) {
  array_[idx] = {};
  is_compact_ = false;
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
//  http://www.codesourcery.com/cxx-abi/abi.html#dso-dtor
//
int __cxa_atexit(void (*func)(void*), void* arg, void* dso) {
  int ret = -1;
  atexit_lock();

  if (g_array.set_writable(true)) {
    g_array.recompact();
    if (g_array.append_entry({.ptr = func, .arg = arg, .dso = dso})) {
      ret = 0;
    }
    g_array.set_writable(false);
  }

  atexit_unlock();
  return ret;
}

void __cxa_finalize(void* dso) {
  atexit_lock();

restart:
  const uint64_t version = g_array.version();

  for (ssize_t i = g_array.size() - 1; i >= 0; --i) {
    if (g_array[i].ptr != nullptr && (dso == nullptr || g_array[i].dso == dso)) {
      const AtexitFn fn = g_array[i];

      // Clear the entry because its DSO handle will become invalid, and to avoid calling an entry
      // again if __cxa_finalize is called recursively.
      if (g_array.set_writable(true)) {
        g_array.erase_entry(i);
        g_array.set_writable(false);
      }

      atexit_unlock();
      fn.ptr(fn.arg);
      atexit_lock();

      if (g_array.version() != version) goto restart;
    }
  }

  atexit_unlock();

  if (dso != nullptr) {
    __unregister_atfork(dso);
  } else {
    // If called via exit(), flush output of all open files.
    __libc_stdio_cleanup();
  }
}
