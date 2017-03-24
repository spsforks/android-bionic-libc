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
# -- Begin  atan2f
ENTRY(atan2f)
# parameter 1: %xmm0
# parameter 2: %xmm1
..B1.1:
..___tag_value_atan2f.1:
        subq      $24, %rsp
..___tag_value_atan2f.3:
        movss     %xmm0, 8(%rsp)
        movss     %xmm1, 16(%rsp)
..B1.2:
        movd      %xmm0, %edx
        movd      %xmm1, %ecx
        movl      %edx, %eax
        andl      $2147483647, %edx
        subl      $528482304, %edx
        cmpl      $1073741824, %edx
        jae       .L_2TAG_PACKET_0.0.2
        movl      %ecx, %edx
        andl      $-2147483648, %ecx
        andl      $2147483647, %edx
        subl      $528482304, %edx
        cmpl      $1073741824, %edx
        jae       .L_2TAG_PACKET_1.0.2
        divss     %xmm1, %xmm0
        movss     8(%rsp), %xmm5
.L_2TAG_PACKET_2.0.2:
        movl      $-262144, %edx
        movd      %edx, %xmm3
        movss     %xmm1, %xmm6
        movl      $262144, %edx
        movd      %edx, %xmm4
        movss     %xmm1, %xmm2
        shrl      $31, %eax
        shrl      $30, %ecx
        orl       %eax, %ecx
        andps     %xmm3, %xmm6
        lea       atan_tbl(%rip), %r11
        subss     %xmm6, %xmm1
        andps     %xmm0, %xmm3
        movd      %xmm0, %edx
        orps      %xmm4, %xmm3
        mulss     %xmm3, %xmm6
        lea       pi_table_add(%rip), %r10
        mulss     %xmm3, %xmm1
        movl      %edx, %eax
        andl      $2146959360, %edx
        subl      $1023410176, %edx
        cmpl      $83361792, %edx
        ja        .L_2TAG_PACKET_3.0.2
        mulss     %xmm5, %xmm3
        subss     %xmm6, %xmm5
        addss     %xmm3, %xmm2
        subss     %xmm1, %xmm5
        divss     %xmm2, %xmm5
        shrl      $16, %edx
        addl      $8, %edx
        andl      $-2147483648, %eax
        movd      %eax, %xmm0
        movss     (%r11,%rdx), %xmm1
        movss     4(%r11,%rdx), %xmm3
        movl      $-1096111855, %eax
        movd      %eax, %xmm4
        movss     (%r10,%rcx,8), %xmm6
        xorps     %xmm0, %xmm1
        xorps     %xmm0, %xmm3
        movss     %xmm1, %xmm7
        addss     4(%r10,%rcx,8), %xmm1
        addss     %xmm6, %xmm3
        movss     4(%r10,%rcx,8), %xmm6
        movss     %xmm5, %xmm2
        mulss     %xmm5, %xmm5
        movss     %xmm1, %xmm0
        subss     %xmm1, %xmm6
        mulss     %xmm2, %xmm4
        addss     %xmm2, %xmm1
        addss     %xmm7, %xmm6
        subss     %xmm1, %xmm0
        mulss     %xmm4, %xmm5
        addss     %xmm6, %xmm3
        addss     %xmm2, %xmm0
        addss     %xmm3, %xmm5
        addss     %xmm5, %xmm0
        addss     %xmm1, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_3.0.2:
        addl      $75497472, %edx
        cmpl      $158859264, %edx
        ja        .L_2TAG_PACKET_4.0.2
        movss     8(%rsp), %xmm6
        movss     16(%rsp), %xmm1
        cvtps2pd  %xmm6, %xmm6
        cvtps2pd  %xmm1, %xmm1
        divsd     %xmm1, %xmm6
        movl      $1045220557, %r8d
        movd      %r8, %xmm7
        movl      $-1096111445, %r9d
        movd      %r9, %xmm2
        cvtss2sd  %xmm0, %xmm5
        movss     (%r10,%rcx,8), %xmm3
        movss     4(%r10,%rcx,8), %xmm4
        mulss     %xmm0, %xmm7
        movss     %xmm0, %xmm1
        mulss     %xmm0, %xmm2
        mulss     %xmm0, %xmm0
        mulss     %xmm0, %xmm2
        mulss     %xmm0, %xmm0
        subsd     %xmm5, %xmm6
        cvtpd2ps  %xmm6, %xmm6
        addss     %xmm3, %xmm2
        mulss     %xmm7, %xmm0
        addss     %xmm6, %xmm2
        addss     %xmm2, %xmm0
        addss     %xmm1, %xmm0
        addss     %xmm4, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_4.0.2:
        addl      $947912704, %edx
        cmpl      $1106771968, %edx
        jae       .L_2TAG_PACKET_5.0.2
        movss     (%r10,%rcx,8), %xmm2
        movss     4(%r10,%rcx,8), %xmm1
        addss     %xmm2, %xmm0
        addss     %xmm1, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_5.0.2:
        cmpl      $1602224128, %edx
        jae       .L_2TAG_PACKET_6.0.2
        movss     8(%rsp), %xmm0
        movss     16(%rsp), %xmm1
        movl      $-2147483648, %edx
        movd      %edx, %xmm7
        xorps     %xmm7, %xmm0
        cvtss2sd  %xmm0, %xmm5
        cvtss2sd  %xmm1, %xmm0
        divsd     %xmm5, %xmm0
        movq      a3d(%rip), %xmm2
        andl      $1, %ecx
        lea       PI2(%rip), %r9
        movq      (%r9,%rcx,8), %xmm3
        mulsd     %xmm0, %xmm2
        movq      %xmm0, %xmm1
        mulsd     %xmm0, %xmm0
        addsd     %xmm3, %xmm1
        mulsd     %xmm2, %xmm0
        addsd     %xmm1, %xmm0
        cvtsd2ss  %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_6.0.2:
        movss     pi_table(%rip), %xmm0
        movss     4+pi_table(%rip), %xmm1
        shll      $31, %ecx
        movd      %ecx, %xmm3
        addss     %xmm1, %xmm0
        xorps     %xmm3, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_0.0.2:
        addl      $528482304, %edx
        cmpl      $2139095040, %edx
        jae       .L_2TAG_PACKET_7.0.2
        cmpl      $0, %edx
        je        .L_2TAG_PACKET_8.0.2
        movl      %ecx, %edx
        andl      $-2147483648, %ecx
        andl      $2147483647, %edx
        subl      $528482304, %edx
