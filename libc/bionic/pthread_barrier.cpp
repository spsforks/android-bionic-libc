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

int pthread_barrierattr_init(pthread_barrierattr_t* attr) {
  *attr = 0;
  return 0;
}

int pthread_barrierattr_destroy(pthread_barrierattr_t* attr) {
  *attr = 0;
  return 0;
}

int pthread_barrierattr_getpshared(pthread_barrierattr_t* attr, int* pshared) {
  *pshared = (*attr & 1) ? PTHREAD_PROCESS_SHARED : PTHREAD_PROCESS_PRIVATE;
  return 0;
}

int pthread_barrierattr_setpshared(pthread_barrierattr_t* attr, int pshared) {
  if (pshared == PTHREAD_PROCESS_SHARED) {
    *attr |= 1;
  } else {
    *attr &= ~1;
  }
  return 0;
}

struct pthread_barrier_internal_t {
  // One barrier can be used for unlimited number of cycles. In each cycle, only [init_count]
  // threads can call pthread_barrier_wait() successfully, others should wait for the next cycle.
  // In each cycle, [init_count] threads must call pthread_barrier_wait() before any of them
  // successfully return from the call.
  uint32_t init_count;
  // The left number of threads can enter the barrier in this cycle.
  atomic_uint thread_count_can_enter;
  // Number of threads having entered but not left the barrier in this cycle.
  atomic_uint wait_count;
  // Barrier epoch number. It is used to wake up the threads waiting on the barrier in this cycle.
  atomic_uint wait_serial;
  // Whether the barrier is shared across processes.
  bool pshared;
  uint32_t __reserved[3];
};

static_assert(sizeof(pthread_barrier_t) == sizeof(pthread_barrier_internal_t),
              "pthread_barrier_t should actually be pthread_barrier_internal_t in implementation.");

static_assert(alignof(pthread_barrier_t) >= 4,
              "pthread_barrier_t should fulfill the alignment of pthread_barrier_internal_t.");

static inline pthread_barrier_internal_t* __get_internal_barrier(pthread_barrier_t* barrier) {
  return reinterpret_cast<pthread_barrier_internal_t*>(barrier);
}

int pthread_barrier_init(pthread_barrier_t* barrier_interface, const pthread_barrierattr_t* attr,
                         unsigned count) {
  pthread_barrier_internal_t* barrier = __get_internal_barrier(barrier_interface);
  if (count == 0) {
    return EINVAL;
  }
  barrier->init_count = count;
  atomic_init(&barrier->thread_count_can_enter, count);
  atomic_init(&barrier->wait_count, 0);
  atomic_init(&barrier->wait_serial, 0);
  barrier->pshared = false;
  if (attr != nullptr && (*attr & 1)) {
    barrier->pshared = true;
  }
  return 0;
}

static void enter_the_barrier(pthread_barrier_internal_t* barrier) {
  while (true) {
    uint32_t can_enter = atomic_load_explicit(&barrier->thread_count_can_enter, memory_order_relaxed);
    if (can_enter == 0) {
      // We should wait for the next cycle.
      __futex_wait_ex(&barrier->thread_count_can_enter, barrier->pshared, 0, nullptr);
      continue;
    }
    if (atomic_compare_exchange_weak(&barrier->thread_count_can_enter, &can_enter, can_enter - 1u)) {
      return;
    }
  }
}

int pthread_barrier_wait(pthread_barrier_t* barrier_interface) {
  pthread_barrier_internal_t* barrier = __get_internal_barrier(barrier_interface);

  // In each cycle, only barrier->init_count threads can enter the barrier.
  enter_the_barrier(barrier);

  // Read wait_serial before updating wait_count. Because we have to make waiting
  // threads read the value of wait_serial before it is updated by the last thread.
  uint32_t serial = atomic_load(&barrier->wait_serial);
  uint32_t prev_wait_count = atomic_fetch_add_explicit(&barrier->wait_count, 1, memory_order_relaxed);

  int result = 0;
  if (prev_wait_count + 1 == barrier->init_count) {
    result = PTHREAD_BARRIER_SERIAL_THREAD;
    if (prev_wait_count != 0) {
      atomic_fetch_add(&barrier->wait_serial, 1);
      __futex_wake_ex(&barrier->wait_serial, barrier->pshared, prev_wait_count);
    }
  } else {
    while (atomic_load_explicit(&barrier->wait_serial, memory_order_relaxed) == serial) {
      __futex_wait_ex(&barrier->wait_serial, barrier->pshared, serial, nullptr);
    }
  }
  if (atomic_fetch_sub_explicit(&barrier->wait_count, 1, memory_order_relaxed) == 1) {
    // Make the barrier ready for the next cycle or to be destroyed.
    atomic_store_explicit(&barrier->thread_count_can_enter, barrier->init_count, memory_order_relaxed);
    __futex_wake_ex(&barrier->thread_count_can_enter, barrier->pshared, barrier->init_count);
  }
  return result;
}

int pthread_barrier_destroy(pthread_barrier_t* barrier_interface) {
  pthread_barrier_internal_t* barrier = __get_internal_barrier(barrier_interface);
  if (barrier->init_count == 0) {
    return EINVAL;
  }
  while (true) {
    uint32_t can_enter = atomic_load_explicit(&barrier->thread_count_can_enter, memory_order_relaxed);
    if (can_enter == 0) {
      __futex_wait_ex(&barrier->thread_count_can_enter, barrier->pshared, 0, nullptr);
      continue;
    }
    if (can_enter == barrier->init_count) {
      if (atomic_compare_exchange_weak(&barrier->thread_count_can_enter, &can_enter, 0)) {
        barrier->init_count = 0;
        return 0;
      }
    } else {
      return EBUSY;
    }
  }
}
