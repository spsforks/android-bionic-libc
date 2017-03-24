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
# -- Begin  hypotf
ENTRY(hypotf)
# parameter 1: %xmm0
# parameter 2: %xmm1
..B1.1:
..___tag_value_hypotf.1:
        pushq     %rsi
..___tag_value_hypotf.3:
..B1.2:
        movl      $-1, %eax
        movd      %rax, %xmm4
        andps     %xmm4, %xmm0
        cvtps2pd  %xmm0, %xmm2
        andps     %xmm4, %xmm1
        cvtps2pd  %xmm1, %xmm3
        mulsd     %xmm2, %xmm2
        mulsd     %xmm3, %xmm3
        addsd     %xmm3, %xmm2
        sqrtsd    %xmm2, %xmm2
        movd      %xmm0, %eax
        movd      %xmm1, %ecx
        andl      $2147483647, %eax
        subl      $2135949311, %eax
        andl      $2147483647, %ecx
        subl      $2135949311, %ecx
        testl     %eax, %ecx
        jge       .L_2TAG_PACKET_0.0.2
        movq      $0x8000000100000000, %rdx
        movd      %rdx, %xmm0
        addl      $2127560703, %eax
        addl      $2127560703, %ecx
        testl     %ecx, %eax
        jl        .L_2TAG_PACKET_1.0.2
        psllq     $4, %xmm2
        paddd     %xmm2, %xmm0
        psrlq     $33, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_0.0.2:
        addl      $2135949311, %eax
        cmpl      $2139095040, %eax
        jae       .L_2TAG_PACKET_2.0.2
        addl      $2135949311, %ecx
        cmpl      $2139095040, %ecx
        jae       .L_2TAG_PACKET_3.0.2
        cvtpd2ps  %xmm2, %xmm0
        movd      %xmm0, %eax
        cmpl      $2139095040, %eax
        jne       .L_2TAG_PACKET_4.0.2
        jmp       .L_2TAG_PACKET_5.0.2
.L_2TAG_PACKET_4.0.2:
        jmp       ..B1.5
.L_2TAG_PACKET_3.0.2:
        movss     %xmm1, %xmm0
        mulss     %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_2.0.2:
        ja        .L_2TAG_PACKET_6.0.2
.L_2TAG_PACKET_7.0.2:
        mulss     %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_6.0.2:
        addl      $2135949311, %ecx
        cmpl      $2139095040, %ecx
        je        .L_2TAG_PACKET_3.0.2
        jb        .L_2TAG_PACKET_7.0.2
        jmp       .L_2TAG_PACKET_3.0.2
.L_2TAG_PACKET_1.0.2:
        cvtpd2ps  %xmm2, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_5.0.2:
        movss     %xmm0, (%rsp)
..B1.3:
        movss     (%rsp), %xmm0
.L_2TAG_PACKET_8.0.2:
..B1.5:
        popq      %rcx
..___tag_value_hypotf.4:
        ret       
..___tag_value_hypotf.5:
END(hypotf)
# -- End  hypotf
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
	.4byte ..___tag_value_hypotf.1-.
	.4byte ..___tag_value_hypotf.5-..___tag_value_hypotf.1
	.2byte 0x0400
	.4byte ..___tag_value_hypotf.3-..___tag_value_hypotf.1
	.2byte 0x100e
	.byte 0x04
	.4byte ..___tag_value_hypotf.4-..___tag_value_hypotf.3
	.2byte 0x080e
	.byte 0x00
# End
