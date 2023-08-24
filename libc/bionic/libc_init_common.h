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

#pragma once

#include <stdint.h>
#include <sys/cdefs.h>

typedef void init_func_t(int, char*[], char*[]);
typedef void fini_func_t(void);

typedef struct {
  union {
    // This field is initialized in crtbegin.c. In static executables, it is preinit_array.
    // In dynamic executables, it is version. New dynamic executables (on Android >= V) set
    // version to 1. Old dynamic executables set version to a pointer value (pointing to
    // preinit_array), which can't be a small value. So we can distinguish them.
    init_func_t** preinit_array;
    size_t version;
  };
  init_func_t** init_array;
  fini_func_t** fini_array;
  // preinit_array_count/init_array_count/fini_array_count are added in crtbegin.c in Android V.
  // When parsing the struct in libc.so, these fields are only available when version == 1.
  size_t preinit_array_count;
  size_t init_array_count;
  size_t fini_array_count;
  // To make new executables work with old libc.so, and old executables work with new libc.so,
  // only append new items, and update version as needed.
} structors_array_t;

__BEGIN_DECLS

extern int main(int argc, char** argv, char** env);

__noreturn void __libc_init(void* raw_args, void (*onexit)(void),
                            int (*slingshot)(int, char**, char**),
                            structors_array_t const* const structors);

__END_DECLS

#if defined(__cplusplus)

__LIBC_HIDDEN__ void __libc_init_globals();

__LIBC_HIDDEN__ void __libc_init_common();

__LIBC_HIDDEN__ void __libc_init_scudo();

__LIBC_HIDDEN__ void __libc_init_mte_late();

__LIBC_HIDDEN__ void __libc_init_AT_SECURE(char** envp);

// The fork handler must be initialised after __libc_init_malloc, as
// pthread_atfork may call malloc() during its once-init.
__LIBC_HIDDEN__ void __libc_init_fork_handler();

__LIBC_HIDDEN__ void __libc_set_target_sdk_version(int target);

#endif
