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

#include <private/bionic_asm.h>
# -- Begin  atanf
ENTRY(atanf)
# parameter 1: %xmm0
..B1.1:
..L1:

        pushq     %rbp
	.cfi_def_cfa_offset 16
        movq      %rsp, %rbp
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
        andq      $-64, %rsp
        subq      $192, %rsp
        vandps    192+__svml_satan_ha_data_internal(%rip), %xmm0, %xmm3
        vmovups   1984+__svml_satan_ha_data_internal(%rip), %xmm6
        vpsubd    1920+__svml_satan_ha_data_internal(%rip), %xmm3, %xmm7
        vpcmpgtd  %xmm6, %xmm7, %xmm4
        vpcmpeqd  %xmm6, %xmm7, %xmm2
        vmovups   384+__svml_satan_ha_data_internal(%rip), %xmm7
        vpor      %xmm2, %xmm4, %xmm1
        vmovups   %xmm12, 176(%rsp)
        vpcmpgtd  %xmm3, %xmm7, %xmm5
        vmovups   %xmm11, 48(%rsp)
        vmovups   256+__svml_satan_ha_data_internal(%rip), %xmm12
        vmovups   320+__svml_satan_ha_data_internal(%rip), %xmm11
        vpcmpgtd  448+__svml_satan_ha_data_internal(%rip), %xmm3, %xmm7
        vmovups   %xmm13, 144(%rsp)
        vmovups   %xmm10, 160(%rsp)
        vmovups   %xmm9, 80(%rsp)
        vpcmpgtd  %xmm3, %xmm11, %xmm9
        vmovups   %xmm0, (%rsp)
        vpandn    %xmm5, %xmm9, %xmm6
        vandps    128+__svml_satan_ha_data_internal(%rip), %xmm0, %xmm10
        vpcmpgtd  %xmm3, %xmm12, %xmm0
        vmovmskps %xmm1, %edx
        vpxor     1728+__svml_satan_ha_data_internal(%rip), %xmm7, %xmm13
        vmovups   768+__svml_satan_ha_data_internal(%rip), %xmm1
        vmovups   832+__svml_satan_ha_data_internal(%rip), %xmm2
        vsubps    %xmm1, %xmm3, %xmm12
        vmovups   896+__svml_satan_ha_data_internal(%rip), %xmm4
        vmovups   %xmm14, 112(%rsp)
        vsubps    %xmm4, %xmm3, %xmm14
        vmulps    %xmm4, %xmm3, %xmm4
        vmovups   %xmm8, 96(%rsp)
        vpandn    %xmm9, %xmm0, %xmm8
        vpandn    %xmm13, %xmm5, %xmm9
        vandps    %xmm0, %xmm3, %xmm13
        vandps    %xmm8, %xmm12, %xmm11
        vsubps    %xmm2, %xmm3, %xmm5
        vmulps    %xmm2, %xmm3, %xmm2
        vandps    %xmm6, %xmm5, %xmm12
        vandps    %xmm9, %xmm14, %xmm5
        vorps     %xmm11, %xmm13, %xmm11
        vorps     %xmm5, %xmm12, %xmm12
        vandps    64+__svml_satan_ha_data_internal(%rip), %xmm7, %xmm14
        vorps     %xmm12, %xmm11, %xmm11
        vmovups   __svml_satan_ha_data_internal(%rip), %xmm13
        vorps     %xmm14, %xmm11, %xmm5
        vandps    %xmm0, %xmm13, %xmm11
        vaddps    %xmm2, %xmm13, %xmm14
        vmulps    %xmm1, %xmm3, %xmm0
        vcvtps2pd %xmm5, %ymm5
        vandps    %xmm6, %xmm14, %xmm14
        vaddps    %xmm0, %xmm13, %xmm1
        vaddps    %xmm4, %xmm13, %xmm13
        vandps    %xmm8, %xmm1, %xmm12
        vandps    %xmm9, %xmm13, %xmm0
        vandps    %xmm7, %xmm3, %xmm13
        vorps     %xmm12, %xmm11, %xmm3
        vorps     %xmm0, %xmm14, %xmm12
        vorps     %xmm12, %xmm3, %xmm11
        vandps    960+__svml_satan_ha_data_internal(%rip), %xmm8, %xmm0
        vorps     %xmm13, %xmm11, %xmm11
        vandps    1088+__svml_satan_ha_data_internal(%rip), %xmm6, %xmm1
        vorps     %xmm1, %xmm0, %xmm4
        vrcpps    %xmm11, %xmm0
        vcvtps2pd %xmm11, %ymm13
        vcvtps2pd %xmm0, %ymm1
        vandps    1216+__svml_satan_ha_data_internal(%rip), %xmm9, %xmm2
        vandps    1344+__svml_satan_ha_data_internal(%rip), %xmm7, %xmm3
        vorps     %xmm3, %xmm2, %xmm14
        vmulpd    %ymm1, %ymm13, %ymm13
        vorps     %xmm14, %xmm4, %xmm12
        vmovupd   1856+__svml_satan_ha_data_internal(%rip), %ymm4
        vandps    1024+__svml_satan_ha_data_internal(%rip), %xmm8, %xmm8
        vandps    1152+__svml_satan_ha_data_internal(%rip), %xmm6, %xmm6
        vandps    1280+__svml_satan_ha_data_internal(%rip), %xmm9, %xmm9
        vorps     %xmm6, %xmm8, %xmm6
        vandps    1408+__svml_satan_ha_data_internal(%rip), %xmm7, %xmm7
        vsubpd    %ymm13, %ymm4, %ymm3
        vorps     %xmm7, %xmm9, %xmm7
        vmulpd    %ymm3, %ymm1, %ymm2
        vmulpd    %ymm3, %ymm3, %ymm14
        vaddpd    %ymm2, %ymm1, %ymm11
        vaddpd    %ymm14, %ymm4, %ymm0
        vmulpd    %ymm0, %ymm11, %ymm1
        vmulpd    %ymm1, %ymm5, %ymm5
        vandpd    1792+__svml_satan_ha_data_internal(%rip), %ymm5, %ymm0
        vcvtpd2ps %ymm0, %xmm11
        vsubpd    %ymm0, %ymm5, %ymm1
        vorps     %xmm7, %xmm6, %xmm0
        vmulps    %xmm11, %xmm11, %xmm13
        vcvtpd2ps %ymm1, %xmm2
        vmulps    %xmm13, %xmm13, %xmm3
        vaddps    %xmm0, %xmm2, %xmm0
        vmulps    1472+__svml_satan_ha_data_internal(%rip), %xmm3, %xmm1
        vmulps    1536+__svml_satan_ha_data_internal(%rip), %xmm3, %xmm4
        vaddps    1600+__svml_satan_ha_data_internal(%rip), %xmm1, %xmm5
        vaddps    1664+__svml_satan_ha_data_internal(%rip), %xmm4, %xmm8
        vmulps    %xmm5, %xmm13, %xmm9
        vaddps    %xmm9, %xmm8, %xmm14
        vmulps    %xmm14, %xmm13, %xmm1
        vmulps    %xmm1, %xmm11, %xmm2
        vmovups   (%rsp), %xmm1
        vaddps    %xmm2, %xmm0, %xmm3
        vaddps    %xmm3, %xmm11, %xmm11
        vaddps    %xmm11, %xmm12, %xmm12
        vorps     %xmm10, %xmm12, %xmm0
        vmovups   176(%rsp), %xmm12
	.cfi_restore 29
        testl     %edx, %edx
        jne       ..B1.3
