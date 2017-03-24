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
# -- Begin  roundf
ENTRY(roundf)
# parameter 1: %xmm0
..B1.1:
..___tag_value_roundf.1:
..___tag_value_roundf.3:
..B1.2:
        movd      %xmm0, %eax
        andl      $2147483647, %eax
        movss     %xmm0, %xmm1
        movss     %xmm0, %xmm2
        cmpl      $2139095040, %eax
        pand      sign_mask(%rip), %xmm2
        movss     %xmm2, %xmm3
        roundss   $0, %xmm0, %xmm0
        jge       .L_2TAG_PACKET_0.0.1
        subss     %xmm0, %xmm1
        pxor      %xmm2, %xmm1
        por       p_one(%rip), %xmm2
        cmpeqss   p_half(%rip), %xmm1
        pand      %xmm2, %xmm1
        addss     %xmm1, %xmm0
        por       %xmm3, %xmm0
.L_2TAG_PACKET_0.0.1:
        ret       
..B1.3:
..___tag_value_roundf.4:
END(roundf)
# -- End  roundf
	.section .rodata, "a"
	.align 16
	.align 16
sign_mask:
	.long	2147483648
	.type	sign_mask,@object
	.size	sign_mask,4
	.space 12, 0x00 	# pad
	.align 16
p_one:
	.long	1065353216
	.type	p_one,@object
	.size	p_one,4
	.space 12, 0x00 	# pad
	.align 16
p_half:
	.long	1056964608
	.type	p_half,@object
	.size	p_half,4
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
	.4byte ..___tag_value_roundf.1-.
	.4byte ..___tag_value_roundf.4-..___tag_value_roundf.1
	.2byte 0x0400
	.4byte ..___tag_value_roundf.3-..___tag_value_roundf.1
	.2byte 0x100e
# End
