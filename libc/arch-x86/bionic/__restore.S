/*
 * Copyright (C) 2014 The Android Open Source Project
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

#define DW_OP_deref 0x06
#define DW_CFA_def_cfa_expression 0x0f
#define DW_CFA_expression 0x10
#define DW_OP_breg4 0x74

// Offsets into struct sigcontext.
#define OFFSET_EDI 16
#define OFFSET_ESI 20
#define OFFSET_EBP 24
#define OFFSET_ESP 28
#define OFFSET_EBX 32
#define OFFSET_EDX 36
#define OFFSET_ECX 40
#define OFFSET_EAX 44
#define OFFSET_EIP 56

#define cfa_reg(reg_number,offset) \
  .byte DW_CFA_expression; \
  .uleb128 reg_number; \
  .uleb128 2f-1f; \
1:.byte DW_OP_breg4; \
  .sleb128 offset; \
2: \

// This function must have exactly this instruction sequence for gdb and libunwind.
// This function must have exactly this name for gdb.

ENTRY_PRIVATE(__restore)
.L__restore_start:
  popl %eax
  movl $__NR_sigreturn, %eax
  int $0x80
.L__restore_end:
END(__restore)

// We should have special unwinding info for __restore_rt for unwinding through signal purpose.
// The only way to have it with the content we want is hand-written assembly below.

.section .eh_frame,"a",@progbits
.L__restore_START_EH_FRAME:
  .long 2f-1f   // CIE length
1:.long 0       // CIE ID.
  .byte 1       // Version.
  .string "zRS" // Augmentation string.
  .uleb128 1    // Code alignment factor.
  .sleb128 -4   // Data alignment factor.
  .uleb128 8    // Return address register.
  .uleb128 1    // 1 byte of augmentation data.
  .byte 0x1b    // FDE encoding: DW_EH_PE_pcrel|DW_EH_PE_sdata4.
  .align 8
2:

  // FDE.
  .long .L__restore_END_FDE-.L__restore_START_FDE      // FDE length
.L__restore_START_FDE:
  .long .L__restore_START_FDE-.L__restore_START_EH_FRAME  // CIE location
  .long (.L__restore_start-1)-.           // pcrel start address (see FDE encoding above)
  .long .L__restore_end-(.L__restore_start-1)      // Function this FDE applies to
  .uleb128 0                     // FDE augmentation length

  .byte DW_CFA_def_cfa_expression
  .uleb128 2f-1f
1:.byte DW_OP_breg4
  .sleb128 OFFSET_ESP + 4
  .byte DW_OP_deref
2:

  cfa_reg(8, OFFSET_EIP + 4)
  cfa_reg(7, OFFSET_EDI + 4)
  cfa_reg(6, OFFSET_ESI + 4)
  cfa_reg(5, OFFSET_EBP + 4)
  cfa_reg(3, OFFSET_EBX + 4)
  cfa_reg(2, OFFSET_EDX + 4)
  cfa_reg(1, OFFSET_ECX + 4)
  cfa_reg(0, OFFSET_EAX + 4)

.L__restore_END_FDE:

ENTRY_PRIVATE(__restore_rt)
.L_start:
  movl $__NR_rt_sigreturn, %eax
  int $0x80
.L_end:
END(__restore_rt)

.section .eh_frame,"a",@progbits
.LSTART_EH_FRAME:
  .long 2f-1f   // CIE length
1:.long 0       // CIE ID.
  .byte 1       // Version.
  .string "zRS" // Augmentation string.
  .uleb128 1    // Code alignment factor.
  .sleb128 -4   // Data alignment factor.
  .uleb128 8    // Return address register.
  .uleb128 1    // 1 byte of augmentation data.
  .byte 0x1b    // FDE encoding: DW_EH_PE_pcrel|DW_EH_PE_sdata4.
  .align 8
2:

  // FDE.
  .long .LEND_FDE-.LSTART_FDE      // FDE length
.LSTART_FDE:
  .long .LSTART_FDE-.LSTART_EH_FRAME  // CIE location
  .long (.L_start-1)-.           // pcrel start address (see FDE encoding above)
  .long .L_end-(.L_start-1)      // Function this FDE applies to
  .uleb128 0                     // FDE augmentation length

  .byte DW_CFA_def_cfa_expression
  .uleb128 2f-1f
1:.byte DW_OP_breg4
  .sleb128 OFFSET_ESP
  .byte DW_OP_deref
2:

  cfa_reg(8, OFFSET_EIP)
  cfa_reg(7, OFFSET_EDI)
  cfa_reg(6, OFFSET_ESI)
  cfa_reg(5, OFFSET_EBP)
  cfa_reg(3, OFFSET_EBX)
  cfa_reg(2, OFFSET_EDX)
  cfa_reg(1, OFFSET_ECX)
  cfa_reg(0, OFFSET_EAX)

.LEND_FDE:
