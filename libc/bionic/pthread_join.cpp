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

#include <errno.h>

#include "private/bionic_futex.h"
#include "pthread_internal.h"

int pthread_join(pthread_t t, void** return_value) {
  if (t == pthread_self()) {
    return EDEADLK;
  }

  pthread_internal_t* thread = __pthread_internal_get(t);
  if (thread == NULL) {
    return ESRCH;
  }

  ThreadJoinState old_state = THREAD_NOT_JOINED;
  while ((old_state == THREAD_NOT_JOINED || old_state == THREAD_EXITED_NOT_JOINED) &&
         !atomic_compare_exchange_weak(&thread->join_state, &old_state, THREAD_JOINED)) {
  }

  if (old_state == THREAD_DETACHED || old_state == THREAD_JOINED) {
    return EINVAL;
  }

  pid_t tid = thread->tid;
  volatile int* tid_ptr = &thread->tid;

  // We set thread->join_state to THREAD_JOINED with atomic operation,
  // so no one is going to remove this thread except us.

  // Wait for the thread to actually exit, if it hasn't already.
  while (*tid_ptr != 0) {
    __futex_wait(tid_ptr, tid, NULL);
  }

  if (return_value) {
    *return_value = thread->return_value;
  }

  __pthread_internal_remove_and_free(thread);
  return 0;
}
