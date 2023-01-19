/*
 * Copyright (C) 2023 The Android Open Source Project
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

#include <signal.h>

extern "C" bool __loader_debuggerd_first_chance_signal_handler(int signal_number, siginfo_t* info,
                                                               void* context);

// This function should be called by the sigchain signal handler in libart,
// before any additional processing of the signal is done. Due to linker
// namespace restrictions, libart is not allowed to directly call functions in
// the loader, so we provide this proxy in libc.
//
// This function returns false if the sigchain handler should continue. This
// function returns true if the sigchain handler shold immediately return, which
// happens when the signal came from GWP-ASan, and we've dumped a debuggerd
// report and patched up the GWP-ASan allocator to recover from the fault, and
// regular execution of the program can continue.
extern "C" bool debuggerd_first_chance_signal_handler(int signal_number, siginfo_t* info,
                                                      void* context) {
  return __loader_debuggerd_first_chance_signal_handler(signal_number, info, context);
}
