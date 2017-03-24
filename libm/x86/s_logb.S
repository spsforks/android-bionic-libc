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
# -- Begin  logb
ENTRY(logb)
# parameter 1: 8 + %ebp
..B2.1:
..B2.2:
        pushl     %ebp
        movl      %esp, %ebp
        subl      $104, %esp
        movl      %ebx, 48(%esp)
        call      static_func
        movl      %eax, %ebx
        movsd     112(%esp), %xmm0
        movq      %xmm0, 8(%esp)
        psrlq     $52, %xmm0
        movd      %xmm0, %eax
        andl      $2047, %eax
        subl      $1, %eax
        cmpl      $2046, %eax
        jae       .L_2TAG_PACKET_0.0.2
        subl      $1022, %eax
        movl      %eax, (%esp)
        fildl     (%esp)
        jmp       .L_2TAG_PACKET_1.0.2
.L_2TAG_PACKET_0.0.2:
        movq      8(%esp), %xmm0
        movq      16(%ebx), %xmm1
        pxor      %xmm2, %xmm2
        testl     %eax, %eax
        jg        .L_2TAG_PACKET_2.0.2
        comisd    %xmm2, %xmm0
        mulsd     %xmm1, %xmm0
        je        .L_2TAG_PACKET_3.0.2
        psrlq     $52, %xmm0
        movd      %xmm0, %eax
        andl      $2047, %eax
        subl      $1078, %eax
        movl      %eax, (%esp)
        fildl     (%esp)
        jmp       .L_2TAG_PACKET_1.0.2
.L_2TAG_PACKET_2.0.2:
        psrlq     $60, %xmm0
        fldl      8(%esp)
        movd      %xmm0, %eax
        andl      $8, %eax
        fmull     (%ebx,%eax)
        jmp       .L_2TAG_PACKET_1.0.2
.L_2TAG_PACKET_3.0.2:
        movq      8(%ebx), %xmm0
        divsd     %xmm2, %xmm0
        movq      %xmm0, (%esp)
        fldl      (%esp)
.L_2TAG_PACKET_1.0.2:
        movl      48(%esp), %ebx
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B2.3:
END(logb)
# -- End  logb
	.text
# Start file scope ASM
ALIAS_SYMBOL(logbl, logb);
# End file scope ASM
	.section .rodata, "a"
	.align 16
	.align 16
static_const_table:
	.long	0
	.long	1072693248
	.long	0
	.long	3220176896
	.long	0
	.long	1130364928
	.long	0
	.long	0
	.type	static_const_table,@object
	.size	static_const_table,32
	.data
	.section .note.GNU-stack, ""
# End
