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
#include <fcntl.h>
#include <limits.h>
#include <linux/uio.h>  // For UIO_MAXIOV.
#include <pthread.h>
#include <stdio.h>  // For FOPEN_MAX.
#include <sys/auxv.h>
#include <sys/sysconf.h>
#include <sys/sysinfo.h>
#include <time.h>
#include <unistd.h>

#include "private/bionic_tls.h"

static int __sysconf_monotonic_clock() {
  timespec t;
  int rc = clock_getres(CLOCK_MONOTONIC, &t);
  return (rc == -1) ? -1 : _POSIX_VERSION;
}

long sysconf(int name) {
  switch (name) {
    case _SC_ARG_MAX:           return ARG_MAX;
    case _SC_BC_BASE_MAX:       return _POSIX2_BC_BASE_MAX;   // Minimum requirement.
    case _SC_BC_DIM_MAX:        return _POSIX2_BC_DIM_MAX;    // Minimum requirement.
    case _SC_BC_SCALE_MAX:      return _POSIX2_BC_SCALE_MAX;  // Minimum requirement.
    case _SC_BC_STRING_MAX:     return _POSIX2_BC_STRING_MAX; // Minimum requirement.
    case _SC_CHILD_MAX:         return CHILD_MAX;
    case _SC_CLK_TCK:           return static_cast<long>(getauxval(AT_CLKTCK));
    case _SC_COLL_WEIGHTS_MAX:  return _POSIX2_COLL_WEIGHTS_MAX;  // Minimum requirement.
    case _SC_EXPR_NEST_MAX:     return _POSIX2_EXPR_NEST_MAX;     // Minimum requirement.
    case _SC_LINE_MAX:          return _POSIX2_LINE_MAX;          // Minimum requirement.
    case _SC_NGROUPS_MAX:       return NGROUPS_MAX;
    case _SC_OPEN_MAX:          return OPEN_MAX;
    case _SC_PASS_MAX:          return PASS_MAX;
    case _SC_2_C_BIND:          return _POSIX2_C_BIND;
    case _SC_2_C_DEV:           return _POSIX2_C_DEV;
    case _SC_2_CHAR_TERM:       return _POSIX2_CHAR_TERM;
    case _SC_2_FORT_DEV:        return _POSIX2_FORT_DEV;
    case _SC_2_FORT_RUN:        return _POSIX2_FORT_RUN;
    case _SC_2_LOCALEDEF:       return _POSIX2_LOCALEDEF;
    case _SC_2_SW_DEV:          return _POSIX2_SW_DEV;
    case _SC_2_UPE:             return _POSIX2_UPE;
    case _SC_2_VERSION:         return _POSIX2_VERSION;
    case _SC_JOB_CONTROL:       return _POSIX_JOB_CONTROL;
    case _SC_SAVED_IDS:         return _POSIX_SAVED_IDS;
    case _SC_VERSION:           return _POSIX_VERSION;
    case _SC_RE_DUP_MAX:        return _POSIX_RE_DUP_MAX;         // Minimum requirement.
    case _SC_STREAM_MAX:        return FOPEN_MAX;
    case _SC_TZNAME_MAX:        return _POSIX_TZNAME_MAX;         // Minimum requirement.
    case _SC_XOPEN_CRYPT:       return _XOPEN_CRYPT;
    case _SC_XOPEN_ENH_I18N:    return _XOPEN_ENH_I18N;
    case _SC_XOPEN_SHM:         return _XOPEN_SHM;
    case _SC_XOPEN_VERSION:     return _XOPEN_VERSION;
    case _SC_XOPEN_REALTIME:    return _XOPEN_REALTIME;
    case _SC_XOPEN_REALTIME_THREADS: return _XOPEN_REALTIME_THREADS;
    case _SC_XOPEN_LEGACY:      return _XOPEN_LEGACY;
    case _SC_ATEXIT_MAX:        return LONG_MAX;    // Unlimited.
    case _SC_IOV_MAX:           return UIO_MAXIOV;

    case _SC_PAGESIZE:          // Fall through, PAGESIZE and PAGE_SIZE always hold the same value.
    case _SC_PAGE_SIZE:         return PAGE_SIZE;
    case _SC_XOPEN_UNIX:        return _XOPEN_UNIX;
    case _SC_AIO_LISTIO_MAX:    return _POSIX_AIO_LISTIO_MAX;     // Minimum requirement.
    case _SC_AIO_MAX:           return _POSIX_AIO_MAX;            // Minimum requirement.
    case _SC_AIO_PRIO_DELTA_MAX:return 0;                         // Minimum requirement.
    case _SC_DELAYTIMER_MAX:    return INT_MAX;
    case _SC_MQ_OPEN_MAX:       return _POSIX_MQ_OPEN_MAX;        // Minimum requirement.
    case _SC_MQ_PRIO_MAX:       return _POSIX_MQ_PRIO_MAX;        // Minimum requirement.
    case _SC_RTSIG_MAX:         return RTSIG_MAX;
    case _SC_SEM_NSEMS_MAX:     return _POSIX_SEM_NSEMS_MAX;      // Minimum requirement.
    case _SC_SEM_VALUE_MAX:     return SEM_VALUE_MAX;
    case _SC_SIGQUEUE_MAX:      return _POSIX_SIGQUEUE_MAX;       // Minimum requirement.
    case _SC_TIMER_MAX:         return _POSIX_TIMER_MAX;          // Minimum requirement.
    case _SC_ASYNCHRONOUS_IO:   return _POSIX_ASYNCHRONOUS_IO;
    case _SC_FSYNC:             return _POSIX_FSYNC;
    case _SC_MAPPED_FILES:      return _POSIX_MAPPED_FILES;
    case _SC_MEMLOCK:           return _POSIX_MEMLOCK;
    case _SC_MEMLOCK_RANGE:     return _POSIX_MEMLOCK_RANGE;
    case _SC_MEMORY_PROTECTION: return _POSIX_MEMORY_PROTECTION;
    case _SC_MESSAGE_PASSING:   return _POSIX_MESSAGE_PASSING;
    case _SC_PRIORITIZED_IO:    return _POSIX_PRIORITIZED_IO;
    case _SC_PRIORITY_SCHEDULING:  return _POSIX_PRIORITY_SCHEDULING;
    case _SC_REALTIME_SIGNALS:  return _POSIX_REALTIME_SIGNALS;
    case _SC_SEMAPHORES:        return _POSIX_SEMAPHORES;
    case _SC_SHARED_MEMORY_OBJECTS:  return _POSIX_SHARED_MEMORY_OBJECTS;
    case _SC_SYNCHRONIZED_IO:   return _POSIX_SYNCHRONIZED_IO;
    case _SC_TIMERS:            return _POSIX_TIMERS;
    case _SC_GETGR_R_SIZE_MAX:  return 1024;
    case _SC_GETPW_R_SIZE_MAX:  return 1024;
    case _SC_LOGIN_NAME_MAX:    return 256;   // Seems default on linux.
    case _SC_THREAD_DESTRUCTOR_ITERATIONS: return PTHREAD_DESTRUCTOR_ITERATIONS;
    case _SC_THREAD_KEYS_MAX:   return PTHREAD_KEYS_MAX;
    case _SC_THREAD_STACK_MIN:    return PTHREAD_STACK_MIN;
    case _SC_THREAD_THREADS_MAX:  return PTHREAD_THREADS_MAX;
    case _SC_TTY_NAME_MAX:        return 32;  // Seems default on linux.
    case _SC_THREADS:             return _POSIX_THREADS;
    case _SC_THREAD_ATTR_STACKADDR:   return _POSIX_THREAD_ATTR_STACKADDR;
    case _SC_THREAD_ATTR_STACKSIZE:   return _POSIX_THREAD_ATTR_STACKSIZE;
    case _SC_THREAD_PRIORITY_SCHEDULING:  return _POSIX_THREAD_PRIORITY_SCHEDULING;
    case _SC_THREAD_PRIO_INHERIT: return _POSIX_THREAD_PRIO_INHERIT;
    case _SC_THREAD_PRIO_PROTECT: return _POSIX_THREAD_PRIO_PROTECT;
    case _SC_THREAD_SAFE_FUNCTIONS:  return _POSIX_THREAD_SAFE_FUNCTIONS;
    case _SC_NPROCESSORS_CONF:  return get_nprocs_conf();
    case _SC_NPROCESSORS_ONLN:  return get_nprocs();
    case _SC_PHYS_PAGES:        return get_phys_pages();
    case _SC_AVPHYS_PAGES:      return get_avphys_pages();
    case _SC_MONOTONIC_CLOCK:   return __sysconf_monotonic_clock();

    case _SC_2_PBS:             return _POSIX2_PBS;             // Obsolescent in POSIX.1-2008.
    case _SC_2_PBS_ACCOUNTING:  return _POSIX2_PBS_ACCOUNTING;  // Obsolescent in POSIX.1-2008.
    case _SC_2_PBS_CHECKPOINT:  return _POSIX2_PBS_CHECKPOINT;  // Obsolescent in POSIX.1-2008.
    case _SC_2_PBS_LOCATE:      return _POSIX2_PBS_LOCATE;      // Obsolescent in POSIX.1-2008.
    case _SC_2_PBS_MESSAGE:     return _POSIX2_PBS_MESSAGE;     // Obsolescent in POSIX.1-2008.
    case _SC_2_PBS_TRACK:       return _POSIX2_PBS_TRACK;       // Obsolescent in POSIX.1-2008.
    case _SC_ADVISORY_INFO:     return _POSIX_ADVISORY_INFO;
    case _SC_BARRIERS:          return _POSIX_BARRIERS;
    case _SC_CLOCK_SELECTION:   return _POSIX_CLOCK_SELECTION;
    case _SC_CPUTIME:           return _POSIX_CPUTIME;
    case _SC_HOST_NAME_MAX:     return _POSIX_HOST_NAME_MAX;    // Minimum requirement.
    case _SC_IPV6:              return _POSIX_IPV6;
    case _SC_RAW_SOCKETS:       return _POSIX_RAW_SOCKETS;
    case _SC_READER_WRITER_LOCKS: return _POSIX_READER_WRITER_LOCKS;
    case _SC_REGEXP:            return _POSIX_REGEXP;
    case _SC_SHELL:             return _POSIX_SHELL;
    case _SC_SPAWN:             return _POSIX_SPAWN;
    case _SC_SPIN_LOCKS:        return _POSIX_SPIN_LOCKS;
    case _SC_SPORADIC_SERVER:   return _POSIX_SPORADIC_SERVER;
    case _SC_SS_REPL_MAX:       return _POSIX_SS_REPL_MAX;
    case _SC_SYMLOOP_MAX:       return _POSIX_SYMLOOP_MAX;      // Minimum requirement.
    case _SC_THREAD_CPUTIME:    return _POSIX_THREAD_CPUTIME;
    case _SC_THREAD_PROCESS_SHARED: return _POSIX_THREAD_PROCESS_SHARED;
    case _SC_THREAD_ROBUST_PRIO_INHERIT:  return _POSIX_THREAD_ROBUST_PRIO_INHERIT;
    case _SC_THREAD_ROBUST_PRIO_PROTECT:  return _POSIX_THREAD_ROBUST_PRIO_PROTECT;
    case _SC_THREAD_SPORADIC_SERVER:      return _POSIX_THREAD_SPORADIC_SERVER;
    case _SC_TIMEOUTS:          return _POSIX_TIMEOUTS;
    case _SC_TRACE:             return _POSIX_TRACE;                    // Obsolescent in POSIX.1-2008.
    case _SC_TRACE_EVENT_FILTER:      return _POSIX_TRACE_EVENT_FILTER; // Obsolescent in POSIX.1-2008.
    case _SC_TRACE_EVENT_NAME_MAX:    return _POSIX_TRACE_EVENT_NAME_MAX;
    case _SC_TRACE_INHERIT:     return _POSIX_TRACE_INHERIT;            // Obsolescent in POSIX.1-2008.
    case _SC_TRACE_LOG:         return _POSIX_TRACE_LOG;                // Obsolescent in POSIX.1-2008.
    case _SC_TRACE_NAME_MAX:    return _POSIX_TRACE_NAME_MAX;
    case _SC_TRACE_SYS_MAX:     return _POSIX_TRACE_SYS_MAX;
    case _SC_TRACE_USER_EVENT_MAX:    return _POSIX_TRACE_USER_EVENT_MAX;
    case _SC_TYPED_MEMORY_OBJECTS:    return _POSIX_TYPED_MEMORY_OBJECTS;
    case _SC_V7_ILP32_OFF32:    return _POSIX_V7_ILP32_OFF32;
    case _SC_V7_ILP32_OFFBIG:   return _POSIX_V7_ILP32_OFFBIG;
    case _SC_V7_LP64_OFF64:     return _POSIX_V7_LP64_OFF64;
    case _SC_V7_LPBIG_OFFBIG:   return _POSIX_V7_LPBIG_OFFBIG;
    case _SC_XOPEN_STREAMS:     return _XOPEN_STREAMS;          // Obsolescent in POSIX.1-2008.
    case _SC_XOPEN_UUCP:        return _XOPEN_UUCP;

    default:
      // Posix says EINVAL is the only error that shall be returned,
      // but glibc uses ENOSYS.
      errno = ENOSYS;
      return -1;
  }
}
