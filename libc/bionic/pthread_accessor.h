/*
 * Copyright (C) 2013 The Android Open Source Project
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

#ifndef PTHREAD_ACCESSOR_H
#define PTHREAD_ACCESSOR_H

#include <pthread.h>
#include <stdatomic.h>

#include "private/bionic_macros.h"
#include "pthread_internal.h"

class pthread_accessor {
 public:
  explicit pthread_accessor(pthread_t t) {
    thread_ = reinterpret_cast<pthread_internal_t*>(t);
    tid_ = atomic_load(&thread_->tid_);
    if (tid_ == 0) {
      // The kernel thinks we're dead.
      thread_ = NULL;
    }
  }

  pthread_internal_t& operator*() const { return *thread_; }
  pthread_internal_t* operator->() const { return thread_; }
  pthread_internal_t* get() const { return thread_; }

  pid_t tid() {
    return tid_;
  }

 private:
  pthread_internal_t* thread_;
  pid_t tid_;

  DISALLOW_COPY_AND_ASSIGN(pthread_accessor);
};

#endif // PTHREAD_ACCESSOR_H
