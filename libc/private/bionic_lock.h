/*
 * Copyright (C) 2015 The Android Open Source Project
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
#ifndef _BIONIC_LOCK_H
#define _BIONIC_LOCK_H

#include <stdatomic.h>
#include "private/bionic_futex.h"

class Lock {
 private:
  enum LockState {
    Unlocked = 0,
    LockedWithoutWaiter,
    LockedWithWaiter,
  };
  _Atomic(LockState) state;
  bool process_shared;

 public:
  Lock(bool process_shared = false) {
    init(process_shared);
  }

  void init(bool process_shared) {
    atomic_init(&state, Unlocked);
    this->process_shared = process_shared;
  }

  void lock() {
    LockState old_state = Unlocked;
    if (__predict_true(atomic_compare_exchange_strong_explicit(&state, &old_state,
                         LockedWithoutWaiter, memory_order_acquire, memory_order_relaxed))) {
      return;
    }
    while (atomic_exchange_explicit(&state, LockedWithWaiter, memory_order_acquire) != Unlocked) {
      __futex_wait_ex(&state, process_shared, LockedWithWaiter, NULL);
    }
    return;
  }

  void unlock() {
    if (atomic_exchange_explicit(&state, Unlocked, memory_order_release) == LockedWithWaiter) {
      __futex_wake_ex(&state, process_shared, 1);
    }
  }
};

#endif  // _BIONIC_LOCK_H
