/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <sys/cdefs.h>

/* https://sourceware.org/glibc/wiki/ThreadPropertiesAPI
 * API for querying various properties of the current thread, used mostly by
 * the sanitizers.
 *
 * Available since API level 31.
 */
__BEGIN_DECLS

/* Gets the bounds of static TLS for the current thread.
 * Returns 0 for success.
 */
int __libc_get_static_tls_bounds(void** stls_beg, void** stls_end) __INTRODUCED_IN(31);

typedef void (*dtls_callback_t)(void *dtls_beg, void *dtls_end, size_t dso_id, void *arg);

// Iterate over all DTLS chunks in the current thread.
// Is 't != pthread_self()', the thread 't' should be blocked before and during this call.
// NOTE: the semantics in case 't != pthread_self()' is not clear! 
// AS-safe.
//
// This must be called in Thread init/shutdown and dlclose interceptors code.
int __libc_iterate_dynamic_tls(dtls_callback_t cb, void *arg);

__END_DECLS
