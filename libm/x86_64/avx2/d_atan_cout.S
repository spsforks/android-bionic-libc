/*
Copyright (C) 2019 The Android Open Source Project
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:
 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in
   the documentation and/or other materials provided with the
   distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
SUCH DAMAGE.
*/

# -- Machine type EM64t
	.text
..TXTST0:
.L_2__routine_start___svml_datan_ha_cout_rare_internal_0:
# -- Begin  __svml_datan_ha_cout_rare_internal
	.text
       .align    16,0x90
	.hidden __svml_datan_ha_cout_rare_internal
	.globl __svml_datan_ha_cout_rare_internal
__svml_datan_ha_cout_rare_internal:
# parameter 1: %rdi
# parameter 2: %rsi
..B1.1:
..L1:

        movzwl    6(%rdi), %r8d
        andl      $32752, %r8d
        shrl      $4, %r8d
        cmpl      $2047, %r8d
        je        ..B1.12
..B1.2:
        movq      (%rdi), %rdx
        movq      %rdx, -16(%rsp)
        shrq      $56, %rdx
        movb      7(%rdi), %al
        andl      $127, %edx
        movb      %dl, -9(%rsp)
        movsd     -16(%rsp), %xmm0
        shrb      $7, %al
        comisd    1888+_vmldAtanHATab(%rip), %xmm0
        movl      -12(%rsp), %ecx
        jb        ..B1.6
..B1.3:
        movsd     1896+_vmldAtanHATab(%rip), %xmm1
        comisd    %xmm0, %xmm1
        jbe       ..B1.5
