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

// This function must have exactly this instruction sequence for gdb and libunwind.
// This function must have exactly this name for gdb.

.globl __restore
.align 16
.LSTART:
.type __restore, @function
__restore:
  popl %eax
  movl $__NR_sigreturn, %eax
  int $0x80
.LEND:

// We should have special unwinding info for __restore_rt for unwinding through signal purpose.
// The only way to have it with the content we want is hand-written assembly below.

// These magic constants are offsetof(struct sigcontext, eax) + 4
// It is not clear why + 4, but kernel version has it and it is required for unwinding.
// Calculations on the fly is tricky so keep it as constants. It should not change in future.

#define OFFSET_EDI 16 + 4
#define OFFSET_ESI 20 + 4
#define OFFSET_EBP 24 + 4
#define OFFSET_ESP 28 + 4
#define OFFSET_EBX 32 + 4
#define OFFSET_EDX 36 + 4
#define OFFSET_ECX 40 + 4
#define OFFSET_EAX 44 + 4
#define OFFSET_EIP 56 + 4

.section .eh_frame,"a",@progbits
.LSTARTFRAME:
  .long .LENDCIE-.LSTARTCIE  /* CIE length */

/* CIE */
.LSTARTCIE:
  .long 0       /* ID */
  .byte 1       /* Version */
  .string "zRS" /* Augmentation string */
  .uleb128 1    /* Code alignment factor */
  .sleb128 -4	/* Data alignment factor */
  .uleb128 8   /* Return address register */
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
  .byte 0x74                    /* DW_OP_breg4 */
  .sleb128 OFFSET_ESP
  .byte 0x06                    /* DW_OP_deref */
.LDEFEXPREND:

/* EDI */
  .byte 0x10
  .uleb128 7
  .uleb128 2f-1f
1:.byte 0x74
  .sleb128 OFFSET_EDI
2:

/* ESI */
  .byte 0x10
  .uleb128 6
  .uleb128 2f-1f
1:.byte 0x74
  .sleb128 OFFSET_ESI
2:

/* EBP */
  .byte 0x10
  .uleb128 5
  .uleb128 2f-1f
1:.byte 0x74
  .sleb128 OFFSET_EBP
2:

/* EBX */
  .byte 0x10
  .uleb128 3
  .uleb128 2f-1f
1:.byte 0x74
  .sleb128 OFFSET_EBX
2:

/* EDX */
  .byte 0x10
  .uleb128 2
  .uleb128 2f-1f
1:.byte 0x74
  .sleb128 OFFSET_EDX
2:

/* EAX */
  .byte 0x10
  .uleb128 0
  .uleb128 2f-1f
1:.byte 0x74
  .sleb128 OFFSET_EAX
2:

/* ECX */
  .byte 0x10
  .uleb128 1
  .uleb128 2f-1f
1:.byte 0x74
  .sleb128 OFFSET_ECX
2:

/* EIP */
  .byte 0x10
  .uleb128 8
  .uleb128 2f-1f
1:.byte 0x74
  .sleb128 OFFSET_EIP
2:

// kernel version of unwinding info repeats here FDE actions above once again
// looks like it is not required for unwinding

.align 8
.LENDFDE:

