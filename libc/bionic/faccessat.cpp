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

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

extern "C" int __faccessat2(int, const char*, int, int);
extern "C" int __faccessat(int, const char*, int);

int faccessat(int dirfd, const char* pathname, int mode, int flags) {
  // On Linux 5.8 or later, the kernel can just do this for us.
  // This supports AT_EACCESS and AT_SYMLINK_NOFOLLOW.
  int result = __faccessat2(dirfd, pathname, mode, flags);
  if (result != -1) return result;
  if (errno != ENOSYS) return -1;

  // Can we fake it?

  // We never fake AT_EACCESS or AT_SYMLINK_NOFOLLOW ourselves, though glibc
  // tries to fake both, and musl fakes the former but not the latter.
  //
  // More details on these mailing lists:
  // * https://www.openwall.com/lists/musl/2015/02/05/2
  // * http://lists.landley.net/pipermail/toybox-landley.net/2014-September/003617.html
  // Plus the original commit of faccessat() to bionic:
  // * https://android.googlesource.com/platform/bionic/+/35778253a5ed71e87a608ca590b63729d9f88567
  if (flags != 0) {
    errno = EINVAL;
    return -1;
  }

  // Before we consider letting the kernel handle simple cases with faccessat()
  // rather than faccessat2(), we need to check the mode ourselves because old
  // kernels didn't.
  if ((mode != F_OK) && ((mode & ~(R_OK | W_OK | X_OK)) != 0) &&
      ((mode & (R_OK | W_OK | X_OK)) == 0)) {
    errno = EINVAL;
    return -1;
  }

  return __faccessat(dirfd, pathname, mode);
}