..B1.4:
        movl      4(%rdi), %edx
        movl      %ecx, %edi
        andl      $-524288, %ecx
        andl      $-1048576, %edi
        addl      $262144, %ecx
        movaps    %xmm0, %xmm8
        andl      $1048575, %ecx
        movaps    %xmm0, %xmm9
        movsd     %xmm0, -56(%rsp)
        orl       %ecx, %edi
        movl      $0, -56(%rsp)
        andl      $1048575, %edx
        movl      %edi, -52(%rsp)
        lea       _vmldAtanHATab(%rip), %rcx
        movsd     1928+_vmldAtanHATab(%rip), %xmm3
        movsd     -56(%rsp), %xmm14
        shll      $20, %r8d
        subsd     -56(%rsp), %xmm8
        mulsd     1928+_vmldAtanHATab(%rip), %xmm9
        shlb      $7, %al
        mulsd     %xmm8, %xmm3
        movsd     %xmm3, -48(%rsp)
        orl       %edx, %r8d
        movsd     -48(%rsp), %xmm4
        addl      $-1069547520, %r8d
        sarl      $18, %r8d
        subsd     %xmm8, %xmm4
        movsd     %xmm4, -40(%rsp)
        andl      $-2, %r8d
        movsd     -48(%rsp), %xmm6
        movsd     -40(%rsp), %xmm5
        movslq    %r8d, %r8
        subsd     %xmm5, %xmm6
        movsd     %xmm6, -48(%rsp)
        movsd     -48(%rsp), %xmm7
        movsd     1904+_vmldAtanHATab(%rip), %xmm5
        subsd     %xmm7, %xmm8
        movsd     %xmm8, -40(%rsp)
        movsd     -48(%rsp), %xmm1
        movsd     -40(%rsp), %xmm2
        movsd     %xmm9, -48(%rsp)
        movsd     -48(%rsp), %xmm10
        movsd     1904+_vmldAtanHATab(%rip), %xmm7
        subsd     -16(%rsp), %xmm10
        movsd     %xmm10, -40(%rsp)
        movsd     -48(%rsp), %xmm12
        movsd     -40(%rsp), %xmm11
        subsd     %xmm11, %xmm12
        movsd     %xmm12, -48(%rsp)
        movsd     -48(%rsp), %xmm13
        subsd     %xmm13, %xmm0
        movsd     1904+_vmldAtanHATab(%rip), %xmm13
        movsd     %xmm0, -40(%rsp)
        movsd     -48(%rsp), %xmm3
        movsd     -40(%rsp), %xmm4
        mulsd     %xmm14, %xmm3
        mulsd     %xmm14, %xmm4
        movaps    %xmm3, %xmm15
        addsd     %xmm4, %xmm15
        movsd     %xmm15, -48(%rsp)
        movsd     -48(%rsp), %xmm0
        subsd     %xmm0, %xmm3
        addsd     %xmm3, %xmm4
        movsd     %xmm4, -40(%rsp)
        movsd     -48(%rsp), %xmm11
        movsd     -40(%rsp), %xmm0
        addsd     %xmm11, %xmm5
        movsd     %xmm5, -48(%rsp)
        movsd     -48(%rsp), %xmm6
        movsd     1928+_vmldAtanHATab(%rip), %xmm4
        subsd     %xmm6, %xmm7
        movsd     %xmm7, -40(%rsp)
        movsd     -48(%rsp), %xmm9
        movsd     -40(%rsp), %xmm8
        addsd     %xmm8, %xmm9
        movsd     1904+_vmldAtanHATab(%rip), %xmm8
        movsd     %xmm9, -32(%rsp)
        movsd     -40(%rsp), %xmm10
        addsd     %xmm10, %xmm11
        movsd     %xmm11, -40(%rsp)
        movsd     -32(%rsp), %xmm12
        subsd     %xmm12, %xmm13
        movsd     %xmm13, -32(%rsp)
        movsd     -40(%rsp), %xmm3
        movsd     -32(%rsp), %xmm14
        addsd     %xmm14, %xmm3
        movsd     %xmm3, -32(%rsp)
        movsd     -48(%rsp), %xmm7
        mulsd     %xmm7, %xmm4
        movsd     -32(%rsp), %xmm15
        movsd     %xmm4, -48(%rsp)
        addsd     %xmm15, %xmm0
        movsd     -48(%rsp), %xmm5
        subsd     %xmm7, %xmm5
        movsd     %xmm5, -40(%rsp)
        movsd     -48(%rsp), %xmm6
        movsd     -40(%rsp), %xmm15
        subsd     %xmm15, %xmm6
        movsd     %xmm6, -48(%rsp)
        movsd     -48(%rsp), %xmm3
        subsd     %xmm3, %xmm7
        movsd     %xmm7, -40(%rsp)
        movsd     -48(%rsp), %xmm13
        divsd     %xmm13, %xmm8
        movsd     -40(%rsp), %xmm3
        addsd     %xmm0, %xmm3
        movsd     1928+_vmldAtanHATab(%rip), %xmm0
        mulsd     %xmm8, %xmm0
        movsd     %xmm0, -40(%rsp)
        movsd     -40(%rsp), %xmm9
        movsd     1904+_vmldAtanHATab(%rip), %xmm0
        subsd     %xmm8, %xmm9
        movsd     %xmm9, -32(%rsp)
        movsd     -40(%rsp), %xmm11
        movsd     -32(%rsp), %xmm10
        subsd     %xmm10, %xmm11
        movsd     %xmm11, -32(%rsp)
        movsd     -32(%rsp), %xmm12
        mulsd     %xmm12, %xmm13
        movsd     -32(%rsp), %xmm14
        subsd     %xmm13, %xmm0
        mulsd     %xmm14, %xmm3
        movsd     %xmm3, -40(%rsp)
        movsd     -40(%rsp), %xmm13
        movsd     (%rcx,%r8,8), %xmm3
        subsd     %xmm13, %xmm0
        movsd     %xmm0, -40(%rsp)
        movaps    %xmm3, %xmm0
        movsd     -40(%rsp), %xmm7
        movsd     -32(%rsp), %xmm6
        movsd     -40(%rsp), %xmm4
        movaps    %xmm6, %xmm9
        movsd     -32(%rsp), %xmm5
        mulsd     %xmm2, %xmm6
        addsd     1904+_vmldAtanHATab(%rip), %xmm7
        mulsd     %xmm1, %xmm9
        mulsd     %xmm4, %xmm7
        mulsd     %xmm5, %xmm7
        movaps    %xmm7, %xmm8
        mulsd     %xmm2, %xmm8
        mulsd     %xmm7, %xmm1
        addsd     %xmm6, %xmm8
        movsd     1872+_vmldAtanHATab(%rip), %xmm4
        addsd     %xmm1, %xmm8
        movsd     %xmm8, -48(%rsp)
        movaps    %xmm9, %xmm1
        movsd     -48(%rsp), %xmm6
        addsd     %xmm6, %xmm1
        movsd     %xmm1, -48(%rsp)
        movsd     -48(%rsp), %xmm2
        subsd     %xmm2, %xmm9
        addsd     %xmm6, %xmm9
        movsd     %xmm9, -40(%rsp)
        movsd     -48(%rsp), %xmm7
        movaps    %xmm7, %xmm5
        addsd     %xmm7, %xmm0
        mulsd     %xmm7, %xmm5
        mulsd     %xmm5, %xmm4
        movsd     -40(%rsp), %xmm8
        movsd     %xmm0, -48(%rsp)
        movsd     -48(%rsp), %xmm1
        addsd     1864+_vmldAtanHATab(%rip), %xmm4
        subsd     %xmm1, %xmm3
        mulsd     %xmm5, %xmm4
        addsd     %xmm7, %xmm3
        addsd     1856+_vmldAtanHATab(%rip), %xmm4
        mulsd     %xmm5, %xmm4
        movsd     %xmm3, -40(%rsp)
        movsd     -48(%rsp), %xmm11
        movsd     -40(%rsp), %xmm10
        addsd     1848+_vmldAtanHATab(%rip), %xmm4
        mulsd     %xmm5, %xmm4
        addsd     1840+_vmldAtanHATab(%rip), %xmm4
        mulsd     %xmm5, %xmm4
        addsd     1832+_vmldAtanHATab(%rip), %xmm4
        mulsd     %xmm5, %xmm4
        addsd     1824+_vmldAtanHATab(%rip), %xmm4
        mulsd     %xmm4, %xmm5
        mulsd     %xmm5, %xmm7
        addsd     %xmm7, %xmm8
        addsd     8(%rcx,%r8,8), %xmm8
        addsd     %xmm8, %xmm10
        addsd     %xmm10, %xmm11
        movsd     %xmm11, -24(%rsp)
        movb      -17(%rsp), %r9b
        andb      $127, %r9b
        orb       %al, %r9b
        movb      %r9b, -17(%rsp)
        movq      -24(%rsp), %rax
        movq      %rax, (%rsi)
        jmp       ..B1.11
