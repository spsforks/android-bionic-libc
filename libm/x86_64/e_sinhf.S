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
# -- Begin  sinhf
ENTRY(sinhf)
# parameter 1: %xmm0
..B1.1:
..___tag_value_sinhf.1:
        subq      $24, %rsp
..___tag_value_sinhf.3:
        movss     %xmm0, 8(%rsp)
..B1.2:
        movd      %xmm0, %ecx
        movl      $-2147483648, %edx
        movq      $0x78471547652b82fe, %r8
        movd      %r8, %xmm3
        movq      Shifter(%rip), %xmm5
        psllq     $33, %xmm0
        psrlq     $4, %xmm0
        movapd    cv(%rip), %xmm4
        andl      %ecx, %edx
        andl      $2147418112, %ecx
        subl      $1052246016, %ecx
        cmpl      $67633152, %ecx
        jae       .L_2TAG_PACKET_0.0.2
        mulsd     %xmm0, %xmm3
        movq      %xmm3, %xmm2
        addsd     %xmm5, %xmm3
        xorpd     %xmm0, %xmm0
        movd      %xmm3, %eax
        subsd     %xmm5, %xmm3
        movapd    16+cv(%rip), %xmm1
        xorpd     %xmm6, %xmm6
        subsd     %xmm3, %xmm2
        shrl      $16, %edx
        orl       $16352, %edx
        cmpl      $59244544, %ecx
        jae       .L_2TAG_PACKET_1.0.2
        movl      $31, %ecx
        andl      %eax, %ecx
        shrl      $1, %eax
        addl      %ecx, %ecx
        andl      $65520, %eax
        addl      %edx, %eax
        pinsrw    $3, %eax, %xmm0
        movl      $32704, %edx
        subl      %eax, %edx
        movapd    32+cv(%rip), %xmm3
        lea       T2f(%rip), %r8
        movapd    (%r8,%rcx,8), %xmm7
        pinsrw    $3, %edx, %xmm6
        pshufd    $68, %xmm2, %xmm2
        shufpd    $0, %xmm6, %xmm0
        movapd    48+cv(%rip), %xmm5
        mulpd     %xmm2, %xmm4
        mulpd     %xmm2, %xmm1
        mulpd     %xmm2, %xmm3
        mulpd     %xmm7, %xmm0
        mulpd     %xmm2, %xmm4
        addpd     %xmm1, %xmm5
        mulpd     %xmm0, %xmm3
        addpd     %xmm5, %xmm4
        mulpd     %xmm3, %xmm4
        addpd     %xmm4, %xmm0
        pshufd    $78, %xmm0, %xmm4
        subpd     %xmm4, %xmm0
        cvtpd2ps  %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_0.0.2:
        addl      $1052246016, %ecx
        cmpl      $1052246016, %ecx
        ja        .L_2TAG_PACKET_2.0.2
        cmpl      $956301312, %ecx
        jae       .L_2TAG_PACKET_3.0.2
        cmpl      $8388608, %ecx
        jae       .L_2TAG_PACKET_4.0.2
        movss     8(%rsp), %xmm0
        mulss     8(%rsp), %xmm0
.L_2TAG_PACKET_4.0.2:
        movss     8(%rsp), %xmm0
        movl      $1283457024, %eax
        movd      %eax, %xmm1
        movl      $847249408, %ecx
        movd      %ecx, %xmm2
        mulss     %xmm0, %xmm1
        addss     %xmm1, %xmm0
        mulss     %xmm2, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_1.0.2:
        movl      $31, %ecx
        andl      %eax, %ecx
        shrl      $1, %eax
        addl      %ecx, %ecx
        andl      $65520, %eax
        addl      %edx, %eax
        pinsrw    $3, %eax, %xmm0
        movq      32+cv(%rip), %xmm3
        pshufd    $68, %xmm2, %xmm2
        lea       T2f(%rip), %r8
        mulsd     (%r8,%rcx,8), %xmm0
        mulsd     %xmm2, %xmm4
        mulsd     %xmm2, %xmm3
        addsd     %xmm1, %xmm4
        mulsd     %xmm2, %xmm4
        mulsd     %xmm3, %xmm4
        addsd     %xmm3, %xmm4
        mulsd     %xmm0, %xmm4
        addsd     %xmm4, %xmm0
        cvtsd2ss  %xmm0, %xmm0
        pextrw    $1, %xmm0, %eax
        andl      $32640, %eax
        cmpl      $32640, %eax
        je        .L_2TAG_PACKET_5.0.2
        jmp       ..B1.5
