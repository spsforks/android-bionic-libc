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

#ifndef _PTHREAD_INTERNAL_H_
#define _PTHREAD_INTERNAL_H_

#include <pthread.h>
#include <stdatomic.h>

struct pthread_internal_t {
  pid_t* tid_address() {
    return reinterpret_cast<pid_t*>(&tid_);
  }

  pid_t tid() {
    return atomic_load(&tid_);
  }

  void set_tid(pid_t tid) {
    atomic_store(&tid_, tid);
  }

  pid_t invalidate_cached_pid() {
    pid_t old_value;
    get_cached_pid(&old_value);
    set_cached_pid(0);
    return old_value;
  }

  void set_cached_pid(pid_t value) {
    cached_pid_ = value;
  }

  bool get_cached_pid(pid_t* cached_pid) {
    *cached_pid = cached_pid_;
    return (*cached_pid != 0);
  }

  void destroy();

 private:
#if !defined(__LP64__)
  void* __cruft0;
  void* __cruft1;
#endif
  _Atomic(pid_t) tid_;
  pid_t cached_pid_;

 public:

  void** tls;

  atomic_int flags;
  pthread_attr_t attr;

  __pthread_cleanup_t* cleanup_stack;

  void* (*start_routine)(void*);
  void* start_routine_arg;
  void* return_value;

  void* alternate_signal_stack;

  pthread_mutex_t startup_handshake_mutex;
  pthread_mutex_t flags_mutex;

  /*
   * The dynamic linker implements dlerror(3), which makes it hard for us to implement this
   * per-thread buffer by simply using malloc(3) and free(3).
   */
#define __BIONIC_DLERROR_BUFFER_SIZE 512
  char dlerror_buffer[__BIONIC_DLERROR_BUFFER_SIZE];
};

__LIBC_HIDDEN__ int __init_thread(pthread_internal_t*);
__LIBC_HIDDEN__ void __init_tls(pthread_internal_t*);
__LIBC_HIDDEN__ void __init_alternate_signal_stack(pthread_internal_t*);

__LIBC_HIDDEN__ void __pthread_key_clean_all();

/* Has the thread been detached by a pthread_join or pthread_detach call? */
#define PTHREAD_ATTR_FLAG_DETACHED 0x00000001

/* Was the thread's stack allocated by the user rather than by us? */
#define PTHREAD_ATTR_FLAG_USER_ALLOCATED_STACK 0x00000002

/* Has the thread been joined by another thread? */
#define PTHREAD_ATTR_FLAG_JOINED 0x00000004

/* Is this the main thread? */
#define PTHREAD_ATTR_FLAG_MAIN_THREAD 0x80000000

/*
 * Traditionally we give threads a 1MiB stack. When we started
 * allocating per-thread alternate signal stacks to ease debugging of
 * stack overflows, we subtracted the same amount we were using there
 * from the default thread stack size. This should keep memory usage
 * roughly constant.
 */
#define PTHREAD_STACK_SIZE_DEFAULT ((1 * 1024 * 1024) - SIGSTKSZ)

__LIBC_HIDDEN__ int __timespec_from_absolute(timespec*, const timespec*, clockid_t);

/* Needed by fork. */
__LIBC_HIDDEN__ extern void __bionic_atfork_run_prepare();
__LIBC_HIDDEN__ extern void __bionic_atfork_run_child();
__LIBC_HIDDEN__ extern void __bionic_atfork_run_parent();

#endif /* _PTHREAD_INTERNAL_H_ */
