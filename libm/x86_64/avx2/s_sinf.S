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
# -- Begin  sin
ENTRYX86(sinf)
# parameter 1: %xmm0
..B1.1:
..L1:

        pushq     %rbp
        movq      %rsp, %rbp
        andq      $-64, %rsp
        subq      $320, %rsp
        xorl      %eax, %eax
        vmovss    4096+__svml_ssin_ha_data_internal(%rip), %xmm4
        vandps    %xmm4, %xmm0, %xmm5
        vmovss    4544+__svml_ssin_ha_data_internal(%rip), %xmm1
        vmovdqa   %xmm5, %xmm7
        vfmadd132ss 5056+__svml_ssin_ha_data_internal(%rip), %xmm1, %xmm7
        vbroadcastss %xmm5, %xmm6
        vpslld    $31, %xmm7, %xmm2
        vcvtps2pd %xmm6, %xmm3
        vsubss    %xmm1, %xmm7, %xmm6
        vbroadcastss %xmm6, %xmm7
        vcvtps2pd %xmm7, %xmm1
        vmovapd   %xmm1, %xmm6
        vfnmadd132sd 5120+__svml_ssin_ha_data_internal(%rip), %xmm3, %xmm6
        vfnmadd132sd 5184+__svml_ssin_ha_data_internal(%rip), %xmm6, %xmm1
        vandnps   %xmm0, %xmm4, %xmm6
        vmovsd    5440+__svml_ssin_ha_data_internal(%rip), %xmm4
        vmulsd    %xmm1, %xmm1, %xmm3
        vfmadd213sd 5376+__svml_ssin_ha_data_internal(%rip), %xmm3, %xmm4
        vfmadd213sd 5312+__svml_ssin_ha_data_internal(%rip), %xmm3, %xmm4
        vfmadd213sd 5248+__svml_ssin_ha_data_internal(%rip), %xmm3, %xmm4
        vmulsd    %xmm3, %xmm4, %xmm4
        vfmadd213sd %xmm1, %xmm1, %xmm4
        vxorps    %xmm6, %xmm2, %xmm1
        vmovddup  %xmm4, %xmm2
        vcvtpd2ps %xmm2, %xmm2
        vxorps    %xmm1, %xmm2, %xmm1
        vcmpnless 4160+__svml_ssin_ha_data_internal(%rip), %xmm5, %xmm2
        vmovmskps %xmm2, %edx
        testb     $1, %dl
        jne       ..B1.8
..B1.2:
        testl     %eax, %eax
        jne       ..B1.4
..B1.3:
        vmovdqa   %xmm1, %xmm0
        movq      %rbp, %rsp
        popq      %rbp
        ret
..B1.4:
        vmovss    %xmm0, 192(%rsp)
        vmovss    %xmm1, 256(%rsp)
        jne       ..B1.6
..B1.5:
        vmovss    256(%rsp), %xmm1
        jmp       ..B1.3
..B1.6:
        je        ..B1.5
..B1.7:
        movq      %rsi, 8(%rsp)
        lea       256(%rsp), %rsi
        movq      %rdi, (%rsp)
        lea       192(%rsp), %rdi
        vmovups   %xmm8, -128(%rsi)
        vmovups   %xmm9, -80(%rdi)
        vmovups   %xmm10, -96(%rdi)
        vmovups   %xmm11, -112(%rdi)
        vmovups   %xmm12, -128(%rdi)
        vmovups   %xmm13, 48(%rsp)
        vmovups   %xmm14, 32(%rsp)
        vmovups   %xmm15, 16(%rsp)
        call      __svml_ssin_ha_cout_rare_internal
..B1.11:
        vmovups   128(%rsp), %xmm8
        vmovups   112(%rsp), %xmm9
        vmovups   96(%rsp), %xmm10
        vmovups   80(%rsp), %xmm11
        vmovups   64(%rsp), %xmm12
        vmovups   48(%rsp), %xmm13
        vmovups   32(%rsp), %xmm14
        vmovups   16(%rsp), %xmm15
        movq      8(%rsp), %rsi
        movq      (%rsp), %rdi
        jmp       ..B1.5
