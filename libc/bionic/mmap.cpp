/*
 * Copyright (C) 2008 The Android Open Source Project
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
#define mmap public_mmap

#include <errno.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

#include "platform/bionic/macros.h"
#include "private/ErrnoRestorer.h"

#undef mmap

// The NDK declarations for this function was skipped above, redeclare
// it with extern "C" here.
extern "C" void* mmap(void* addr, size_t size, int prot, int flags, int fd,
                      __bionic_legacy_compat_off_t offset);

// mmap2(2) is like mmap(2), but the offset is in 4096-byte blocks, not bytes.
extern "C" void*  __mmap2(void*, size_t, int, int, int, size_t);

#define MMAP2_SHIFT 12 // 2**12 == 4096

void* mmap64(void* addr, size_t size, int prot, int flags, int fd, off64_t offset) {
  if (offset < 0 || (offset & ((1UL << MMAP2_SHIFT)-1)) != 0) {
    errno = EINVAL;
    return MAP_FAILED;
  }

  // Prevent allocations large enough for `end - start` to overflow.
  size_t rounded = __BIONIC_ALIGN(size, PAGE_SIZE);
  if (rounded < size || rounded > PTRDIFF_MAX) {
    errno = ENOMEM;
    return MAP_FAILED;
  }

  return __mmap2(addr, size, prot, flags, fd, offset >> MMAP2_SHIFT);
}

void* mmap(void* addr, size_t size, int prot, int flags, int fd,
           __bionic_legacy_compat_off_t offset) {
  return mmap64(addr, size, prot, flags, fd, static_cast<off64_t>(offset));
}
