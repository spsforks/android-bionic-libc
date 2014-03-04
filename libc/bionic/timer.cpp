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

#include "pthread_internal.h"
#include "private/bionic_pthread.h"
#include "private/kernel_sigset_t.h"

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>




//for debugging
#include <unistd.h>
#include "private/libc_logging.h"
//for debugging






// System calls.
extern "C" int __timer_create(clockid_t, sigevent*, __kernel_timer_t*);
extern "C" int __timer_delete(__kernel_timer_t);
extern "C" int __timer_gettime(__kernel_timer_t, itimerspec*);
extern "C" int __timer_settime(__kernel_timer_t, int, const itimerspec*, itimerspec*);
extern "C" int __timer_getoverrun(__kernel_timer_t);
extern "C" int __rt_sigtimedwait(const sigset_t*, siginfo_t*, const struct timespec*, size_t);

// Most POSIX timers are handled directly by the kernel. We translate SIGEV_THREAD timers
// into SIGEV_THREAD_ID timers so the kernel handles all the time-related stuff and we just
// need to worry about running user code on a thread.

struct timer_internal_t {
  __kernel_timer_t kernel_timer_id;

  int sigev_notify;

  // These fields are only needed for a SIGEV_THREAD timer.
  pthread_t callback_thread;
  void (*callback)(sigval_t);
  sigval_t callback_argument;
};

static __kernel_timer_t kernel_timer_id(timer_t timer) {
  return reinterpret_cast<timer_internal_t*>(timer)->kernel_timer_id;
}

static void* __timer_thread_start(void* arg) {
  timer_internal_t* timer = reinterpret_cast<timer_internal_t*>(arg);

  kernel_sigset_t sigset;
  sigaddset(sigset.get(), SIGALRM);

  kernel_sigset_t old_sigset;
  pthread_sigmask(SIG_BLOCK, sigset.get(), old_sigset.get());

  while (true) {
    // wait for a signal...
    siginfo_t si;
    memset(&si, 0, sizeof(si));
    int rc = __rt_sigtimedwait(sigset.get(), &si, NULL, sizeof(sigset));
//    __libc_format_log(ANDROID_LOG_INFO, "__timer_thread_start", "__rt_sigtimedwait %d si.si_code=%d", rc, si.si_code);

    // TODO: recognize when we should die!

    // call the callback...
//    __libc_format_log(ANDROID_LOG_INFO, "__timer_thread_start", "callback %p(%p)", timer->callback, &timer->callback_argument);
    timer->callback(timer->callback_argument);
  }

  pthread_sigmask(SIG_SETMASK, old_sigset.get(), NULL);

  return NULL;
}

// http://pubs.opengroup.org/onlinepubs/9699919799/functions/timer_create.html
int timer_create(clockid_t clock_id, sigevent* evp, timer_t* timer_id) {
  timer_internal_t* new_timer = reinterpret_cast<timer_internal_t*>(malloc(sizeof(timer_internal_t)));
  if (new_timer == NULL) {
    return -1;
  }

  new_timer->sigev_notify = (evp == NULL) ? SIGEV_SIGNAL : evp->sigev_notify;

  // If not a SIGEV_THREAD timer, the kernel can handle it without our help.
  if (new_timer->sigev_notify != SIGEV_THREAD) {
    if (__timer_create(clock_id, evp, &new_timer->kernel_timer_id) == -1) {
      free(new_timer);
      return -1;
    }

    *timer_id = new_timer;
    return 0;
  }

  // Otherwise, this must be SIGEV_THREAD timer...
  sigevent se = *evp;
  new_timer->callback = se.sigev_notify_function;
  new_timer->callback_argument = se.sigev_value;

  // Check arguments that the kernel doesn't care about but we do.
  if (se.sigev_notify_function == NULL) {
    free(new_timer);
    errno = EINVAL;
    return -1;
  }

  // Create this timer's thread.
  pthread_attr_t thread_attributes;
  if (evp->sigev_notify_attributes == NULL) {
    pthread_attr_init(&thread_attributes);
  } else {
    thread_attributes = *reinterpret_cast<pthread_attr_t*>(evp->sigev_notify_attributes);
  }
  pthread_attr_setdetachstate(&thread_attributes, PTHREAD_CREATE_DETACHED);
  int rc = pthread_create(&new_timer->callback_thread, &thread_attributes, __timer_thread_start, new_timer);
  if (rc != 0) {
    free(new_timer);
    errno = rc;
    return -1;
  }

  // Give the thread a meaningful name.
  char name[32];
  snprintf(name, sizeof(name), "POSIX interval timer %d", new_timer->kernel_timer_id);
  pthread_setname_np(new_timer->callback_thread, name);

  se.sigev_signo = SIGALRM;
  se.sigev_notify = SIGEV_THREAD_ID;
  se.sigev_notify_thread_id = __pthread_gettid(new_timer->callback_thread);
  se.sigev_value.sival_ptr = new_timer;
  if (__timer_create(clock_id, &se, &new_timer->kernel_timer_id) == -1) {
    // TODO: tell the thread to exit as if via timer_delete.
    __libc_format_log(ANDROID_LOG_INFO, "timer_create", "timer_create SIGEV_THREAD doing pthread_kill... (%s)", strerror(errno));
    pthread_kill(new_timer->callback_thread, SIGALRM);
    free(new_timer);
    return -1;
  }

  __libc_format_log(ANDROID_LOG_INFO, "timer_create", "timer_create SIGEV_THREAD timer_id=%d thread_id=%d", new_timer->kernel_timer_id, __pthread_gettid(new_timer->callback_thread));

  // TODO: wait for the thread to start without sleeping! (or start it with SIGALRM already blocked?)
  sleep(1);

  *timer_id = new_timer;
  return 0;
}

// http://pubs.opengroup.org/onlinepubs/9699919799/functions/timer_delete.html
int timer_delete(timer_t id) {
  int rc = __timer_delete(kernel_timer_id(id));
  if (rc == -1) {
      return -1;
  }

  timer_internal_t* timer = reinterpret_cast<timer_internal_t*>(id);
  if (timer->sigev_notify == SIGEV_THREAD) {
    // TODO: signal the thread to stop.
    free(timer);
  }
  return 0;
}

// http://pubs.opengroup.org/onlinepubs/9699919799/functions/timer_getoverrun.html
int timer_gettime(timer_t id, itimerspec* ts) {
  return __timer_gettime(kernel_timer_id(id), ts);
}

// http://pubs.opengroup.org/onlinepubs/9699919799/functions/timer_getoverrun.html
int timer_settime(timer_t id, int flags, const itimerspec* ts, itimerspec* ots) {
  return __timer_settime(kernel_timer_id(id), flags, ts, ots);
}

// http://pubs.opengroup.org/onlinepubs/9699919799/functions/timer_getoverrun.html
int timer_getoverrun(timer_t id) {
  return __timer_getoverrun(kernel_timer_id(id));
}