.L_2TAG_PACKET_1.0.2:
        addl      $528482304, %edx
        cmpl      $2139095040, %edx
        jae       .L_2TAG_PACKET_9.0.2
        cmpl      $0, %edx
        je        .L_2TAG_PACKET_10.0.2
        cvtss2sd  %xmm0, %xmm0
        cvtss2sd  %xmm1, %xmm3
        divsd     %xmm3, %xmm0
        movss     8(%rsp), %xmm5
        pextrw    $3, %xmm0, %edx
        andl      $32767, %edx
        subl      $15344, %edx
        cmpl      $2048, %edx
        jae       .L_2TAG_PACKET_11.0.2
        movd      %xmm1, %edx
        cvtsd2ss  %xmm0, %xmm0
        andl      $2147483647, %edx
        cmpl      $276824064, %edx
        jb        .L_2TAG_PACKET_12.0.2
        cmpl      $1602224128, %edx
        ja        .L_2TAG_PACKET_13.0.2
        jmp       .L_2TAG_PACKET_2.0.2
.L_2TAG_PACKET_13.0.2:
        movl      $796917760, %edx
        movd      %edx, %xmm4
        mulss     %xmm4, %xmm1
        mulss     %xmm4, %xmm5
        jmp       .L_2TAG_PACKET_2.0.2
.L_2TAG_PACKET_12.0.2:
        movl      $1333788672, %edx
        movd      %edx, %xmm4
        mulss     %xmm4, %xmm1
        mulss     %xmm4, %xmm5
        jmp       .L_2TAG_PACKET_2.0.2
