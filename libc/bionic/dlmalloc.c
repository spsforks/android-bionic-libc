/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include "dlmalloc.h"

#include <stdlib.h>

#include <private/bionic_ashmem.h>
#include <private/debug_format.h>
#include <private/logd.h>

// Send dlmalloc errors to the log.
static void __bionic_heap_corruption_error(const char* function);
static void __bionic_heap_usage_error(const char* function, void* address);
#define PROCEED_ON_ERROR 0
#define CORRUPTION_ERROR_ACTION(m) __bionic_heap_corruption_error(__FUNCTION__)
#define USAGE_ERROR_ACTION(m,p) __bionic_heap_usage_error(__FUNCTION__, p)

// We use ashmem to name the anonymous private regions created by dlmalloc.
static void* __bionic_heap_mmap(size_t s);
#define MMAP(s) __bionic_heap_mmap(s)

// Ugly inclusion of C file so that bionic specific #defines configure dlmalloc.
#include "../upstream-dlmalloc/malloc.c"

static void __bionic_heap_corruption_error(const char* function) {
  __libc_format_log(ANDROID_LOG_FATAL, "libc", "@@@ ABORTING: heap corruption detected by %s",
                    function);
  abort();
}

static void __bionic_heap_usage_error(const char* function, void* address) {
  __libc_format_log(ANDROID_LOG_FATAL, "libc", "@@@ ABORTING: invalid address %p passed to %s",
                    address, function);
  // So that we can get a memory dump around the specific address.
  *((int**) 0xdeadbaad) = (int*) address;
}

static void* __bionic_heap_mmap(size_t byte_count) {
  return __libc_ashmem_mmap("libc malloc", byte_count);
}
