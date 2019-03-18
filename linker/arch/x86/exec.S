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

#include <private/bionic_asm.h>

/*
 * A helper function for the last step of userspace exec().  It pushes the
 * argument block data for the target executable onto the stack and jumps to
 * the target entry address.
 *
 * It takes 3 arguments:
 *   - The pointer to the argument block data.
 *   - The number of words contained in the argument block.
 *   - The target entry address.
 *
 * This function does not return.
 */
ENTRY_PRIVATE(push_to_stack_and_jump)
  // Force unwinds to end in this function.
  .cfi_undefined %eip

  pop %eax  // Throw away the return address.
  pop %eax  // Pointer to data.
  pop %ebx  // Number of words to push.
  pop %ecx  // Address to jump to.

  // Add padding on the stack so that after pushing the data block onto the
  // stack, ESP is 12-mod-16 (not aligned to 16 bytes because we are not calling
  // the entry point but jumping to it.)
  mov %esp, %edx
  shrl $2, %edx
  subl %ebx, %edx
  andl $3, %edx
  addl $1, %edx
paddingbegin:
  test %edx, %edx
  jz loopbegin
  pushl $0
  dec %edx  // esp -= ((esp / 4 - ebx) % 4 + 1) * 4
  jmp paddingbegin

loopbegin:
  pushl -4(%eax, %ebx, 4)
  dec %ebx
  jnz loopbegin
  jmp *%ecx
END(push_to_stack_and_jump)
