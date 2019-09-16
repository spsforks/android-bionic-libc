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
# -- Begin  atan
ENTRYX86(atan)
# parameter 1: %xmm0
..B1.1:
..L1:

        pushq     %rbp
        movq      %rsp, %rbp
        andq      $-64, %rsp
        subq      $256, %rsp
        vmovapd   %xmm0, %xmm3
        vandpd    1152+__svml_datan_ha_data_internal(%rip), %xmm3, %xmm0
        lea       __svml_datan_ha_data_internal(%rip), %r8
        vpshufd   $221, %xmm0, %xmm6
        vxorpd    %xmm0, %xmm3, %xmm2
        vmovq     2560+__svml_datan_ha_data_internal(%rip), %xmm1
        vpsubd    %xmm1, %xmm6, %xmm7
        vmovq     2624+__svml_datan_ha_data_internal(%rip), %xmm1
        vmovups   %xmm14, 160(%rsp)
        vpcmpgtd  %xmm1, %xmm7, %xmm14
        vmovups   %xmm13, 144(%rsp)
        vpcmpeqd  %xmm1, %xmm7, %xmm13
        vpor      %xmm13, %xmm14, %xmm5
        vmovmskps %xmm5, %edx
        vmovq     1408+__svml_datan_ha_data_internal(%rip), %xmm13
        vmovq     1472+__svml_datan_ha_data_internal(%rip), %xmm4
        vpsubd    %xmm6, %xmm13, %xmm13
        vmovq     1536+__svml_datan_ha_data_internal(%rip), %xmm5
        vpsubd    %xmm6, %xmm4, %xmm4
        vmovq     1600+__svml_datan_ha_data_internal(%rip), %xmm7
        vpsrad    $31, %xmm13, %xmm13
        vpsubd    %xmm6, %xmm7, %xmm7
        vpsubd    %xmm6, %xmm5, %xmm6
        vpsrad    $31, %xmm7, %xmm7
        vpsrad    $31, %xmm4, %xmm4
        vpsrad    $31, %xmm6, %xmm6
        vpaddd    %xmm7, %xmm13, %xmm7
        vpaddd    %xmm6, %xmm4, %xmm13
        vmovq     1664+__svml_datan_ha_data_internal(%rip), %xmm14
        vpaddd    %xmm13, %xmm7, %xmm5
        vpaddd    %xmm14, %xmm5, %xmm14
        vpslld    $5, %xmm14, %xmm4
        vmovd     %xmm4, %eax
        vmovupd   1728+__svml_datan_ha_data_internal(%rip), %xmm1
        vpextrd   $1, %xmm4, %ecx
        vmovq     8(%rax,%r8), %xmm7
        vmovq     (%rax,%r8), %xmm6
        vmovq     16(%rax,%r8), %xmm13
        vmovhpd   8(%rcx,%r8), %xmm7, %xmm4
        vmovhpd   (%rcx,%r8), %xmm6, %xmm5
        vmovhpd   16(%rcx,%r8), %xmm13, %xmm6
        vandpd    %xmm0, %xmm4, %xmm13
        vmovq     24(%rax,%r8), %xmm14
        vmovhpd   24(%rcx,%r8), %xmm14, %xmm7
        vsubpd    %xmm5, %xmm13, %xmm14
        vandpd    %xmm1, %xmm4, %xmm13
        vmovapd   %xmm1, %xmm4
        vfmadd231pd %xmm5, %xmm0, %xmm13
        vcvtpd2ps %xmm13, %xmm0
        vmovlhps  %xmm0, %xmm0, %xmm0
        vrcpps    %xmm0, %xmm5
        vcvtps2pd %xmm5, %xmm5
        vfnmadd231pd %xmm13, %xmm5, %xmm4
        vfmadd213pd %xmm4, %xmm4, %xmm4
        vfmadd213pd %xmm5, %xmm4, %xmm5
        vfnmadd231pd %xmm13, %xmm5, %xmm1
        vfmadd213pd %xmm5, %xmm1, %xmm5
        vmulpd    %xmm5, %xmm14, %xmm0
        vfnmadd213pd %xmm14, %xmm0, %xmm13
        vmovupd   1792+__svml_datan_ha_data_internal(%rip), %xmm14
        vmulpd    %xmm13, %xmm5, %xmm1
        vmulpd    %xmm0, %xmm0, %xmm5
        vaddpd    %xmm1, %xmm7, %xmm1
        vmulpd    %xmm5, %xmm5, %xmm4
        vmovupd   1856+__svml_datan_ha_data_internal(%rip), %xmm13
        vfmadd213pd 1920+__svml_datan_ha_data_internal(%rip), %xmm4, %xmm14
        vfmadd213pd 1984+__svml_datan_ha_data_internal(%rip), %xmm4, %xmm13
        vfmadd213pd 2048+__svml_datan_ha_data_internal(%rip), %xmm4, %xmm14
        vfmadd213pd 2112+__svml_datan_ha_data_internal(%rip), %xmm4, %xmm13
        vfmadd213pd 2176+__svml_datan_ha_data_internal(%rip), %xmm4, %xmm14
        vfmadd213pd 2240+__svml_datan_ha_data_internal(%rip), %xmm4, %xmm13
        vfmadd213pd 2304+__svml_datan_ha_data_internal(%rip), %xmm4, %xmm14
        vfmadd213pd 2368+__svml_datan_ha_data_internal(%rip), %xmm4, %xmm13
        vfmadd213pd 2432+__svml_datan_ha_data_internal(%rip), %xmm4, %xmm14
        vfmadd213pd 2496+__svml_datan_ha_data_internal(%rip), %xmm4, %xmm13
        vfmadd213pd %xmm13, %xmm5, %xmm14
        vmulpd    %xmm14, %xmm5, %xmm4
        vfmadd213pd %xmm1, %xmm0, %xmm4
        vaddpd    %xmm4, %xmm0, %xmm7
        vaddpd    %xmm7, %xmm6, %xmm0
        vorpd     %xmm2, %xmm0, %xmm0
        andl      $3, %edx
        jne       ..B1.3
