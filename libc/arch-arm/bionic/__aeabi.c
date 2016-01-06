/*
 * Copyright (C) 2008 The Android Open Source Project
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

#if defined(__clang__)
// clang interprets -fno-builtin more loosely than you might expect,
// and thinks it's okay to still substitute builtins as long as they're
// named __aeabi_* rather than __builtin_*, which causes infinite
// recursion if we have the fortified memcpy visible in this file.
#undef _FORTIFY_SOURCE
#endif

#include <stddef.h>
#include <string.h>

extern int __cxa_atexit(void (*)(void*), void*, void*);

// All of these are weak symbols to avoid multiple definition errors when
// linking with libstdc++-v3 or compiler-rt.

/* The "C++ ABI for ARM" document states that static C++ constructors,
 * which are called from the .init_array, should manually call
 * __aeabi_atexit() to register static destructors explicitly.
 *
 * Note that 'dso_handle' is the address of a magic linker-generate
 * variable from the shared object that contains the constructor/destructor
 */

int __attribute__((weak))
__aeabi_atexit_libc(void *object, void (*destructor) (void *), void *dso_handle) {
    return __cxa_atexit(destructor, object, dso_handle);
}

int __attribute__((weak))
__aeabi_atexit_private(void *object, void (*destructor) (void *), void *dso_handle) {
    return __cxa_atexit(destructor, object, dso_handle);
}

__asm__(".symver __aeabi_atexit_libc, __aeabi_atexit@@LIBC");
__asm__(".symver __aeabi_atexit_private, __aeabi_atexit@LIBC_PRIVATE");

#define __AEABI_PROXY_TO_MEMFN(aeabi_fn_name, mem_fn_name) \
void __attribute__((weak)) \
aeabi_fn_name ## _libc(void *dest, const void *src, size_t n) { \
  mem_fn_name(dest, src, n); \
} \
\
void __attribute__((weak)) \
aeabi_fn_name ## _private(void *dest, const void *src, size_t n) { \
  mem_fn_name(dest, src, n); \
} \
\
__asm__(".symver " #aeabi_fn_name "_libc, " #aeabi_fn_name "@@LIBC"); \
__asm__(".symver " #aeabi_fn_name "_private , " #aeabi_fn_name "@LIBC_PRIVATE")

__AEABI_PROXY_TO_MEMFN(__aeabi_memcpy8, memcpy);
__AEABI_PROXY_TO_MEMFN(__aeabi_memcpy4, memcpy);
__AEABI_PROXY_TO_MEMFN(__aeabi_memcpy, memcpy);
__AEABI_PROXY_TO_MEMFN(__aeabi_memmove8, memmove);
__AEABI_PROXY_TO_MEMFN(__aeabi_memmove4, memmove);
__AEABI_PROXY_TO_MEMFN(__aeabi_memmove, memmove);

#undef __AEABI_PROXY_TO_MEMFN


/*
 * __aeabi_memset has the order of its second and third arguments reversed.
 *  This allows __aeabi_memclr to tail-call __aeabi_memset
 */

#define __AEABI_PROXY_TO_MEMSET(aeabi_fn_name) \
void __attribute__((weak)) \
aeabi_fn_name ## _libc(void *dest, size_t n, int c) { \
  memset(dest, c, n); \
} \
\
void __attribute__((weak)) \
aeabi_fn_name ## _private(void *dest, size_t n, int c) { \
  memset(dest, c, n); \
} \
\
__asm__(".symver " #aeabi_fn_name "_libc, " #aeabi_fn_name "@@LIBC"); \
__asm__(".symver " #aeabi_fn_name "_private , " #aeabi_fn_name "@LIBC_PRIVATE")

__AEABI_PROXY_TO_MEMSET(__aeabi_memset8);
__AEABI_PROXY_TO_MEMSET(__aeabi_memset4);
__AEABI_PROXY_TO_MEMSET(__aeabi_memset);

#undef __AEABI_PROXY_TO_MEMSET

#define __AEABI_PROXY_TO_MEMCLR(aeabi_fn_name) \
void __attribute__((weak)) \
aeabi_fn_name ## _libc(void *dest, size_t n) { \
  memset(dest, 0, n); \
} \
\
void __attribute__((weak)) \
aeabi_fn_name ## _private(void *dest, size_t n) { \
  memset(dest, 0, n); \
} \
\
__asm__(".symver " #aeabi_fn_name "_libc, " #aeabi_fn_name "@@LIBC"); \
__asm__(".symver " #aeabi_fn_name "_private , " #aeabi_fn_name "@LIBC_PRIVATE")

__AEABI_PROXY_TO_MEMCLR(__aeabi_memclr8);
__AEABI_PROXY_TO_MEMCLR(__aeabi_memclr4);
__AEABI_PROXY_TO_MEMCLR(__aeabi_memclr);

#undef __AEABI_PROXY_TO_MEMCLR
