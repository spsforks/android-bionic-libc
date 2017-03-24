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
# -- Begin  sincosf
ENTRY(sincosf)
# parameter 1: %xmm0
# parameter 2: %rdi
# parameter 3: %rsi
..B1.1:
..___tag_value_sincosf.1:
        subq      $24, %rsp
..___tag_value_sincosf.3:
        movq      %rdi, (%rsp)
        movq      %rsi, 8(%rsp)
..B1.2:
        pshuflw   $68, %xmm0, %xmm1
        movd      %xmm0, %ecx
        cvtps2pd  %xmm1, %xmm1
        movq      $0x40445f306e000000, %rax
        movd      %rax, %xmm2
        movl      $2147483647, %r10d
        movl      $1241513983, %r8d
        andl      %ecx, %r10d
        subl      %r10d, %r8d
        subl      $964689920, %r10d
        orl       %r10d, %r8d
        jl        .L_2TAG_PACKET_0.0.1
        movq      $0x4338000000000000, %r11
        movd      %r11, %xmm4
        mulsd     %xmm1, %xmm2
        movq      $0xbe5b1bbead603d8b, %rdx
        movd      %rdx, %xmm3
        movq      %xmm2, %xmm5
        addsd     %xmm4, %xmm2
        movd      %xmm2, %r9d
        subsd     %xmm4, %xmm2
        mulsd     %xmm3, %xmm1
.L_2TAG_PACKET_1.0.1:
        lea       Ctable(%rip), %r10
        movl      $256, %eax
        movapd    1040(%r10), %xmm4
        subsd     %xmm2, %xmm5
        addl      %r9d, %r9d
        movl      %r9d, %r8d
        sarb      $7, %r9b
        andl      %r8d, %eax
        addb      %r9b, %r8b
        xorb      %r9b, %r8b
        addsd     %xmm5, %xmm1
        movq      $0x3f9921fb54442d18, %r11
        movd      %r11, %xmm6
        andl      $254, %r8d
        movapd    (%r10,%r8,8), %xmm3
        movddup   %xmm1, %xmm2
        shrl      $4, %eax
        movapd    1072(%r10,%rax), %xmm7
        movapd    1056(%r10), %xmm5
        mulpd     %xmm2, %xmm2
        mulsd     %xmm6, %xmm1
        andb      $1, %r9b
        shlq      $63, %r9
        movd      %r9, %xmm0
        xorpd     %xmm0, %xmm3
        xorpd     %xmm7, %xmm3
        mulpd     %xmm4, %xmm2
        pshufd    $78, %xmm3, %xmm6
        mulsd     %xmm1, %xmm3
        addpd     %xmm2, %xmm5
        mulsd     %xmm1, %xmm6
        mulpd     %xmm5, %xmm3
        mulpd     %xmm5, %xmm6
        pshufd    $238, %xmm3, %xmm0
        pshufd    $238, %xmm6, %xmm1
        addsd     %xmm3, %xmm0
        subsd     %xmm6, %xmm1
        cvtpd2ps  %xmm0, %xmm0
        cvtpd2ps  %xmm1, %xmm1
        movq      (%rsp), %rax
        movss     %xmm0, (%rax)
        movq      8(%rsp), %rax
        movss     %xmm1, (%rax)
        jmp       ..B1.4
.L_2TAG_PACKET_0.0.1:
        andl      $2147483647, %ecx
        cmpl      $1040187392, %ecx
        ja        .L_2TAG_PACKET_2.0.1
        cvtss2sd  %xmm0, %xmm0
        mulsd     ONE_M230(%rip), %xmm0
        cvtsd2ss  %xmm0, %xmm0
        movq      (%rsp), %rax
        movss     %xmm0, (%rax)
        movq      8(%rsp), %rax
        movl      $1065353216, %edx
        movl      %edx, (%rax)
        jmp       ..B1.4
.L_2TAG_PACKET_2.0.1:
        movd      %xmm0, %eax
        andl      $2139095040, %eax
        cmpl      $2139095040, %eax
        je        .L_2TAG_PACKET_3.0.1
        shrl      $23, %r10d
        subl      $27, %r10d
        andl      $65528, %r10d
        lea       Pitable(%rip), %rax
        movq      (%rax,%r10,2), %xmm3
        movq      8(%rax,%r10,2), %xmm5
        movq      $0xffffffffff000000, %r9
        movd      %r9, %xmm2
        movq      $0x4338000000000000, %r11
        movd      %r11, %xmm4
        andpd     %xmm3, %xmm2
        psllq     $40, %xmm3
        mulsd     %xmm1, %xmm2
        mulsd     %xmm1, %xmm3
        mulsd     %xmm5, %xmm1
        movq      %xmm2, %xmm0
        addsd     %xmm3, %xmm2
        movq      %xmm2, %xmm5
        subsd     %xmm2, %xmm0
        addsd     %xmm4, %xmm2
        addsd     %xmm0, %xmm3
        movd      %xmm2, %r9d
        subsd     %xmm4, %xmm2
        addsd     %xmm3, %xmm1
        jmp       .L_2TAG_PACKET_1.0.1