..B1.5:
        movsd     1912+_vmldAtanHATab(%rip), %xmm0
        shlb      $7, %al
        addsd     1920+_vmldAtanHATab(%rip), %xmm0
        movsd     %xmm0, -24(%rsp)
        movb      -17(%rsp), %dl
        andb      $127, %dl
        orb       %al, %dl
        movb      %dl, -17(%rsp)
        movq      -24(%rsp), %rax
        movq      %rax, (%rsi)
        jmp       ..B1.11
..B1.6:
        comisd    1880+_vmldAtanHATab(%rip), %xmm0
        jb        ..B1.8
..B1.7:
        movaps    %xmm0, %xmm2
        mulsd     %xmm0, %xmm2
        shlb      $7, %al
        movsd     1872+_vmldAtanHATab(%rip), %xmm1
        mulsd     %xmm2, %xmm1
        addsd     1864+_vmldAtanHATab(%rip), %xmm1
        mulsd     %xmm2, %xmm1
        addsd     1856+_vmldAtanHATab(%rip), %xmm1
        mulsd     %xmm2, %xmm1
        addsd     1848+_vmldAtanHATab(%rip), %xmm1
        mulsd     %xmm2, %xmm1
        addsd     1840+_vmldAtanHATab(%rip), %xmm1
        mulsd     %xmm2, %xmm1
        addsd     1832+_vmldAtanHATab(%rip), %xmm1
        mulsd     %xmm2, %xmm1
        addsd     1824+_vmldAtanHATab(%rip), %xmm1
        mulsd     %xmm1, %xmm2
        mulsd     %xmm0, %xmm2
        addsd     %xmm0, %xmm2
        movsd     %xmm2, -24(%rsp)
        movb      -17(%rsp), %dl
        andb      $127, %dl
        orb       %al, %dl
        movb      %dl, -17(%rsp)
        movq      -24(%rsp), %rax
        movq      %rax, (%rsi)
        jmp       ..B1.11
..B1.8:
        movzwl    -10(%rsp), %edx
        testl     $32752, %edx
        je        ..B1.10
..B1.9:
        movsd     1904+_vmldAtanHATab(%rip), %xmm1
        shlb      $7, %al
        addsd     %xmm0, %xmm1
        movsd     %xmm1, -48(%rsp)
        movsd     -48(%rsp), %xmm0
        mulsd     -16(%rsp), %xmm0
        movsd     %xmm0, -24(%rsp)
        movb      -17(%rsp), %dl
        andb      $127, %dl
        orb       %al, %dl
        movb      %dl, -17(%rsp)
        movq      -24(%rsp), %rax
        movq      %rax, (%rsi)
        jmp       ..B1.11
