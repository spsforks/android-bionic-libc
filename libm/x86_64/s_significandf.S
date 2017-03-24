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
# -- Begin  significandf
ENTRY(significandf)
# parameter 1: %xmm0
..B1.1:
..___tag_value_significandf.1:
..___tag_value_significandf.3:
..B1.2:
        pextrw    $1, %xmm0, %eax
        andl      $32640, %eax
        subl      $128, %eax
        cmpl      $32512, %eax
        jae       .L_2TAG_PACKET_0.0.1
.L_2TAG_PACKET_1.0.1:
        andps     MANTISSA_MASK(%rip), %xmm0
        orps      ONE(%rip), %xmm0
.L_2TAG_PACKET_2.0.1:
        ret       
.L_2TAG_PACKET_0.0.1:
        je        .L_2TAG_PACKET_3.0.1
        pxor      %xmm3, %xmm3
        ucomiss   %xmm3, %xmm0
        je        .L_2TAG_PACKET_2.0.1
        mulss     TWO_25(%rip), %xmm0
        jmp       .L_2TAG_PACKET_1.0.1
.L_2TAG_PACKET_3.0.1:
        ucomiss   %xmm0, %xmm0
        jpo       .L_2TAG_PACKET_2.0.1
.L_2TAG_PACKET_4.0.1:
        xorps     %xmm1, %xmm1
        addss     %xmm1, %xmm0
        ret       
..B1.3:
..___tag_value_significandf.4:
END(significandf)
# -- End  significandf
	.section .rodata, "a"
	.align 16
	.align 16
MANTISSA_MASK:
	.long	2155872255
	.long	0
	.long	0
	.long	0
	.type	MANTISSA_MASK,@object
	.size	MANTISSA_MASK,16
	.align 16
ONE:
	.long	1065353216
	.long	0
	.long	0
	.long	0
	.type	ONE,@object
	.size	ONE,16
	.align 16
TWO_25:
	.long	1275068416
	.long	0
	.long	0
	.long	0
	.type	TWO_25,@object
	.size	TWO_25,16
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
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte ..___tag_value_significandf.1-.
	.4byte ..___tag_value_significandf.4-..___tag_value_significandf.1
	.2byte 0x0400
	.4byte ..___tag_value_significandf.3-..___tag_value_significandf.1
	.2byte 0x100e
# End
