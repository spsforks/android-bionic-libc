/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <private/bionic_asm.h>

// void _exit_with_stack_teardown(void* stackBase, size_t stackSize, int madvCommand, void** cacheMmapBase)
ENTRY_PRIVATE(_exit_with_stack_teardown)
  cbz x3, .Ldo_unmap                 // If cacheMmapBase==nullptr, unmap
  mov x6, x0                         // Preserve x0 across madvise
  cbz x2, .Ldo_validate_cache_entry  // If madvCommand==MADV_NORMAL, skip the madvise
  mov w8, __NR_madvise
  svc #0                             // Clobbers x0
  // Fall through
.Ldo_validate_cache_entry:
  stlr x6, [x3]                      // Make the cache entry valid: release semantics
  // Fall through
.Ldo_exit:
  mov x0, #0
  mov w8, __NR_exit
  svc #0                            // The exit syscall does not return
.Ldo_unmap:
  mov w8, __NR_munmap
  svc #0
  b .Ldo_exit                       // If munmap failed, we ignore the failure and exit anyway
END(_exit_with_stack_teardown)
