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

/**
 * @file thread_properties.h
 * @brief Thread properties API.
 */

#include <sys/cdefs.h>
#include <unistd.h>

#include "thread_properties_defines.h"

/**
 * https://sourceware.org/glibc/wiki/ThreadPropertiesAPI
 * API for querying various properties of the current thread, used mostly by
 * the sanitizers.
 *
 * Available since API level 31.
 */
__BEGIN_DECLS

/**
 * Gets the bounds of static TLS for the current thread.
 */
void __libc_get_static_tls_bounds(void** __static_tls_begin,
                                  void** __static_tls_end) __INTRODUCED_IN(31);


/**
 * Registers callback to be called right before the thread is totally destroyed.
 * The callbacks are chained, they are called in the order opposite to the order
 * they were registered.
 *
 * The callbacks must be registered only before any threads were created, at
 * most 8 callbacks can be registered. No signals may arrive during the calls to
 * these callbacks; immediately after the last of these calls the thread is dead.
 *
 * Returns 0 on success registration or 1 for failure when there has been 8
 * callbacks already registered.
 */
int __libc_register_thread_exit_callback(thread_exit_cb_t __cb)  __INTRODUCED_IN(31);

/**
 * Iterates over all dynamic TLS chunks for the given thread.
 * The thread should have been suspended. It is undefined-behaviour if there is concurrent
 * modification of the target thread's dynamic TLS
 *
 * Returns 0 on success iterations or 1 if the thread pointer register cannot be read.
 */
int __libc_iterate_dynamic_tls(pid_t __tid, dtls_visitor_t __cb, void* __arg)  __INTRODUCED_IN(31);

/**
 * Register on_creation and on_destruction callbacks that will be called after
 * a dynamic TLS creation and before dynamic TLS destruction, respectively.
 */
void __libc_register_dynamic_tls_listener(dtls_listener_t __on_creation, dtls_listener_t __on_destruction)  __INTRODUCED_IN(31);

__END_DECLS
