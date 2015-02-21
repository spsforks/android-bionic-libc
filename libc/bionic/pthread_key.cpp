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
#include <pthread.h>
#include <stdatomic.h>

#include "private/bionic_tls.h"
#include "pthread_internal.h"

typedef void (*key_destructor_t)(void*);

struct pthread_key_map_t {
  atomic_uintptr_t seq;
  key_destructor_t key_destructor;
};

static pthread_key_map_t key_map[BIONIC_PTHREAD_KEY_COUNT];

static inline bool KEY_IN_USE(uintptr_t seq) {
  return seq & 1;
}

static inline bool IsValidKey(pthread_key_t key) {
  return key >= 0 && key < BIONIC_PTHREAD_KEY_COUNT &&
         KEY_IN_USE(atomic_load_explicit(&key_map[key].seq, memory_order_relaxed));
}

// Called from pthread_exit() to remove all pthread keys. This must call the destructor of
// all keys that have a non-NULL data value and a non-NULL destructor.
__LIBC_HIDDEN__ void pthread_key_clean_all() {
  // Because destructors can do funky things like deleting/creating other keys,
  // we need to implement this in a loop.
  pthread_key_data_t* key_data = __get_thread()->key_data;
  for (size_t rounds = PTHREAD_DESTRUCTOR_ITERATIONS; rounds > 0; --rounds) {
    size_t called_destructor_count = 0;
    for (size_t i = 0; i < BIONIC_PTHREAD_KEY_COUNT; ++i) {
      uintptr_t seq = atomic_load_explicit(&key_map[i].seq, memory_order_relaxed);
      if (KEY_IN_USE(seq) && key_map[i].key_destructor != NULL) {
        if (key_data[i].seq == seq && key_data[i].data != NULL) {
          // We need to clear the key data now, this will prevent the destructor (or a later one)
          // from seeing the old value if it calls pthread_getspecific().
          // We don't do this if 'key_destructor == NULL' just in case another destructor
          // function is responsible for manually releasing the corresponding data.
          void* data = key_data[i].data;
          key_data[i].data = NULL;

          (*(key_map[i].key_destructor))(data);
          ++called_destructor_count;
        }
      }
    }

    // If we didn't call any destructors, there is no need to check the pthread keys again.
    if (called_destructor_count == 0) {
      break;
    }
  }
}

int pthread_key_create(pthread_key_t* key, void (*key_destructor)(void*)) {
  for (size_t i = 0; i < BIONIC_PTHREAD_KEY_COUNT; ++i) {
    uintptr_t value = atomic_load_explicit(&key_map[i].seq, memory_order_relaxed);
    while (!KEY_IN_USE(value)) {
      if (atomic_compare_exchange_weak_explicit(&key_map[i].seq, &value, value + 1,
                                                memory_order_relaxed, memory_order_relaxed)) {
        key_map[i].key_destructor = key_destructor;
        *key = i;
        return 0;
      }
    }
  }
  return EAGAIN;
}

// Deletes a pthread_key_t. note that the standard mandates that this does
// not call the destructors for non-NULL key values. Instead, it is the
// responsibility of the caller to properly dispose of the corresponding data
// and resources, using any means it finds suitable.
int pthread_key_delete(pthread_key_t key) {
  if (!IsValidKey(key)) {
    return EINVAL;
  }
  // Increase seq to invalidate values in all threads.
  uintptr_t value = atomic_load_explicit(&key_map[key].seq, memory_order_relaxed);
  if (KEY_IN_USE(value) &&
      atomic_compare_exchange_strong_explicit(&key_map[key].seq, &value, value + 1,
                                              memory_order_relaxed, memory_order_relaxed)) {
    return 0;
  }
  return EINVAL;
}

void* pthread_getspecific(pthread_key_t key) {
  if (!IsValidKey(key)) {
    return NULL;
  }
  pthread_key_data_t* data = &(__get_thread()->key_data[key]);
  if (__predict_true(data->seq == atomic_load_explicit(&key_map[key].seq, memory_order_relaxed))) {
    return data->data;
  }
  data->data = NULL;
  return NULL;
}

int pthread_setspecific(pthread_key_t key, const void* ptr) {
  if (!IsValidKey(key)) {
    return EINVAL;
  }
  pthread_key_data_t* data = &(__get_thread()->key_data[key]);
  data->seq = atomic_load_explicit(&key_map[key].seq, memory_order_relaxed);
  data->data = const_cast<void*>(ptr);
  return 0;
}
