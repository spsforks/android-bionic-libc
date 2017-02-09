/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include "linker_allocator.h"

#include <stdlib.h>
#include <sys/cdefs.h>

#include "linker.h"

static LinkerMemoryAllocator g_linker_allocator;
thread_local bool use_fallback = false;

static LinkerMemoryAllocator& get_fallback_allocator() {
  static LinkerMemoryAllocator fallback_allocator;
  return fallback_allocator;
}

static LinkerMemoryAllocator& get_allocator() {
  return __predict_false(use_fallback) ? get_fallback_allocator() : g_linker_allocator;
}

void __android_use_fallback_allocator() {
  use_fallback = true;
}

void* malloc(size_t byte_count) {
  return get_allocator().alloc(byte_count);
}

void* calloc(size_t item_count, size_t item_size) {
  return get_allocator().alloc(item_count*item_size);
}

void* realloc(void* p, size_t byte_count) {
  return get_allocator().realloc(p, byte_count);
}

void free(void* ptr) {
  get_allocator().free(ptr);
}