..B1.8:
        vpand     .L_2il0floatpacket.17(%rip), %xmm0, %xmm6
        lea       __svml_ssin_ha_reduction_data_internal(%rip), %rcx
        vmovups   %xmm12, 64(%rsp)
        vpsrld    $23, %xmm6, %xmm12
        vmovups   %xmm10, 96(%rsp)
        lea       __svml_ssin_ha_data_internal(%rip), %r9
        vmovss    4224+__svml_ssin_ha_data_internal(%rip), %xmm10
        vmovups   %xmm11, 80(%rsp)
        vpslld    $1, %xmm12, %xmm11
        vmovups   %xmm9, 112(%rsp)
        vandps    %xmm5, %xmm10, %xmm9
        vpaddd    %xmm12, %xmm11, %xmm7
        vmovups   %xmm14, 32(%rsp)
        vcmpeqss  %xmm10, %xmm9, %xmm14
        vmovups   %xmm8, 128(%rsp)
        vpslld    $2, %xmm7, %xmm8
        vmovd     %xmm8, %edx
        vpand     .L_2il0floatpacket.18(%rip), %xmm0, %xmm3
        vmovups   %xmm13, 48(%rsp)
        vmovmskps %xmm14, %eax
        vmovd     8(%rcx,%rdx), %xmm13
        vpaddd    .L_2il0floatpacket.19(%rip), %xmm3, %xmm10
        vmovdqu   .L_2il0floatpacket.20(%rip), %xmm14
        vpsrld    $16, %xmm10, %xmm11
        vmovups   %xmm2, (%rsp)
        vpsrld    $16, %xmm13, %xmm2
        vmovd     4(%rcx,%rdx), %xmm4
        vpand     %xmm14, %xmm10, %xmm8
        vpand     %xmm14, %xmm13, %xmm13
        vpand     %xmm14, %xmm4, %xmm3
        vpmulld   %xmm13, %xmm11, %xmm5
        vpsrld    $16, %xmm4, %xmm7
        vpmulld   %xmm2, %xmm11, %xmm13
        andl      $1, %eax
        vpmulld   %xmm2, %xmm8, %xmm2
        vpmulld   %xmm3, %xmm8, %xmm4
        vpsrld    $16, %xmm2, %xmm2
        vmovups   %xmm15, 16(%rsp)
        vpsrld    $16, %xmm5, %xmm15
        vpand     %xmm14, %xmm4, %xmm5
        vpaddd    %xmm2, %xmm13, %xmm13
        vpaddd    %xmm13, %xmm5, %xmm5
        vmovd     (%rdx,%rcx), %xmm12
        vpaddd    %xmm5, %xmm15, %xmm13
        vpmulld   %xmm7, %xmm8, %xmm6
        vpand     %xmm14, %xmm12, %xmm9
        vpmulld   %xmm3, %xmm11, %xmm15
        vpsrld    $16, %xmm4, %xmm3
        vpand     %xmm14, %xmm6, %xmm5
        vpaddd    %xmm3, %xmm15, %xmm4
        vpmulld   %xmm9, %xmm8, %xmm10
        vpsrld    $16, %xmm13, %xmm2
        vpmulld   %xmm7, %xmm11, %xmm7
        vpaddd    %xmm4, %xmm5, %xmm5
        vpsrld    $16, %xmm6, %xmm6
        vpaddd    %xmm5, %xmm2, %xmm5
        vpand     %xmm14, %xmm10, %xmm2
        vpaddd    %xmm6, %xmm7, %xmm6
        vpsrld    $16, %xmm12, %xmm12
        vpsrld    $16, %xmm5, %xmm15
        vpmulld   %xmm12, %xmm8, %xmm8
        vpaddd    %xmm6, %xmm2, %xmm7
        vpmulld   %xmm9, %xmm11, %xmm9
        vpsrld    $16, %xmm10, %xmm11
        vpaddd    %xmm7, %xmm15, %xmm3
        vpand     %xmm14, %xmm8, %xmm15
        vpaddd    %xmm11, %xmm9, %xmm9
        vpsrld    $16, %xmm3, %xmm4
        vpaddd    %xmm9, %xmm15, %xmm10
        vpand     %xmm14, %xmm3, %xmm12
        vpaddd    %xmm10, %xmm4, %xmm6
        vpslld    $16, %xmm5, %xmm5
        vpslld    $16, %xmm6, %xmm11
        vpand     %xmm14, %xmm13, %xmm14
        vpand     .L_2il0floatpacket.21(%rip), %xmm0, %xmm8
        vpaddd    %xmm12, %xmm11, %xmm7
        vpxor     .L_2il0floatpacket.22(%rip), %xmm8, %xmm9
        vpsrld    $9, %xmm7, %xmm13
        vmovups   .L_2il0floatpacket.23(%rip), %xmm10
        vpaddd    %xmm14, %xmm5, %xmm12
        vpor      %xmm9, %xmm13, %xmm14
        vpand     .L_2il0floatpacket.27(%rip), %xmm7, %xmm7
        vaddss    %xmm14, %xmm10, %xmm6
        vpxor     .L_2il0floatpacket.24(%rip), %xmm8, %xmm4
        vsubss    %xmm10, %xmm6, %xmm5
        vpand     .L_2il0floatpacket.25(%rip), %xmm12, %xmm15
        vpsrld    $18, %xmm12, %xmm12
        vpxor     .L_2il0floatpacket.26(%rip), %xmm8, %xmm10
        vpslld    $14, %xmm7, %xmm8
        vpor      %xmm12, %xmm8, %xmm13
        vpslld    $5, %xmm15, %xmm2
        vpor      %xmm10, %xmm13, %xmm9
        vpor      %xmm4, %xmm2, %xmm3
        vpand     .L_2il0floatpacket.32(%rip), %xmm6, %xmm6
        vsubss    %xmm5, %xmm14, %xmm11
        vsubss    %xmm10, %xmm9, %xmm14
        vsubss    %xmm4, %xmm3, %xmm15
        vaddss    %xmm14, %xmm11, %xmm5
        vmovups   .L_2il0floatpacket.28(%rip), %xmm10
        vmovaps   %xmm5, %xmm9
        vmulss    %xmm10, %xmm5, %xmm3
        vsubss    %xmm5, %xmm11, %xmm11
        vfmsub213ss %xmm3, %xmm10, %xmm9
        vaddss    %xmm11, %xmm14, %xmm13
        vandps    .L_2il0floatpacket.30(%rip), %xmm0, %xmm14
        vfmadd132ss .L_2il0floatpacket.29(%rip), %xmm9, %xmm5
        vmovss    4352+__svml_ssin_ha_data_internal(%rip), %xmm11
        vaddss    %xmm15, %xmm13, %xmm12
        vmovups   .L_2il0floatpacket.31(%rip), %xmm15
        vpslld    $4, %xmm6, %xmm13
        vmovd     %xmm13, %r8d
        vcmpgtss  %xmm15, %xmm14, %xmm8
        vfmadd213ss %xmm5, %xmm10, %xmm12
        vcmpless  %xmm15, %xmm14, %xmm2
        vandps    %xmm0, %xmm2, %xmm4
        vandps    %xmm3, %xmm8, %xmm7
        vorps     %xmm7, %xmm4, %xmm9
        vandps    %xmm12, %xmm8, %xmm10
        vmovd     4(%r9,%r8), %xmm14
        vmovaps   %xmm9, %xmm3
        vfmadd132ss 12(%r9,%r8), %xmm14, %xmm3
        vmovaps   %xmm9, %xmm8
        vmulss    %xmm9, %xmm9, %xmm5
        vmovd     (%r8,%r9), %xmm15
        vsubss    %xmm3, %xmm14, %xmm2
        vfmadd213ss 4288+__svml_ssin_ha_data_internal(%rip), %xmm5, %xmm11
        vaddss    12(%r9,%r8), %xmm15, %xmm12
        vfmadd132ss 12(%r9,%r8), %xmm2, %xmm8
        vmulss    %xmm5, %xmm11, %xmm6
        vmovdqa   %xmm15, %xmm13
        vmovdqa   %xmm15, %xmm7
        vfmadd213ss %xmm3, %xmm9, %xmm13
        vmovss    4480+__svml_ssin_ha_data_internal(%rip), %xmm15
        vmovups   80(%rsp), %xmm11
        vfmadd213ss 4416+__svml_ssin_ha_data_internal(%rip), %xmm5, %xmm15
        vsubss    %xmm13, %xmm3, %xmm4
        vmulss    %xmm9, %xmm6, %xmm3
        vmulss    %xmm5, %xmm15, %xmm5
        vfmadd213ss %xmm4, %xmm9, %xmm7
        vfnmadd213ss %xmm12, %xmm14, %xmm9
        vmovups   64(%rsp), %xmm12
        vmovups   16(%rsp), %xmm15
        vaddss    %xmm8, %xmm7, %xmm2
        vmovups   128(%rsp), %xmm8
        vfmadd213ss 8(%r9,%r8), %xmm9, %xmm10
        vfmadd213ss %xmm2, %xmm9, %xmm3
        vfmadd213ss %xmm10, %xmm14, %xmm5
        vmovups   (%rsp), %xmm14
        vaddss    %xmm3, %xmm5, %xmm9
        vaddss    %xmm9, %xmm13, %xmm10
        vmovups   112(%rsp), %xmm9
        vmovups   48(%rsp), %xmm13
        vblendvps %xmm14, %xmm10, %xmm1, %xmm1
        vmovups   96(%rsp), %xmm10
        vmovups   32(%rsp), %xmm14
        jmp       ..B1.2
