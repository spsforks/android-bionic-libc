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

#ifndef LIBC_BIONIC_JEMALLOC_H_
#define LIBC_BIONIC_JEMALLOC_H_

#include <jemalloc/jemalloc.h>

// Need to wrap memalign since je_memalign fails on non-power of 2 alignments.
#define je_memalign je_memalign_round_up_boundary

__BEGIN_DECLS

unsigned android_mallinfo_narenas() __LIBC_ABI_PUBLIC__;
unsigned android_mallinfo_nbins() __LIBC_ABI_PUBLIC__;
struct mallinfo android_mallinfo_arena_info(unsigned) __LIBC_ABI_PUBLIC__;
struct mallinfo android_mallinfo_bin_info(unsigned, unsigned) __LIBC_ABI_PUBLIC__;

unsigned je_mallinfo_narenas();
unsigned je_mallinfo_nbins();
struct mallinfo je_mallinfo();
struct mallinfo je_mallinfo_arena_info(unsigned);
struct mallinfo je_mallinfo_bin_info(unsigned, unsigned);
void* je_memalign_round_up_boundary(size_t, size_t);
void* je_pvalloc(size_t);

__END_DECLS

#endif  // LIBC_BIONIC_DLMALLOC_H_
