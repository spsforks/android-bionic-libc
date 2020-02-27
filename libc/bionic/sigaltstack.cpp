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

#include <signal.h>

#include "pthread_internal.h"

extern "C" int __sigaltstack(const stack_t *ss, stack_t *old_ss);

int sigaltstack(const stack_t *ss, stack_t *old_ss) {
  int retval = __sigaltstack(ss, old_ss);
  if (retval == 0 && ss) {
    if (ss->ss_flags & SS_DISABLE) {
      __get_thread()->sigaltstack_bottom = __get_thread()->sigaltstack_top = 0;
    } else {
      __get_thread()->sigaltstack_bottom = reinterpret_cast<uintptr_t>(ss->ss_sp);
      __get_thread()->sigaltstack_top = reinterpret_cast<uintptr_t>(ss->ss_sp) + ss->ss_size;
    }
    if (ss->ss_flags & ~SS_DISABLE) {
      // SS_AUTODISARM, and possibly other flags that may be created in the future, could cause
      // sigaltstack_{bottom,top} to go out of sync with the kernel's knowledge of where the
      // alternate signal stack is, so we just give up on creating stack traces if we see an unknown
      // flag.
      __get_thread()->sigaltstack_unreliable = true;
    }
  }
  return retval;
}
