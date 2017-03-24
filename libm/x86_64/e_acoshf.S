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
# -- Begin  acoshf
ENTRY(acoshf)
# parameter 1: %xmm0
..B1.1:
..___tag_value_acoshf.1:
        subq      $24, %rsp
..___tag_value_acoshf.3:
        movss     %xmm0, 8(%rsp)
..B1.2:
        movl      8(%rsp), %eax
        subl      $1065353217, %eax
        cmpl      $1073741823, %eax
        jae       .L_2TAG_PACKET_0.0.2
        cvtps2pd  %xmm0, %xmm0
        subl      $4194303, %eax
        js        .L_2TAG_PACKET_1.0.2
        cmpl      $130023424, %eax
        ja        .L_2TAG_PACKET_2.0.2
        movapd    %xmm0, %xmm1
        mulsd     %xmm0, %xmm0
        subsd     dbOne(%rip), %xmm0
        sqrtsd    %xmm0, %xmm0
        addsd     %xmm1, %xmm0
        lea       __libm_double_rcp_table_256@GOTPCREL(%rip), %r8
        movq      (%r8), %r8
        lea       __libm_logf_table_256@GOTPCREL(%rip), %r9
        movq      (%r9), %r9
        pshufd    $85, %xmm0, %xmm1
        movd      %xmm1, %eax
        movl      %eax, %ecx
        shrl      $20, %eax
        shrl      $9, %ecx
        subl      $1023, %eax
        cvtsi2sd  %eax, %xmm2
        andpd     dbMantissaMask(%rip), %xmm0
        orpd      dbOne(%rip), %xmm0
        andl      $2040, %ecx
        movq      8+PL(%rip), %xmm1
        mulsd     (%r8,%rcx), %xmm0
        subsd     dbOne(%rip), %xmm0
        mulsd     dbLn2(%rip), %xmm2
        mulsd     %xmm0, %xmm1
        addsd     PL(%rip), %xmm1
        mulsd     %xmm0, %xmm1
        addsd     (%r9,%rcx), %xmm2
        mulsd     %xmm0, %xmm1
        addsd     %xmm1, %xmm0
        addsd     %xmm2, %xmm0
        cvtpd2ps  %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_2.0.2:
        lea       __libm_double_rcp_table_256@GOTPCREL(%rip), %r8
        movq      (%r8), %r8
        lea       __libm_logf_table_256@GOTPCREL(%rip), %r9
        movq      (%r9), %r9
        addl      $1069547520, %eax
        movl      %eax, %ecx
        shrl      $23, %eax
        subl      $126, %eax
        shrl      $12, %ecx
        cvtsi2sd  %eax, %xmm2
        andpd     dbMantissaMask(%rip), %xmm0
        orpd      dbOne(%rip), %xmm0
        andl      $2040, %ecx
        movq      8+PL(%rip), %xmm1
        mulsd     (%r8,%rcx), %xmm0
        subsd     dbOne(%rip), %xmm0
        mulsd     dbLn2(%rip), %xmm2
        mulsd     %xmm0, %xmm1
        addsd     PL(%rip), %xmm1
        mulsd     %xmm0, %xmm1
        addsd     (%r9,%rcx), %xmm2
        mulsd     %xmm0, %xmm1
        addsd     %xmm1, %xmm0
        addsd     %xmm2, %xmm0
        cvtpd2ps  %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_1.0.2:
        movdqa    48+QQ(%rip), %xmm3
        subsd     dbOne(%rip), %xmm0
        movdqa    %xmm0, %xmm2
        movdqa    %xmm0, %xmm1
        addsd     %xmm0, %xmm0
        sqrtsd    %xmm0, %xmm0
        mulsd     %xmm2, %xmm2
        pshufd    $68, %xmm1, %xmm1
        pshufd    $68, %xmm2, %xmm2
        mulpd     %xmm2, %xmm3
        addpd     32+QQ(%rip), %xmm3
        mulpd     %xmm2, %xmm3
        addpd     16+QQ(%rip), %xmm3
        mulpd     %xmm2, %xmm3
        addpd     QQ(%rip), %xmm3
        mulpd     %xmm1, %xmm3
        pshufd    $238, %xmm3, %xmm2
        mulsd     %xmm3, %xmm1
        addsd     %xmm2, %xmm1
        mulsd     %xmm0, %xmm1
        addsd     %xmm1, %xmm0
        cvtpd2ps  %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_0.0.2:
        incl      %eax
        movd      %xmm0, %edx
        je        .L_2TAG_PACKET_3.0.2
        andl      $2147483647, %edx
        cmpl      $2139095040, %edx
        ja        .L_2TAG_PACKET_4.0.2
        cmpl      $1073741824, %eax
        je        .L_2TAG_PACKET_4.0.2
        xorps     %xmm0, %xmm0
        mulss     inf(%rip), %xmm0
        jmp       .L_2TAG_PACKET_5.0.2
.L_2TAG_PACKET_3.0.2:
        xorps     %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_4.0.2:
        movss     onef(%rip), %xmm0
        mulss     8(%rsp), %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_5.0.2:
        movss     %xmm0, (%rsp)
..B1.3:
        movss     (%rsp), %xmm0
.L_2TAG_PACKET_6.0.2:
..B1.5:
        addq      $24, %rsp
..___tag_value_acoshf.4:
        ret       
..___tag_value_acoshf.5:
END(acoshf)
# -- End  acoshf
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
QQ:
	.long	305580873
	.long	1066611507
	.long	1429119421
	.long	3216332117
	.long	2886529429
	.long	1063197348
	.long	3670928078
	.long	3212237666
	.long	2610913564
	.long	1060186234
	.long	1100460448
	.long	3209089799
	.long	3532580246
	.long	1056164631
	.long	3531562621
	.long	3205972992
	.type	QQ,@object
	.size	QQ,64
	.align 4
inf:
	.long	2139095040
	.type	inf,@object
	.size	inf,4
	.align 4
onef:
	.long	1065353216
	.type	onef,@object
	.size	onef,4
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
	.4byte ..___tag_value_acoshf.1-.
	.4byte ..___tag_value_acoshf.5-..___tag_value_acoshf.1
	.2byte 0x0400
	.4byte ..___tag_value_acoshf.3-..___tag_value_acoshf.1
	.2byte 0x200e
	.byte 0x04
	.4byte ..___tag_value_acoshf.4-..___tag_value_acoshf.3
	.2byte 0x080e
	.byte 0x00
# End
