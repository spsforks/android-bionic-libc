/*
 * Copyright (C) 2013 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <pthread.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/system_properties.h>
#include <unistd.h>

#include "debug_stacktrace.h"
#include "private/bionic_name_mem.h"
#include "private/libc_logging.h"
#include "private/mmap_debug.h"
#include "private/ScopedPthreadMutexLocker.h"

extern "C" void*  __mmap2(void*, size_t, int, int, int, size_t);

#define SYSPROP_NAME              "libc.debug.mmap"
#define HASHTABLE_SIZE            1024 /* Must be a power of 2. */
#define BT_DEPTH                  12
#define BT_DEPTH_SKIP             2
#define BT_ENTRIES_PER_POOL_PAGE  (PAGE_SIZE / sizeof(BacktraceEntry) - 1)
#define BT_STR_LEN                (BT_DEPTH * (sizeof(uintptr_t) * 2 + 1) + 1)
#define MAX_POOL_MEMORY_PAGES     64

// The all point of this hashtable is to share strings for identical backtraces.
// In other words, if the same code path keeps calling mmap over and over, only
// one BacktraceEntry will exists in the pool. The need of this pool lies on the
// kernel-side implementation of named vma regions, which requires the strings
// passed to __bionic_name_mem to be long lived.
// This approach allows to keep MAX_POOL_MEMORY_PAGES * BT_ENTRIES_PER_POOL_PAGE
// (~ 1200) distinct mmap backtraces in the pool.

typedef struct BacktraceEntry {
  struct BacktraceEntry* next;
  char str[BT_STR_LEN];
  uint32_t hash;
} BacktraceEntry;

typedef struct BacktraceHashTable {
  BacktraceEntry* slots[HASHTABLE_SIZE];
} BacktraceHashTable;

typedef struct BacktraceMemoryPoolPage {
  BacktraceEntry entries[BT_ENTRIES_PER_POOL_PAGE];
  BacktraceMemoryPoolPage* next;
} BacktraceMemoryPoolPage;

static pthread_mutex_t bt_lock = PTHREAD_MUTEX_INITIALIZER;
static BacktraceHashTable bt_table;
static BacktraceMemoryPoolPage* pool;
static size_t entries_in_current_pool;
static int pool_pages_count;

static BacktraceEntry* alloc_backtrace_entry() {
  if (pool == NULL || entries_in_current_pool >= BT_ENTRIES_PER_POOL_PAGE) {
    if (pool_pages_count >= MAX_POOL_MEMORY_PAGES)
      return NULL;
    BacktraceMemoryPoolPage* current = pool;
    pool = (BacktraceMemoryPoolPage*) __mmap2(0,
                                              PAGE_SIZE,
                                              PROT_READ | PROT_WRITE,
                                              MAP_PRIVATE | MAP_ANONYMOUS,
                                              -1,
                                              0);
    if (pool == NULL)
      return NULL;

    // MAP_ANONYMOUS is guaranteed to be zero-initialized. No need to memset.
    entries_in_current_pool = 0;
    pool->next = current;
    ++pool_pages_count;
  }
  return &pool->entries[entries_in_current_pool++];
}

static size_t hashmap_get_slot(uint32_t hash) {
  // Secondary hashing discovered by Doug Lea to defend against bad hashes.
  hash += ~(hash << 9);
  hash ^= (((unsigned int) hash) >> 14);
  hash += (hash << 4);
  hash ^= (((unsigned int) hash) >> 10);
  return (size_t)hash & (HASHTABLE_SIZE - 1);
}

static BacktraceEntry* lookup_or_create_entry_locked(uint32_t hash,
                                                     const char* bt_str) {
  const size_t slot = hashmap_get_slot(hash);
  BacktraceEntry* entry;
  {
    ScopedPthreadMutexLocker locker(&bt_lock);
    entry = bt_table.slots[slot];
    while (entry != NULL && entry->hash != hash &&
        strncmp(bt_str, entry->str, BT_STR_LEN) != 0) {
      entry = entry->next;
    }
    if (entry == NULL) {
      // No matching entry found. Create a new one and fill it (if not OOM).
      entry = alloc_backtrace_entry();
      if (entry != NULL) {
        entry->next = bt_table.slots[slot];
        entry->hash = hash;
        strlcpy(entry->str, bt_str, BT_STR_LEN);
        bt_table.slots[slot] = entry;
      }
    }
  }
  return entry;
}

static void mmap_record_backtrace(void* addr, size_t size) {
  uintptr_t bt_frames[BT_DEPTH + BT_DEPTH_SKIP];
  const size_t depth = get_backtrace(bt_frames, BT_DEPTH + BT_DEPTH_SKIP);

  // Sanity check.
  if (depth == 0 || depth > (BT_DEPTH + BT_DEPTH_SKIP))
    return;

  // Build up the backtrace string that will become the vma name.
  char bt_str[BT_STR_LEN];
  char* s = bt_str;
  uintptr_t hash = 0;
  for (size_t i = BT_DEPTH_SKIP; i < depth; ++i) {
    const char* fmt = (sizeof(uintptr_t) == 8) ? "%016lx " : "%08x ";
    const size_t n_chars = sizeof(uintptr_t) * 2 + 1;
    snprintf(s, n_chars + 1, fmt, bt_frames[i]);
    s += n_chars;
    hash += bt_frames[i];
  }
  *(s-1) = '\0'; // -1 to strip off the last space.

  // Lookup the string in the hashtable. Insert if not exists.
  BacktraceEntry* entry = lookup_or_create_entry_locked(hash, bt_str);
  const char* vma_name;
  if (entry == NULL) {
    // The pool ran out of pages.
    vma_name = "MMAP_DEBUG_OOM";
  } else {
    vma_name = entry->str;
  }
  int ret = __bionic_name_mem(addr, size, vma_name);
  if (ret != 0) {
    __libc_format_log(ANDROID_LOG_WARN, "libc",
        "Kernel support for " SYSPROP_NAME " is not enabled. "
        "__bionic_name_mem returned %d.", ret);
  }

}

extern "C" int is_mmap_debug_enabled(void) {
  static bool did_check = false;
  static bool is_enabled = false;

  // Reading a system property every time can be expensive. Cache it.
  if (did_check)
    return is_enabled;

  char env[PROP_VALUE_MAX];
  __system_property_get(SYSPROP_NAME, env);
  is_enabled = (env[0] == '1');
  did_check = true;
  return is_enabled;
}

extern "C" void mmap_debug_init(void) {
  if (!is_mmap_debug_enabled())
    return;
  mmap_debug_anon_fn = mmap_record_backtrace;
}

extern "C" void mmap_debug_fini(void) {
  BacktraceMemoryPoolPage* next;
  mmap_debug_anon_fn = NULL;
  for (; pool != NULL; pool = next) {
    next = pool->next;
    munmap(pool, PAGE_SIZE);
  }
}
