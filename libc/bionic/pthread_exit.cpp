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
#include <sys/mman.h>

#include "private/ScopedPthreadMutexLocker.h"
#include "pthread_internal.h"

extern "C" __noreturn void _exit_with_stack_teardown(void*, size_t);
extern "C" __noreturn void __exit(int);
extern "C" int __set_tid_address(int*);

/* CAVEAT: our implementation of pthread_cleanup_push/pop doesn't support C++ exceptions
 *         and thread cancelation
 */

void __pthread_cleanup_push(__pthread_cleanup_t* c, __pthread_cleanup_func_t routine, void* arg) {
  pthread_internal_t* self = reinterpret_cast<pthread_internal_t*>(pthread_self());
  c->__cleanup_routine = routine;
  c->__cleanup_arg = arg;
  c->__cleanup_prev = self->cleanup_stack;
  self->cleanup_stack = c;
}

void __pthread_cleanup_pop(__pthread_cleanup_t* c, int execute) {
  pthread_internal_t* self = reinterpret_cast<pthread_internal_t*>(pthread_self());
  self->cleanup_stack = c->__cleanup_prev;
  if (execute) {
    c->__cleanup_routine(c->__cleanup_arg);
  }
}

void pthread_exit(void* return_value) {
  pthread_internal_t* self = reinterpret_cast<pthread_internal_t*>(pthread_self());
  self->return_value = return_value;

  // Call the cleanup handlers first.
  while (self->cleanup_stack) {
    __pthread_cleanup_t* c = self->cleanup_stack;
    self->cleanup_stack = c->__cleanup_prev;
    c->__cleanup_routine(c->__cleanup_arg);
  }

  // Call the TLS destructors. It is important to do that before removing this
  // thread from the global list. This will ensure that if someone else deletes
  // a TLS key, the corresponding value will be set to NULL in this thread's TLS
  // space (see pthread_key_delete).
  __pthread_key_clean_all();

  if (self->alternate_signal_stack != NULL) {
    // Tell the kernel to stop using the alternate signal stack.
    stack_t ss;
    ss.ss_sp = NULL;
    ss.ss_flags = SS_DISABLE;
    sigaltstack(&ss, NULL);

    // Free it.
    munmap(self->alternate_signal_stack, SIGSTKSZ);
    self->alternate_signal_stack = NULL;
  }

  // Keep track of what we need to know about the stack before we lose the pthread_internal_t.
  void* stack_base = self->attr.stack_base;
  size_t stack_size = self->attr.stack_size;
  bool user_allocated_stack = ((self->attr.flags & PTHREAD_ATTR_FLAG_USER_ALLOCATED_STACK) != 0);

  {
    ScopedPthreadMutexLocker locker(&self->flags_mutex);

    if ((self->attr.flags & PTHREAD_ATTR_FLAG_DETACHED) != 0) {
      // The thread is detached, so we can free the pthread_internal_t.
      // First make sure that the kernel does not try to clear the tid field
      // because we'll have freed the memory before the thread actually exits.
      __set_tid_address(NULL);
      self->destroy();
    } else {
      // Make sure that the pthread_internal_t doesn't have stale pointers to a stack that
      // will be unmapped after the exit call below.
      if (!user_allocated_stack) {
        self->attr.stack_base = NULL;
        self->attr.stack_size = 0;
        self->tls = NULL;
      }
      // pthread_join is responsible for destroying the pthread_internal_t for non-detached threads.
      // The kernel will futex_wake on the pthread_internal_t::tid field to wake pthread_join.
    }
  }

  if (user_allocated_stack) {
    // Cleaning up this thread's stack is the creator's responsibility, not ours.
    __exit(0);
  } else {
    // We need to munmap the stack we're running on before calling exit.
    // That's not something we can do in C.

    // We don't want to take a signal after we've unmapped the stack.
    // That's one last thing we can handle in C.
    sigset_t mask;
    sigfillset(&mask);
    sigprocmask(SIG_SETMASK, &mask, NULL);

    _exit_with_stack_teardown(stack_base, stack_size);
  }
}
