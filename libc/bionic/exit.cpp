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

#include <android/exit_mode.h>
#include <platform/bionic/reserved_signals.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "private/bionic_defs.h"
#include "private/bionic_globals.h"
#include "private/bionic_inline_raise.h"
#include "pthread_internal.h"

extern "C" void __cxa_finalize(void* dso_handle);
extern "C" void __cxa_thread_finalize();
extern "C" __noreturn void __exit_group(int status);

__attribute__((no_sanitize("memtag"))) void _exit(int status) {
  __exit_group(status);
}

__strong_alias(_Exit, _exit);

android_exit_mode android_set_exit_mode(android_exit_mode mode) {
  return atomic_exchange(&__libc_shared_globals()->exit_mode, mode);
}

android_exit_mode android_get_exit_mode() {
  return __libc_shared_globals()->exit_mode;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
void exit(int status) {
  int exit_mode = android_get_exit_mode();
  static constexpr char abort_message_str[] =
      "exit() called in multithreaded process, aborting instead.";
  if (exit_mode & ANDROID_EXIT_MODE_ABORT) {
    async_safe_fatal(abort_message_str);
  } else if (exit_mode & ANDROID_EXIT_MODE_WARN) {
    struct {
      size_t size;
      char buf[512];
    } abort_message = {};
    memcpy(abort_message.buf, abort_message_str, strlen(abort_message_str));
    abort_message.size = strlen(abort_message_str) + sizeof(size_t);
    inline_raise(BIONIC_SIGNAL_DEBUGGER, &abort_message);
  }

  if ((exit_mode & ANDROID_EXIT_MODE_SKIP_THREAD_DESTRUCTORS) == 0) {
    __cxa_thread_finalize();
  }

  if ((exit_mode & ANDROID_EXIT_MODE_SKIP_GLOBAL_DESTRUCTORS) == 0) {
    __cxa_finalize(nullptr);
  }

  _exit(status);
}
