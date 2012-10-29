/*
 * Copyright (C) 2012 The Android Open Source Project
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
#include <pthread.h>

int pthread_barrierattr_init(pthread_barrierattr_t* attr) {
  if (attr != NULL) {
    *attr = PTHREAD_PROCESS_PRIVATE;
    return 0;
  } else
    return EINVAL;
}

int pthread_barrierattr_destroy(pthread_barrierattr_t* attr) {
  if (attr != NULL) {
    *attr = 0;
    return 0;
  } else {
    return EINVAL;
  }
}

int pthread_barrierattr_setpshared(pthread_barrierattr_t* attr,
                                   int pshared) {
  if (attr == NULL)
    return EINVAL;
  if (pshared == PTHREAD_PROCESS_PRIVATE ||
      pshared == PTHREAD_PROCESS_SHARED) {
    *attr = pshared;
    return 0;
  } else
    return EINVAL;
}

int pthread_barrierattr_getpshared(pthread_barrierattr_t* attr,
                                   int *pshared) {
  if (attr == NULL || pshared == NULL)
    return EINVAL;
  if (*attr == PTHREAD_PROCESS_PRIVATE ||
      *attr == PTHREAD_PROCESS_SHARED) {
    *pshared = *attr;
    return 0;
  } else
    return EINVAL;
}

int pthread_barrier_init(pthread_barrier_t* barrier,
                         const pthread_barrierattr_t* attr,
                         unsigned count) {
  if (barrier == NULL || count == 0)
    return EINVAL;

  barrier->count = count;

  if (attr == NULL || *attr == PTHREAD_PROCESS_PRIVATE) {
    pthread_mutex_init(&barrier->lock, NULL);
    pthread_cond_init(&barrier->cond, NULL);
  } else if (*attr == PTHREAD_PROCESS_SHARED) {
    pthread_mutexattr_t lock_attr;
    pthread_mutexattr_init(&lock_attr);
    pthread_mutexattr_setpshared(&lock_attr, PTHREAD_PROCESS_SHARED);
    pthread_mutex_init(&barrier->lock, &lock_attr);
    pthread_mutexattr_destroy(&lock_attr);

    pthread_condattr_t  cond_attr;
    pthread_condattr_init(&cond_attr);
    pthread_condattr_setpshared(&cond_attr, PTHREAD_PROCESS_SHARED);
    pthread_cond_init(&barrier->cond, &cond_attr);
    pthread_condattr_destroy(&cond_attr);
  } else {
    return EINVAL;
  }
  return 0;
}

int pthread_barrier_wait(pthread_barrier_t* barrier) {
  if (barrier == NULL)
    return EINVAL;

  int result;
  pthread_mutex_lock(&barrier->lock);
  if (barrier->count == 0)
    result = EINVAL;
  else {
    barrier->count--;
    if (barrier->count == 0) {
      result = PTHREAD_BARRIER_SERIAL_THREAD;
      // Wake up all waiting threads.
      pthread_cond_broadcast(&barrier->cond);
    } else {
      // Wait to be waked up by final thread.
      pthread_cond_wait(&barrier->cond, &barrier->lock);
      result = 0;
    }
  }
  pthread_mutex_unlock(&barrier->lock);
  return result;
}

int pthread_barrier_destroy(pthread_barrier_t* barrier) {
  if (barrier == NULL)
    return EINVAL;

  pthread_cond_destroy(&barrier->cond);
  pthread_mutex_destroy(&barrier->lock);
  barrier->count = 0;

  return 0;
}
