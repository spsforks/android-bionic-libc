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

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

extern int __pthread_cond_timedwait(pthread_cond_t*, pthread_mutex_t*, const timespec*, clockid_t);
extern int __pthread_cond_timedwait_relative(pthread_cond_t*, pthread_mutex_t*, const timespec*);

extern "C" int __timer_create(clockid_t, sigevent*, timer_t*);
extern "C" int __timer_delete(timer_t);
extern "C" int __timer_gettime(timer_t, itimerspec*);
extern "C" int __timer_settime(timer_t, int, const itimerspec*, itimerspec*);
extern "C" int __timer_getoverrun(timer_t);

static void* __timer_thread_start(void*);

// Normal (i.e. non-SIGEV_THREAD) timers are created directly by the kernel
// and are passed as is to/from the caller.
//
// This file also implements the support required for SIGEV_THREAD ("POSIX interval")
// timers. See the following pages for additional details:
//
// www.opengroup.org/onlinepubs/000095399/functions/timer_create.html
// www.opengroup.org/onlinepubs/000095399/functions/timer_settime.html
// www.opengroup.org/onlinepubs/000095399/functions/xsh_chap02_04.html#tag_02_04_01
//
// The Linux kernel doesn't support these, so we need to implement them in the
// C library. We use a very basic scheme where each timer is associated to a
// thread that will loop, waiting for timeouts or messages from the program
// corresponding to calls to timer_settime() and timer_delete().
//
// A SIGEV_THREAD timer ID will always have its TIMER_ID_WRAP_BIT
// set to 1. In this implementation, this is always bit 31, which is
// guaranteed to never be used by kernel-provided timer ids
//
// (See code in <kernel>/lib/idr.c, used to manage IDs, to see why.)

#define  TIMER_ID_WRAP_BIT        0x80000000
#define  TIMER_ID_WRAP(id)        ((timer_t)(((uintptr_t) id) |  TIMER_ID_WRAP_BIT))
#define  TIMER_ID_UNWRAP(id)      ((timer_t)(((uintptr_t) id) & ~TIMER_ID_WRAP_BIT))
#define  TIMER_ID_IS_WRAPPED(id)  ((((uintptr_t) id) & TIMER_ID_WRAP_BIT) != 0)

// This value is used internally to indicate a 'free' or 'zombie'
// thr_timer_t structure. Here, 'zombie' means that timer_delete()
// has been called, but that the corresponding thread hasn't
// exited yet.
#define  TIMER_ID_NONE            ((timer_t)~0)

// True iff a timer id is valid.
#define  TIMER_ID_IS_VALID(id)    ((id) != TIMER_ID_NONE)

// The maximum value of overrun counters.
#define  DELAYTIMER_MAX    0x7fffffff

struct thr_timer_t {
    thr_timer_t*       next;     /* next in free list */

    timer_t            id;       /* TIMER_ID_NONE iff free or dying */
    clockid_t          clock;
    pthread_t          thread;
    pthread_attr_t     thread_attributes;
    void (*callback)(sigval_t);
    sigval_t           value;

    /* the following are used to communicate between
     * the timer thread and the timer_XXX() functions
     */
    pthread_mutex_t           mutex;     /* lock */
    pthread_cond_t            cond;      /* signal a state change to thread */
    volatile bool deleted;
    volatile timespec expires;   /* next expiration time, or 0 */
    volatile timespec period;    /* reload value, or 0 */
    volatile int overruns;  /* current number of overruns */
};

#define  MAX_THREAD_TIMERS  32

struct thr_timer_table_t {
  void Init() {
    pthread_mutex_init(&lock_, NULL);

    for (size_t i = 0; i < MAX_THREAD_TIMERS; ++i) {
      timers_[i].id = TIMER_ID_NONE;
    }

    free_list_head_ = &timers_[0];
    for (size_t i = 1; i < MAX_THREAD_TIMERS; ++i) {
      timers_[i - 1].next = &timers_[i];
    }
  }

  thr_timer_t* AllocateTimer() {
    pthread_mutex_lock(&lock_);
    thr_timer_t* timer = free_list_head_;
    if (timer != NULL) {
      free_list_head_ = timer->next;
      timer->next = NULL;
      timer->id = TIMER_ID_WRAP((timer - timers_));
    }
    pthread_mutex_unlock(&lock_);
    return timer;
  }

  void FreeTimer(thr_timer_t* timer) {
    pthread_mutex_lock(&lock_);
    timer->id = TIMER_ID_NONE;
    timer->thread = 0;
    timer->next = free_list_head_;
    free_list_head_ = timer;
    pthread_mutex_unlock(&lock_);
  }

  // Converts a timer_id into the corresponding thr_timer_t* pointer.
  // Returns NULL if the id is not wrapped or is invalid/free.
  thr_timer_t* thr_timer_table_from_id(timer_t id, int remove) {
    if (!TIMER_ID_IS_WRAPPED(id)) {
      return NULL;
    }

    // TODO: timer_t should just be a pointer.
    size_t index = reinterpret_cast<size_t>(TIMER_ID_UNWRAP(id));
    if (index >= MAX_THREAD_TIMERS) {
      return NULL;
    }

    pthread_mutex_lock(&lock_);

    thr_timer_t* timer = &timers_[index];

    if (!TIMER_ID_IS_VALID(timer->id)) {
      timer = NULL;
    } else {
      // If we're removing this timer, clear the id
      // right now to prevent another thread to
      // use the same id after the unlock.
      if (remove) {
        timer->id = TIMER_ID_NONE;
      }
    }
    pthread_mutex_unlock(&lock_);

    return timer;
  }

 private:
  pthread_mutex_t lock_;
  thr_timer_t* free_list_head_;
  thr_timer_t timers_[MAX_THREAD_TIMERS];
};

// The static timer table - we only create it if the process
// really wants to use SIGEV_THREAD timers, which should be
// pretty infrequent.
static pthread_once_t __timer_table_once = PTHREAD_ONCE_INIT;
static thr_timer_table_t* __timer_table;

static void __timer_table_init() {
  __timer_table = reinterpret_cast<thr_timer_table_t*>(calloc(1, sizeof(*__timer_table)));
  if (__timer_table != NULL) {
    __timer_table->Init();
  }
}

static void __timer_table_reset_atfork() {
  __timer_table_init();
}

static void __timer_table_init_once() {
  __timer_table_init();
  pthread_atfork(NULL, NULL, __timer_table_reset_atfork);
}

static thr_timer_table_t* __timer_table_get() {
  pthread_once(&__timer_table_once, __timer_table_init_once);
  return __timer_table;
}

static thr_timer_t* thr_timer_from_id(timer_t id) {
  thr_timer_table_t*  table = __timer_table_get();
  if (table == NULL) {
    return NULL;
  }
  return table->thr_timer_table_from_id(id, 0);
}


static __inline__ void
thr_timer_lock( thr_timer_t*  t )
{
    pthread_mutex_lock(&t->mutex);
}

static __inline__ void
thr_timer_unlock( thr_timer_t*  t )
{
    pthread_mutex_unlock(&t->mutex);
}


static __inline__ void timespec_add(timespec* a, const timespec* b) {
  a->tv_sec  += b->tv_sec;
  a->tv_nsec += b->tv_nsec;
  if (a->tv_nsec >= 1000000000) {
    a->tv_nsec -= 1000000000;
    a->tv_sec  += 1;
  }
}

static __inline__ void timespec_sub(timespec* a, const timespec* b) {
  a->tv_sec  -= b->tv_sec;
  a->tv_nsec -= b->tv_nsec;
  if (a->tv_nsec < 0) {
    a->tv_nsec += 1000000000;
    a->tv_sec  -= 1;
  }
}

static __inline__ void timespec_zero(timespec* a) {
  a->tv_sec = a->tv_nsec = 0;
}

static __inline__ int timespec_is_zero(const timespec* a) {
  return (a->tv_sec == 0 && a->tv_nsec == 0);
}

static __inline__ int timespec_cmp(const timespec* a, const timespec* b) {
  if (a->tv_sec  < b->tv_sec)  return -1;
  if (a->tv_sec  > b->tv_sec)  return +1;
  if (a->tv_nsec < b->tv_nsec) return -1;
  if (a->tv_nsec > b->tv_nsec) return +1;
  return 0;
}

static __inline__ int timespec_cmp0(const timespec* a) {
  if (a->tv_sec < 0) return -1;
  if (a->tv_sec > 0) return +1;
  if (a->tv_nsec < 0) return -1;
  if (a->tv_nsec > 0) return +1;
  return 0;
}

