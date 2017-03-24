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
# -- Begin  logb
ENTRY(logb)
# parameter 1: %xmm0
..B1.1:
..___tag_value_logb.1:
        subq      $24, %rsp
..___tag_value_logb.3:
        movsd     %xmm0, (%rsp)
..B1.2:
        psrlq     $52, %xmm0
        movd      %xmm0, %eax
        andl      $2047, %eax
        subl      $1, %eax
        cmpl      $2046, %eax
        jae       .L_2TAG_PACKET_0.0.2
.L_2TAG_PACKET_1.0.2:
        subl      $1022, %eax
        cvtsi2sd  %eax, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_0.0.2:
        movq      (%rsp), %xmm0
        movq      two_55(%rip), %xmm1
        pxor      %xmm2, %xmm2
        testl     %eax, %eax
        jg        .L_2TAG_PACKET_2.0.2
.L_2TAG_PACKET_3.0.2:
        comisd    %xmm2, %xmm0
        mulsd     %xmm1, %xmm0
        je        .L_2TAG_PACKET_4.0.2
.L_2TAG_PACKET_5.0.2:
        psrlq     $52, %xmm0
        movd      %xmm0, %eax
        andl      $2047, %eax
        subl      $1078, %eax
        cvtsi2sd  %eax, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_2.0.2:
        psrlq     $60, %xmm0
        movd      %xmm0, %eax
        movq      (%rsp), %xmm0
        andl      $8, %eax
        lea       ones(%rip), %r8
        movq      (%r8,%rax), %xmm1
        mulsd     %xmm1, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_4.0.2:
        movq      8+ones(%rip), %xmm0
        divsd     %xmm2, %xmm0
        movq      %xmm0, 8(%rsp)
..B1.3:
        movq      8(%rsp), %xmm0
.L_2TAG_PACKET_6.0.2:
..B1.5:
        addq      $24, %rsp
..___tag_value_logb.4:
        ret       
..___tag_value_logb.5:
END(logb)
# -- End  logb
	.section .rodata, "a"
	.align 16
	.align 16
ones:
	.long	0
	.long	1072693248
	.long	0
	.long	3220176896
	.type	ones,@object
	.size	ones,16
	.align 4
two_55:
	.long	0
	.long	1130364928
	.type	two_55,@object
	.size	two_55,8
	.data
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 1
	.4byte 0x00000014
	.8byte 0x00527a0100000000
	.8byte 0x08070c1b01107801
	.4byte 0x00000190
	.4byte 0x0000001c
	.4byte 0x0000001c
	.4byte ..___tag_value_logb.1-.
	.4byte ..___tag_value_logb.5-..___tag_value_logb.1
	.2byte 0x0400
	.4byte ..___tag_value_logb.3-..___tag_value_logb.1
	.2byte 0x200e
	.byte 0x04
	.4byte ..___tag_value_logb.4-..___tag_value_logb.3
	.2byte 0x080e
	.byte 0x00
# End
