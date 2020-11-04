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

#include <sanitizer/hwasan_interface.h>
#include <signal.h>
#include <sys/cdefs.h>

#include "platform/bionic/mte.h"

extern "C" __LIBC_HIDDEN__ bool __vfork_is_fork() {
  return mte_supported();
}

static inline __attribute__((no_sanitize("hwaddress", "memtag"))) void untag_memory(void* from,
                                                                                    void* to) {
  __asm__ __volatile__(
      ".arch_extension mte\n"
      "1:\n"
      "stg %[Ptr], [%[Ptr]], #16\n"
      "cmp %[Ptr], %[End]\n"
      "b.lt 1b\n"
      : [ Ptr ] "+&r"(from)
      : [ End ] "r"(to)
      : "memory");
}

extern "C" __LIBC_HIDDEN__ __attribute__((no_sanitize("hwaddress", "memtag"))) void
__memtag_handle_longjmp(void* sp_dst) {
  if (mte_supported()) {
    // Restore memory tags if either we are on the regular stack, or both source and destination are
    // on sigaltstack.
    // FIXME: Find a way to handle longjmp from sigaltstack to the regular stack.
    stack_t ss;
    if (sigaltstack(nullptr, &ss) == 0) {
      if (ss.ss_flags & SS_ONSTACK) {
        uintptr_t stack_begin = reinterpret_cast<uintptr_t>(ss.ss_sp);
        uintptr_t stack_end = reinterpret_cast<uintptr_t>(ss.ss_sp) + ss.ss_size;
        uintptr_t dst = reinterpret_cast<uintptr_t>(sp_dst);
        if (dst > stack_begin && dst < stack_end) {
          untag_memory(__builtin_frame_address(0), sp_dst);
        }
      } else if (!(ss.ss_flags & SS_DISABLE)) {
        untag_memory(__builtin_frame_address(0), sp_dst);
      }
    }
  }

#if __has_feature(hwaddress_sanitizer)
  __hwasan_handle_longjmp(sp_dst);
#endif  // __has_feature(hwaddress_sanitizer)
}
