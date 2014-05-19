/*
 * Copyright (C) 2010 The Android Open Source Project
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
#include <sys/atomics.h>
#include "private/bionic_atomic_inline.h"
/* Technical note:
 *
 * Possible states of a read/write lock:
 *
 *  - no readers and no writer (unlocked)
 *  - one or more readers sharing the lock at the same time (read-locked)
 *  - one writer holding the lock (write-lock)
 *
 * Additionally:
 *  - trying to get the write-lock while there are any readers blocks
 *  - trying to get the read-lock while there is a writer blocks
 *  - a single thread can acquire the lock multiple times in the same mode
 *
 *  - Posix states that behavior is undefined it a thread tries to acquire
 *    the lock in two distinct modes (e.g. write after read, or read after write).
 *
 *  - This implementation tries to avoid writer starvation by making the readers
 *    block as soon as there is a waiting writer on the lock. However, it cannot
 *    completely eliminate it: each time the lock is unlocked, all waiting threads
 *    are woken and battle for it, which one gets it depends on the kernel scheduler
 *    and is semi-random.
 *
 */

#define  RWLOCKATTR_DEFAULT     0
#define  RWLOCKATTR_SHARED_MASK 0x0010

extern pthread_internal_t* __get_thread(void);

int pthread_rwlockattr_init(pthread_rwlockattr_t *attr)
{
    *attr = PTHREAD_PROCESS_PRIVATE;
    return 0;
}

int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr)
{
    *attr = -1;
    return 0;
}

int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int  pshared)
{
    switch (pshared) {
    case PTHREAD_PROCESS_PRIVATE:
    case PTHREAD_PROCESS_SHARED:
        *attr = pshared;
        return 0;
    default:
        return EINVAL;
    }
}

int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t* attr, int* pshared) {
    *pshared = *attr;
    return 0;
}

int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr)
{
    pthread_mutexattr_t*  lock_attr = NULL;
    pthread_condattr_t*   cond_attr = NULL;
    pthread_mutexattr_t   lock_attr0;
    pthread_condattr_t    cond_attr0;
    int                   ret;

    if (attr && *attr == PTHREAD_PROCESS_SHARED) {
        lock_attr = &lock_attr0;
        pthread_mutexattr_init(lock_attr);
        pthread_mutexattr_setpshared(lock_attr, PTHREAD_PROCESS_SHARED);

        cond_attr = &cond_attr0;
        pthread_condattr_init(cond_attr);
        pthread_condattr_setpshared(cond_attr, PTHREAD_PROCESS_SHARED);
    }

    ret = pthread_mutex_init(&rwlock->lock, lock_attr);
    if (ret != 0)
        return ret;

    ret = pthread_cond_init(&rwlock->cond, cond_attr);
    if (ret != 0) {
        pthread_mutex_destroy(&rwlock->lock);
        return ret;
    }

    rwlock->state = 0;
    rwlock->pendingReaders = 0;
    rwlock->pendingWriters = 0;
    rwlock->writerThreadId = 0;

    return 0;
}

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock)
{
    if (rwlock->state != 0)
        return EBUSY;

    pthread_cond_destroy(&rwlock->cond);
    pthread_mutex_destroy(&rwlock->lock);
    return 0;
}


static int __pthread_rwlock_timedrdlock(pthread_rwlock_t* rwlock, const timespec* abs_timeout) {
  bool done = false;
  do {
    int32_t cur_state = rwlock->state;
    if (__predict_true(cur_state >= 0)) {
      // Add as an extra reader.
      done = __atomic_cmpxchg(cur_state, cur_state + 1, &rwlock->state) == 0;
    } else {
      timespec ts;
      timespec* tsp;
      if (abs_timeout != NULL) {
        if (__timespec_from_absolute(&ts, abs_timeout, CLOCK_REALTIME) < 0) {
          return ETIMEDOUT;
        }
        tsp = &ts;
      } else {
        tsp = NULL;
      }
      // Owner holds it exclusively, hang up.
      __atomic_inc(&rwlock->pendingReaders);
      if (__futex_wait(&rwlock->state, cur_state, abs_timeout) != 0) {
        if (errno == ETIMEDOUT) {
          __atomic_dec(&rwlock->pendingReaders);
          return ETIMEDOUT;
        }
      }
      __atomic_dec(&rwlock->pendingReaders);
    }
  } while (!done);
  return 0;
}

