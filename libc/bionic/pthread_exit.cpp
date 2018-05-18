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

#include <pthread.h>

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "elf_tls.h"
#include "private/bionic_defs.h"
#include "private/bionic_globals.h"
#include "private/ScopedSignalBlocker.h"
#include "pthread_internal.h"

extern "C" __noreturn void _exit_with_stack_teardown(void*, size_t);
extern "C" __noreturn void __exit(int);
extern "C" int __set_tls(void* ptr);
extern "C" int __set_tid_address(int*);
extern "C" void __cxa_thread_finalize();

/* CAVEAT: our implementation of pthread_cleanup_push/pop doesn't support C++ exceptions
 *         and thread cancelation
 */

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
void __pthread_cleanup_push(__pthread_cleanup_t* c, __pthread_cleanup_func_t routine, void* arg) {
  pthread_internal_t* thread = __get_thread();
  c->__cleanup_routine = routine;
  c->__cleanup_arg = arg;
  c->__cleanup_prev = thread->cleanup_stack;
  thread->cleanup_stack = c;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
void __pthread_cleanup_pop(__pthread_cleanup_t* c, int execute) {
  pthread_internal_t* thread = __get_thread();
  thread->cleanup_stack = c->__cleanup_prev;
  if (execute) {
    c->__cleanup_routine(c->__cleanup_arg);
  }
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
void pthread_exit(void* return_value) {
  // Call dtors for thread_local objects first.
  __cxa_thread_finalize();

  pthread_internal_t* thread = __get_thread();
  thread->return_value = return_value;

  // Call the cleanup handlers.
  while (thread->cleanup_stack) {
    __pthread_cleanup_t* c = thread->cleanup_stack;
    thread->cleanup_stack = c->__cleanup_prev;
    c->__cleanup_routine(c->__cleanup_arg);
  }

  // Call the TLS destructors. It is important to do that before removing this
  // thread from the global list. This will ensure that if someone else deletes
  // a TLS key, the corresponding value will be set to NULL in this thread's TLS
  // space (see pthread_key_delete).
  pthread_key_clean_all();

  if (thread->alternate_signal_stack != NULL) {
    // Tell the kernel to stop using the alternate signal stack.
    stack_t ss;
    memset(&ss, 0, sizeof(ss));
    ss.ss_flags = SS_DISABLE;
    sigaltstack(&ss, NULL);

    // Free it.
    munmap(thread->alternate_signal_stack, SIGNAL_STACK_SIZE);
    thread->alternate_signal_stack = NULL;
  }

  ThreadJoinState old_state = THREAD_NOT_JOINED;
  while (old_state == THREAD_NOT_JOINED &&
         !atomic_compare_exchange_weak(&thread->join_state, &old_state, THREAD_EXITED_NOT_JOINED)) {
  }

  bool should_unmap_stack = false;

  if (old_state == THREAD_DETACHED) {
    // The thread is detached, no one will use pthread_internal_t after pthread_exit.
    // So we can free mapped space, which includes pthread_internal_t and thread stack.
    // First make sure that the kernel does not try to clear the tid field
    // because we'll have freed the memory before the thread actually exits.
    __set_tid_address(NULL);

    // pthread_internal_t is freed below with stack, not here.
    __pthread_internal_remove(thread);

    if (thread->mmap_size != 0) {
      // We need to free mapped space for detached threads when they exit.
      // That's not something we can do in C.
      should_unmap_stack = true;
    }
  }

  // We don't want to take a signal after we've unmapped the stack or after we've freed
  // the TCB (which contains the slot for errno).
  // XXX: If blocking signals is a problem, there are other ways to deal with this.
  // XXX: This isn't my idea from above, but I think(?) we could allocate the TCB with the stack...
  // ... except that's problematic when the user provides their own stack. (The user wouldn't be
  // expecting all their __thread variables to be allocated on it, so it might be too small.)
  ScopedSignalBlocker ssb;
  __free_bionic_tls(thread->bionic_tls);
  __free_tls_dtv(thread);

  // XXX: We're freeing our own TCB, which frees this thread's stack protector... Returning from __free_tcb might read freed memory.
  // XXX: This is really ugly. It will crash if anything uses a pthread key, and __get_thread()->tcb points to the old (freed) TCB.
  void* temp_tcb_slots[BIONIC_TLS_SLOTS] {};
#if defined(__i386__) || defined(__x86_64__)
  temp_tcb_slots[TLS_SLOT_SELF] = temp_tcb_slots;
#endif
  temp_tcb_slots[TLS_SLOT_THREAD_ID] = __get_thread();
  temp_tcb_slots[TLS_SLOT_STACK_GUARD] = __get_tls()[TLS_SLOT_STACK_GUARD];
  __set_tls(temp_tcb_slots);
  __free_tcb(__libc_shared_globals->tls_modules, thread->tcb, thread->tcb_free_func);

  if (should_unmap_stack) {
    _exit_with_stack_teardown(thread->attr.stack_base, thread->mmap_size);
  } else {
    __exit(0);
  }
}
