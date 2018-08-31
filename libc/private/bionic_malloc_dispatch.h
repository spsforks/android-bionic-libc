/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef _PRIVATE_BIONIC_MALLOC_DISPATCH_H
#define _PRIVATE_BIONIC_MALLOC_DISPATCH_H

#include <stddef.h>
#include <stdint.h>
#include <stdatomic.h>
#include <private/bionic_config.h>

// Entry in malloc dispatch table.
typedef void* (*_Atomic MallocCalloc)(size_t, size_t);
typedef void (*_Atomic MallocFree)(void*);
typedef struct mallinfo (*_Atomic MallocMallinfo)();
typedef void* (*_Atomic MallocMalloc)(size_t);
typedef size_t (*_Atomic MallocMallocUsableSize)(const void*);
typedef void* (*_Atomic MallocMemalign)(size_t, size_t);
typedef int (*_Atomic MallocPosixMemalign)(void**, size_t, size_t);
typedef void* (*_Atomic MallocRealloc)(void*, size_t);
typedef int (*_Atomic MallocIterate)(uintptr_t, size_t, void (*)(uintptr_t, size_t, void*), void*);
typedef void (*_Atomic MallocMallocDisable)();
typedef void (*_Atomic MallocMallocEnable)();
typedef int (*_Atomic MallocMallopt)(int, int);
typedef void* (*_Atomic MallocAlignedAlloc)(size_t, size_t);

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
typedef void* (*_Atomic MallocPvalloc)(size_t);
typedef void* (*_Atomic MallocValloc)(size_t);
#endif

struct MallocDispatch {
  MallocCalloc calloc;
  MallocFree free;
  MallocMallinfo mallinfo;
  MallocMalloc malloc;
  MallocMallocUsableSize malloc_usable_size;
  MallocMemalign memalign;
  MallocPosixMemalign posix_memalign;
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  MallocPvalloc pvalloc;
#endif
  MallocRealloc realloc;
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  MallocValloc valloc;
#endif
  MallocIterate iterate;
  MallocMallocDisable malloc_disable;
  MallocMallocEnable malloc_enable;
  MallocMallopt mallopt;
  MallocAlignedAlloc aligned_alloc;
} __attribute__((aligned(32)));

#endif