..B1.2:
        vzeroupper
        vmovups   96(%rsp), %xmm8
	.cfi_restore 25
        vmovups   80(%rsp), %xmm9
	.cfi_restore 26
        vmovups   160(%rsp), %xmm10
	.cfi_restore 27
        vmovups   48(%rsp), %xmm11
	.cfi_restore 28
        vmovups   144(%rsp), %xmm13
	.cfi_restore 30
        vmovups   112(%rsp), %xmm14
	.cfi_restore 31
        movq      %rbp, %rsp
        popq      %rbp
	.cfi_def_cfa 7, 8
	.cfi_restore 6
        ret
        .cfi_def_cfa 6, 16
	.cfi_offset 6, -16
..B1.3:
        vmovups   %xmm1, 64(%rsp)
        vmovups   %xmm0, 128(%rsp)
..B1.6:
        xorl      %eax, %eax
        vmovups   %xmm12, 176(%rsp)
        vmovups   %xmm15, (%rsp)
        movq      %rsi, 24(%rsp)
        movq      %rdi, 16(%rsp)
        movq      %r12, 40(%rsp)
        movl      %eax, %r12d
        movq      %r13, 32(%rsp)
        movl      %edx, %r13d
..B1.7:
        btl       %r12d, %r13d
        jc        ..B1.10
..B1.8:
        incl      %r12d
        cmpl      $4, %r12d
        jl        ..B1.7
..B1.9:
        vmovups   176(%rsp), %xmm12
	.cfi_restore 29
        vmovups   (%rsp), %xmm15
	.cfi_restore 32
        movq      24(%rsp), %rsi
	.cfi_restore 4
        movq      16(%rsp), %rdi
	.cfi_restore 5
        movq      40(%rsp), %r12
	.cfi_restore 12
        movq      32(%rsp), %r13
	.cfi_restore 13
        vmovups   128(%rsp), %xmm0
        jmp       ..B1.2
..B1.10:
        vzeroupper
        lea       64(%rsp,%r12,4), %rdi
        lea       128(%rsp,%r12,4), %rsi
        call      __svml_satan_ha_cout_rare_internal
        jmp       ..B1.8
END(atanf)        
# -- End  __svml_atanf4_ha_e9
	.data
	.hidden __svml_satan_ha_data_internal
	.hidden __svml_satan_ha_cout_rare_internal
	.section .note.GNU-stack, ""
# End