.L_2TAG_PACKET_3.0.1:
        subss     %xmm0, %xmm0
        movq      (%rsp), %rax
        movss     %xmm0, (%rax)
        movq      8(%rsp), %rax
        movss     %xmm0, (%rax)
.L_2TAG_PACKET_4.0.1:
..B1.4:
        addq      $24, %rsp
..___tag_value_sincosf.4:
        ret       
..___tag_value_sincosf.5:
END(sincosf)
# -- End  sincosf
	.section .rodata, "a"
	.align 16
	.align 16
Ctable:
	.long	0
	.long	1072693248
	.long	0
	.long	0
	.long	1619315981
	.long	1072692616
	.long	4154680958
	.long	1067000149
	.long	3816385918
	.long	1072690721
	.long	4044216340
	.long	1068048229
	.long	2902558778
	.long	1072687565
	.long	2462980598
	.long	1068684576
	.long	2748392742
	.long	1072683149
	.long	3156849708
	.long	1069094822
	.long	1893834707
	.long	1072677476
	.long	1453945614
	.long	1069504078
	.long	2131272983
	.long	1072670549
	.long	1854824762
	.long	1069729808
	.long	4201728937
	.long	1072662372
	.long	1149976518
	.long	1069933076
	.long	3489094832
	.long	1072652951
	.long	1013556747
	.long	1070135480
	.long	2892293513
	.long	1072642291
	.long	1786662755
	.long	1070336898
	.long	4220661975
	.long	1072630398
	.long	2065784603
	.long	1070537209
	.long	3882371427
	.long	1072617280
	.long	1655830135
	.long	1070666194
	.long	1455828442
	.long	1072602945
	.long	785751814
	.long	1070765062
	.long	3964199186
	.long	1072587400
	.long	2484561409
	.long	1070863196
	.long	83265253
	.long	1072570657
	.long	1974149085
	.long	1070960538
	.long	3066872380
	.long	1072552723
	.long	821708191
	.long	1071057029
	.long	3476196678
	.long	1072533611
	.long	2796464483
	.long	1071152610
	.long	3511535930
	.long	1072513332
	.long	1135144330
	.long	1071247225
	.long	4097623923
	.long	1072491898
	.long	165764288
	.long	1071340816
	.long	557060597
	.long	1072469323
	.long	2570175582
	.long	1071433326
	.long	4051746225
	.long	1072445618
	.long	939980347
	.long	1071524701
	.long	2892993834
	.long	1072420800
	.long	3690502842
	.long	1071614884
	.long	1159294526
	.long	1072394883
	.long	2569207790
	.long	1071674247
	.long	1477771776
	.long	1072367882
	.long	1411074851
	.long	1071718067
	.long	688824739
	.long	1072339814
	.long	967731400
	.long	1071761211
	.long	2688952194
	.long	1072310695
	.long	1289556703
	.long	1071803653
	.long	1207142209
	.long	1072280544
	.long	4242632757
	.long	1071845367
	.long	1234330619
	.long	1072249378
	.long	689983673
	.long	1071886330
	.long	1796544321
	.long	1072217216
	.long	621354454
	.long	1071926515
	.long	201299822
	.long	1072184078
	.long	3152274921
	.long	1071965898
	.long	577417135
	.long	1072149983
	.long	881650848
	.long	1072004457
	.long	938475414
	.long	1072114952
	.long	1427975391
	.long	1072042167
	.long	1719614413
	.long	1072079006
	.long	1719614413
	.long	1072079006
	.long	1427975391
	.long	1072042167
	.long	938475414
	.long	1072114952
	.long	881650848
	.long	1072004457
	.long	577417135
	.long	1072149983
	.long	3152274921
	.long	1071965898
	.long	201299822
	.long	1072184078
	.long	621354454
	.long	1071926515
	.long	1796544321
	.long	1072217216
	.long	689983673
	.long	1071886330
	.long	1234330619
	.long	1072249378
	.long	4242632757
	.long	1071845367
	.long	1207142209
	.long	1072280544
	.long	1289556703
	.long	1071803653
	.long	2688952194
	.long	1072310695
	.long	967731400
	.long	1071761211
	.long	688824739
	.long	1072339814
	.long	1411074851
	.long	1071718067
	.long	1477771776
	.long	1072367882
	.long	2569207790
	.long	1071674247
	.long	1159294526
	.long	1072394883
	.long	3690502842
	.long	1071614884
	.long	2892993834
	.long	1072420800
	.long	939980347
	.long	1071524701
	.long	4051746225
	.long	1072445618
	.long	2570175582
	.long	1071433326
	.long	557060597
	.long	1072469323
	.long	165764288
	.long	1071340816
	.long	4097623923
	.long	1072491898
	.long	1135144330
	.long	1071247225
	.long	3511535930
	.long	1072513332
	.long	2796464483
	.long	1071152610
	.long	3476196678
	.long	1072533611
	.long	821708191
	.long	1071057029
	.long	3066872380
	.long	1072552723
	.long	1974149085
	.long	1070960538
	.long	83265253
	.long	1072570657
	.long	2484561409
	.long	1070863196
	.long	3964199186
	.long	1072587400
	.long	785751814
	.long	1070765062
	.long	1455828442
	.long	1072602945
	.long	1655830135
	.long	1070666194
	.long	3882371427
	.long	1072617280
	.long	2065784603
	.long	1070537209
	.long	4220661975
	.long	1072630398
	.long	1786662755
	.long	1070336898
	.long	2892293513
	.long	1072642291
	.long	1013556747
	.long	1070135480
	.long	3489094832
	.long	1072652951
	.long	1149976518
	.long	1069933076
	.long	4201728937
	.long	1072662372
	.long	1854824762
	.long	1069729808
	.long	2131272983
	.long	1072670549
	.long	1453945614
	.long	1069504078
	.long	1893834707
	.long	1072677476
	.long	3156849708
	.long	1069094822
	.long	2748392742
	.long	1072683149
	.long	2462980598
	.long	1068684576
	.long	2902558778
	.long	1072687565
	.long	4044216340
	.long	1068048229
	.long	3816385918
	.long	1072690721
	.long	4154680958
	.long	1067000149
	.long	1619315981
	.long	1072692616
	.long	0
	.long	0
	.long	0
	.long	1072693248
	.long	1649608659
	.long	3206173094
	.long	3384690142
	.long	3207839036
	.long	0
	.long	1072693248
	.long	0
	.long	1072693248
	.long	0
	.long	0
	.long	0
	.long	0
	.long	0
	.long	2147483648
	.long	0
	.long	2147483648
	.type	Ctable,@object
	.size	Ctable,1104
	.align 16
