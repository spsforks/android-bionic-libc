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

#ifndef _BITS_MALLOC_H_
#define _BITS_MALLOC_H_

#include <sys/cdefs.h>
#include <stddef.h>

__BEGIN_DECLS

// Remove the workaround once b/37423073 is fixed.
#if defined(__clang__) && !__has_attribute(alloc_size)
#define __BIONIC_ALLOC_SIZE(...)
#else
#define __BIONIC_ALLOC_SIZE(...) __attribute__((__alloc_size__(__VA_ARGS__)))
#endif

void* malloc(size_t __byte_count) __mallocfunc __BIONIC_ALLOC_SIZE(1) __wur;
void* calloc(size_t __item_count, size_t __item_size) __mallocfunc __BIONIC_ALLOC_SIZE(1,2) __wur;
void* realloc(void* __ptr, size_t __byte_count) __BIONIC_ALLOC_SIZE(2) __wur;
void free(void* __ptr);

void* memalign(size_t __alignment, size_t __byte_count) __mallocfunc __BIONIC_ALLOC_SIZE(2) __wur;

__END_DECLS

#endif
