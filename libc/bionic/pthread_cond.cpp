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

#include <errno.h>
#include <limits.h>
#include <stdatomic.h>
#include <sys/mman.h>
#include <time.h>
#include <unistd.h>

#include "pthread_internal.h"

#include "private/bionic_futex.h"
#include "private/bionic_time_conversions.h"
#include "private/bionic_tls.h"

// We use one bit in pthread_condattr_t (long) values as the 'shared' flag
// and one bit for the clock type (CLOCK_REALTIME is ((clockid_t) 1), and
// CLOCK_MONOTONIC is ((clockid_t) 0).). The rest of the bits are a counter.
//
// The 'value' field pthread_cond_t has the same layout.

#define COND_SHARED_MASK 0x0001
#define COND_CLOCK_MASK 0x0002
#define COND_COUNTER_STEP 0x0004
#define COND_FLAGS_MASK (COND_SHARED_MASK | COND_CLOCK_MASK)
#define COND_COUNTER_MASK (~COND_FLAGS_MASK)

#define COND_IS_SHARED(c) (((c) & COND_SHARED_MASK) != 0)
#define COND_GET_CLOCK(c) (((c) & COND_CLOCK_MASK) >> 1)
#define COND_SET_CLOCK(attr, c) ((attr) | (c << 1))


int pthread_condattr_init(pthread_condattr_t* attr) {
  *attr = 0;
  *attr |= PTHREAD_PROCESS_PRIVATE;
  *attr |= (CLOCK_REALTIME << 1);
  return 0;
}

int pthread_condattr_getpshared(const pthread_condattr_t* attr, int* pshared) {
  *pshared = static_cast<int>(COND_IS_SHARED(*attr));
  return 0;
}

int pthread_condattr_setpshared(pthread_condattr_t* attr, int pshared) {
  if (pshared != PTHREAD_PROCESS_SHARED && pshared != PTHREAD_PROCESS_PRIVATE) {
    return EINVAL;
  }

  *attr |= pshared;
  return 0;
}

int pthread_condattr_getclock(const pthread_condattr_t* attr, clockid_t* clock) {
  *clock = COND_GET_CLOCK(*attr);
  return 0;
}

int pthread_condattr_setclock(pthread_condattr_t* attr, clockid_t clock) {
  if (clock != CLOCK_MONOTONIC && clock != CLOCK_REALTIME) {
    return EINVAL;
  }

  *attr = COND_SET_CLOCK(*attr, clock);
  return 0;
}

int pthread_condattr_destroy(pthread_condattr_t* attr) {
  *attr = 0xdeada11d;
  return 0;
}


// XXX *technically* there is a race condition that could allow
// XXX a signal to be missed.  If thread A is preempted in _wait()
// XXX after unlocking the mutex and before waiting, and if other
// XXX threads call signal or broadcast UINT_MAX/2 times (exactly),
// XXX before thread A is scheduled again and calls futex_wait(),
// XXX then the signal will be lost.

int pthread_cond_init(pthread_cond_t* cond, const pthread_condattr_t* attr) {
  if (attr != NULL) {
    cond->value = (*attr & COND_FLAGS_MASK);
  } else {
    cond->value = 0;
  }

  return 0;
}

int pthread_cond_destroy(pthread_cond_t* cond) {
  cond->value = 0xdeadc04d;
  return 0;
}

// This function is used by pthread_cond_broadcast and
// pthread_cond_signal to atomically decrement the counter
// then wake up 'counter' threads.
static int __pthread_cond_pulse(pthread_cond_t* cond, int counter) {
  static_assert(sizeof(atomic_int) == sizeof(cond->value),
                "cond->value should actually be atomic_int in implementation.");

  // We prefer casting to atomic_int instead of declaring cond->value to be atomic_int directly.
  // Because using the second method pollutes pthread.h, and causes an error when compiling libcxx.
  atomic_int* cond_value_ptr = reinterpret_cast<atomic_int*>(&cond->value);

  int old_value = atomic_load_explicit(cond_value_ptr, memory_order_relaxed);
  int flags = old_value & COND_FLAGS_MASK;
  int new_value;

  // Use compare_exchange to change value, so we can use futex_wake to wake up waiters.
  // If exchanged successfully, a release fence is required to expose all stores to the
  // waiting threads.
  do {
    new_value = ((old_value - COND_COUNTER_STEP) & COND_COUNTER_MASK) | flags;
  } while (!atomic_compare_exchange_weak_explicit(cond_value_ptr, &old_value, new_value,
                                                  memory_order_release, memory_order_relaxed));

  __futex_wake_ex(&cond->value, COND_IS_SHARED(cond->value), counter);
  return 0;
}

__LIBC_HIDDEN__
int __pthread_cond_timedwait_relative(pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* reltime) {
  int old_value = cond->value;

  pthread_mutex_unlock(mutex);
  int status = __futex_wait_ex(&cond->value, COND_IS_SHARED(cond->value), old_value, reltime);
  // pthread_mutex_lock has an acquire fence, which makes all stores made before
  // pthread_cond_signal/pthread_cond_broadcast visible here.
  pthread_mutex_lock(mutex);

  if (status == -ETIMEDOUT) {
    return ETIMEDOUT;
  }
  return 0;
}

__LIBC_HIDDEN__
int __pthread_cond_timedwait(pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* abs_ts, clockid_t clock) {
  timespec ts;
  timespec* tsp;

  if (abs_ts != NULL) {
    if (!timespec_from_absolute_timespec(ts, *abs_ts, clock)) {
      return ETIMEDOUT;
    }
    tsp = &ts;
  } else {
    tsp = NULL;
  }

  return __pthread_cond_timedwait_relative(cond, mutex, tsp);
}

int pthread_cond_broadcast(pthread_cond_t* cond) {
  return __pthread_cond_pulse(cond, INT_MAX);
}

int pthread_cond_signal(pthread_cond_t* cond) {
  return __pthread_cond_pulse(cond, 1);
}

int pthread_cond_wait(pthread_cond_t* cond, pthread_mutex_t* mutex) {
  return __pthread_cond_timedwait(cond, mutex, NULL, COND_GET_CLOCK(cond->value));
}

int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t * mutex, const timespec *abstime) {
  return __pthread_cond_timedwait(cond, mutex, abstime, COND_GET_CLOCK(cond->value));
}

#if !defined(__LP64__)
// TODO: this exists only for backward binary compatibility on 32 bit platforms.
extern "C" int pthread_cond_timedwait_monotonic(pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* abstime) {
  return __pthread_cond_timedwait(cond, mutex, abstime, CLOCK_MONOTONIC);
}

extern "C" int pthread_cond_timedwait_monotonic_np(pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* abstime) {
  return __pthread_cond_timedwait(cond, mutex, abstime, CLOCK_MONOTONIC);
}

extern "C" int pthread_cond_timedwait_relative_np(pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* reltime) {
  return __pthread_cond_timedwait_relative(cond, mutex, reltime);
}

extern "C" int pthread_cond_timeout_np(pthread_cond_t* cond, pthread_mutex_t* mutex, unsigned ms) {
  timespec ts;
  timespec_from_ms(ts, ms);
  return __pthread_cond_timedwait_relative(cond, mutex, &ts);
}
#endif // !defined(__LP64__)
