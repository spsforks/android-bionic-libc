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

#include <async_safe/log.h>
#include <errno.h>
#include <stdarg.h>
#include <stdint.h>
#include <sys/mman.h>
#include <unistd.h>

#include "platform/bionic/macros.h"
#include "platform/bionic/page.h"
#include "pthread_internal.h"

extern "C" int __mprotect(void*, size_t, int);

static void check_stack_collision(void* addr, size_t len) {
  if (!kUseStackCache) {
    return;
  }

  pthread_internal_t* thread = __get_thread();
  if (!thread->stack_cache_entry) {
    return;
  }

  // The stack is never at the end of the address space
  uintptr_t stack_begin = reinterpret_cast<uintptr_t>(thread->mmap_base);
  uintptr_t stack_end = stack_begin + thread->mmap_size;

  uintptr_t prot_begin = reinterpret_cast<uintptr_t>(addr);
  uintptr_t prot_end;

  if (__builtin_add_overflow(prot_begin, len, &prot_end)) {
    // Pretend the range ends at the maximum address so that the range check below works.
    prot_end = UINTPTR_MAX;
  }

  prot_begin = PAGE_START(prot_begin);
  if (UINTPTR_MAX - prot_end < PAGE_SIZE) {
    prot_end = UINTPTR_MAX;
  } else {
    prot_end = PAGE_END(prot_end);
  }

  if (prot_begin < stack_end && stack_begin < prot_end && !thread->stack_maybe_modified) {
    if (__is_thread_cache_debug_enabled()) {
      async_safe_format_log(ANDROID_LOG_DEBUG, "libc",
                            "disabling reuse of stack of current thread");
    }
    thread->stack_maybe_modified = true;
  }
}

int mprotect(void* addr, size_t len, int prot) {
  check_stack_collision(addr, len);
  return __mprotect(addr, len, prot);
}