Pitable:
	.long	1832810809
	.long	1078222640
	.long	4036845384
	.long	1030771879
	.long	1832810809
	.long	1078222640
	.long	4036845384
	.long	1030771879
	.long	1916669064
	.long	1072155675
	.long	660066805
	.long	1024797180
	.long	3829231700
	.long	1062745828
	.long	2884172442
	.long	1017445907
	.long	540874025
	.long	1055617650
	.long	4198409884
	.long	1011844330
	.long	2839315448
	.long	1046639136
	.long	4204424928
	.long	1000582120
	.long	1329339431
	.long	1038229673
	.long	1295478839
	.long	993391741
	.long	658191860
	.long	1024797180
	.long	3691886121
	.long	979325773
	.long	1966800710
	.long	1020436418
	.long	1882642597
	.long	977095991
	.long	2100977488
	.long	1012908661
	.long	1840335565
	.long	967470816
	.long	4198093645
	.long	1000582120
	.long	3307942521
	.long	957055414
	.long	2839123165
	.long	996851343
	.long	1387472776
	.long	952638316
	.long	2788761024
	.long	988712873
	.long	3005712498
	.long	944467026
	.long	3694676832
	.long	979325773
	.long	1326507024
	.long	934847846
	.long	221775557
	.long	970149340
	.long	1133527550
	.long	924162364
	.type	Pitable,@object
	.size	Pitable,240
	.align 8
ONE_M230:
	.long	4286578688
	.long	1072693247
	.type	ONE_M230,@object
	.size	ONE_M230,8
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
	.4byte ..___tag_value_sincosf.1-.
	.4byte ..___tag_value_sincosf.5-..___tag_value_sincosf.1
	.2byte 0x0400
	.4byte ..___tag_value_sincosf.3-..___tag_value_sincosf.1
	.2byte 0x200e
	.byte 0x04
	.4byte ..___tag_value_sincosf.4-..___tag_value_sincosf.3
	.2byte 0x080e
	.byte 0x00
# End
