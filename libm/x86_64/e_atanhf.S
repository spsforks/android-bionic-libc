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
# -- Begin  atanhf
ENTRY(atanhf)
# parameter 1: %xmm0
..B1.1:
..___tag_value_atanhf.1:
        pushq     %rsi
..___tag_value_atanhf.3:
..B1.2:
        movd      %xmm0, %eax
        andl      $2147483647, %eax
        cmpl      $1065353216, %eax
        jge       .L_2TAG_PACKET_0.0.2
        cmpl      $1056964608, %eax
        jl        .L_2TAG_PACKET_1.0.2
        lea       __libm_double_rcp_table_256@GOTPCREL(%rip), %r8
        movq      (%r8), %r8
        lea       __libm_logf_table_256@GOTPCREL(%rip), %rdx
        movq      (%rdx), %rdx
        pshufd    $0, %xmm0, %xmm1
        movapd    dbOne(%rip), %xmm0
        cvtps2pd  %xmm1, %xmm1
        addsubpd  %xmm1, %xmm0
        movapd    %xmm0, %xmm1
        psrlq     $41, %xmm0
        pextrw    $0, %xmm0, %eax
        pextrw    $4, %xmm0, %ecx
        psrlq     $11, %xmm0
        movhlps   %xmm0, %xmm2
        andl      $2040, %eax
        andl      $2040, %ecx
        movq      (%rdx,%rcx), %xmm3
        subsd     (%rdx,%rax), %xmm3
        movq      (%r8,%rax), %xmm4
        movhpd    (%r8,%rcx), %xmm4
        psubd     %xmm0, %xmm2
        movd      %xmm2, %eax
        movapd    16+PL(%rip), %xmm5
        cvtsi2sd  %eax, %xmm2
        mulsd     dbLn2(%rip), %xmm2
        andpd     dbMantissaMask(%rip), %xmm1
        orpd      dbOne(%rip), %xmm1
        mulpd     %xmm4, %xmm1
        subpd     dbOne(%rip), %xmm1
        mulpd     %xmm1, %xmm5
        addpd     PL(%rip), %xmm5
        mulpd     %xmm1, %xmm5
        mulpd     %xmm1, %xmm5
        addpd     %xmm5, %xmm1
        movhlps   %xmm1, %xmm0
        addsd     %xmm3, %xmm2
        subsd     %xmm1, %xmm0
        addsd     %xmm2, %xmm0
        psubw     dbHighSomeLowOneInExp(%rip), %xmm0
        cvtpd2ps  %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_1.0.2:
        cmpl      $838860800, %eax
        jl        .L_2TAG_PACKET_2.0.2
        pshufd    $0, %xmm0, %xmm0
        cvtps2pd  %xmm0, %xmm0
        movapd    48+Q(%rip), %xmm3
        movapd    %xmm0, %xmm1
        mulpd     %xmm0, %xmm0
        movapd    %xmm0, %xmm2
        mulpd     %xmm0, %xmm0
        mulpd     %xmm0, %xmm3
        addpd     32+Q(%rip), %xmm3
        mulpd     %xmm0, %xmm3
        addpd     16+Q(%rip), %xmm3
        unpcklpd  %xmm0, %xmm2
        mulpd     %xmm0, %xmm3
        addpd     Q(%rip), %xmm3
        mulpd     %xmm2, %xmm3
        pshufd    $238, %xmm3, %xmm0
        addsd     %xmm3, %xmm0
        mulsd     %xmm1, %xmm0
        addsd     %xmm1, %xmm0
        cvtpd2ps  %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_2.0.2:
        movaps    %xmm0, %xmm1
        andl      $2139095040, %eax
        je        .L_2TAG_PACKET_3.0.2
        addss     flOne(%rip), %xmm0
        mulss     %xmm1, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_3.0.2:
        movaps    %xmm0, %xmm2
        andps     flSign(%rip), %xmm2
        mulss     %xmm0, %xmm0
        addss     %xmm1, %xmm0
        orps      %xmm2, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_0.0.2:
        je        .L_2TAG_PACKET_4.0.2
        cmpl      $2139095040, %eax
        jg        .L_2TAG_PACKET_5.0.2
        xorps     %xmm0, %xmm0
        movl      $133, (%rsp)
        jmp       .L_2TAG_PACKET_6.0.2
.L_2TAG_PACKET_4.0.2:
        movl      $134, (%rsp)
.L_2TAG_PACKET_6.0.2:
        xorps     %xmm1, %xmm1
        divss     %xmm1, %xmm0
        jmp       .L_2TAG_PACKET_7.0.2
.L_2TAG_PACKET_5.0.2:
        addss     %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_7.0.2:
        movss     %xmm0, 4(%rsp)
..B1.3:
        movss     4(%rsp), %xmm0
.L_2TAG_PACKET_8.0.2:
..B1.5:
        popq      %rcx
..___tag_value_atanhf.4:
        ret       
..___tag_value_atanhf.5:
END(atanhf)
# -- End  atanhf
	.section .rodata, "a"
	.align 16
	.align 16
dbOne:
	.long	0
	.long	1072693248
	.long	0
	.long	1072693248
	.type	dbOne,@object
	.size	dbOne,16
	.align 16
PL:
	.long	3879088107
	.long	3219128321
	.long	3879088107
	.long	3219128321
	.long	1754324240
	.long	1070945626
	.long	1754324240
	.long	1070945626
	.type	PL,@object
	.size	PL,32
	.align 16
dbLn2:
	.long	4277811695
	.long	1072049730
	.long	4277811695
	.long	1072049730
	.type	dbLn2,@object
	.size	dbLn2,16
	.align 16
dbMantissaMask:
	.long	4294967295
	.long	1048575
	.long	4294967295
	.long	1048575
	.type	dbMantissaMask,@object
	.size	dbMantissaMask,16
	.align 16
dbHighSomeLowOneInExp:
	.long	0
	.long	1048576
	.long	0
	.long	3222274048
	.type	dbHighSomeLowOneInExp,@object
	.size	dbHighSomeLowOneInExp,16
	.align 16
Q:
	.long	1370222587
	.long	1070945621
	.long	2417412325
	.long	1070176668
	.long	2969394279
	.long	1069697198
	.long	1510013680
	.long	1069319176
	.long	1692635884
	.long	1068924049
	.long	3693828232
	.long	1069068511
	.long	4025937356
	.long	3211315437
	.long	476477515
	.long	1070012413
	.type	Q,@object
	.size	Q,64
	.align 16
flOne:
	.long	1065353216
	.long	1065353216
	.long	1065353216
	.long	1065353216
	.type	flOne,@object
	.size	flOne,16
	.align 16
flSign:
	.long	2147483648
	.long	2147483648
	.long	2147483648
	.long	2147483648
	.type	flSign,@object
	.size	flSign,16
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
	.4byte ..___tag_value_atanhf.1-.
	.4byte ..___tag_value_atanhf.5-..___tag_value_atanhf.1
	.2byte 0x0400
	.4byte ..___tag_value_atanhf.3-..___tag_value_atanhf.1
	.2byte 0x100e
	.byte 0x04
	.4byte ..___tag_value_atanhf.4-..___tag_value_atanhf.3
	.2byte 0x080e
	.byte 0x00
# End
