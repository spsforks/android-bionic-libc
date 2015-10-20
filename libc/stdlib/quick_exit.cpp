/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <pthread.h>
#include <stdlib.h>
#include <sys/types.h>
#include "private/ScopedPthreadMutexLocker.h"
#include "private/bionic_globals.h"
#include "private/WPVector.h"

static pthread_mutex_t quick_exit_mutex = PTHREAD_MUTEX_INITIALIZER;

void __libc_init_at_quick_exit(libc_globals* globals) {
  globals->at_quick_exit =
    WPVector<at_quick_exit_t>::allocate("at_quick_exit handlers");
}

int at_quick_exit(at_quick_exit_t func) {
  ScopedPthreadMutexLocker locker(&quick_exit_mutex);
  __libc_globals->at_quick_exit->mutate().push_back(func);
  return 0;
}

__attribute__((noreturn)) void quick_exit(int exit_code) {
  ScopedPthreadMutexLocker locker(&quick_exit_mutex);
  auto vec = __libc_globals->at_quick_exit->mutate();
  for (size_t i = vec.size() - 1; i < SIZE_MAX; --i) {
  restart:
    if (!vec[i]) {
      continue;
    }

    at_quick_exit_t fn = vec[i];
    vec[i] = nullptr;

    size_t orig_size = vec.size();
    locker.unlock();
    fn();
    locker.lock();

    if (orig_size != vec.size()) {
      // The handler added additional handlers, restart the loop.
      i = vec.size() - 1;
      goto restart;
    }
  }

  _Exit(exit_code);
}
