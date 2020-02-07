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

#include <sys/thread_properties.h>

#include <pthread.h>
#include <unistd.h>

#include "private/bionic_elf_tls.h"
#include "private/bionic_globals.h"
#include "private/bionic_tls.h"
#include "pthread_internal.h"

int __libc_get_static_tls_bounds(void** stls_beg, void** stls_end) {
  const StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
  *stls_beg = reinterpret_cast<char*>(__get_bionic_tcb()) - layout.offset_bionic_tcb();
  *stls_end = reinterpret_cast<char*>(*stls_beg) + layout.size();
  return 0;
}

void __libc_register_thread_exit_callback(void (*cb)()) {
  __libc_shared_globals()->tls_modules.thread_exit_cb = cb;
}

int __libc_iterate_dynamic_tls(dtls_callback_t cb, void* arg) {
  const TlsModules& modules = __libc_shared_globals()->tls_modules;
  if (modules.modules_count == modules.static_modules_count) {
    return 0;
  }

  // Block signals and take a read-lock while we iterate over the
  // dynamic tls blocks.
  ScopedSignalBlocker ssb;
  ScopedReadLock locker(&modules.rwlock);

  TlsDtv* const dtv = __get_tcb_dtv(__get_bionic_tcb());
  assert(dtv->count > modules.static_modules_count);

  for (int i = modules.static_modules_count; i < dtv->count; ++i) {
    cb(/*dtls_beg=*/dtv->modules[i],
       /*dtls_end=*/dtv->modules[i] + modules.module_table[i].segment.size,
       /*dso_id=*/__tls_module_idx_to_id(i), arg);
  }
  return 0;
}

void __libc_register_dynamic_tls_listener(dtls_callback_t on_creation,
                                          dtls_callback_t on_destruction) {
  TlsModules& tls_modules = __libc_shared_globals()->tls_modules;
  tls_modules.on_creation_cb = on_creation;
  tls_modules.on_destruction_cb = on_destruction;
}