ENDX86(sinf)
	.section .rodata, "a"
	.align 16
	.align 16
.L_2il0floatpacket.17:
	.long	0x7f800000,0x7f800000,0x7f800000,0x7f800000
	.type	.L_2il0floatpacket.17,@object
	.size	.L_2il0floatpacket.17,16
	.align 16
.L_2il0floatpacket.18:
	.long	0x007fffff,0x007fffff,0x007fffff,0x007fffff
	.type	.L_2il0floatpacket.18,@object
	.size	.L_2il0floatpacket.18,16
	.align 16
.L_2il0floatpacket.19:
	.long	0x00800000,0x00800000,0x00800000,0x00800000
	.type	.L_2il0floatpacket.19,@object
	.size	.L_2il0floatpacket.19,16
	.align 16
.L_2il0floatpacket.20:
	.long	0x0000ffff,0x0000ffff,0x0000ffff,0x0000ffff
	.type	.L_2il0floatpacket.20,@object
	.size	.L_2il0floatpacket.20,16
	.align 16
.L_2il0floatpacket.21:
	.long	0x80000000,0x80000000,0x80000000,0x80000000
	.type	.L_2il0floatpacket.21,@object
	.size	.L_2il0floatpacket.21,16
	.align 16
.L_2il0floatpacket.22:
	.long	0x3f800000,0x3f800000,0x3f800000,0x3f800000
	.type	.L_2il0floatpacket.22,@object
	.size	.L_2il0floatpacket.22,16
	.align 16