..B1.10:
        mulsd     %xmm0, %xmm0
        shlb      $7, %al
        movsd     %xmm0, -48(%rsp)
        movsd     -48(%rsp), %xmm0
        addsd     -16(%rsp), %xmm0
        movsd     %xmm0, -24(%rsp)
        movb      -17(%rsp), %dl
        andb      $127, %dl
        orb       %al, %dl
        movb      %dl, -17(%rsp)
        movq      -24(%rsp), %rax
        movq      %rax, (%rsi)
..B1.11:
        xorl      %eax, %eax
        ret
..B1.12:
        testl     $1048575, 4(%rdi)
        jne       ..B1.15
..B1.13:
        cmpl      $0, (%rdi)
        jne       ..B1.15
..B1.14:
        movsd     1912+_vmldAtanHATab(%rip), %xmm0
        movb      7(%rdi), %al
        andb      $-128, %al
        addsd     1920+_vmldAtanHATab(%rip), %xmm0
        movsd     %xmm0, -24(%rsp)
        movb      -17(%rsp), %dl
        andb      $127, %dl
        orb       %al, %dl
        movb      %dl, -17(%rsp)
        movq      -24(%rsp), %rcx
        movq      %rcx, (%rsi)
        jmp       ..B1.11
..B1.15:
        movsd     (%rdi), %xmm0
        addsd     %xmm0, %xmm0
        movsd     %xmm0, (%rsi)
        jmp       ..B1.11
        .align    16,0x90
	.type	__svml_datan_ha_cout_rare_internal,@function
	.size	__svml_datan_ha_cout_rare_internal,.-__svml_datan_ha_cout_rare_internal
..LN__svml_datan_ha_cout_rare_internal.0:
	.data
# -- End  __svml_datan_ha_cout_rare_internal
	.section .rodata, "a"
	.align 32
	.align 32