.L_2TAG_PACKET_11.0.2:
        shrl      $31, %eax
        shrl      $30, %ecx
        orl       %eax, %ecx
        addl      $15344, %edx
        cmpl      $17392, %edx
        jae       .L_2TAG_PACKET_6.0.2
        cmpl      $2, %ecx
        jb        .L_2TAG_PACKET_14.0.2
        lea       pi_table_add(%rip), %r10
        movss     (%r10,%rcx,8), %xmm2
        movss     4(%r10,%rcx,8), %xmm0
        addss     %xmm2, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_14.0.2:
        movq      M230(%rip), %xmm1
        mulsd     %xmm1, %xmm0
        cvtsd2ss  %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_7.0.2:
        ja        .L_2TAG_PACKET_15.0.2
        movl      %ecx, %edx
        andl      $-2147483648, %ecx
        andl      $2147483647, %edx
        cmpl      $2139095040, %edx
        je        .L_2TAG_PACKET_16.0.2
        ja        .L_2TAG_PACKET_17.0.2
        movss     pi_table(%rip), %xmm0
        movss     4+pi_table(%rip), %xmm1
        andl      $-2147483648, %eax
        movd      %eax, %xmm3
        addss     %xmm1, %xmm0
        xorps     %xmm3, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_16.0.2:
        movss     pi_table(%rip), %xmm0
        movss     4+pi_table(%rip), %xmm1
        movl      $1056964608, %edx
        sarl      $31, %ecx
        andl      $12582912, %ecx
        addl      %ecx, %edx
        movd      %edx, %xmm2
        addss     %xmm1, %xmm0
        mulss     %xmm2, %xmm0
        andl      $-2147483648, %eax
        movd      %eax, %xmm3
        xorps     %xmm3, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_15.0.2:
        addss     %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_8.0.2:
        movl      %ecx, %edx
        andl      $-2147483648, %ecx
        andl      $2147483647, %edx
        cmpl      $2139095040, %edx
        ja        .L_2TAG_PACKET_17.0.2
        cmpl      $0, %edx
        je        .L_2TAG_PACKET_18.0.2
        cmpl      $0, %ecx
        jne       .L_2TAG_PACKET_19.0.2
        jmp       ..B1.5
.L_2TAG_PACKET_20.0.2:
.L_2TAG_PACKET_19.0.2:
        movss     16+pi_table_add(%rip), %xmm0
        movss     20+pi_table_add(%rip), %xmm1
        andl      $-2147483648, %eax
        movd      %eax, %xmm3
        addss     %xmm1, %xmm0
        xorps     %xmm3, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_21.0.2:
        movss     16+pi_table_add(%rip), %xmm0
        movss     20+pi_table_add(%rip), %xmm1
        andl      $-2147483648, %eax
        movd      %eax, %xmm3
        addss     %xmm1, %xmm0
        xorps     %xmm3, %xmm0
        jmp       .L_2TAG_PACKET_22.0.2
.L_2TAG_PACKET_18.0.2:
        cmpl      $0, %ecx
        jne       .L_2TAG_PACKET_21.0.2
        jmp       .L_2TAG_PACKET_22.0.2
.L_2TAG_PACKET_17.0.2:
        movss     %xmm1, %xmm0
        addss     %xmm0, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_9.0.2:
        ja        .L_2TAG_PACKET_17.0.2
        cmpl      $0, %ecx
        jne       .L_2TAG_PACKET_20.0.2
        andl      $-2147483648, %eax
        movd      %eax, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_10.0.2:
        movss     pi_table(%rip), %xmm0
        movss     4+pi_table(%rip), %xmm1
        andl      $-2147483648, %eax
        movd      %eax, %xmm3
        addss     %xmm1, %xmm0
        xorps     %xmm3, %xmm0
        jmp       ..B1.5
.L_2TAG_PACKET_22.0.2:
        movss     %xmm0, (%rsp)
..B1.3:
        movss     (%rsp), %xmm0
.L_2TAG_PACKET_23.0.2:
..B1.5:
        addq      $24, %rsp
..___tag_value_atan2f.4:
        ret       
..___tag_value_atan2f.5:
END(atan2f)
# -- End  atan2f
	.section .rodata, "a"
	.align 16
	.align 16
