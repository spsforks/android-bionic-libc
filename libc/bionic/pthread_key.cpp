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

#include "private/bionic_tls.h"
#include "pthread_internal.h"

// There are BIONIC_TLS_SLOTS valid TLS keys in a given process.
// The keys below TLS_SLOT_FIRST_USER_SLOT are reserved for Bionic to hold
// special thread-specific variables such as errno, and cannot be accessed through
// pthread_getspecific(3), pthread_setspecific(3), pthread_key_delete(3).

// Each thread has a TLS area that is a simple array of void pointers.
// The TLS area of the main thread is stack-allocated in
// __libc_init_common, while the TLS area of other threads is placed at
// the top of their stack in pthread_create.

static inline bool IsValidUserKey(pthread_key_t key) {
  return (key >= TLS_SLOT_FIRST_USER_SLOT && key < BIONIC_TLS_SLOTS);
}

struct keys {
  typedef void (*destructor_t)(void*);

  keys() {
  }

  bool valid(pthread_key_t key) {
    return IsValidUserKey(key) && atomic_load(&in_use_[key]);
  }

  bool create_key(pthread_key_t key, void (*key_destructor)(void*)) {
    bool expected = false;
    if (!atomic_compare_exchange_strong(&in_use_[key], &expected, true)) {
      return false;
    }
    destructor_[key] = key_destructor;
    return true;
  }

  int delete_key(pthread_key_t key) {
    if (!IsValidUserKey(key)) {
      return EINVAL;
    }

    bool expected = true;
    return atomic_compare_exchange_strong(&in_use_[key], &expected, false) ? 0 : EINVAL;
  }

  destructor_t destructor(pthread_key_t key) {
    return destructor_[key];
  }

 private:
  atomic_bool in_use_[BIONIC_TLS_SLOTS];
  destructor_t destructor_[BIONIC_TLS_SLOTS];

  DISALLOW_COPY_AND_ASSIGN(keys);
};

static keys g_keys;

// Called from pthread_exit(3) to remove all TLS key data
// from this thread's TLS area. This must call the destructor of all keys
// that have a non-NULL data value and a non-NULL destructor.
__LIBC_HIDDEN__ void __pthread_key_clean_all() {
  void** tls = __get_tls();

  // Because destructors can do funky things like deleting/creating other
  // keys, we need to implement this in a loop.
  for (int rounds = PTHREAD_DESTRUCTOR_ITERATIONS; rounds > 0; --rounds) {
    size_t called_destructor_count = 0;
    for (pthread_key_t key = 0; key < BIONIC_TLS_SLOTS; ++key) {
      if (g_keys.valid(key)) {
        void* key_data = tls[key];
        keys::destructor_t key_destructor = g_keys.destructor(key);

        if (key_data != NULL && key_destructor != NULL) {
          // We need to clear the key data now to prevent the destructor (or a later one)
          // from seeing the old value if it calls pthread_getspecific(3).

          // We do not do this if 'key_destructor == NULL' in case another
          // destructor function is responsible for manually releasing the data.
          tls[key] = NULL;

          key_destructor(key_data);
          ++called_destructor_count;
        }
      }
    }

    // If we didn't call any destructors, there is no need to check the TLS data again.
    if (called_destructor_count == 0) {
      break;
    }
  }
}

int pthread_key_create(pthread_key_t* key_out, void (*key_destructor)(void*)) {
  // Take the first unallocated key.
  for (pthread_key_t key = TLS_SLOT_FIRST_USER_SLOT; key < BIONIC_TLS_SLOTS; ++key) {
    if (g_keys.create_key(key, key_destructor)) {
      *key_out = key;
      return 0;
    }
  }
  // All keys are in use. POSIX says EAGAIN for this case.
  return EAGAIN;
}

int pthread_key_delete(pthread_key_t key) {
  return g_keys.delete_key(key);
}

void* pthread_getspecific(pthread_key_t key) {
  if (!g_keys.valid(key)) {
    return NULL;
  }

  return __get_tls()[key];
}

int pthread_setspecific(pthread_key_t key, const void* ptr) {
  if (!g_keys.valid(key)) {
    return EINVAL;
  }

  __get_tls()[key] = const_cast<void*>(ptr);
  return 0;
}
