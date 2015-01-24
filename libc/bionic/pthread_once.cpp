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
#include <stdatomic.h>

#include "private/bionic_futex.h"

#define ONCE_INITIALIZING           1
#define ONCE_COMPLETED              2

/* NOTE: this implementation doesn't support a init function that throws a C++ exception
 *       or calls fork()
 */
int pthread_once(pthread_once_t* once_control, void (*init_routine)(void)) {
  static_assert(sizeof(atomic_int) == sizeof(pthread_once_t),
                "pthread_once_t should actually be atomic_int in implementation.");

  // We prefer casting to atomic_int instead of declaring pthread_once_t to be atomic_int directly.
  // Because using the second method pollutes pthread.h, and causes error when compiling libcxx.
  atomic_int* once_control_ptr = reinterpret_cast<atomic_int*>(once_control);

  // PTHREAD_ONCE_INIT is 0, we use the following flags exclusively:
  //   ONCE_INTIALIZING set  -> initialization is under way
  //   ONCE_COMPLETED set  -> initialization is complete

  // First check if the once is already initialized. This will be the common
  // case and we want to make this as fast as possible. Note that this still
  // requires a load_acquire operation here to ensure that all the
  // stores performed by the initialization function are observable on
  // this CPU after we exit.
  int old_value = atomic_load_explicit(once_control_ptr, memory_order_acquire);

  while (true) {
    // Try to atomically set the INITIALIZING flag.
    // This requires a cmpxchg loop, and we may need
    // to exit prematurely if we detect that
    // COMPLETED is now set.
    if (__predict_true(old_value == ONCE_COMPLETED)) {
      return 0;
    }
    int new_value = ONCE_INITIALIZING;
    if (!atomic_compare_exchange_weak_explicit(once_control_ptr, &old_value, new_value,
      memory_order_acquire, memory_order_acquire)) {
      continue;
    }
    // When exchange successfully, old_value can be 0 or ONCE_INITIALIZING.
    if (old_value == 0) {
      // We got here first, we can jump out of the loop to handle the initialization.
      break;
    }
    __futex_wait_ex(once_control_ptr, 0, old_value, NULL);
    old_value = atomic_load_explicit(once_control_ptr, memory_order_acquire);
  }

  // Call the initialization function.
  (*init_routine)();

  // Do a store_release indicating that initialization is complete.
  atomic_store_explicit(once_control_ptr, ONCE_COMPLETED, memory_order_release);

  // Wake up any waiters, if any.
  __futex_wake_ex(once_control_ptr, 0, INT_MAX);

  return 0;
}