.L_2il0floatpacket.23:
	.long	0x47400000,0x47400000,0x47400000,0x47400000
	.type	.L_2il0floatpacket.23,@object
	.size	.L_2il0floatpacket.23,16
	.align 16
.L_2il0floatpacket.24:
	.long	0x28800000,0x28800000,0x28800000,0x28800000
	.type	.L_2il0floatpacket.24,@object
	.size	.L_2il0floatpacket.24,16
	.align 16
.L_2il0floatpacket.25:
	.long	0x0003ffff,0x0003ffff,0x0003ffff,0x0003ffff
	.type	.L_2il0floatpacket.25,@object
	.size	.L_2il0floatpacket.25,16
	.align 16
.L_2il0floatpacket.26:
	.long	0x34000000,0x34000000,0x34000000,0x34000000
	.type	.L_2il0floatpacket.26,@object
	.size	.L_2il0floatpacket.26,16
	.align 16
.L_2il0floatpacket.27:
	.long	0x000001ff,0x000001ff,0x000001ff,0x000001ff
	.type	.L_2il0floatpacket.27,@object
	.size	.L_2il0floatpacket.27,16
	.align 16
.L_2il0floatpacket.28:
	.long	0x40c90fdb,0x40c90fdb,0x40c90fdb,0x40c90fdb
	.type	.L_2il0floatpacket.28,@object
	.size	.L_2il0floatpacket.28,16
	.align 16
.L_2il0floatpacket.29:
	.long	0xb43bbd2e,0xb43bbd2e,0xb43bbd2e,0xb43bbd2e
	.type	.L_2il0floatpacket.29,@object
	.size	.L_2il0floatpacket.29,16
	.align 16
.L_2il0floatpacket.30:
	.long	0x7fffffff,0x7fffffff,0x7fffffff,0x7fffffff
	.type	.L_2il0floatpacket.30,@object
	.size	.L_2il0floatpacket.30,16
	.align 16
.L_2il0floatpacket.31:
	.long	0x35800000,0x35800000,0x35800000,0x35800000
	.type	.L_2il0floatpacket.31,@object
	.size	.L_2il0floatpacket.31,16
	.align 16
.L_2il0floatpacket.32:
	.long	0x000000ff,0x000000ff,0x000000ff,0x000000ff
	.type	.L_2il0floatpacket.32,@object
	.size	.L_2il0floatpacket.32,16
	.data
	.hidden __svml_ssin_ha_reduction_data_internal
	.hidden __svml_ssin_ha_data_internal
	.hidden __svml_ssin_ha_cout_rare_internal
	.section .note.GNU-stack, ""
# End
