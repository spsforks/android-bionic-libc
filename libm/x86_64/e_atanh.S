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
# -- Begin  atanh
ENTRY(atanh)
# parameter 1: %xmm0
..B1.1:
..___tag_value_atanh.1:
        subq      $24, %rsp
..___tag_value_atanh.3:
..B1.2:
        .byte     102
        .byte     72
        .byte     15
        .byte     126
        .byte     192
        movq      %rax, %rcx
        shrq      $32, %rax
        andl      $2147483647, %eax
        cmpl      $1072693248, %eax
        jge       .L_2TAG_PACKET_0.0.2
        cmpl      $1068498944, %eax
        jl        .L_2TAG_PACKET_1.0.2
        pand      dbAbsMask(%rip), %xmm0
        movq      dbmOne(%rip), %xmm4
        lea       dbHalfs(%rip), %rdx
        movapd    %xmm0, %xmm1
        pand      dbHighMaskf(%rip), %xmm0
        shrq      $63, %rcx
        subsd     %xmm0, %xmm1
        movapd    %xmm1, %xmm3
        movapd    %xmm0, %xmm2
        addsd     %xmm4, %xmm0
        subsd     %xmm4, %xmm2
        movapd    %xmm0, %xmm5
        movapd    %xmm0, %xmm6
        addsd     %xmm1, %xmm0
        divsd     %xmm0, %xmm4
        movapd    %xmm4, %xmm7
        movapd    %xmm4, %xmm0
        mulsd     %xmm4, %xmm3
        pand      dbHighMaskf(%rip), %xmm4
        mulsd     %xmm4, %xmm5
        subsd     %xmm4, %xmm7
        mulsd     %xmm7, %xmm6
        subsd     dbmOne(%rip), %xmm5
        addsd     %xmm3, %xmm6
        addsd     %xmm6, %xmm5
        movq      (%rdx,%rcx,8), %xmm6
        mulsd     %xmm5, %xmm0
        addsd     %xmm7, %xmm0
        movapd    %xmm0, %xmm3
        addsd     %xmm4, %xmm0
        mulsd     %xmm0, %xmm1
        mulsd     %xmm2, %xmm3
        mulsd     %xmm4, %xmm2
        addsd     %xmm3, %xmm1
        movapd    %xmm2, %xmm0
        addsd     %xmm1, %xmm0
        .byte     102
        .byte     72
        .byte     15
        .byte     126
        .byte     192
        movq      %rax, %rcx
        andpd     dbHighMask(%rip), %xmm0
        subsd     %xmm0, %xmm2
        addsd     %xmm2, %xmm1
        andpd     dbMantissaMask(%rip), %xmm0
        orpd      dbOne(%rip), %xmm0
        shrq      $52, %rax
        movl      $2046, %edx
        subl      %eax, %edx
        subl      $1023, %eax
        cvtsi2sd  %eax, %xmm4
        shll      $4, %edx
        xorpd     %xmm3, %xmm3
        pinsrw    $3, %edx, %xmm3
        mulsd     %xmm3, %xmm1
        shrq      $41, %rcx
        andl      $2040, %ecx
        lea       __libm_double_rcp_table_256@GOTPCREL(%rip), %rax
        movq      (%rax), %rax
        lea       __libm_log_table_256@GOTPCREL(%rip), %rdx
        movq      (%rdx), %rdx
        movq      32+PL(%rip), %xmm2
        movapd    %xmm4, %xmm5
        mulsd     dbLn2(%rip), %xmm4
        mulsd     8+dbLn2(%rip), %xmm5
        mulsd     (%rax,%rcx), %xmm0
        mulsd     (%rax,%rcx), %xmm1
        subsd     dbOne(%rip), %xmm0
        movapd    %xmm0, %xmm3
        addsd     %xmm1, %xmm0
        addsd     (%rdx,%rcx,2), %xmm4
        addsd     8(%rdx,%rcx,2), %xmm5
        mulsd     %xmm0, %xmm2
        addsd     24+PL(%rip), %xmm2
        mulsd     %xmm0, %xmm2
        addsd     16+PL(%rip), %xmm2
        mulsd     %xmm0, %xmm2
        addsd     8+PL(%rip), %xmm2
        mulsd     %xmm0, %xmm2
        mulsd     %xmm0, %xmm0
        addsd     PL(%rip), %xmm2
        mulsd     %xmm2, %xmm0
        addsd     %xmm1, %xmm0
        addsd     %xmm4, %xmm0
        addsd     %xmm5, %xmm3
        addsd     %xmm3, %xmm0
        mulsd     %xmm6, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_1.0.2:
        cmpl      $1012924416, %eax
        jl        .L_2TAG_PACKET_2.0.2
        movapd    32+PA(%rip), %xmm1
        movapd    %xmm0, %xmm2
        mulsd     %xmm0, %xmm0
        movapd    %xmm0, %xmm3
        mulsd     %xmm0, %xmm0
        unpcklpd  %xmm0, %xmm0
        unpcklpd  %xmm0, %xmm3
        mulpd     %xmm0, %xmm1
        addpd     16+PA(%rip), %xmm1
        mulpd     %xmm0, %xmm1
        addpd     PA(%rip), %xmm1
        mulpd     %xmm3, %xmm1
        movhlps   %xmm1, %xmm0
        addsd     %xmm1, %xmm0
        mulsd     %xmm2, %xmm0
        addsd     %xmm2, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_2.0.2:
        movapd    %xmm0, %xmm5
        andl      $2146435072, %eax
        je        .L_2TAG_PACKET_3.0.2
        addsd     dbOne(%rip), %xmm0
        mulsd     %xmm5, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_3.0.2:
        movapd    %xmm5, %xmm2
        andpd     dbSignBit(%rip), %xmm2
        mulsd     %xmm0, %xmm0
        addsd     %xmm5, %xmm0
        orpd      %xmm2, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_0.0.2:
        jne       .L_2TAG_PACKET_4.0.2
        testl     %ecx, %ecx
        je        .L_2TAG_PACKET_5.0.2
.L_2TAG_PACKET_4.0.2:
        movq      $0x7fffffffffffffff, %rdx
        movq      $0x7ff0000000000000, %rax
        andq      %rdx, %rcx
        cmpq      %rax, %rcx
        jg        .L_2TAG_PACKET_6.0.2
        xorpd     %xmm0, %xmm0
        movl      $131, (%rsp)
        jmp       .L_2TAG_PACKET_7.0.2
.L_2TAG_PACKET_5.0.2:
        movl      $132, (%rsp)
.L_2TAG_PACKET_7.0.2:
        xorpd     %xmm1, %xmm1
        divsd     %xmm1, %xmm0
        jmp       .L_2TAG_PACKET_8.0.2
.L_2TAG_PACKET_6.0.2:
        addsd     %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_8.0.2:
        movq      %xmm0, 8(%rsp)
..B1.3:
        movq      8(%rsp), %xmm0
.L_2TAG_PACKET_9.0.2:
..B1.5:
        addq      $24, %rsp
..___tag_value_atanh.4:
        ret       
..___tag_value_atanh.5:
END(atanh)
# -- End  atanh
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
dbmOne:
	.long	0
	.long	3220176896
	.long	0
	.long	3220176896
	.type	dbmOne,@object
	.size	dbmOne,16
	.align 16
dbHalfs:
	.long	0
	.long	1071644672
	.long	0
	.long	3219128320
	.type	dbHalfs,@object
	.size	dbHalfs,16
	.align 16
dbHighMaskf:
	.long	4026531840
	.long	4294967295
	.long	4026531840
	.long	4294967295
	.type	dbHighMaskf,@object
	.size	dbHighMaskf,16
	.align 16
dbHighMask:
	.long	0
	.long	4294967295
	.long	0
	.long	4294967295
	.type	dbHighMask,@object
	.size	dbHighMask,16
	.align 16
dbMantissaMask:
	.long	4294967295
	.long	1048575
	.long	4294967295
	.long	1048575
	.type	dbMantissaMask,@object
	.size	dbMantissaMask,16
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
	.long	0
	.long	3219128320
	.long	1431621855
	.long	1070945621
	.long	4294842013
	.long	3218079743
	.long	1289448124
	.long	1070176674
	.long	2077359316
	.long	3217380703
	.type	PL,@object
	.size	PL,40
	.space 8, 0x00 	# pad
	.align 16
dbLn2:
	.long	897137782
	.long	1038760431
	.long	4276092928
	.long	1072049730
	.type	dbLn2,@object
	.size	dbLn2,16
	.align 16
PA:
	.long	1431655765
	.long	1070945621
	.long	2576980801
	.long	1070176665
	.long	2453616913
	.long	1069697316
	.long	1427436931
	.long	1069314503
	.long	2129349532
	.long	1068975486
	.long	1629438381
	.long	1068756329
	.type	PA,@object
	.size	PA,48
	.align 16
dbSignBit:
	.long	0
	.long	2147483648
	.long	0
	.long	2147483648
	.type	dbSignBit,@object
	.size	dbSignBit,16
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
	.4byte ..___tag_value_atanh.1-.
	.4byte ..___tag_value_atanh.5-..___tag_value_atanh.1
	.2byte 0x0400
	.4byte ..___tag_value_atanh.3-..___tag_value_atanh.1
	.2byte 0x200e
	.byte 0x04
	.4byte ..___tag_value_atanh.4-..___tag_value_atanh.3
	.2byte 0x080e
	.byte 0x00
# End
