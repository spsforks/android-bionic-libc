/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <string.h>
#include <stdint.h>

#include <stdatomic.h>

#include "private/libc_logging.h"

constexpr size_t MEMCPY_MAX_TRACK_ENTRIES = 10000;
constexpr size_t MEMCPY_NUM_ARRAY_ENTRIES = 500000;
static atomic_ulong g_index;
static size_t g_size_list[MEMCPY_MAX_TRACK_ENTRIES];
static atomic_ulong g_copies[MEMCPY_NUM_ARRAY_ENTRIES];

extern "C" void* __memcpy(void*, const void*, size_t);

extern "C" void* memcpy(void* __restrict dest, const void* __restrict src, size_t n) {
  if (n < MEMCPY_NUM_ARRAY_ENTRIES) {
    atomic_fetch_add(&g_copies[n], 1U);
  } else {
    size_t index = atomic_fetch_add(&g_index, 1U);
    if (index < MEMCPY_MAX_TRACK_ENTRIES) {
      g_size_list[index] = n;
    }
  }
  return __memcpy(dest, src, n);
}

void __dump_memcpy() {
  for (size_t i = 0; i < MEMCPY_NUM_ARRAY_ENTRIES; i++) {
    size_t value = atomic_load(&g_copies[i]);
    if (value > 0) {
      __libc_format_log(ANDROID_LOG_ERROR, "memcpy", "MEMCPY %zu = %zu", i, value);
    }
  }
  // Increase the index beyond the max value so no more values can be written
  // while we dump these entries.
  size_t index = atomic_fetch_add(&g_index, MEMCPY_MAX_TRACK_ENTRIES);
  for (size_t i = 0; i < index; i++) {
    __libc_format_log(ANDROID_LOG_ERROR, "memcpy", "MEMCPY %zu", g_size_list[i]);
  }
}
