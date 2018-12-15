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

#include "libc_init_common.h"

#include <async_safe/log.h>

#include "private/KernelArgumentBlock.h"
#include "private/bionic_arc4random.h"
#include "private/bionic_defs.h"
#include "private/bionic_elf_tls.h"
#include "private/bionic_globals.h"
#include "private/bionic_ssp.h"
#include "pthread_internal.h"

extern "C" pid_t __getpid();
extern "C" int __set_tid_address(int* tid_address);

// Declared in "private/bionic_ssp.h".
uintptr_t __stack_chk_guard = 0;

// Setup for the main thread. For dynamic executables, this is called by the
// linker _before_ libc is mapped in memory. This means that all writes to
// globals from this function will apply to linker-private copies and will not
// be visible from libc later on.
//
// Note: this function creates a pthread_internal_t for the initial thread and
// stores the pointer in TLS, but does not add it to pthread's thread list. This
// has to be done later from libc itself (see __libc_init_common).
//
// This is in a file by itself because it needs to be built with
// -fno-stack-protector because it's responsible for setting up the main
// thread's TLS (which stack protector relies on).

// Do enough setup to:
//  - Let the dynamic linker invoke system calls (and access errno)
//  - Ensure that TLS access functions (__get_{tls,thread}) never return NULL
//  - Allow the stack protector to work (with a zero cookie)
// Avoid doing much more because, when this code is called within the dynamic
// linker, the linker binary hasn't been relocated yet, so certain kinds of code
// are hazardous, such as accessing non-hidden global variables.
__BIONIC_WEAK_FOR_NATIVE_BRIDGE
void __libc_init_main_thread_early(const KernelArgumentBlock& args,
                                   bionic_tcb* temp_tcb,
                                   pthread_internal_t* temp_thread) {
  __libc_shared_globals()->auxv = args.auxv;
#if defined(__i386__)
  __libc_init_sysinfo(); // uses AT_SYSINFO auxv entry
#endif
  __init_tls(temp_tcb, temp_thread);
  __set_tls(&temp_tcb->tls_slot(0));
  const pid_t pid = __getpid();
  temp_thread->tid = pid;
  temp_thread->set_cached_pid(pid);
}

// Finish initializing the main thread.
__BIONIC_WEAK_FOR_NATIVE_BRIDGE
void __libc_init_main_thread_late() {
  pthread_internal_t* temp_thread = __get_thread();
  temp_thread->bionic_tls = __allocate_temp_bionic_tls();
  __get_tls()[TLS_SLOT_BIONIC_TLS] = temp_thread->bionic_tls;

  pthread_attr_init(&temp_thread->attr);
  // We don't want to explicitly set the main thread's scheduler attributes (http://b/68328561).
  pthread_attr_setinheritsched(&temp_thread->attr, PTHREAD_INHERIT_SCHED);
  // The main thread has no guard page.
  pthread_attr_setguardsize(&temp_thread->attr, 0);
  // User code should never see this; we'll compute it when asked.
  pthread_attr_setstacksize(&temp_thread->attr, 0);

  // The TLS stack guard is set from the global, so ensure that we've initialized the global
  // before we initialize the TLS. Dynamic executables will initialize their copy of the global
  // stack protector from the one in the main thread's TLS.
  __libc_safe_arc4random_buf(&__stack_chk_guard, sizeof(__stack_chk_guard));
  __init_tls_stack_guard(__get_bionic_tcb());

  __init_thread(temp_thread);

  __init_additional_stacks(temp_thread);
}

// Once all ELF modules are loaded, allocate the final copy of the main thread's
// static TLS memory. For consistency with other threads, this mapping includes
// the same per-thread data as every other thread (TCB, pthread_internal_t,
// pthread keys, bionic_tls).
__BIONIC_WEAK_FOR_NATIVE_BRIDGE
void __libc_init_main_thread_final() {
  bionic_tcb* temp_tcb = __get_bionic_tcb();
  bionic_tls* temp_bionic_tls = &__get_bionic_tls();

  // Allocate the main thread's static TLS. (This mapping doesn't include a
  // stack.)
  ThreadMapping mapping = __create_thread_mapping(0, PTHREAD_GUARD_SIZE, temp_tcb);
  if (mapping.start == nullptr) {
    async_safe_fatal("failed to mmap main thread static TLS: %s", strerror(errno));
  }

  __set_tls(&mapping.tcb->tls_slot(0));

  // Tell the kernel to clear our tid field when we exit, so we're like any other pthread.
  // For threads created by pthread_create, this setup happens during the clone syscall (i.e.
  // CLONE_CHILD_CLEARTID).
  __set_tid_address(&__get_thread()->tid);

  __free_temp_bionic_tls(temp_bionic_tls);
}