static int __pthread_rwlock_timedwrlock(pthread_rwlock_t* rwlock, const timespec* abs_timeout) {
  int tid = __get_thread()->tid;
  bool done = false;
  do {
    int32_t cur_state = rwlock->state;
    if (__predict_true(cur_state == 0)) {
      // Change state from 0 to -1.
      done =  __atomic_cmpxchg(0 /* cur_state*/, -1 /* new state */, &rwlock->state);
    } else {
      timespec ts;
      timespec* tsp;
      if (abs_timeout != NULL) {
        if (__timespec_from_absolute(&ts, abs_timeout, CLOCK_REALTIME) < 0) {
          return ETIMEDOUT;
        }
        tsp = &ts;
      } else {
        tsp = NULL;
      }
      // Failed to acquire, hang up.
      __atomic_inc(&rwlock->pendingWriters);
      if (__futex_wait(&rwlock->state, cur_state, NULL) != 0) {
        if (errno == ETIMEDOUT) {
          __atomic_dec(&rwlock->pendingWriters);
          return ETIMEDOUT;
        }
      }
      __atomic_dec(&rwlock->pendingWriters);
    }
  } while (!done);
  rwlock->writerThreadId = tid;
  return 0;
}

int pthread_rwlock_rdlock(pthread_rwlock_t* rwlock) {
  return __pthread_rwlock_timedrdlock(rwlock, NULL);
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock)
{
  int32_t cur_state = rwlock->state;
  if (cur_state >= 0) {
    // Add as an extra reader.
    if(__atomic_cmpxchg(cur_state, cur_state + 1, &rwlock->state) == 0) {
      return EBUSY;
    }
  } else {
    // Owner holds it exclusively.
    return EBUSY;
  }
  return 0;
}

int pthread_rwlock_timedrdlock(pthread_rwlock_t* rwlock, const timespec* abs_timeout) {
  return __pthread_rwlock_timedrdlock(rwlock, abs_timeout);
}

int pthread_rwlock_wrlock(pthread_rwlock_t* rwlock) {
  return __pthread_rwlock_timedwrlock(rwlock, NULL);
}

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{
  int tid = __get_thread()->tid;
  int32_t cur_state = rwlock->state;
  if (cur_state == 0) {
    if(__atomic_cmpxchg(0, -1, &rwlock->state) != 0) {
      return EBUSY;
    }
  }
  rwlock->writerThreadId = tid;
  return 0;
}

int pthread_rwlock_timedwrlock(pthread_rwlock_t* rwlock, const timespec* abs_timeout) {
  return __pthread_rwlock_timedwrlock(rwlock, abs_timeout);
}

int pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
{
  int tid = __get_thread()->tid;
  bool done = false;
  do {
    int32_t cur_state = rwlock->state;
    if (cur_state == 0) {
      return EPERM;
    }
    if (cur_state == -1) {
      if (rwlock->writerThreadId != tid) {
        return EPERM;
      }
      // Change state from -1 to 0.
      done =  __atomic_cmpxchg(-1 /* cur_state*/, 0 /* new state */, &rwlock->state);
      if (__predict_true(done)) {  // cmpxchg may fail due to noise?
        // We're no longer the owner.
        rwlock->writerThreadId = 0;
        // Wake any waiters.
        if (__predict_false(rwlock->pendingReaders > 0 || rwlock->pendingWriters > 0)) {
          __futex_wake(&rwlock->state, 0x7fffffff);
        }
      }
    } else { // cur_state > 0
      // Reduce state by 1.
      done = __atomic_cmpxchg(cur_state, cur_state - 1, &rwlock->state) == 0;
      if (done && (cur_state - 1) == 0) {  // cmpxchg may fail due to noise?
        if (rwlock->pendingReaders > 0 || rwlock->pendingWriters > 0) {
          // Wake any exclusive waiters as there are now no readers.
          __futex_wake(&rwlock->state, 0x7fffffff);
        }
      }
    }
  } while (!done);

  return 0;
}