.L_2TAG_PACKET_3.0.2:
        psrlq     $29, %xmm0
        movss     pv(%rip), %xmm7
        movss     %xmm0, %xmm5
        mulss     %xmm0, %xmm0
        movss     4+pv(%rip), %xmm2
        movss     %xmm0, %xmm1
        mulss     %xmm0, %xmm7
        movl      $1042983595, %eax
        movd      %eax, %xmm3
        mulss     %xmm0, %xmm0
        addss     %xmm2, %xmm7
        movd      %edx, %xmm4
        mulss     %xmm3, %xmm1
        mulss     %xmm7, %xmm0
        addss     %xmm1, %xmm0
        orps      %xmm4, %xmm5
        mulss     %xmm5, %xmm0
        addss     %xmm5, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_2.0.2:
        cmpl      $2139095040, %ecx
        jae       .L_2TAG_PACKET_6.0.2
        movl      $2130706432, %eax
        movd      %eax, %xmm0
        orl       %edx, %eax
        movd      %eax, %xmm1
        mulss     %xmm1, %xmm0
        jmp       .L_2TAG_PACKET_5.0.2
.L_2TAG_PACKET_6.0.2:
        movss     8(%rsp), %xmm0
        movss     %xmm0, %xmm1
        pslld     $1, %xmm0
        psrld     $1, %xmm0
        mulss     %xmm1, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_5.0.2:
        movss     %xmm0, (%rsp)
..B1.3:
        movss     (%rsp), %xmm0
.L_2TAG_PACKET_7.0.2:
..B1.5:
        addq      $24, %rsp
..___tag_value_sinhf.4:
        ret       
..___tag_value_sinhf.5:
END(sinhf)
# -- End  sinhf
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
	.long	4289495988
	.long	1058308051
	.long	4289495988
	.long	1058308051
	.long	4277811695
	.long	1065758274
	.long	4277811695
	.long	3213241922
	.long	4277811695
	.long	1066806850
	.long	4277811695
	.long	3214290498
	.long	0
	.long	1072693248
	.long	0
	.long	1072693248
	.type	cv,@object
	.size	cv,64
	.align 16
T2f:
	.long	0
	.long	1072693248
	.long	0
	.long	1072693248
	.long	3541402996
	.long	1072716208
	.long	1533953344
	.long	1072648310
	.long	1828292879
	.long	1072739672
	.long	2728693978
	.long	1072604335
	.long	3490863953
	.long	1072763649
	.long	863738719
	.long	1072561303
	.long	1014845819
	.long	1072788152
	.long	3707479175
	.long	1072519192
	.long	828946858
	.long	1072813191
	.long	3706687593
	.long	1072477984
	.long	1853186616
	.long	1072838778
	.long	3716502172
	.long	1072437659
	.long	4112506593
	.long	1072864925
	.long	4076559943
	.long	1072398198
	.long	171030293
	.long	1072891646
	.long	2572866477
	.long	1072359583
	.long	926591435
	.long	1072918951
	.long	2990417245
	.long	1072321795
	.long	1276261410
	.long	1072946854
	.long	2191782032
	.long	1072284817
	.long	1617004845
	.long	1072975368
	.long	2966275557
	.long	1072248631
	.long	3577096743
	.long	1073004506
	.long	1110089947
	.long	1072213221
	.long	1453150082
	.long	1073034283
	.long	2571947539
	.long	1072178569
	.long	3712504873
	.long	1073064711
	.long	1944781191
	.long	1072144660
	.long	2956612997
	.long	1073095806
	.long	3907805044
	.long	1072111477
	.long	1719614413
	.long	1073127582
	.long	1719614413
	.long	1072079006
	.long	3907805044
	.long	1073160053
	.long	2956612997
	.long	1072047230
	.long	1944781191
	.long	1073193236
	.long	3712504873
	.long	1072016135
	.long	2571947539
	.long	1073227145
	.long	1453150082
	.long	1071985707
	.long	1110089947
	.long	1073261797
	.long	3577096743
	.long	1071955930
	.long	2966275557
	.long	1073297207
	.long	1617004845
	.long	1071926792
	.long	2191782032
	.long	1073333393
	.long	1276261410
	.long	1071898278
	.long	2990417245
	.long	1073370371
	.long	926591435
	.long	1071870375
	.long	2572866477
	.long	1073408159
	.long	171030293
	.long	1071843070
	.long	4076559943
	.long	1073446774
	.long	4112506593
	.long	1071816349
	.long	3716502172
	.long	1073486235
	.long	1853186616
	.long	1071790202
	.long	3706687593
	.long	1073526560
	.long	828946858
	.long	1071764615
	.long	3707479175
	.long	1073567768
	.long	1014845819
	.long	1071739576
	.long	863738719
	.long	1073609879
	.long	3490863953
	.long	1071715073
	.long	2728693978
	.long	1073652911
	.long	1828292879
	.long	1071691096
	.long	1533953344
	.long	1073696886
	.long	3541402996
	.long	1071667632
	.type	T2f,@object
	.size	T2f,512
	.align 16
pv:
	.long	961547521
	.long	1007192201
	.long	1042983595
	.type	pv,@object
	.size	pv,12
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
	.4byte ..___tag_value_sinhf.1-.
	.4byte ..___tag_value_sinhf.5-..___tag_value_sinhf.1
	.2byte 0x0400
	.4byte ..___tag_value_sinhf.3-..___tag_value_sinhf.1
	.2byte 0x200e
	.byte 0x04
	.4byte ..___tag_value_sinhf.4-..___tag_value_sinhf.3
	.2byte 0x080e
	.byte 0x00
# End
