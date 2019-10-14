/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <stdlib.h>
#include <stdint.h>
#include <sys/reboot.h>
#include <unistd.h>

#include <async_safe/log.h>
#include <private/bionic_globals.h>

// We choose a static pointer tag here for performance reasons. Dynamic tagging
// doesn't improve our detection, and simply hurts performance. This tag is
// deliberately chosen to always point to inaccessible memory on a standard
// 64-bit userspace process, and be easily identifiable by developers. This tag
// is also deliberately different from the standard pattern-init tag (0xAA), as
// to be distinguishable from an uninitialised-pointer access. The first and
// second nibbles are also deliberately designed to be the bitset-mirror of each
// other (0b1100, 0b0011) in order to reduce incidental matches. Users must not
// rely on the implementation-defined value of this pointer tag, as it may
// change.
static constexpr uintptr_t POINTER_TAG = 0xC3;
static constexpr unsigned TAG_SHIFT = 56;

// Return a forcibly-tagged pointer.
static inline void* TagPointer(void* ptr) {
#if defined(__aarch64__)
  static constexpr uintptr_t TAGGED_MASK = POINTER_TAG << TAG_SHIFT;
  return reinterpret_cast<void*>(
      reinterpret_cast<uintptr_t>(ptr) | TAGGED_MASK);
#else
  async_safe_fatal("Attempting to tag a pointer (%p) on non-aarch64.", ptr);
#endif
}

// Return a forcibly-untagged pointer. The pointer tag is not checked for
// validity.
static inline void* UntagPointer(const volatile void* ptr) {
#if defined(__aarch64__)
  static constexpr uintptr_t UNTAG_MASK =
    (static_cast<uintptr_t>(1) << TAG_SHIFT) - 1;
  return reinterpret_cast<void*>(
      reinterpret_cast<uintptr_t>(ptr) & UNTAG_MASK);
#endif
  return const_cast<void*>(ptr);
}

// Return a tagged pointer iff the platform (hardware and kernel) supports
// tagged pointers, and `ptr` is non-null.
static inline void* MaybeTagPointer(void* ptr) {
#if defined(__aarch64__) && !__has_feature(hwaddress_sanitizer)
  if (__predict_true(__libc_globals->tagged_pointers_supported) &&
     __predict_true(ptr != nullptr)) {
    return TagPointer(ptr);
  }
#endif
  return ptr;
}

// Untag the pointer, and check the pointer tag iff the platform (hardware and
// kernel) supports tagged pointers. If the tag is incorrect, trap.
static inline void* UntagAndMaybeCheckPointer(const volatile void* ptr) {
#if defined(__aarch64__) && !__has_feature(hwaddress_sanitizer)
  if ((__predict_true(__libc_globals->tagged_pointers_supported)) &&
      (__predict_true(ptr != nullptr)) &&
      (__predict_false(
        reinterpret_cast<uintptr_t>(ptr) >> TAG_SHIFT != POINTER_TAG))) {
    async_safe_fatal(
      "Pointer tag for %p was truncated (Googlers: see go/tagged-pointers)",
      ptr);
  }
#endif
  return UntagPointer(ptr);
}
