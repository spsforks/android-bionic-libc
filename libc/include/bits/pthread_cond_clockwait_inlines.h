/*
 * Copyright (C) 2019 The Android Open Source Project
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

#pragma once

#include <errno.h>
#include <linux/futex.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <sys/cdefs.h>
#include <sys/syscall.h>
#include <unistd.h>

#if __ANDROID_API__ < __ANDROID_API_R__ || defined(__BIONIC_PTHREAD_COND_INLINE)

__BEGIN_DECLS

static __inline int pthread_cond_clockwait(pthread_cond_t* __cond_interface,
                                           pthread_mutex_t* __mutex, clockid_t __clock,
                                           const struct timespec* __timeout) {
  struct __pthread_cond_internal_t {
    atomic_uint __state;
  };

  struct __pthread_cond_internal_t* __cond = (struct __pthread_cond_internal_t*)__cond_interface;
  bool __process_shared = (atomic_load_explicit(&__cond->__state, memory_order_relaxed) & 0x1) != 0;
  bool __use_realtime_clock = false;

  switch (__clock) {
    case CLOCK_MONOTONIC:
      break;
    case CLOCK_REALTIME:
      __use_realtime_clock = true;
      break;
    default:
      return EINVAL;
  }

  /* check_timespec(__timeout, true); */
  if (__timeout != NULL) {
    if (__timeout->tv_nsec < 0 || __timeout->tv_nsec >= 1000000000) {
      return EINVAL;
    }
    if (__timeout->tv_sec < 0) {
      return ETIMEDOUT;
    }
  }

  unsigned int __old_state = atomic_load_explicit(&__cond->__state, memory_order_relaxed);
  pthread_mutex_unlock(__mutex);
  /* int __status = __futex_wait_ex(&__cond->__state, __process_shared, __old_state,
                                    __use_realtime_clock, __timeout); */

  const struct timespec* __futex_timeout = __timeout;
  struct timespec __converted_monotonic_abs_timeout = *__timeout;
  if (__timeout != NULL && __use_realtime_clock) {
    /* monotonic_time_from_realtime_time(__converted_monotonic_abs_timeout, *__timeout) */
    struct timespec __cur_monotonic_time;
    clock_gettime(CLOCK_MONOTONIC, &__cur_monotonic_time);
    struct timespec __cur_realtime_time;
    clock_gettime(CLOCK_REALTIME, &__cur_realtime_time);

    __converted_monotonic_abs_timeout.tv_nsec -= __cur_realtime_time.tv_nsec;
    __converted_monotonic_abs_timeout.tv_nsec += __cur_monotonic_time.tv_nsec;
    if (__converted_monotonic_abs_timeout.tv_nsec >= 1000000000) {
      __converted_monotonic_abs_timeout.tv_nsec -= 1000000000;
      __converted_monotonic_abs_timeout.tv_sec += 1;
    } else if (__converted_monotonic_abs_timeout.tv_nsec < 0) {
      __converted_monotonic_abs_timeout.tv_nsec += 1000000000;
      __converted_monotonic_abs_timeout.tv_sec -= 1;
    }
    __converted_monotonic_abs_timeout.tv_sec -= __cur_realtime_time.tv_sec;
    __converted_monotonic_abs_timeout.tv_sec += __cur_monotonic_time.tv_sec;

    if (__converted_monotonic_abs_timeout.tv_sec < 0) {
      return ETIMEDOUT;
    }
    __futex_timeout = &__converted_monotonic_abs_timeout;
  }

  /* return __futex(&__cond->__state, op, __old_state, __futex_timeout, bitset); */
  int __saved_errno = errno;
  int __result = syscall(__NR_futex, &__cond->__state,
                         (__process_shared ? FUTEX_WAIT_BITSET : FUTEX_WAIT_BITSET_PRIVATE),
                         __old_state, __futex_timeout, NULL, FUTEX_BITSET_MATCH_ANY);
  if (__result == -1) {
    __result = -errno;
    errno = __saved_errno;
  }

  pthread_mutex_lock(__mutex);

  if (__result == -ETIMEDOUT) {
    return ETIMEDOUT;
  }
  return 0;
}

__END_DECLS

#endif
