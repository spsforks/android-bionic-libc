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

#ifndef _PRIVATE_SAFESTACK_H
#define _PRIVATE_SAFESTACK_H

#include <bionic/pthread_internal.h>

__BEGIN_DECLS

#if defined(BIONIC_SAFESTACK)
__LIBC_HIDDEN__ void __unsafe_stack_main_thread_init();
__LIBC_HIDDEN__ int __unsafe_stack_alloc(pthread_internal_t* thr,
                                          size_t stack_size, size_t guard);
__LIBC_HIDDEN__ void __unsafe_stack_free(pthread_internal_t* thr);
__LIBC_HIDDEN__ void __unsafe_stack_set_vma_name(pthread_internal_t* thr,
                                                 size_t guard, char* buf,
                                                 size_t buf_size);
#else
static inline void __unsafe_stack_main_thread_init() {}
static inline int __unsafe_stack_alloc(pthread_internal_t*, size_t, size_t) {
  return 0;
}
static inline void __unsafe_stack_free(pthread_internal_t*) {}
static inline void __unsafe_stack_set_vma_name(pthread_internal_t*, size_t,
                                               char*, size_t) {}
#endif

__END_DECLS

#endif
