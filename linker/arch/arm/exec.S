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
  // r0 = data, r1 = count, r2 = entry

  // Force unwinds to end in this function.
  .cfi_undefined r14

  // Add padding on the stack so that SP is aligned to 16 bytes after the data
  // block is pushed onto the stack.
  sub r4, sp, r1, lsl #2
  and r4, r4, #15
  tst r4, r4
  beq loopbegin
  mov r5, #0
paddingbegin:
  push {r5}
  subs r4, r4, #4
  bne paddingbegin

loopbegin:
  mov r3, r0  // r3 = data
  add r3, r3, r1, lsl #2  // r3 = data + count * 4
  sub r3, r3, #4  // r3 -= 4
  ldr r3, [r3]  // r3 = *r3 -> r3 = data[count-1]
  push {r3}
  subs r1, r1, #1
  bne loopbegin
  bx r2
END(push_to_stack_and_jump)
