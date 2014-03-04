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

#include "pthread_accessor.h"

int pthread_detach(pthread_t t) {
  volatile int* tid_ptr;

  pthread_accessor thread(t);
  if (thread.get() == NULL) {
      return ESRCH;
  }

  if (thread->attr.flags & PTHREAD_ATTR_FLAG_DETACHED) {
    return EINVAL; // Already detached.
  }

  if (thread->attr.flags & PTHREAD_ATTR_FLAG_JOINED) {
    return 0; // Already being joined; silently do nothing, like glibc.
  }

  tid_ptr = &thread->tid;
  if( tid_ptr == 0 ){
    pthread_mutex_lock(&gThreadListLock);
    _pthread_internal_remove_locked(thread.get());
    pthread_mutex_unlock(&gThreadListLock);
    return 0;
  }

  thread->attr.flags |= PTHREAD_ATTR_FLAG_DETACHED;
  return 0;
}
