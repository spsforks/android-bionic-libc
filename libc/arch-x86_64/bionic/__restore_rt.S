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

// This function must have exactly this instruction sequence for gdb and libunwind.
// This function must have exactly this name for gdb.
// Labels should start with .L so that they are excluded from symbols

  nop
.globl __restore_rt
.align 16
.LSTART:
.type __restore_rt, @function
__restore_rt:
  mov $__NR_rt_sigreturn, %rax
  syscall
.LEND:

// We should have special unwinding info for __restore_rt for unwinding through signal purpose.
// The only way to have it with the content we want is hand-written assembly below.

// These magic constants are offsetof(ucontext_t, uc_mcontext.gregs[REG_XXX])
// Calculations on the fly is tricky so keep it as constants. It should not change in future.

#define OFFSET_R8 40
#define OFFSET_R9 48
#define OFFSET_R10 56
#define OFFSET_R11 64
#define OFFSET_R12 72
#define OFFSET_R13 80
#define OFFSET_R14 88
#define OFFSET_R15 96
#define OFFSET_RDI 104
#define OFFSET_RSI 112
#define OFFSET_RBP 120
#define OFFSET_RSP 160
#define OFFSET_RBX 128
#define OFFSET_RDX 136
#define OFFSET_RAX 144
#define OFFSET_RCX 152
#define OFFSET_RIP 168

.section .eh_frame,"a",@progbits
.LSTARTFRAME:
  .long .LENDCIE-.LSTARTCIE  /* CIE length */

/* CIE */
.LSTARTCIE:
  .long 0       /* ID */
  .byte 1       /* Version */
  .string "zRS" /* Augmentation string */
  .uleb128 1    /* Code alignment factor */
  .sleb128 -8   /* Data alignment factor */
  .uleb128 16   /* Return address register */
  .uleb128 .LENDAUG-.LSTARTAUG /* Augmentation data length */
.LSTARTAUG:
  .byte 0x1b    /* FDE encoding: DW_EH_PE_pcrel|DW_EH_PE_sdata4. */
.LENDAUG:
.align 8
.LENDCIE:

/* FDE */
  .long .LENDFDE-.LSTARTFDE     /* FDE length */
.LSTARTFDE:
  .long .LSTARTFDE-.LSTARTFRAME /* CIE location */
  .long (.LSTART-1)-.           /* pcrel start address (see FDE encoding above) */
  .long .LEND-(.LSTART-1)       /* Function this FDE applies to */
  .uleb128 0                    /* FDE augmentation length */

/* FDE data */

  .byte 0x0f                    /* DW_CFA_def_cfa_expression */
  .uleb128 .LDEFEXPREND-.LDEFEXPRSTART  /* Expression length */
.LDEFEXPRSTART:
  .byte 0x77                    /* DW_OP_breg7 */
  .sleb128 OFFSET_RSP
  .byte 0x06                    /* DW_OP_deref */
.LDEFEXPREND:

/* R8 */
  .byte 0x10                    /* DW_CFA_expression */
  .uleb128 8                    /* Register */
  .uleb128 2f-1f                /* length */
1:.byte 0x77                    /* DW_OP_breg7 */
  .sleb128 OFFSET_R8            /* Register offset */
2:

/* R9 */
  .byte 0x10
  .uleb128 9
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_R9
2:

/* R10 */
  .byte 0x10
  .uleb128 10
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_R10
2:

/* R11 */
  .byte 0x10
  .uleb128 11
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_R11
2:

/* R12 */
  .byte 0x10
  .uleb128 12
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_R12
2:

/* R13 */
  .byte 0x10
  .uleb128 13
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_R13
2:

/* R14 */
  .byte 0x10
  .uleb128 14
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_R14
2:

/* R15 */
  .byte 0x10
  .uleb128 15
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_R15
2:

/* RDI */
  .byte 0x10
  .uleb128 5
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_RDI
2:

/* RSI */
  .byte 0x10
  .uleb128 4
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_RSI
2:

/* RBP */
  .byte 0x10
  .uleb128 6
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_RBP
2:

/* RBX */
  .byte 0x10
  .uleb128 3
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_RBX
2:

/* RDX */
  .byte 0x10
  .uleb128 1
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_RDX
2:

/* RAX */
  .byte 0x10
  .uleb128 0
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_RAX
2:

/* RCX */
  .byte 0x10
  .uleb128 2
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_RCX
2:

/* RSP */
  .byte 0x10
  .uleb128 7
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_RSP
2:

/* RIP */
  .byte 0x10
  .uleb128 16
  .uleb128 2f-1f
1:.byte 0x77
  .sleb128 OFFSET_RIP
2:

.align 8
.LENDFDE:

