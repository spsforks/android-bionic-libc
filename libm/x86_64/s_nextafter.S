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
# -- Begin  nextafter
ENTRY(nextafter)
# parameter 1: %xmm0
# parameter 2: %xmm1
..B1.1:
..___tag_value_nextafter.1:
        subq      $24, %rsp
..___tag_value_nextafter.3:
..B1.2:
        ucomisd   %xmm1, %xmm0
        jp        .L_2TAG_PACKET_0.0.2
        je        .L_2TAG_PACKET_1.0.2
        cmpltsd   %xmm0, %xmm1
        comisd    ZERO(%rip), %xmm0
        je        .L_2TAG_PACKET_2.0.2
        xorpd     %xmm0, %xmm1
        paddq     INTEGER_ONE(%rip), %xmm0
        psrlq     $63, %xmm1
        psllq     $1, %xmm1
        psubq     %xmm1, %xmm0
        pextrw    $3, %xmm0, %eax
        andl      $32767, %eax
        subl      $16, %eax
        cmpl      $32736, %eax
        jae       .L_2TAG_PACKET_3.0.2
        jmp       ..B1.5
.L_2TAG_PACKET_0.0.2:
        addsd     %xmm1, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_1.0.2:
        movapd    %xmm1, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_2.0.2:
        movq      MIN_SUBNORMAL(%rip), %xmm0
        andpd     SIGN_MASK(%rip), %xmm1
        orpd      %xmm1, %xmm0
        jmp       .L_2TAG_PACKET_4.0.2
.L_2TAG_PACKET_3.0.2:
        testl     %eax, %eax
        jns       .L_2TAG_PACKET_5.0.2
.L_2TAG_PACKET_4.0.2:
        movq      SMALL_VALUE(%rip), %xmm1
        movl      $268, 8(%rsp)
        jmp       .L_2TAG_PACKET_6.0.2
.L_2TAG_PACKET_5.0.2:
        movq      LARGE_VALUE(%rip), %xmm1
        movl      $154, 8(%rsp)
.L_2TAG_PACKET_6.0.2:
        mulsd     %xmm1, %xmm1
        movq      %xmm0, (%rsp)
..B1.3:
        movq      (%rsp), %xmm0
.L_2TAG_PACKET_7.0.2:
..B1.5:
        addq      $24, %rsp
..___tag_value_nextafter.4:
        ret       
..___tag_value_nextafter.5:
END(nextafter)
# -- End  nextafter
	.section .rodata, "a"
	.align 16
	.align 16
INTEGER_ONE:
	.long	1
	.long	0
	.long	1
	.long	0
	.type	INTEGER_ONE,@object
	.size	INTEGER_ONE,16
	.align 16
SIGN_MASK:
	.long	0
	.long	-2147483648
	.long	0
	.long	-2147483648
	.type	SIGN_MASK,@object
	.size	SIGN_MASK,16
	.align 8
ZERO:
	.long	0
	.long	0
	.type	ZERO,@object
	.size	ZERO,8
	.align 8
MIN_SUBNORMAL:
	.long	1
	.long	0
	.type	MIN_SUBNORMAL,@object
	.size	MIN_SUBNORMAL,8
	.align 8
SMALL_VALUE:
	.long	0
	.long	24117248
	.type	SMALL_VALUE,@object
	.size	SMALL_VALUE,8
	.align 8
LARGE_VALUE:
	.long	0
	.long	2121269248
	.type	LARGE_VALUE,@object
	.size	LARGE_VALUE,8
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
	.4byte ..___tag_value_nextafter.1-.
	.4byte ..___tag_value_nextafter.5-..___tag_value_nextafter.1
	.2byte 0x0400
	.4byte ..___tag_value_nextafter.3-..___tag_value_nextafter.1
	.2byte 0x200e
	.byte 0x04
	.4byte ..___tag_value_nextafter.4-..___tag_value_nextafter.3
	.2byte 0x080e
	.byte 0x00
# End
