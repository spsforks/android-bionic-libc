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

#include "pthread_internal.h"

#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include "private/bionic_futex.h"
#include "private/bionic_tls.h"
#include "private/libc_logging.h"
#include "private/ScopedPthreadMutexLocker.h"

pthread_internal_t* g_thread_list = NULL;
pthread_mutex_t g_thread_list_lock = PTHREAD_MUTEX_INITIALIZER;

void _pthread_internal_remove_locked(pthread_internal_t* thread, bool free_thread) {
  if (thread->next != NULL) {
    thread->next->prev = thread->prev;
  }
  if (thread->prev != NULL) {
    thread->prev->next = thread->next;
  } else {
    g_thread_list = thread->next;
  }

  // For threads using user allocated stack (including the main thread), the pthread_internal_t
  // can't be freed since it is on the stack.
  if (free_thread && !thread->user_allocated_stack()) {
    // Free mmaped space, including thread stack and pthread_internal_t.
    munmap(thread->attr.stack_base, thread->mmap_size);
  }
}

void _pthread_internal_add(pthread_internal_t* thread) {
  ScopedPthreadMutexLocker locker(&g_thread_list_lock);

  // We insert at the head.
  thread->next = g_thread_list;
  thread->prev = NULL;
  if (thread->next != NULL) {
    thread->next->prev = thread;
  }
  g_thread_list = thread;
}

pthread_internal_t* __get_thread(void) {
  return reinterpret_cast<pthread_internal_t*>(__get_tls()[TLS_SLOT_THREAD_ID]);
}
