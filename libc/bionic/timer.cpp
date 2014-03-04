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
  sigval_t callback_argument; // TODO: can't we just take the one the kernel hands us?
};

static __kernel_timer_t kernel_timer_id(timer_t timer) {
  return reinterpret_cast<timer_internal_t*>(timer)->kernel_timer_id;
}

static void* __timer_thread_start(void* arg) {
  timer_internal_t* timer = reinterpret_cast<timer_internal_t*>(arg);

  kernel_sigset_t sigset;
  sigaddset(sigset.get(), SIGALRM);

  while (true) {
    // Wait for a signal...
    siginfo_t si;
    memset(&si, 0, sizeof(si));
    int rc = __rt_sigtimedwait(sigset.get(), &si, NULL, sizeof(sigset));
    if (rc == -1) {
      continue;
    }

    if (si.si_code == SI_TIMER) {
      // Signals sent because a timer fired have code SI_TIMER.
      timer->callback(timer->callback_argument);
    } else if (si.si_code == SI_TKILL) {
      // Signals sent because we're being asked to exit have code SI_TKILL.
      return NULL;
    }
  }
}

static void __timer_thread_stop(timer_internal_t* timer) {
  // We detach the timer's thread so we can join it.
  // That way our callers know that the thread has already exited when we return.
  pthread_detach(timer->callback_thread);
  pthread_kill(timer->callback_thread, SIGALRM);
  pthread_join(timer->callback_thread, NULL);
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

  // We start the thread with SIGALRM blocked, in case it starts receiving
  // signals before it's had a change to block SIGALRM itself.
  kernel_sigset_t sigset;
  sigaddset(sigset.get(), SIGALRM);
  kernel_sigset_t old_sigset;
  pthread_sigmask(SIG_BLOCK, sigset.get(), old_sigset.get());

  int rc = pthread_create(&new_timer->callback_thread, &thread_attributes, __timer_thread_start, new_timer);

  pthread_sigmask(SIG_SETMASK, old_sigset.get(), NULL);

  if (rc != 0) {
    free(new_timer);
    errno = rc;
    return -1;
  }

  se.sigev_signo = SIGALRM;
  se.sigev_notify = SIGEV_THREAD_ID;
  se.sigev_notify_thread_id = __pthread_gettid(new_timer->callback_thread);
  se.sigev_value.sival_ptr = new_timer; // TODO: if we leave this unmolested, we don't need a copy in timer_internal_t.
  if (__timer_create(clock_id, &se, &new_timer->kernel_timer_id) == -1) {
    __timer_thread_stop(new_timer);
    free(new_timer);
    return -1;
  }

  // Give the thread a meaningful name.
  // It can't do this itself because the kernel timer isn't created until after it's running.
  char name[32];
  snprintf(name, sizeof(name), "POSIX interval timer %d", new_timer->kernel_timer_id);
  pthread_setname_np(new_timer->callback_thread, name);

  __libc_format_log(ANDROID_LOG_INFO, "__timer_thread_start", "\"%s\" started...", name);

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
    __timer_thread_stop(timer);
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
