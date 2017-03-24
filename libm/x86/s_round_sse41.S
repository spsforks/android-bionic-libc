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
# -- Begin  round
ENTRY(round)
# parameter 1: 8 + %ebp
..B2.1:
..B2.2:
        pushl     %ebp
        movl      %esp, %ebp
        subl      $104, %esp
        movl      %ebx, 40(%esp)
        call      static_func
        movl      %eax, %ebx
        pxor      %xmm0, %xmm0
        movsd     112(%esp), %xmm0
        pextrw    $3, %xmm0, %ecx
        movl      $32752, %eax
        movsd     %xmm0, %xmm1
        movsd     %xmm0, %xmm2
        andl      %ecx, %eax
        cmpl      $32752, %eax
        pand      (%ebx), %xmm2
        movsd     %xmm2, %xmm3
        roundsd   $0, %xmm0, %xmm0
        jge       .L_2TAG_PACKET_0.0.2
        subsd     %xmm0, %xmm1
        pxor      %xmm2, %xmm1
        por       16(%ebx), %xmm2
        cmpeqsd   32(%ebx), %xmm1
        pand      %xmm2, %xmm1
        addsd     %xmm1, %xmm0
        por       %xmm3, %xmm0
.L_2TAG_PACKET_0.0.2:
        movsd     %xmm0, 24(%esp)
        fldl      24(%esp)
        movl      40(%esp), %ebx
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B2.3:
END(round)
# -- End  round
	.section .rodata, "a"
	.align 16
	.align 16
static_const_table:
	.long	0
	.long	2147483648
	.long	0
	.long	2147483648
	.long	0
	.long	1072693248
	.long	0
	.long	1072693248
	.long	0
	.long	1071644672
	.long	0
	.long	1071644672
	.type	static_const_table,@object
	.size	static_const_table,48
	.data
	.section .note.GNU-stack, ""
# End
