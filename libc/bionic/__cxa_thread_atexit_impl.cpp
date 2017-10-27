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
#include <sys/cdefs.h>

#include <private/bionic_defs.h>

#include "async_safe/log.h"
#include "pthread_internal.h"
#include "thread_local_dtor.h"

extern "C" int __cxa_thread_atexit_impl(void (*func) (void *), void *arg, void *dso_handle);

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int __cxa_thread_atexit_impl(void (*func) (void *), void *arg, void *dso_handle) {
  thread_local_dtor* dtor = new thread_local_dtor();

  dtor->func = func;
  dtor->arg = arg;
  dtor->dso_handle = dso_handle;

  pthread_internal_t* thread = __get_thread();
  dtor->next = thread->thread_local_dtors;
  thread->thread_local_dtors = dtor;

  // The linker needs to know that the DSO has thread local destructors because
  // it can't unload a dlclose'd library until the destructors have run,
  // otherwise the destructor jumps to arbitrary.
  if (dl_add_thread_local_dtor == nullptr) {
    async_safe_fatal("dl_add_thread_local_dtor is null");
  }

  if (!dl_add_thread_local_dtor(dso_handle)) {
    async_safe_fatal("Could not find DSO containing %p", dso_handle);
  }
  return 0;
}

extern "C" __LIBC_HIDDEN__ void __cxa_thread_finalize() {
  pthread_internal_t* thread = __get_thread();
  while (thread->thread_local_dtors != nullptr) {
    thread_local_dtor* current = thread->thread_local_dtors;
    thread->thread_local_dtors = current->next;

    current->func(current->arg);

    if (dl_remove_thread_local_dtor == nullptr) {
      async_safe_fatal("dl_remove_thread_local_dtor is null");
    }

    // If all the threads with thread local destructors have already joined,
    // dlclose can unload the library without issues.
    if (!dl_remove_thread_local_dtor(current->dso_handle)) {
      async_safe_fatal("Could not find DSO containing %p", current->dso_handle);
    }
    delete current;
  }
}
