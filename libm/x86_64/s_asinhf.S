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
# -- Begin  asinhf
ENTRY(asinhf)
# parameter 1: %xmm0
..B1.1:
..___tag_value_asinhf.1:
..___tag_value_asinhf.3:
..B1.2:
        movd      %xmm0, %eax
        movl      %eax, %ecx
        andl      $2147483647, %eax
        cmpl      $2139095040, %eax
        jge       .L_2TAG_PACKET_0.0.1
        cmpl      $1056964608, %eax
        jle       .L_2TAG_PACKET_1.0.1
        pshufd    $0, %xmm0, %xmm0
        cvtps2pd  %xmm0, %xmm0
        movapd    %xmm0, %xmm3
        andpd     dbAbsMask(%rip), %xmm0
        andpd     dbSignBit(%rip), %xmm3
        cmpl      $1199570944, %eax
        jge       .L_2TAG_PACKET_2.0.1
        movapd    %xmm0, %xmm1
        mulsd     %xmm0, %xmm0
        addsd     dbOne(%rip), %xmm0
        sqrtsd    %xmm0, %xmm0
        addsd     %xmm1, %xmm0
        .byte     102
        .byte     72
        .byte     15
        .byte     126
        .byte     192
        movq      %rax, %rcx
        shrq      $52, %rax
        subl      $1023, %eax
        shrq      $41, %rcx
        jmp       .L_2TAG_PACKET_3.0.1
.L_2TAG_PACKET_2.0.1:
        movl      %eax, %ecx
        shrl      $23, %eax
        subl      $126, %eax
        shrl      $12, %ecx
.L_2TAG_PACKET_3.0.1:
        cvtsi2sd  %eax, %xmm2
        andpd     dbMantissaMask(%rip), %xmm0
        orpd      dbOne(%rip), %xmm0
        andl      $2040, %ecx
        movq      8+PL(%rip), %xmm1
        lea       __libm_double_rcp_table_256@GOTPCREL(%rip), %r8
        movq      (%r8), %r8
        lea       __libm_logf_table_256@GOTPCREL(%rip), %rdx
        movq      (%rdx), %rdx
        mulsd     (%r8,%rcx), %xmm0
        subsd     dbOne(%rip), %xmm0
        mulsd     dbLn2(%rip), %xmm2
        mulsd     %xmm0, %xmm1
        addsd     PL(%rip), %xmm1
        mulsd     %xmm0, %xmm1
        addsd     (%rdx,%rcx), %xmm2
        mulsd     %xmm0, %xmm1
        addsd     %xmm1, %xmm0
        addsd     %xmm2, %xmm0
        orpd      %xmm3, %xmm0
        cvtpd2ps  %xmm0, %xmm0
        ret       
.L_2TAG_PACKET_1.0.1:
        cmpl      $838860800, %eax
        jle       .L_2TAG_PACKET_4.0.1
        cvtss2sd  %xmm0, %xmm0
        movapd    48+Q(%rip), %xmm3
        movapd    %xmm0, %xmm1
        mulsd     %xmm0, %xmm0
        movapd    %xmm0, %xmm2
        mulsd     %xmm0, %xmm0
        unpcklpd  %xmm0, %xmm2
        unpcklpd  %xmm0, %xmm0
        mulpd     %xmm0, %xmm3
        addpd     32+Q(%rip), %xmm3
        mulpd     %xmm0, %xmm3
        addpd     16+Q(%rip), %xmm3
        mulpd     %xmm0, %xmm3
        addpd     Q(%rip), %xmm3
        mulpd     %xmm2, %xmm3
        pshufd    $238, %xmm3, %xmm0
        addsd     %xmm3, %xmm0
        mulsd     %xmm1, %xmm0
        addsd     %xmm1, %xmm0
        cvtpd2ps  %xmm0, %xmm0
        ret       
.L_2TAG_PACKET_4.0.1:
        andl      $2139095040, %eax
        je        .L_2TAG_PACKET_5.0.1
        lea       flMinNormSign(%rip), %rax
        mulss     flScale(%rip), %xmm0
        shrl      $31, %ecx
        subss     (%rax,%rcx,4), %xmm0
        mulss     4+flScale(%rip), %xmm0
        ret       
.L_2TAG_PACKET_5.0.1:
        movaps    %xmm0, %xmm2
        andps     flSign(%rip), %xmm2
        movaps    %xmm0, %xmm1
        mulss     %xmm0, %xmm0
        addss     %xmm1, %xmm0
        orps      %xmm2, %xmm0
        ret       
.L_2TAG_PACKET_0.0.1:
        addss     %xmm0, %xmm0
        ret       
..B1.3:
..___tag_value_asinhf.4:
END(asinhf)
# -- End  asinhf
	.section .rodata, "a"
	.align 16
	.align 16
dbAbsMask:
	.long	4294967295
	.long	2147483647
	.long	4294967295
	.long	2147483647
	.type	dbAbsMask,@object
	.size	dbAbsMask,16
	.align 16
dbSignBit:
	.long	0
	.long	2147483648
	.long	0
	.long	2147483648
	.type	dbSignBit,@object
	.size	dbSignBit,16
	.align 16
dbOne:
	.long	0
	.long	1072693248
	.long	0
	.long	1072693248
	.type	dbOne,@object
	.size	dbOne,16
	.align 16
dbMantissaMask:
	.long	4294967295
	.long	1048575
	.long	4294967295
	.long	1048575
	.type	dbMantissaMask,@object
	.size	dbMantissaMask,16
	.align 16
PL:
	.long	3879088107
	.long	3219128321
	.long	1754324240
	.long	1070945626
	.type	PL,@object
	.size	PL,16
	.align 16
dbLn2:
	.long	4277811695
	.long	1072049730
	.long	4277811695
	.long	1072049730
	.type	dbLn2,@object
	.size	dbLn2,16
	.align 16
Q:
	.long	1429119421
	.long	3217380693
	.long	305580873
	.long	1068708659
	.long	3670928078
	.long	3215383394
	.long	2886529429
	.long	1067391652
	.long	1100460448
	.long	3214332679
	.long	2610913564
	.long	1066477690
	.long	3531562621
	.long	3213313024
	.long	3532580246
	.long	1064553239
	.type	Q,@object
	.size	Q,64
	.align 16
flMinNormSign:
	.long	8388608
	.long	2155872256
	.long	8388608
	.long	2155872256
	.type	flMinNormSign,@object
	.size	flMinNormSign,16
	.align 16
flScale:
	.long	1904214016
	.long	226492416
	.long	1904214016
	.long	226492416
	.type	flScale,@object
	.size	flScale,16
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
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte ..___tag_value_asinhf.1-.
	.4byte ..___tag_value_asinhf.4-..___tag_value_asinhf.1
	.2byte 0x0400
	.4byte ..___tag_value_asinhf.3-..___tag_value_asinhf.1
	.2byte 0x100e
# End
