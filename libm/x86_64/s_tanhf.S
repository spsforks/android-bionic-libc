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
# -- Begin  tanhf
ENTRY(tanhf)
# parameter 1: %xmm0
..B1.1:
..___tag_value_tanhf.1:
        subq      $24, %rsp
..___tag_value_tanhf.3:
        movss     %xmm0, 8(%rsp)
..B1.2:
        xorpd     %xmm1, %xmm1
        movq      L2E(%rip), %xmm3
        movl      $14336, %eax
        pinsrw    $3, %eax, %xmm1
        movq      Shifter(%rip), %xmm6
        pextrw    $1, %xmm0, %ecx
        psllq     $33, %xmm0
        psrlq     $4, %xmm0
        paddw     %xmm0, %xmm1
        movapd    cv(%rip), %xmm4
        movl      $32768, %edx
        andl      %ecx, %edx
        andl      $32767, %ecx
        subl      $15744, %ecx
        cmpl      $1056, %ecx
        jae       .L_2TAG_PACKET_0.0.1
        mulsd     %xmm1, %xmm3
        cvtsd2si  %xmm3, %eax
        movq      %xmm3, %xmm2
        addsd     %xmm6, %xmm3
        xorpd     %xmm0, %xmm0
        subsd     %xmm6, %xmm3
        movapd    16+cv(%rip), %xmm1
        subsd     %xmm3, %xmm2
        movl      $31, %ecx
        andl      %eax, %ecx
        lea       T2_neg_f(%rip), %r8
        movq      (%r8,%rcx,8), %xmm5
        shrl      $1, %eax
        andl      $65520, %eax
        subl      $16368, %eax
        negl      %eax
        pshufd    $68, %xmm2, %xmm2
        pinsrw    $3, %eax, %xmm0
        mulpd     %xmm2, %xmm4
        mulpd     %xmm2, %xmm1
        movq      ONEMASK(%rip), %xmm7
        xorpd     %xmm3, %xmm3
        mulpd     %xmm2, %xmm4
        mulpd     %xmm2, %xmm2
        mulsd     %xmm5, %xmm0
        addpd     %xmm4, %xmm1
        pinsrw    $3, %edx, %xmm3
        mulsd     %xmm2, %xmm1
        movq      %xmm7, %xmm6
        pshufd    $238, %xmm1, %xmm2
        addsd     %xmm0, %xmm7
        addsd     %xmm2, %xmm1
        subsd     %xmm0, %xmm6
        mulsd     %xmm1, %xmm0
        addsd     %xmm0, %xmm7
        subsd     %xmm0, %xmm6
        xorpd     %xmm3, %xmm7
        divsd     %xmm7, %xmm6
        cvtpd2ps  %xmm6, %xmm0
        jmp       ..B1.4
.L_2TAG_PACKET_0.0.1:
        addl      $1152, %ecx
        cmpl      $1152, %ecx
        jae       .L_2TAG_PACKET_1.0.1
        movl      $-1117975087, %eax
        movd      %eax, %xmm1
        movss     8(%rsp), %xmm0
        movl      $-1096111445, %ecx
        movd      %ecx, %xmm3
        movl      $1040746633, %edx
        movd      %edx, %xmm2
        pshufd    $68, %xmm0, %xmm4
        mulss     %xmm0, %xmm0
        mulss     %xmm0, %xmm1
        mulss     %xmm0, %xmm3
        mulss     %xmm0, %xmm0
        addss     %xmm2, %xmm1
        mulss     %xmm1, %xmm0
        addss     %xmm3, %xmm0
        mulss     %xmm4, %xmm0
        addss     %xmm4, %xmm0
        jmp       ..B1.4
.L_2TAG_PACKET_1.0.1:
        addl      $14592, %ecx
        cmpl      $15744, %ecx
        jae       .L_2TAG_PACKET_2.0.1
        movss     8(%rsp), %xmm0
        cmpl      $128, %ecx
        jb        .L_2TAG_PACKET_3.0.1
        movl      $1333788672, %eax
        movd      %eax, %xmm2
        mulss     %xmm0, %xmm2
        addss     %xmm0, %xmm2
        jmp       ..B1.4