int timer_create(clockid_t clock_id, sigevent* evp, timer_t* timer_id) {
  // If not a SIGEV_THREAD timer, the kernel can handle it without our help.
  if (evp == NULL || evp->sigev_notify != SIGEV_THREAD) {
    return __timer_create(clock_id, evp, timer_id);
  }

  // Check arguments.
  if (evp->sigev_notify_function == NULL) {
    errno = EINVAL;
    return -1;
  }

  // Check that the clock id is supported by the kernel.
  timespec dummy;
  if (clock_gettime(clock_id, &dummy) < 0 && errno == EINVAL) {
    return -1;
  }

  // Create a new timer and its thread.
  // TODO: use a single global thread for all timers.
  thr_timer_table_t* table = __timer_table_get();
  if (table == NULL) {
    errno = ENOMEM;
    return -1;
  }
  thr_timer_t* timer = table->AllocateTimer();
  if (timer == NULL) {
    errno = ENOMEM;
    return -1;
  }

  // Copy the thread attributes.
  if (evp->sigev_notify_attributes == NULL) {
    pthread_attr_init(&timer->thread_attributes);
  } else {
    timer->thread_attributes = ((pthread_attr_t*) evp->sigev_notify_attributes)[0];
  }

  // Posix says that the default is PTHREAD_CREATE_DETACHED and
  // that PTHREAD_CREATE_JOINABLE has undefined behavior.
  // So simply always use DETACHED :-)
  pthread_attr_setdetachstate(&timer->thread_attributes, PTHREAD_CREATE_DETACHED);

  timer->callback = evp->sigev_notify_function;
  timer->value = evp->sigev_value;
  timer->clock = clock_id;

  pthread_mutex_init(&timer->mutex, NULL);
  pthread_cond_init(&timer->cond, NULL);

  timer->deleted = false;
  timer->expires.tv_sec = timer->expires.tv_nsec = 0;
  timer->period.tv_sec = timer->period.tv_nsec  = 0;
  timer->overruns = 0;

  // Create the thread.
  int rc = pthread_create(&timer->thread, &timer->thread_attributes, __timer_thread_start, timer);
  if (rc != 0) {
    table->FreeTimer(timer);
    errno = rc;
    return -1;
  }

  *timer_id = timer->id;
  return 0;
}


int timer_delete(timer_t id) {
  if (__predict_true(!TIMER_ID_IS_WRAPPED(id))) {
    return __timer_delete(id);
  }

  thr_timer_table_t*  table = __timer_table_get();
  if (table == NULL) {
    errno = EINVAL;
    return -1;
  }
  thr_timer_t* timer = table->thr_timer_table_from_id(id, 1);
  if (timer == NULL) {
    errno = EINVAL;
    return -1;
  }

  // Tell the timer's thread to stop.
  thr_timer_lock(timer);
  timer->deleted = true;
  pthread_cond_signal( &timer->cond );
  thr_timer_unlock(timer);

  // NOTE: the thread will call __timer_table_free() to free the
  // timer object. the '1' parameter to thr_timer_table_from_id
  // above ensured that the object and its timer_id cannot be
  // reused before that.
  return 0;
}

/* return the relative time until the next expiration, or 0 if
 * the timer is disarmed */
static void timer_gettime_internal(thr_timer_t* timer, itimerspec* spec) {
  timespec diff = const_cast<timespec&>(timer->expires);
  if (!timespec_is_zero(&diff)) {
    timespec now;

    clock_gettime(timer->clock, &now);
    timespec_sub(&diff, &now);

    /* in case of overrun, return 0 */
    if (timespec_cmp0(&diff) < 0) {
      timespec_zero(&diff);
    }
  }

  spec->it_value = diff;
  spec->it_interval = const_cast<timespec&>(timer->period);
}


int timer_gettime(timer_t id, itimerspec* ospec) {
    if (ospec == NULL) {
        errno = EINVAL;
        return -1;
    }

    if ( __predict_true(!TIMER_ID_IS_WRAPPED(id)) ) {
        return __timer_gettime( id, ospec );
    } else {
        thr_timer_t*  timer = thr_timer_from_id(id);

        if (timer == NULL) {
            errno = EINVAL;
            return -1;
        }
        thr_timer_lock(timer);
        timer_gettime_internal( timer, ospec );
        thr_timer_unlock(timer);
    }
    return 0;
}