..B1.2:
        vmovups   144(%rsp), %xmm13
        vmovups   160(%rsp), %xmm14
        movq      %rbp, %rsp
        popq      %rbp
        ret
..B1.3:
        vmovupd   %xmm3, 128(%rsp)
        vmovupd   %xmm0, 192(%rsp)
        je        ..B1.2
..B1.6:
        xorl      %eax, %eax
        vmovups   %xmm8, 80(%rsp)
        vmovups   %xmm9, 64(%rsp)
        vmovups   %xmm10, 48(%rsp)
        vmovups   %xmm11, 32(%rsp)
        vmovups   %xmm12, 16(%rsp)
        vmovups   %xmm15, (%rsp)
        movq      %rsi, 104(%rsp)
        movq      %rdi, 96(%rsp)
        movq      %r12, 120(%rsp)
        movl      %eax, %r12d
        movq      %r13, 112(%rsp)
        movl      %edx, %r13d
..B1.7:
        btl       %r12d, %r13d
        jc        ..B1.10
..B1.8:
        incl      %r12d
        cmpl      $2, %r12d
        jl        ..B1.7
..B1.9:
        vmovups   80(%rsp), %xmm8
        vmovups   64(%rsp), %xmm9
        vmovups   48(%rsp), %xmm10
        vmovups   32(%rsp), %xmm11
        vmovups   16(%rsp), %xmm12
        vmovups   (%rsp), %xmm15
        movq      104(%rsp), %rsi
        movq      96(%rsp), %rdi
        movq      120(%rsp), %r12
        movq      112(%rsp), %r13
        vmovupd   192(%rsp), %xmm0
        jmp       ..B1.2
..B1.10:
        lea       128(%rsp,%r12,8), %rdi
        lea       192(%rsp,%r12,8), %rsi
        call      __svml_datan_ha_cout_rare_internal
        jmp       ..B1.8
ENDX86(atan)
	.data
# -- End  __svml_atan2_ha_l9
	.data
	.hidden __svml_datan_ha_data_internal
	.hidden __svml_datan_ha_cout_rare_internal
	.section .note.GNU-stack, ""
# End
