/*
Copyright (c) 2014, Intel Corporation
All rights reserved.
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright notice,
    * this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
    * this list of conditions and the following disclaimer in the documentation
    * and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
    * may be used to endorse or promote products derived from this software
    * without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <private/bionic_asm.h>

	.text
..TXTST0:
# -- Begin  static_func
        .text
        .align __bionic_asm_align
        .type static_func, @function
static_func:
..B1.1:
        call      ..L2
..L2:
        popl      %eax
        lea       _GLOBAL_OFFSET_TABLE_+[. - ..L2](%eax), %eax
        lea       static_const_table@GOTOFF(%eax), %eax
        ret
        .size   static_func,.-static_func
# -- End  static_func
	.text
# -- Begin  roundf
ENTRY(roundf)
# parameter 1: 8 + %ebp
..B2.1:
..B2.2:
        pushl     %ebp
        movl      %esp, %ebp
        subl      $104, %esp
        movl      %ebx, 40(%esp)
        call      static_func
        movl      %eax, %ebx
        movss     112(%esp), %xmm0
        movl      112(%esp), %eax
        andl      $2147483647, %eax
        movss     %xmm0, %xmm1
        movss     %xmm0, %xmm2
        cmpl      $2139095040, %eax
        pand      (%ebx), %xmm2
        movss     %xmm2, %xmm3
        roundss   $0, %xmm0, %xmm0
        jge       .L_2TAG_PACKET_0.0.2
        subss     %xmm0, %xmm1
        pxor      %xmm2, %xmm1
        por       16(%ebx), %xmm2
        cmpeqss   32(%ebx), %xmm1
        pand      %xmm2, %xmm1
        addss     %xmm1, %xmm0
        por       %xmm3, %xmm0
.L_2TAG_PACKET_0.0.2:
        movss     %xmm0, 24(%esp)
        flds      24(%esp)
        movl      40(%esp), %ebx
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B2.3:
END(roundf)
# -- End  roundf
	.section .rodata, "a"
	.align 16
	.align 16
static_const_table:
	.long	2147483648
	.long	0
	.long	0
	.long	0
	.long	1065353216
	.long	0
	.long	0
	.long	0
	.long	1056964608
	.long	0
	.long	0
	.long	0
	.type	static_const_table,@object
	.size	static_const_table,48
	.data
	.section .note.GNU-stack, ""
# End
