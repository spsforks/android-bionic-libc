/*
 * Copyright (C) 2014 The Android Open Source Project
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

#ifndef __LINKER_MEMORY_H
#define __LINKER_MEMORY_H
#include <sys/cdefs.h>
#include <stddef.h>
__BEGIN_DECLS

void* malloc(size_t byte_count) __mallocfunc __wur __attribute__((alloc_size(1)));
void* calloc(size_t item_count, size_t item_size) __mallocfunc __wur __attribute__((alloc_size(1,2)));
void* realloc(void* p, size_t byte_count) __wur __attribute__((alloc_size(2)));

void free(void* ptr);

__END_DECLS
#endif  /* __LINKER_MEMORY_H */