atan_tbl:
	.long	0
	.long	0
	.long	1023669327
	.long	803928991
	.long	1024193038
	.long	2966692170
	.long	1024716678
	.long	811692700
	.long	1025240245
	.long	792964392
	.long	1025763734
	.long	802148141
	.long	1026287141
	.long	819324013
	.long	1026810463
	.long	790037051
	.long	1027333695
	.long	759154391
	.long	1027856833
	.long	817525057
	.long	1028379874
	.long	808527090
	.long	1028902813
	.long	821626264
	.long	1029425647
	.long	817580405
	.long	1029948372
	.long	2952445505
	.long	1030470983
	.long	801005003
	.long	1030993477
	.long	800387983
	.long	1031515850
	.long	2942256123
	.long	1032048979
	.long	2968250322
	.long	1032570964
	.long	822790803
	.long	1033092672
	.long	826289861
	.long	1033614088
	.long	2976224418
	.long	1034135194
	.long	824490938
	.long	1034655977
	.long	822527213
	.long	1035176421
	.long	784232806
	.long	1035696510
	.long	822531604
	.long	1036216230
	.long	2967672936
	.long	1036735564
	.long	826515440
	.long	1037254499
	.long	2944102134
	.long	1037773019
	.long	2977774463
	.long	1038291108
	.long	795957866
	.long	1038808753
	.long	2972700386
	.long	1039325938
	.long	2975253635
	.long	1039842648
	.long	807079588
	.long	1040402092
	.long	822355304
	.long	1040917290
	.long	805504040
	.long	1041431409
	.long	2978896586
	.long	1041944391
	.long	836680090
	.long	1042456182
	.long	826352980
	.long	1042966726
	.long	2973619487
	.long	1043475968
	.long	2937629464
	.long	1043983855
	.long	831159584
	.long	1044490335
	.long	820394738
	.long	1044995356
	.long	2965290852
	.long	1045498867
	.long	772017454
	.long	1046000819
	.long	2978687103
	.long	1046501162
	.long	836200518
	.long	1046999850
	.long	836883769
	.long	1047496836
	.long	826905524
	.long	1047992074
	.long	824423170
	.long	1048653779
	.long	2993658277
	.long	1049143506
	.long	2988293347
	.long	1049629355
	.long	838635411
	.long	1050111172
	.long	2981083232
	.long	1050588809
	.long	840069395
	.long	1051062131
	.long	2985487843
	.long	1051531009
	.long	2987007755
	.long	1051995325
	.long	2954909954
	.long	1052454971
	.long	2992105648
	.long	1052909846
	.long	2984512837
	.long	1053359860
	.long	2974535271
	.long	1053804931
	.long	834850816
	.long	1054244987
	.long	2990399399
	.long	1054679962
	.long	2986843574
	.long	1055109800
	.long	841668384
	.long	1055534454
	.long	840878179
	.long	1056161628
	.long	2959648204
	.long	1056972004
	.long	2999182783
	.long	1057370275
	.long	2987693957
	.long	1057757908
	.long	848029633
	.long	1058134927
	.long	833824667
	.long	1058501396
	.long	838916473
	.long	1058857417
	.long	2982869562
	.long	1059203123
	.long	2987447445
	.long	1059538675
	.long	2986943103
	.long	1059864257
	.long	2962018373
	.long	1060180073
	.long	3001690183
	.long	1060486341
	.long	3000978470
	.long	1060783292
	.long	855353569
	.long	1061071169
	.long	847903241
	.long	1061350219
	.long	2995336148
	.long	1061620693
	.long	848940038
	.long	1062010885
	.long	848518105
	.long	1062503512
	.long	2989792343
	.long	1062966414
	.long	2998000194
	.long	1063401558
	.long	2966602495
	.long	1063810833
	.long	841421013
	.long	1064196033
	.long	853285487
	.long	1064558850
	.long	831341538
	.long	1064900868
	.long	2999260393
	.long	1065223564
	.long	3001104089
	.long	1065440763
	.long	856880849
	.long	1065584798
	.long	852830020
	.long	1065721082
	.long	862757435
	.long	1065850159
	.long	2998927867
	.long	1065972528
	.long	2998512290
	.long	1066088651
	.long	2998663603
	.long	1066198953
	.long	2996780932
	.long	1066354340
	.long	3007554211
	.long	1066544552
	.long	2996943326
	.long	1066717600
	.long	3000009859
	.long	1066875577
	.long	856175525
	.long	1067020272
	.long	836800017
	.long	1067153216
	.long	3008748444
	.long	1067275724
	.long	851983949
	.long	1067388933
	.long	862585050
	.long	1067493827
	.long	3009301766
	.long	1067591258
	.long	2988483069
	.long	1067681973
	.long	3005223458
	.long	1067766623
	.long	839061141
	.long	1067845782
	.long	2995523700
	.long	1067919954
	.long	2995663457
	.long	1067989585
	.long	858133164
	.long	1068055072
	.long	829335339
	.long	1068146290
	.long	856294518
	.long	1068256389
	.long	3003850806
	.long	1068355146
	.long	856998103
	.long	1068444206
	.long	2999569255
	.long	1068524912
	.long	3011255227
	.long	1068598373
	.long	841513866
	.long	1068665514
	.long	841710807
	.long	1068727109
	.long	2949159239
	.long	1068783812
	.long	2995542644
	.long	1068836179
	.long	3007665083
	.long	1068884685
	.long	3006087365
	.long	1068929739
	.long	2996180184
	.long	1068971695
	.long	3006313179
	.long	1069010859
	.long	849476222
	.long	1069047500
	.long	860722755
	.long	1069081853
	.long	859062603
	.long	1069129538
	.long	2979745798
	.long	1069186846
	.long	842801502
	.long	1069238035
	.long	3010794384
	.long	1069284031
	.long	3010162926
	.long	1069325584
	.long	3005589807
	.long	1069363306
	.long	853875835
	.long	1069397703
	.long	851842476
	.long	1069429195
	.long	3001807968
	.long	1069458133
	.long	859424355
	.long	1069484817
	.long	3004202910
	.long	1069509498
	.long	843503196
	.long	1069532394
	.long	856475636
	.long	1069553692
	.long	2995114457
	.long	1069573552
	.long	860725119
	.long	1069592116
	.long	859745384
	.long	1069609507
	.long	3009201804
	.long	1069633623
	.long	855498560
	.long	1069662574
	.long	858167444
	.long	1069688405
	.long	853306415
	.long	1069711594
	.long	846705105
	.long	1069732526
	.long	858976312
	.long	1069751516
	.long	2999172562
	.long	1069768821
	.long	2999539371
	.long	1069784656
	.long	2998679696
	.long	1069799200
	.long	861304267
	.long	1069812606
	.long	843012447
	.long	1069825002
	.long	3010989671
	.long	1069836497
	.long	3005173143
	.long	1069847186
	.long	853102954
	.long	1069857152
	.long	2993551140
	.long	1069866465
	.long	3000116992
	.long	1069875187
	.long	827148142
	.type	atan_tbl,@object
	.size	atan_tbl,1288
	.space 8, 0x00 	# pad
	.align 16