.L_2TAG_PACKET_3.0.1:
        movss     %xmm0, %xmm2
        mulss     %xmm2, %xmm2
        jmp       ..B1.4
.L_2TAG_PACKET_2.0.1:
        cmpl      $32640, %ecx
        jae       .L_2TAG_PACKET_4.0.1
        movl      $796917760, %eax
        movd      %eax, %xmm2
        movss     %xmm2, %xmm3
        mulss     %xmm2, %xmm2
        addss     %xmm3, %xmm2
.L_2TAG_PACKET_5.0.1:
        xorps     %xmm0, %xmm0
        orl       $16256, %edx
        pinsrw    $1, %edx, %xmm0
        jmp       ..B1.4
.L_2TAG_PACKET_4.0.1:
        movl      8(%rsp), %eax
        andl      $2147483647, %eax
        cmpl      $2139095040, %eax
        je        .L_2TAG_PACKET_5.0.1
        movss     8(%rsp), %xmm0
        addss     %xmm0, %xmm0
        movss     %xmm0, (%rsp)
.L_2TAG_PACKET_6.0.1:
..B1.4:
        addq      $24, %rsp
..___tag_value_tanhf.4:
        ret       
..___tag_value_tanhf.5:
END(tanhf)
# -- End  tanhf
	.section .rodata, "a"
	.align 16
	.align 16
Shifter:
	.long	0
	.long	1127743488
	.long	0
	.long	3275227136
	.type	Shifter,@object
	.size	Shifter,16
	.align 16
cv:
	.long	1874480759
	.long	1044624043
	.long	4286760334
	.long	1060028349
	.long	3607404735
	.long	3200019208
	.long	4277811695
	.long	3214290498
	.type	cv,@object
	.size	cv,32
	.align 16
T2_neg_f:
	.long	0
	.long	1072693248
	.long	1533953344
	.long	1072648310
	.long	2728693978
	.long	1072604335
	.long	863738719
	.long	1072561303
	.long	3707479175
	.long	1072519192
	.long	3706687593
	.long	1072477984
	.long	3716502172
	.long	1072437659
	.long	4076559943
	.long	1072398198
	.long	2572866477
	.long	1072359583
	.long	2990417245
	.long	1072321795
	.long	2191782032
	.long	1072284817
	.long	2966275557
	.long	1072248631
	.long	1110089947
	.long	1072213221
	.long	2571947539
	.long	1072178569
	.long	1944781191
	.long	1072144660
	.long	3907805044
	.long	1072111477
	.long	1719614413
	.long	1072079006
	.long	2956612997
	.long	1072047230
	.long	3712504873
	.long	1072016135
	.long	1453150082
	.long	1071985707
	.long	3577096743
	.long	1071955930
	.long	1617004845
	.long	1071926792
	.long	1276261410
	.long	1071898278
	.long	926591435
	.long	1071870375
	.long	171030293
	.long	1071843070
	.long	4112506593
	.long	1071816349
	.long	1853186616
	.long	1071790202
	.long	828946858
	.long	1071764615
	.long	1014845819
	.long	1071739576
	.long	3490863953
	.long	1071715073
	.long	1828292879
	.long	1071691096
	.long	3541402996
	.long	1071667632
	.type	T2_neg_f,@object
	.size	T2_neg_f,256
	.align 4
L2E:
	.long	1697350398
	.long	1079448903
	.type	L2E,@object
	.size	L2E,8
	.align 4
ONEMASK:
	.long	0
	.long	1072693248
	.type	ONEMASK,@object
	.size	ONEMASK,8
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
	.4byte ..___tag_value_tanhf.1-.
	.4byte ..___tag_value_tanhf.5-..___tag_value_tanhf.1
	.2byte 0x0400
	.4byte ..___tag_value_tanhf.3-..___tag_value_tanhf.1
	.2byte 0x200e
	.byte 0x04
	.4byte ..___tag_value_tanhf.4-..___tag_value_tanhf.3
	.2byte 0x080e
	.byte 0x00
# End
