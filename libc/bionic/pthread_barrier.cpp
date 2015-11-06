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

#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>

#include "private/bionic_futex.h"

struct pthread_barrier_internal_t {
  uint32_t init_count;
  atomic_uint wait_count;
  atomic_uint wait_serial;
  uint32_t __reserved[5];
};

static_assert(sizeof(pthread_barrier_t) == sizeof(pthread_barrier_internal_t),
              "pthread_barrier_t should actually be pthread_barrier_internal_t in implementation.");

static_assert(alignof(pthread_barrier_t) >= 4,
              "pthread_barrier_t should fulfill the alignment of pthread_barrier_internal_t.");

static inline pthread_barrier_internal_t* __get_internal_barrier(pthread_barrier_t* barrier) {
  return reinterpret_cast<pthread_barrier_internal_t*>(barrier);
}

int pthread_barrier_init(pthread_barrier_t* barrier_interface, const pthread_barrierattr_t*,
                         unsigned count) {
  pthread_barrier_internal_t* barrier = __get_internal_barrier(barrier_interface);
  if (count == 0) {
    return EINVAL;
  }
  barrier->init_count = count;
  atomic_init(&barrier->wait_count, 0);
  atomic_init(&barrier->wait_serial, 0);
  return 0;
}

int pthread_barrier_wait(pthread_barrier_t* barrier_interface) {
  pthread_barrier_internal_t* barrier = __get_internal_barrier(barrier_interface);

  // Read wait_serial before updating wait_count. Because we have to make non-serial
  // threads read the value of wait_serial before it is updated by the serial thread.
  uint32_t serial = atomic_load(&barrier->wait_serial);
  uint32_t value = atomic_fetch_add_explicit(&barrier->wait_count, 1, memory_order_relaxed);

  if (value + 1 == barrier->init_count) {
    atomic_fetch_add(&barrier->wait_serial, 1);
    __futex_wake(&barrier->wait_serial, barrier->init_count);
    atomic_fetch_sub_explicit(&barrier->wait_count, 1, memory_order_relaxed);
    return PTHREAD_BARRIER_SERIAL_THREAD;
  }

  while (atomic_load_explicit(&barrier->wait_serial, memory_order_relaxed) == serial) {
    __futex_wait(&barrier->wait_serial, serial, nullptr);
  }
  atomic_fetch_sub_explicit(&barrier->wait_count, 1, memory_order_relaxed);
  return 0;
}

int pthread_barrier_destroy(pthread_barrier_t* barrier_interface) {
  pthread_barrier_internal_t* barrier = __get_internal_barrier(barrier_interface);
  if (barrier->init_count == 0u) {
    return EINVAL;
  }
  if (atomic_load(&barrier->wait_count) != 0u) {
    return EBUSY;
  }
  barrier->init_count = 0u;
  return 0;
}