PI2:
	.long	1413754136
	.long	1073291771
	.long	1413754136
	.long	3220775419
	.type	PI2,@object
	.size	PI2,16
	.align 8
pi_table_add:
	.long	0
	.long	0
	.long	0
	.long	0
	.long	874652008
	.long	1078530010
	.long	3022135656
	.long	3226013658
	.type	pi_table_add,@object
	.size	pi_table_add,32
	.align 8
a3d:
	.long	1431655765
	.long	3218429269
	.type	a3d,@object
	.size	a3d,8
	.align 8
M230:
	.long	4194304
	.long	1072693248
	.type	M230,@object
	.size	M230,8
	.align 4
pi_table:
	.long	1070141402
	.long	866263400
	.long	1078530010
	.long	874652008
	.long	1061752794
	.long	857874792
	.type	pi_table,@object
	.size	pi_table,24
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
	.4byte ..___tag_value_atan2f.1-.
	.4byte ..___tag_value_atan2f.5-..___tag_value_atan2f.1
	.2byte 0x0400
	.4byte ..___tag_value_atan2f.3-..___tag_value_atan2f.1
	.2byte 0x200e
	.byte 0x04
	.4byte ..___tag_value_atan2f.4-..___tag_value_atan2f.3
	.2byte 0x080e
	.byte 0x00
# End
