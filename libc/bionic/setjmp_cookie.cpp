/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <assert.h>
#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>

#include "private/libc_logging.h"

extern "C" __LIBC_HIDDEN__ int getentropy(void*, size_t);
static long __bionic_setjmp_cookie;

extern "C" void __bionic_setjmp_cookie_init() {
  long value = 0;
  do {
    if (getentropy(&value, sizeof(value)) != 0) {
      __libc_format_log(
        ANDROID_LOG_WARN,
        "error generating entropy for setjmp cookie, disabling setjmp/longjmp",
        strerror(errno));
      __bionic_setjmp_cookie = 0;
      return;
    }

    // Mask off the low bit to store the signal flag.
    value &= ~1;
  } while (!value);

  __bionic_setjmp_cookie = value;
}

extern "C" long __bionic_setjmp_cookie_get(long sigflag) {
  if (sigflag & ~1) {
    __libc_fatal("unexpected sigflag value: %ld", sigflag);
  }

  if (!__bionic_setjmp_cookie) {
    __libc_fatal("failed to generate entropy for setjmp cookie during libc init"
                 ", aborting in setjmp");
  }

  return __bionic_setjmp_cookie | sigflag;
}

// Aborts if cookie doesn't match, returns the signal flag otherwise.
extern "C" long __bionic_setjmp_cookie_check(long cookie) {
  if (!__bionic_setjmp_cookie) {
    __libc_fatal("failed to generate entropy for setjmp cookie during libc init"
                 ", aborting in longjmp");
  }

  if (__bionic_setjmp_cookie != (cookie & ~1)) {
    __libc_fatal("setjmp cookie mismatch");
  }

  return cookie & 1;
}