int
timer_settime(timer_t id, int flags, const itimerspec* spec, itimerspec* ospec) {
    if (spec == NULL) {
        errno = EINVAL;
        return -1;
    }

    if ( __predict_true(!TIMER_ID_IS_WRAPPED(id)) ) {
        return __timer_settime( id, flags, spec, ospec );
    } else {
        thr_timer_t*        timer = thr_timer_from_id(id);
        timespec     expires, now;

        if (timer == NULL) {
            errno = EINVAL;
            return -1;
        }
        thr_timer_lock(timer);

        /* return current timer value if ospec isn't NULL */
        if (ospec != NULL) {
            timer_gettime_internal(timer, ospec );
        }

        /* compute next expiration time. note that if the
         * new it_interval is 0, we should disarm the timer
         */
        expires = spec->it_value;
        if (!timespec_is_zero(&expires)) {
            clock_gettime( timer->clock, &now );
            if (!(flags & TIMER_ABSTIME)) {
                timespec_add(&expires, &now);
            } else {
                if (timespec_cmp(&expires, &now) < 0)
                    expires = now;
            }
        }
        const_cast<timespec&>(timer->expires) = expires;
        const_cast<timespec&>(timer->period) = spec->it_interval;
        thr_timer_unlock( timer );

        /* signal the change to the thread */
        pthread_cond_signal( &timer->cond );
    }
    return 0;
}


int
timer_getoverrun(timer_t  id)
{
    if ( __predict_true(!TIMER_ID_IS_WRAPPED(id)) ) {
        return __timer_getoverrun( id );
    } else {
        thr_timer_t*  timer = thr_timer_from_id(id);
        int           result;

        if (timer == NULL) {
            errno = EINVAL;
            return -1;
        }

        thr_timer_lock(timer);
        result = timer->overruns;
        thr_timer_unlock(timer);

        return result;
    }
}


static void* __timer_thread_start(void* arg) {
  thr_timer_t* timer = reinterpret_cast<thr_timer_t*>(arg);

  thr_timer_lock(timer);

  // Give this thread a meaningful name.
  char name[64];
  snprintf(name, sizeof(name), "POSIX interval timer %p", timer->id);
  pthread_setname_np(pthread_self(), name);

  while (!timer->deleted) {
    timespec expires = const_cast<timespec&>(timer->expires);
    timespec period = const_cast<timespec&>(timer->period);

    // If the timer is disarmed, wait indefinitely
    // for a state change from timer_settime/timer_delete.
    if (timespec_is_zero(&expires)) {
      pthread_cond_wait(&timer->cond, &timer->mutex);
      continue;
    }

    // Otherwise, we need to do a timed wait until either a
    // state change of the timer expiration time.
    timespec now;
    clock_gettime(timer->clock, &now);

    if (timespec_cmp(&expires, &now) > 0) {
      // Cool, there was no overrun, so compute the
      // relative timeout as 'expires - now', then wait.
      timespec diff = expires;
      timespec_sub(&diff, &now);

      int ret = __pthread_cond_timedwait_relative(&timer->cond, &timer->mutex, &diff);

      // If we didn't time out, it means that a state change
      // occurred, so loop to take care of it.
      if (ret != ETIMEDOUT) {
        continue;
      }
    } else {
      // Overrun was detected before we could wait!
      if (!timespec_is_zero(&period)) {
        // For periodic timers, compute total overrun count.
        do {
          timespec_add(&expires, &period);
          if (timer->overruns < DELAYTIMER_MAX) {
            timer->overruns += 1;
          }
        } while (timespec_cmp(&expires, &now) < 0);

        // Backtrack the last one, because we're going to
        // add the same value just a bit later.
        timespec_sub(&expires, &period);
      } else {
        // For non-periodic timers, things are simple.
        timer->overruns = 1;
      }
    }

    // If we get here, a timeout was detected.
    // First reload/disarm the timer as needed.
    if (!timespec_is_zero(&period)) {
      timespec_add(&expires, &period);
    } else {
      timespec_zero(&expires);
    }
    const_cast<timespec&>(timer->expires) = expires;

    // Now call the timer callback function. Release the
    // lock to allow the function to modify the timer setting
    // or call timer_getoverrun().
    // NOTE: at this point we trust the callback not to be a
    //      total moron and pthread_kill() the timer thread
    thr_timer_unlock(timer);
    timer->callback(timer->value);
    thr_timer_lock(timer);

    // Now clear the overruns counter. it only makes sense
    // within the callback.
    timer->overruns = 0;
  }

  thr_timer_unlock(timer);

  // Free the timer object.
  __timer_table_get()->FreeTimer(timer);

  return NULL;
}
