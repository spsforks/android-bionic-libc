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

#pragma once

#include <sys/cdefs.h>

__BEGIN_DECLS

/*
 * Calling exit(3) in a multithreaded C++ program is risky, because exit will invoke
 * destructors on globals that are potentially being simultaneously accessed by other threads.
 *
 * It's also inefficient to spend time cleaning up resources that the kernel will clean up for you,
 * and if you have code that must run on exit, your code is likely already broken because exit-time
 * destructors will not run on oom-kill, explicit SIGKILL, power loss, etc.
 *
 * Bugs caused by incorrect use of exit typically show up as explosions on unrelated threads. To
 * make finding these easier, bionic provides a method to change the behavior of exit to abort
 * immediately, so that you can replace them with _exit, which does not call destructors.
 *
 * By default, processes spawned from the zygote use ANDROID_EXIT_MODE_ABORT when target API level
 * is 35 or greater, and ANDROID_EXIT_MODE_WARN otherwise.
 */

/*
 * Bitmask controlling the behavior of exit(3).
 */
enum android_exit_mode {
  /* exit() runs destructors as normal. */
  ANDROID_EXIT_MODE_NORMAL = 0,

  /* Skip destruction of globals upon exit(). */
  ANDROID_EXIT_MODE_SKIP_GLOBAL_DESTRUCTORS = 1,

  /* Skip destruction of threadlocal variables upon exit(). */
  ANDROID_EXIT_MODE_SKIP_THREAD_DESTRUCTORS = 2,

  /* Skip all destructors upon exit(). */
  ANDROID_EXIT_MODE_SKIP_DESTRUCTORS = 3,

  /* exit() generates a tombstone when invoked. */
  ANDROID_EXIT_MODE_WARN = 4,

  /* exit() immediately aborts instead of running destructors and then exiting. */
  ANDROID_EXIT_MODE_ABORT = 8,
};

/*
 * Set the behavior of exit, and return the previous state.
 */
enum android_exit_mode android_set_exit_mode(enum android_exit_mode mode);

/*
 * Get the current behavior of exit.
 */
enum android_exit_mode android_get_exit_mode(void);

__END_DECLS
