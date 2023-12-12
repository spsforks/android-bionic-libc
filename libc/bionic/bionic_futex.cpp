/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "private/bionic_futex.h"

#include <time.h>

#include "private/bionic_time_conversions.h"

static inline __always_inline int FutexWithTimeout(volatile void* ftx, int op, int value,
                                                   bool use_realtime_clock,
                                                   const timespec* abs_timeout, int bitset) {
  const timespec* _Nonnull futex_abs_timeout = abs_timeout;
  // pthread's and semaphore's default behavior is to use CLOCK_REALTIME, however this behavior is
  // essentially never intended, as that clock is prone to change discontinuously.
  //
  // What users really intend is to use CLOCK_MONOTONIC, however only pthread_cond_timedwait()
  // provides this as an option and even there, a large amount of existing code does not opt into
  // CLOCK_MONOTONIC.
  //
  // We have seen numerous bugs directly attributable to this difference.  Therefore, we provide
  // this general workaround to always use CLOCK_MONOTONIC for waiting, regardless of what the input
  // timespec is.
  timespec converted_timeout;
  if (abs_timeout) {
    // check the clock base.
    const int opcmd = op & FUTEX_CMD_MASK;
    const int opcmdrt = op & (FUTEX_CMD_MASK | FUTEX_CLOCK_REALTIME);
    if (opcmd == FUTEX_WAIT || opcmdrt == FUTEX_WAIT_BITSET || opcmdrt == FUTEX_LOCK_PI2 ||
        opcmdrt == FUTEX_WAIT_REQUEUE_PI) {
      // futex op is monotonic, may need to convert abs_timeout.
      if (use_realtime_clock) {
        monotonic_time_from_realtime_time(converted_timeout, *abs_timeout);
        futex_abs_timeout = &converted_timeout;
      }
    } else if (opcmd == FUTEX_LOCK_PI || opcmdrt == (FUTEX_WAIT_BITSET | FUTEX_CLOCK_REALTIME) ||
               opcmdrt == (FUTEX_LOCK_PI2 | FUTEX_CLOCK_REALTIME) ||
               opcmdrt == (FUTEX_WAIT_REQUEUE_PI | FUTEX_CLOCK_REALTIME)) {
      // futex op is realtime, may need to convert abs_timeout.
      if (!use_realtime_clock) {
        realtime_time_from_monotonic_time(converted_timeout, *abs_timeout);
        futex_abs_timeout = &converted_timeout;
      }
    }

    if (futex_abs_timeout->tv_sec < 0) {
      return -ETIMEDOUT;
    }
  }

  return __futex(ftx, op, value, futex_abs_timeout, bitset);
}

int __futex_wait_ex(volatile void* ftx, bool shared, int value, bool use_realtime_clock,
                    const timespec* abs_timeout) {
  return FutexWithTimeout(ftx, (shared ? FUTEX_WAIT_BITSET : FUTEX_WAIT_BITSET_PRIVATE), value,
                          use_realtime_clock, abs_timeout, FUTEX_BITSET_MATCH_ANY);
}

int __futex_pi_lock_ex(volatile void* ftx, bool shared, bool use_realtime_clock,
                       const timespec* abs_timeout) {
  // We really want FUTEX_LOCK_PI2 which is default CLOCK_MONOTONIC, but that isn't supported
  // on linux before 5.14.  FUTEX_LOCK_PI uses CLOCK_REALTIME.  Here we verify support.

  static const bool supports_PI2 = [] {
    uint32_t tmp_ftx_word = 0;
    if (FutexWithTimeout(&tmp_ftx_word, FUTEX_LOCK_PI2, 0 /* value */,
                         false /* use_realtime_clock */, nullptr /* abs_timeout */,
                         0 /* bitset */) == 0) {
      // tmp_ftx_word contains tid
      FutexWithTimeout(&tmp_ftx_word, FUTEX_UNLOCK_PI, 0 /* value */,
                       false /* use_realtime_clock */, nullptr /* abs_timeout */, 0 /* bitset */);
      return true;
    }
    return false;
  }();

  int op = supports_PI2 ? FUTEX_LOCK_PI2 : FUTEX_LOCK_PI;
  if (!shared) op |= FUTEX_PRIVATE_FLAG;
  return FutexWithTimeout(ftx, op, 0 /* value */, use_realtime_clock, abs_timeout, 0 /* bitset */);
}
