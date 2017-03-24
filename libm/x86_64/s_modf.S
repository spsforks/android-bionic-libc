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
# -- Begin  modf
ENTRY(modf)
# parameter 1: %xmm0
# parameter 2: %rdi
..B1.1:
..___tag_value_modf.1:
..___tag_value_modf.3:
..B1.2:
        movq      shift_base(%rip), %xmm2
        xorpd     %xmm4, %xmm4
        movdqa    %xmm0, %xmm1
        psrlq     $52, %xmm1
        movq      exponent_mask(%rip), %xmm3
        addsd     %xmm0, %xmm4
        movd      %xmm1, %eax
        andl      $2047, %eax
        subl      $1023, %eax
        js        .L_2TAG_PACKET_0.0.1
        cmpl      $52, %eax
        jae       .L_2TAG_PACKET_1.0.1
        pand      %xmm3, %xmm1
        movdqa    %xmm0, %xmm4
        psubd     %xmm1, %xmm2
        psrlq     %xmm2, %xmm4
        psllq     %xmm2, %xmm4
        movq      %xmm4, (%rdi)
        movq      sign_mask(%rip), %xmm5
        pand      %xmm0, %xmm5
        subsd     %xmm4, %xmm0
        por       %xmm5, %xmm0
        ret       
.L_2TAG_PACKET_0.0.1:
        movq      sign_mask(%rip), %xmm1
        pand      %xmm0, %xmm1
        movq      %xmm1, (%rdi)
        ret       
.L_2TAG_PACKET_1.0.1:
        ucomisd   %xmm0, %xmm0
        movq      sign_mask(%rip), %xmm1
        jp        .L_2TAG_PACKET_2.0.1
        pand      %xmm0, %xmm1
        movq      %xmm0, (%rdi)
        movq      %xmm1, %xmm0
        ret       
.L_2TAG_PACKET_2.0.1:
        mulsd     %xmm0, %xmm0
        movq      %xmm0, (%rdi)
        ret       
..B1.3:
..___tag_value_modf.4:
END(modf)
# -- End  modf
	.section .rodata, "a"
	.align 8
	.align 8
shift_base:
	.long	1075
	.long	0
	.type	shift_base,@object
	.size	shift_base,8
	.align 8
exponent_mask:
	.long	2047
	.long	0
	.type	exponent_mask,@object
	.size	exponent_mask,8
	.align 8
sign_mask:
	.long	0
	.long	2147483648
	.type	sign_mask,@object
	.size	sign_mask,8
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
	.4byte ..___tag_value_modf.1-.
	.4byte ..___tag_value_modf.4-..___tag_value_modf.1
	.2byte 0x0400
	.4byte ..___tag_value_modf.3-..___tag_value_modf.1
	.2byte 0x100e
# End
