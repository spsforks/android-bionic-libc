/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <inttypes.h>
#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <bionic/reserved_signals.h>

#include "libs_utils.h"

// Assists execve() tests by capturing reserved signal dispositions between an
// execve() syscall returning, and the new process image's libc being
// initialized (which usually installs handlers for those signals).

static struct sigaction debugger_act = {};
static struct sigaction prof_act = {};
static struct sigaction art_prof_act = {};

static void __reserved_signals_capture_preinit() {
  // sigaction wrapper should be safe to use at preinit
  sigaction(BIONIC_SIGNAL_DEBUGGER, nullptr, &debugger_act);
  sigaction(BIONIC_SIGNAL_PROFILER, nullptr, &prof_act);
  sigaction(BIONIC_SIGNAL_ART_PROFILER, nullptr, &art_prof_act);
}

__attribute__((section(".preinit_array"), __used__)) void (*__reserved_signals_capture_preinit_p)(
    void) = __reserved_signals_capture_preinit;

int main() {
  auto print_disposition = [](struct sigaction* act, const char* prefix) {
    if (act->sa_flags & SA_SIGINFO) {
      CHECK(act->sa_sigaction != nullptr);
      printf("%s: custom sa_sigaction\n", prefix);
    } else if (act->sa_handler == SIG_IGN) {
      printf("%s: SIG_IGN\n", prefix);
    } else if (act->sa_handler == SIG_DFL) {
      printf("%s: SIG_DFL\n", prefix);
    } else if (act->sa_handler == SIG_ERR) {
      printf("%s: SIG_ERR\n", prefix);
    } else {
      CHECK(act->sa_handler != nullptr);
      printf("%s: custom sa_handler\n", prefix);
    };
  };

  print_disposition(&debugger_act, "preinit BIONIC_SIGNAL_DEBUGGER");
  print_disposition(&prof_act, "preinit BIONIC_SIGNAL_PROFILER");
  print_disposition(&art_prof_act, "preinit BIONIC_SIGNAL_ART_PROFILER");
}