_vmldAtanHATab:
	.long	3892314112
	.long	1069799150
	.long	2332892550
	.long	1039715405
	.long	1342177280
	.long	1070305495
	.long	270726690
	.long	1041535749
	.long	939524096
	.long	1070817911
	.long	2253973841
	.long	3188654726
	.long	3221225472
	.long	1071277294
	.long	3853927037
	.long	1043226911
	.long	2818572288
	.long	1071767563
	.long	2677759107
	.long	1044314101
	.long	3355443200
	.long	1072103591
	.long	1636578514
	.long	3191094734
	.long	1476395008
	.long	1072475260
	.long	1864703685
	.long	3188646936
	.long	805306368
	.long	1072747407
	.long	192551812
	.long	3192726267
	.long	2013265920
	.long	1072892781
	.long	2240369452
	.long	1043768538
	.long	0
	.long	1072999953
	.long	3665168337
	.long	3192705970
	.long	402653184
	.long	1073084787
	.long	1227953434
	.long	3192313277
	.long	2013265920
	.long	1073142981
	.long	3853283127
	.long	1045277487
	.long	805306368
	.long	1073187261
	.long	1676192264
	.long	3192868861
	.long	134217728
	.long	1073217000
	.long	4290763938
	.long	1042034855
	.long	671088640
	.long	1073239386
	.long	994303084
	.long	3189643768
	.long	402653184
	.long	1073254338
	.long	1878067156
	.long	1042652475
	.long	1610612736
	.long	1073265562
	.long	670314820
	.long	1045138554
	.long	3221225472
	.long	1073273048
	.long	691126919
	.long	3189987794
	.long	3489660928
	.long	1073278664
	.long	1618990832
	.long	3188194509
	.long	1207959552
	.long	1073282409
	.long	2198872939
	.long	1044806069
	.long	3489660928
	.long	1073285217
	.long	2633982383
	.long	1042307894
	.long	939524096
	.long	1073287090
	.long	1059367786
	.long	3189114230
	.long	2281701376
	.long	1073288494
	.long	3158525533
	.long	1044484961
	.long	3221225472
	.long	1073289430
	.long	286581777
	.long	1044893263
	.long	4026531840
	.long	1073290132
	.long	2000245215
	.long	3191647611
	.long	134217728
	.long	1073290601
	.long	4205071590
	.long	1045035927
	.long	536870912
	.long	1073290952
	.long	2334392229
	.long	1043447393
	.long	805306368
	.long	1073291186
	.long	2281458177
	.long	3188885569
	.long	3087007744
	.long	1073291361
	.long	691611507
	.long	1044733832
	.long	3221225472
	.long	1073291478
	.long	1816229550
	.long	1044363390
	.long	2281701376
	.long	1073291566
	.long	1993843750
	.long	3189837440
	.long	134217728
	.long	1073291625
	.long	3654754496
	.long	1044970837
	.long	4026531840
	.long	1073291668
	.long	3224300229
	.long	3191935390
	.long	805306368
	.long	1073291698
	.long	2988777976
	.long	3188950659
	.long	536870912
	.long	1073291720
	.long	1030371341
	.long	1043402665
	.long	3221225472
	.long	1073291734
	.long	1524463765
	.long	1044361356
	.long	3087007744
	.long	1073291745
	.long	2754295320
	.long	1044731036
	.long	134217728
	.long	1073291753
	.long	3099629057
	.long	1044970710
	.long	2281701376
	.long	1073291758
	.long	962914160
	.long	3189838838
	.long	805306368
	.long	1073291762
	.long	3543908206
	.long	3188950786
	.long	4026531840
	.long	1073291764
	.long	1849909620
	.long	3191935434
	.long	3221225472
	.long	1073291766
	.long	1641333636
	.long	1044361352
	.long	536870912
	.long	1073291768
	.long	1373968792
	.long	1043402654
	.long	134217728
	.long	1073291769
	.long	2033191599
	.long	1044970710
	.long	3087007744
	.long	1073291769
	.long	4117947437
	.long	1044731035
	.long	805306368
	.long	1073291770
	.long	315378368
	.long	3188950787
	.long	2281701376
	.long	1073291770
	.long	2428571750
	.long	3189838838
	.long	3221225472
	.long	1073291770
	.long	1608007466
	.long	1044361352
	.long	4026531840
	.long	1073291770
	.long	1895711420
	.long	3191935434
	.long	134217728
	.long	1073291771
	.long	2031108713
	.long	1044970710
	.long	536870912
	.long	1073291771
	.long	1362518342
	.long	1043402654
	.long	805306368
	.long	1073291771
	.long	317461253
	.long	3188950787
	.long	939524096
	.long	1073291771
	.long	4117231784
	.long	1044731035
	.long	1073741824
	.long	1073291771
	.long	1607942376
	.long	1044361352
	.long	1207959552
	.long	1073291771
	.long	2428929577
	.long	3189838838
	.long	1207959552
	.long	1073291771
	.long	2031104645
	.long	1044970710
	.long	1342177280
	.long	1073291771
	.long	1895722602
	.long	3191935434
	.long	1342177280
	.long	1073291771
	.long	317465322
	.long	3188950787
	.long	1342177280
	.long	1073291771
	.long	1362515546
	.long	1043402654
	.long	1342177280
	.long	1073291771
	.long	1607942248
	.long	1044361352
	.long	1342177280
	.long	1073291771
	.long	4117231610
	.long	1044731035
	.long	1342177280
	.long	1073291771
	.long	2031104637
	.long	1044970710
	.long	1342177280
	.long	1073291771
	.long	1540251232
	.long	1045150466
	.long	1342177280
	.long	1073291771
	.long	2644671394
	.long	1045270303
	.long	1342177280
	.long	1073291771
	.long	2399244691
	.long	1045360181
	.long	1342177280
	.long	1073291771
	.long	803971124
	.long	1045420100
	.long	1476395008
	.long	1073291771
	.long	3613709523
	.long	3192879152
	.long	1476395008
	.long	1073291771
	.long	2263862659
	.long	3192849193
	.long	1476395008
	.long	1073291771
	.long	177735686
	.long	3192826724
	.long	1476395008
	.long	1073291771
	.long	1650295902
	.long	3192811744
	.long	1476395008
	.long	1073291771
	.long	2754716064
	.long	3192800509
	.long	1476395008
	.long	1073291771
	.long	3490996172
	.long	3192793019
	.long	1476395008
	.long	1073291771
	.long	1895722605
	.long	3192787402
	.long	1476395008
	.long	1073291771
	.long	2263862659
	.long	3192783657
	.long	1476395008
	.long	1073291771
	.long	3613709523
	.long	3192780848
	.long	1476395008
	.long	1073291771
	.long	1650295902
	.long	3192778976
	.long	1476395008
	.long	1073291771
	.long	177735686
	.long	3192777572
	.long	1476395008
	.long	1073291771
	.long	3490996172
	.long	3192776635
	.long	1476395008
	.long	1073291771
	.long	2754716064
	.long	3192775933
	.long	1476395008
	.long	1073291771
	.long	2263862659
	.long	3192775465
	.long	1476395008
	.long	1073291771
	.long	1895722605
	.long	3192775114
	.long	1476395008
	.long	1073291771
	.long	1650295902
	.long	3192774880
	.long	1476395008
	.long	1073291771
	.long	3613709523
	.long	3192774704
	.long	1476395008
	.long	1073291771
	.long	3490996172
	.long	3192774587
	.long	1476395008
	.long	1073291771
	.long	177735686
	.long	3192774500
	.long	1476395008
	.long	1073291771
	.long	2263862659
	.long	3192774441
	.long	1476395008
	.long	1073291771
	.long	2754716064
	.long	3192774397
	.long	1476395008
	.long	1073291771
	.long	1650295902
	.long	3192774368
	.long	1476395008
	.long	1073291771
	.long	1895722605
	.long	3192774346
	.long	1476395008
	.long	1073291771
	.long	3490996172
	.long	3192774331
	.long	1476395008
	.long	1073291771
	.long	3613709523
	.long	3192774320
	.long	1476395008
	.long	1073291771
	.long	2263862659
	.long	3192774313
	.long	1476395008
	.long	1073291771
	.long	177735686
	.long	3192774308
	.long	1476395008
	.long	1073291771
	.long	1650295902
	.long	3192774304
	.long	1476395008
	.long	1073291771
	.long	2754716064
	.long	3192774301
	.long	1476395008
	.long	1073291771
	.long	3490996172
	.long	3192774299
	.long	1476395008
	.long	1073291771
	.long	1895722605
	.long	3192774298
	.long	1476395008
	.long	1073291771
	.long	2263862659
	.long	3192774297
	.long	1476395008
	.long	1073291771
	.long	3613709523
	.long	3192774296
	.long	1476395008
	.long	1073291771
	.long	1650295902
	.long	3192774296
	.long	1476395008
	.long	1073291771
	.long	177735686
	.long	3192774296
	.long	1476395008
	.long	1073291771
	.long	3490996172
	.long	3192774295
	.long	1476395008
	.long	1073291771
	.long	2754716064
	.long	3192774295
	.long	1476395008
	.long	1073291771
	.long	2263862659
	.long	3192774295
	.long	1476395008
	.long	1073291771
	.long	1895722605
	.long	3192774295
	.long	1476395008
	.long	1073291771
	.long	1650295902
	.long	3192774295
	.long	1476395008
	.long	1073291771
	.long	1466225875
	.long	3192774295
	.long	1476395008
	.long	1073291771
	.long	1343512524
	.long	3192774295
	.long	1476395008
	.long	1073291771
	.long	1251477510
	.long	3192774295
	.long	1476395008
	.long	1073291771
	.long	1190120835
	.long	3192774295
	.long	1476395008
	.long	1073291771
	.long	1144103328
	.long	3192774295
	.long	1476395008
	.long	1073291771
	.long	1113424990
	.long	3192774295
	.long	1476395008
	.long	1073291771
	.long	1090416237
	.long	3192774295
	.long	1476395008
	.long	1073291771
	.long	1075077068
	.long	3192774295
	.long	1431655765
	.long	3218429269
	.long	2576978363
	.long	1070176665
	.long	2453154343
	.long	3217180964
	.long	4189149139
	.long	1069314502
	.long	1775019125
	.long	3216459198
	.long	273199057
	.long	1068739452
	.long	874748308
	.long	3215993277
	.long	0
	.long	1017118720
	.long	0
	.long	1069547520
	.long	0
	.long	1129316352
	.long	0
	.long	1072693248
	.long	1413754136
	.long	1073291771
	.long	856972295
	.long	1016178214
	.long	33554432
	.long	1101004800
	.type	_vmldAtanHATab,@object
	.size	_vmldAtanHATab,1936
	.data
	.section .note.GNU-stack, ""
# End
