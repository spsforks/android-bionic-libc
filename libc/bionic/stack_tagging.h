/*
 * Copyright 2020 Arm Ltd. All rights reserved.
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

#include <errno.h>
#include <sys/auxv.h>
#include <sys/mman.h>

#include <async_safe/log.h>

#include "platform/bionic/mte_kernel.h"
#include "platform/bionic/page.h"

#if defined(__aarch64__) && defined(ANDROID_EXPERIMENTAL_MTE)
void EnableMainThreadStackTagging() {
  uintptr_t first_page = PAGE_START(static_cast<uintptr_t>(getauxval(AT_PLATFORM)));
  int stack_prot = PROT_READ | PROT_WRITE | PROT_MTE | PROT_GROWSDOWN;

  /* The PROT_GROWSDOWN will ensure that even the pages below first_page (even
     if already allocated) will have their flags updated. */
  if (mprotect(reinterpret_cast<void*>(first_page), PAGE_SIZE, stack_prot) != 0) {
    async_safe_fatal("Failed to map stack as tagged: error %d", errno);
  }
}
#else
void EnableMainThreadStackTagging() {}
#endif
