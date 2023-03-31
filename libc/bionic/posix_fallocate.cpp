/*
 * Copyright (C) 2014 The Android Open Source Project
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

// This function is in the LP32 NDK ABI with a 32-bit off_t, but the
// platform is compiled with _FILE_OFFSET_BITS=64.  Rename the NDK
// declarations out of the way so __RENAME_IF_FILE_OFFSET64 doesn't rename
// the 32-bit off_t implementation.
#define posix_fallocate public_posix_fallocate

#include <fcntl.h>

#include "private/ErrnoRestorer.h"

#undef posix_fallocate

// The NDK declarations for this function was skipped above, redeclare
// it with extern "C" here.
extern "C" int posix_fallocate(int fd, off_t offset, __bionic_legacy_compat_off_t length);

int posix_fallocate(int fd, off_t offset, __bionic_legacy_compat_off_t length) {
  ErrnoRestorer errno_restorer;
  return (fallocate(fd, 0, offset, length) == 0) ? 0 : errno;
}

int posix_fallocate64(int fd, off64_t offset, off64_t length) {
  ErrnoRestorer errno_restorer;
  return (fallocate64(fd, 0, offset, length) == 0) ? 0 : errno;
}
