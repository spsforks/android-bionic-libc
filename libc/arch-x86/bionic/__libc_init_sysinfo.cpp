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

#include "private/KernelArgumentBlock.h"
#include "private/bionic_globals.h"

// This file is compiled without stack protection, because it runs before TLS
// has been set up.

__LIBC_HIDDEN__ __attribute__((__naked__)) void __libc_int0x80() {
  __asm__ volatile("int $0x80; ret");
}

__LIBC_HIDDEN__ void __libc_init_sysinfo(KernelArgumentBlock& args) {
  void* at_sysinfo = reinterpret_cast<void*>(args.getauxval(AT_SYSINFO));
  __libc_sysinfo = (at_sysinfo != nullptr) ? at_sysinfo :
      reinterpret_cast<void*>(__libc_int0x80);
}

// TODO: lose this function and just access __libc_sysinfo directly.
__LIBC_HIDDEN__ extern "C" void* __kernel_syscall() {
  return __libc_sysinfo;
}
