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

#ifndef __BIONIC_PRIVATE_BIONIC_TLS_H_
#define __BIONIC_PRIVATE_BIONIC_TLS_H_

#include <locale.h>
#include <mntent.h>
#include <stdio.h>
#include <sys/cdefs.h>
#include <sys/param.h>

#include "bionic_asm_tls.h"
#include "bionic_macros.h"
#include "__get_tls.h"
#include "grp_pwd.h"

__BEGIN_DECLS

/** WARNING WARNING WARNING
 **
 ** This header file is *NOT* part of the public Bionic ABI/API
 ** and should not be used/included by user-serviceable parts of
 ** the system (e.g. applications).
 **
 ** It is only provided here for the benefit of the system dynamic
 ** linker and the OpenGL sub-system (which needs to access the
 ** pre-allocated slot directly for performance reason).
 **/

// Well-known TLS slots. What data goes in which slot is arbitrary unless otherwise noted.
enum {
#if defined(__i386__) || defined(__x86_64__)
  TLS_SLOT_SELF = 0, // The kernel requires this specific slot for x86.
#endif
#ifdef BIONIC_TLS_VARIANT1
  TLS_SLOT_DTV = 0, // Variant 1 TLS (arm/arm64) uses slot 0 for the DTV instead.
#endif
  TLS_SLOT_THREAD_ID = 1,
  TLS_SLOT_ERRNO,

  // These two aren't used by bionic itself, but allow the graphics code to
  // access TLS directly rather than using the pthread API.
  TLS_SLOT_OPENGL_API = 3,
  TLS_SLOT_OPENGL = 4,

  // This slot is only used to pass information from the dynamic linker to
  // libc.so when the C library is loaded in to memory. The C runtime init
  // function will then clear it. Since its use is extremely temporary,
  // we reuse an existing location that isn't needed during libc startup.
  TLS_SLOT_BIONIC_PREINIT = TLS_SLOT_OPENGL_API,

  TLS_SLOT_STACK_GUARD = 5, // GCC requires this specific slot for x86.
  TLS_SLOT_DLERROR,

  // Fast storage for Thread::Current() in ART.
  TLS_SLOT_ART_THREAD_SELF,

  // Lets TSAN avoid using pthread_getspecific for finding the current thread
  // state.
  TLS_SLOT_TSAN,

  // Reserve this slot for the TLS DTV on variant 2 architectures (x86/x86_64).
  TLS_SLOT_VARIANT2_DTV = 9,
#ifdef BIONIC_TLS_VARIANT2
  TLS_SLOT_DTV = TLS_SLOT_VARIANT2_DTV,
#endif

  BIONIC_TLS_SLOTS // Must come last!
};

static_assert(TLS_SLOT_DTV == TLS_SLOT_DTV_MACRO, "TLS_SLOT_DTV != TLS_SLOT_DTV_MACRO");

// ~3 pages.
struct bionic_tls {
  locale_t locale;

  char basename_buf[MAXPATHLEN];
  char dirname_buf[MAXPATHLEN];

  mntent mntent_buf;
  char mntent_strings[BUFSIZ];

  char ptsname_buf[32];
  char ttyname_buf[64];

  char strerror_buf[NL_TEXTMAX];
  char strsignal_buf[NL_TEXTMAX];

  group_state_t group;
  passwd_state_t passwd;
};

#define BIONIC_TLS_SIZE (__BIONIC_ALIGN(sizeof(bionic_tls), PAGE_SIZE))

/*
 * Bionic uses some pthread keys internally. All pthread keys used internally
 * should be created in constructors, except for keys that may be used in or
 * before constructors.
 *
 * We need to manually maintain the count of pthread keys used internally, but
 * pthread_test should fail if we forget.
 *
 * These are the pthread keys currently used internally by libc:
 *  _res_key               libc (constructor in BSD code)
 */

#define LIBC_PTHREAD_KEY_RESERVED_COUNT 1

/* Internally, jemalloc uses a single key for per thread data. */
#define JEMALLOC_PTHREAD_KEY_RESERVED_COUNT 1
#define BIONIC_PTHREAD_KEY_RESERVED_COUNT (LIBC_PTHREAD_KEY_RESERVED_COUNT + JEMALLOC_PTHREAD_KEY_RESERVED_COUNT)

/*
 * Maximum number of pthread keys allocated.
 * This includes pthread keys used internally and externally.
 */
#define BIONIC_PTHREAD_KEY_COUNT (BIONIC_PTHREAD_KEY_RESERVED_COUNT + PTHREAD_KEYS_MAX)

__END_DECLS

#if defined(__cplusplus)
class KernelArgumentBlock;
class bionic_tcb;
struct TlsModules;
void __libc_init_main_thread_early(KernelArgumentBlock& args, bionic_tcb& temp_tcb);
void __libc_init_main_thread_late(KernelArgumentBlock&);
void __libc_init_main_thread_final_tcb(TlsModules& tls_modules,
                                       void* memalign_func(size_t, size_t),
                                       void free_func(void*));
#endif

#endif /* __BIONIC_PRIVATE_BIONIC_TLS_H_ */
