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

/* A technical note regarding our thread-local-storage (TLS) implementation:
 *
 * There can be up to BIONIC_TLS_SLOTS independent TLS keys in a given process,
 * The keys below TLS_SLOT_FIRST_USER_SLOT are reserved for Bionic to hold
 * special thread-specific variables like errno or a pointer to
 * the current thread's descriptor. These entries cannot be accessed through
 * pthread_getspecific() / pthread_setspecific() or pthread_key_delete()
 *
 * The 'tls_map_t' type defined below implements a shared global map of
 * currently created/allocated TLS keys and the destructors associated
 * with them.
 *
 * The global TLS map simply contains a bitmap of allocated keys, and
 * an array of destructors.
 *
 * Each thread has a TLS area that is a simple array of BIONIC_TLS_SLOTS void*
 * pointers. the TLS area of the main thread is stack-allocated in
 * __libc_init_common, while the TLS area of other threads is placed at
 * the top of their stack in pthread_create.
 *
 * When pthread_key_delete() is called it will erase the key's bitmap bit
 * and its destructor, and will also clear the key data in the TLS area of
 * all created threads. As mandated by Posix, it is the responsibility of
 * the caller of pthread_key_delete() to properly reclaim the objects that
 * were pointed to by these data fields (either before or after the call).
 */

#define TLSMAP_BITS       32
#define TLSMAP_WORDS      ((BIONIC_PTHREAD_KEY_COUNT + TLSMAP_BITS - 1) / TLSMAP_BITS)
#define TLSMAP_WORD(m,k)  (m).map[(k)/TLSMAP_BITS]
#define TLSMAP_MASK(k)    (1U << ((k)&(TLSMAP_BITS-1)))


typedef void (*key_destructor_t)(void*);

struct pthread_key_map_t {
  atomic_uintptr_t seq;
  key_destructor_t destr;
};

#define KEY_IN_USE(seq) ((seq) & 1)

static pthread_key_map_t key_map[BIONIC_PTHREAD_KEY_COUNT];

static inline bool IsValidKey(pthread_key_t key) {
  return key < BIONIC_PTHREAD_KEY_COUNT &&
         KEY_IN_USE(atomic_load_explicit(&key_map[key].seq, memory_order_relaxed));
}

// Called from pthread_exit() to remove all pthread keys. This must call the destructor of
// all keys that have a non-NULL data value and a non-NULL destructor.
__LIBC_HIDDEN__ void pthread_key_clean_all() {
  // Because destructors can do funky things like deleting/creating other keys,
  // we need to implement this in a loop.
  pthread_key_data_t* key_data = __get_thread()->key_data;
  for (int rounds = PTHREAD_DESTRUCTOR_ITERATIONS; rounds > 0; --rounds) {
    size_t called_destructor_count = 0;
    for (unsigned int i = 0; i < BIONIC_PTHREAD_KEY_COUNT; ++i) {
      uintptr_t seq = atomic_load_explicit(&key_map[i].seq, memory_order_relaxed);
      if (KEY_IN_USE(seq) && key_map[i].destr != NULL) {
        void* data = key_data[i].data;
        if (key_data[i].seq == seq && data != NULL) {
          // We need to clear the key data now, this will prevent the destructor (or a later one)
          // from seeing the old value if it calls pthread_getspecific() for some odd reason.
          // We don't do this if 'key_destructor == NULL' just in case another destructor
          // function might be responsible for manually releasing the corresponding data.
          key_data[i].data = NULL;

          (*(key_map[i].destr))(data);
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

int pthread_key_create(pthread_key_t* key, void (*key_desctructor)(void*)) {
  for (size_t i = 0; i < BIONIC_PTHREAD_KEY_COUNT; ++i) {
    uintptr_t value = atomic_load_explicit(&key_map[i].seq, memory_order_relaxed);
    while (!KEY_IN_USE(value)) {
      if (atomic_compare_exchange_weak_explicit(&key_map[i].seq, &value, value + 1,
                                                memory_order_relaxed, memory_order_relaxed)) {
        key_map[i].destr = key_desctructor;
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
  // Increase seq to invalid values in all threads.
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
