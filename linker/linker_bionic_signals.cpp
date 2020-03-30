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
void linker_unblock_bionic_signals() {
  // Ignore a specific subset of the reserved signals that might be left pending
  // across the execve of the image that we're linking (as they're delivered
  // from out of process by design, which is inherently asynchronous). We don't
  // want these signals to fire upon unblocking, as the default disposition for
  // RT signals is to terminate the process.
  signal(BIONIC_SIGNAL_DEBUGGER, SIG_IGN);
  signal(BIONIC_SIGNAL_PROFILER, SIG_IGN);
  signal(BIONIC_SIGNAL_ART_PROFILER, SIG_IGN);

  // Unblock all reserved signal handlers by using the bionic wrapper of
  // sigprocmask64, which internally adds the reserved signals to the set.
  sigset64_t set = {};
  sigprocmask64(SIG_UNBLOCK, &set, nullptr);
}
#else
// NOP on host.
void linker_unblock_bionic_signals() {}
#endif
