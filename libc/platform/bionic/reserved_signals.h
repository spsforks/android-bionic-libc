/*
 * Copyright (C) 2018 The Android Open Source Project
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

#pragma once

#include <sys/cdefs.h>

#include <signal.h>

#include "macros.h"

// Realtime signals reserved for internal use:
//   32 (__SIGRTMIN + 0)        POSIX timers
//   33 (__SIGRTMIN + 1)        libbacktrace
//   34 (__SIGRTMIN + 2)        libcore
//   35 (__SIGRTMIN + 3)        debuggerd
//   36 (__SIGRTMIN + 4)        platform profilers (heapprofd, traced_perf)
//   37 (__SIGRTMIN + 5)        coverage (libprofile-extras)
//   38 (__SIGRTMIN + 6)        heapprofd ART managed heap dumps
//   39 (__SIGRTMIN + 7)        fdtrack
//
// If you change this, also:
// * change __ndk_legacy___libc_current_sigrtmin in
//   <android/legacy_signal_inlines.h> to match.
// * if the signal will have a custom handler, and can be delivered
//   asynchronously during the process calling execve, see
//   linker/linker_bionic_signals.cpp.

#define BIONIC_SIGNAL_POSIX_TIMERS (__SIGRTMIN + 0)
#define BIONIC_SIGNAL_DEBUGGER (__SIGRTMIN + 3)
#define BIONIC_SIGNAL_PROFILER (__SIGRTMIN + 4)
#define BIONIC_SIGNAL_ART_PROFILER (__SIGRTMIN + 6)
#define BIONIC_SIGNAL_FDTRACK (__SIGRTMIN + 7)

#define __SIGRT_RESERVED 8

static inline __always_inline sigset64_t filter_reserved_signals(sigset64_t sigset, int how) {
  int (*block)(sigset64_t*, int);
  int (*unblock)(sigset64_t*, int);
  switch (how) {
    case SIG_BLOCK:
      __BIONIC_FALLTHROUGH;
    case SIG_SETMASK:
      block = sigaddset64;
      unblock = sigdelset64;
      break;

    case SIG_UNBLOCK:
      block = sigdelset64;
      unblock = sigaddset64;
      break;
  }

  // The POSIX timer signal must be blocked.
  block(&sigset, __SIGRTMIN + 0);

  // Everything else must remain unblocked.
  for (int signo = __SIGRTMIN + 1; signo < (__SIGRTMIN + __SIGRT_RESERVED); signo++) {
    unblock(&sigset, signo);
  }
  return sigset;
}

static inline __always_inline sigset64_t reserved_signal_set() {
  sigset64_t sigset = {};
  for (int signo = __SIGRTMIN; signo < (__SIGRTMIN + __SIGRT_RESERVED); signo++) {
    sigaddset64(&sigset, signo);
  }
  return sigset;
}
