/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include "linker_bionic_signals.h"

#include <signal.h>
#include "platform/bionic/reserved_signals.h"

#if defined(__ANDROID__)
// See the callsite and libc/bionic/exec.cpp for extra context.
void linker_unblock_bionic_signals() {
  // Ignore the subset of signals that might be delivered asynchronously
  // (usually from out-of-process), and which usually have have a custom handler
  // (which would've been reset by the kernel to the default of terminating the
  // program). Other reserved signals are assumed to never be left pending
  // across an execve, so we avoid extra syscalls. The ignoring will cause us to
  // lose all such pending and new signals after the unblocking below.
  //
  // A slightly better alternative would be to keep the signals blocked until
  // the code responsible for installing their respective handler runs, as we
  // then wouldn't have a window where these signals are discarded. But that
  // requires reworking bionic's filtering of reserved handlers, otherwise it is
  // too easy for harmless-looking code to accidentally unblock the signals
  // prematurely.
  signal(BIONIC_SIGNAL_DEBUGGER, SIG_IGN);
  signal(BIONIC_SIGNAL_PROFILER, SIG_IGN);
  signal(BIONIC_SIGNAL_ART_PROFILER, SIG_IGN);

  // Unblock all reserved signal handlers (except BIONIC_SIGNAL_POSIX_TIMERS) by
  // using the bionic wrapper of sigprocmask64, the signal filtering will fill
  // the set appropriately.
  sigset64_t set = {};
  sigprocmask64(SIG_UNBLOCK, &set, nullptr);
}
#else
// NOP on host.
void linker_unblock_bionic_signals() {}
#endif
