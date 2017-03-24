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
# -- Begin  logbf
ENTRY(logbf)
# parameter 1: %xmm0
..B1.1:
..___tag_value_logbf.1:
        movd      %xmm0, %eax
        movss     %xmm0, -8(%rsp)
        movl      %eax, %edx
        andl      $2147483647, %edx
        addl      $-8388608, %edx
        cmpl      $2130706432, %edx
        jb        ..B1.7
..B1.2:
        testl     %edx, %edx
        jge       ..B1.6
..B1.3:
        cmpl      $-8388608, %edx
        je        ..B1.8
..B1.4:
        movss     -8(%rsp), %xmm0
        mulss     .L_2il0floatpacket.1(%rip), %xmm0
        movd      %xmm0, %eax
        movss     %xmm0, -8(%rsp)
        pxor      %xmm0, %xmm0
        andl      $2147483647, %eax
        shrl      $23, %eax
        addl      $-152, %eax
        cvtsi2ss  %eax, %xmm0
..B1.5:
        ret       
..B1.6:
        shrl      $31, %eax
        lea       _ones(%rip), %rdx
        movss     (%rdx,%rax,4), %xmm1
        mulss     %xmm1, %xmm0
        ret       
..B1.7:
        shrl      $23, %edx
        pxor      %xmm0, %xmm0
        addl      $-126, %edx
        cvtsi2ss  %edx, %xmm0
        ret       
..B1.8:
        movss     .L_2il0floatpacket.0(%rip), %xmm0
        pxor      %xmm1, %xmm1
        divss     %xmm1, %xmm0
        ret       
..___tag_value_logbf.3:
END(logbf)
# -- End  logbf
	.section .rodata, "a"
	.align 4
	.align 4
.L_2il0floatpacket.0:
	.long	0xbf800000
	.type	.L_2il0floatpacket.0,@object
	.size	.L_2il0floatpacket.0,4
	.align 4
.L_2il0floatpacket.1:
	.long	0x4c000000
	.type	.L_2il0floatpacket.1,@object
	.size	.L_2il0floatpacket.1,4
	.align 4
_ones:
	.long	1065353216
	.long	3212836864
	.type	_ones,@object
	.size	_ones,8
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
	.4byte ..___tag_value_logbf.1-.
	.4byte ..___tag_value_logbf.3-..___tag_value_logbf.1
	.8byte 0x0000000000000000
# End
