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

#include <async_safe/CHECK.h>
#include <async_safe/log.h>

#include <elf.h>  // for NT_PRSTATUS
#include <pthread.h>
#include <unistd.h>  // for gettid

#include <sys/ptrace.h>
#include <sys/uio.h>   // for iovec
#include <sys/user.h>  // for user_regs_struct

#include "private/bionic_elf_tls.h"
#include "private/bionic_globals.h"
#include "private/bionic_tls.h"
#include "pthread_internal.h"

void __libc_get_static_tls_bounds(void** stls_begin, void** stls_end) {
  const StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
  *stls_begin = reinterpret_cast<char*>(__get_bionic_tcb()) - layout.offset_bionic_tcb();
  *stls_end = reinterpret_cast<char*>(*stls_begin) + layout.size();
}

int __libc_register_thread_exit_callback(thread_exit_cb_t cb) {
  TlsModules& modules = __libc_shared_globals()->tls_modules;
  if (modules.thread_exit_callback_count >= TlsModules::MAX_THREAD_EXIT_CALLBACK_COUNT) {
    return 1;
  }

  modules.thread_exit_callbacks[modules.thread_exit_callback_count++] = cb;
  return 0;
}

static inline __always_inline bionic_tcb* __get_bionic_tcb_for_thread(pid_t tid) {
  // If tid is same as self, then we don't need ptrace.
  if (gettid() == tid) return __get_bionic_tcb();

  // Find the thread-pointer register for the given thread.
  void** tp_reg = nullptr;

#if defined(__x86_64__) || defined(__i386__)
  struct user_regs_struct regs;
  struct iovec pt_iov = {
      .iov_base = &regs,
      .iov_len = sizeof(regs),
  };
  if (ptrace(PTRACE_GETREGSET, tid, NT_PRSTATUS, &pt_iov) == 0) {
#if defined(__x86_64__)
    tp_reg = reinterpret_cast<void**>(regs.fs);
#elif defined(__i386__)
    tp_reg = reinterpret_cast<void**>(regs.gs);
#endif
  }
#elif defined(__aarch64__) || defined(__arm__)
  uint64_t reg;
  struct iovec pt_iov {
    .iov_base = &reg, .iov_len = sizeof(reg),
  };

  if (ptrace(PTRACE_GETREGSET, tid, NT_ARM_TLS, &pt_iov) == 0) {
    tp_reg = reinterpret_cast<void**>(reg);
  }
#endif

  if (tp_reg == nullptr) {
    async_safe_write_log(ANDROID_LOG_ERROR, "libc",
                         "__get_bionic_tcb_for_thread failed to read thread register.");
    return nullptr;
  }

  return reinterpret_cast<bionic_tcb*>(&tp_reg[MIN_TLS_SLOT]);
}

int __libc_iterate_dynamic_tls(pid_t tid, dtls_visitor_t cb, void* arg) {
  bionic_tcb* const tcb = __get_bionic_tcb_for_thread(tid);
  if (tcb == nullptr) {
    return 1;
  }

  TlsDtv* const dtv = __get_tcb_dtv(tcb);
  // TODO: not all of these will be DTLS
  for (size_t i = 0; i < dtv->count; ++i) {
    struct ModuleSizePair& mod = dtv->modules[i];
    cb(/*dynamic_tls_begin=*/mod.module,
       /*dynamic_tls_end=*/static_cast<void*>(static_cast<char*>(mod.module) + mod.segment_size),
       /*dso_id=*/__tls_module_idx_to_id(i), arg);
  }

  return 0;
}

void __libc_register_dynamic_tls_listener(dtls_listener_t on_creation,
                                          dtls_listener_t on_destruction) {
  TlsModules& tls_modules = __libc_shared_globals()->tls_modules;
  tls_modules.on_creation_cb = on_creation;
  tls_modules.on_destruction_cb = on_destruction;
}
