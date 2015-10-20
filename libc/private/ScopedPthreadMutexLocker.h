/*
 * Copyright (C) 2012 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SCOPED_PTHREAD_MUTEX_LOCKER_H
#define SCOPED_PTHREAD_MUTEX_LOCKER_H

#include <pthread.h>

#include "bionic_macros.h"
#include "libc_logging.h"

class ScopedPthreadMutexLocker {
 public:
  explicit ScopedPthreadMutexLocker(pthread_mutex_t* mu) : mu_(mu) {
    lock();
  }

  ~ScopedPthreadMutexLocker() {
    if (locked_) {
      unlock();
    }
  }

  void lock() {
    if (__predict_false(locked_)) {
      __libc_fatal("ScopedPthreadMutexLocker locked while already locked");
    }
    pthread_mutex_lock(mu_);
    locked_ = true;
  }

  void unlock() {
    if (__predict_false(!locked_)) {
      __libc_fatal("ScopedPthreadMutexLocker unlocked while already unlocked");
    }
    locked_ = false;
    pthread_mutex_unlock(mu_);
  }

 private:
  pthread_mutex_t* mu_;
  bool locked_ = false;

  DISALLOW_COPY_AND_ASSIGN(ScopedPthreadMutexLocker);
};

#endif // SCOPED_PTHREAD_MUTEX_LOCKER_H
