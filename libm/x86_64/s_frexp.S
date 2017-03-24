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
# -- Begin  frexp
ENTRY(frexp)
# parameter 1: %xmm0
# parameter 2: %rdi
..B1.1:
..___tag_value_frexp.1:
..___tag_value_frexp.3:
..B1.2:
        pextrw    $3, %xmm0, %eax
        movq      clear_exponent(%rip), %xmm1
        andl      $32767, %eax
        movq      set_exponent(%rip), %xmm2
        subl      $16, %eax
        js        .L_2TAG_PACKET_0.0.1
        cmpl      $32736, %eax
        jae       .L_2TAG_PACKET_1.0.1
.L_2TAG_PACKET_2.0.1:
        shrl      $4, %eax
        pand      %xmm1, %xmm0
        subl      $1021, %eax
        por       %xmm2, %xmm0
        movl      %eax, (%rdi)
        ret       
.L_2TAG_PACKET_0.0.1:
        pxor      %xmm3, %xmm3
        movq      two_55(%rip), %xmm4
        xorl      %eax, %eax
        comisd    %xmm3, %xmm0
        je        .L_2TAG_PACKET_3.0.1
        mulsd     %xmm4, %xmm0
        pextrw    $3, %xmm0, %eax
        andl      $32767, %eax
        pand      %xmm1, %xmm0
        shrl      $4, %eax
        por       %xmm2, %xmm0
        subl      $1077, %eax
.L_2TAG_PACKET_3.0.1:
        movl      %eax, (%rdi)
        ret       
.L_2TAG_PACKET_1.0.1:
        xorl      %eax, %eax
        movsd     set_exponent(%rip), %xmm2
        movl      %eax, (%rdi)
        mulsd     %xmm2, %xmm0
        ret       
..B1.3:
..___tag_value_frexp.4:
END(frexp)
# -- End  frexp
	.section .rodata, "a"
	.align 4
	.align 4
clear_exponent:
	.long	4294967295
	.long	2148532223
	.type	clear_exponent,@object
	.size	clear_exponent,8
	.align 4
set_exponent:
	.long	0
	.long	1071644672
	.type	set_exponent,@object
	.size	set_exponent,8
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
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte ..___tag_value_frexp.1-.
	.4byte ..___tag_value_frexp.4-..___tag_value_frexp.1
	.2byte 0x0400
	.4byte ..___tag_value_frexp.3-..___tag_value_frexp.1
	.2byte 0x100e
# End
