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
# -- Begin  exp
ENTRY(exp)
# parameter 1: %xmm0
..B1.1:
..L1:

        pushq     %rbp
        movq      %rsp, %rbp
        andq      $-64, %rsp
        pushq     %rsi
        pushq     %rdi
        subq      $176, %rsp
        vmovapd   %xmm0, %xmm1
        vmovsd    1088+__avx2_exp_data_internal(%rip), %xmm2
        lea       __avx2_exp_data_internal(%rip), %rdx
        vfmadd132sd 1024+__avx2_exp_data_internal(%rip), %xmm2, %xmm1
        vmovsd    1728+__avx2_exp_data_internal(%rip), %xmm3
        vandpd    %xmm3, %xmm0, %xmm4
        vmovups   %xmm9, 16(%rsp)
        vmovups   %xmm8, 32(%rsp)
        vmovups   %xmm10, 128(%rsp)
        vsubsd    %xmm2, %xmm1, %xmm1
        vcmpnlesd 1792+__avx2_exp_data_internal(%rip), %xmm4, %xmm5
        vmovapd   %xmm1, %xmm7
        vfnmadd132sd 1216+__avx2_exp_data_internal(%rip), %xmm0, %xmm7
        vmovsd    1536+__avx2_exp_data_internal(%rip), %xmm2
        vmovq     1664+__avx2_exp_data_internal(%rip), %xmm4
        vmovmskpd %xmm5, %ecx
        vaddsd    1152+__avx2_exp_data_internal(%rip), %xmm1, %xmm5
        vfnmadd132sd 1280+__avx2_exp_data_internal(%rip), %xmm7, %xmm1
        vpand     %xmm4, %xmm5, %xmm6
        vpandn    %xmm5, %xmm4, %xmm8
        vpsllq    $42, %xmm8, %xmm10
        vfmadd213sd 1472+__avx2_exp_data_internal(%rip), %xmm1, %xmm2
        vmulsd    %xmm1, %xmm1, %xmm7
        vmovd     %xmm6, %eax
        vfmadd213sd 1408+__avx2_exp_data_internal(%rip), %xmm1, %xmm2
        vmovups   %xmm15, 96(%rsp)
        vmovups   %xmm14, 144(%rsp)
        vmovups   %xmm13, 48(%rsp)
        vmovups   %xmm12, 80(%rsp)
        vfmadd213sd 1344+__avx2_exp_data_internal(%rip), %xmm1, %xmm2
        vmovups   %xmm11, 112(%rsp)
        movslq    %eax, %rax
        vfmadd213sd %xmm1, %xmm7, %xmm2
        vmovupd   (%rdx,%rax), %xmm3
        vmovddup  8(%rdx,%rax), %xmm6
        vfmadd213sd %xmm6, %xmm3, %xmm2
        vaddsd    %xmm3, %xmm2, %xmm9
        vpaddq    %xmm10, %xmm9, %xmm1
        testb     $1, %cl
        jne       ..B1.3
..B1.2:
        vmovups   32(%rsp), %xmm8
        vmovapd   %xmm1, %xmm0
        vmovups   16(%rsp), %xmm9
        vmovups   128(%rsp), %xmm10
        vmovups   112(%rsp), %xmm11
        vmovups   80(%rsp), %xmm12
        vmovups   48(%rsp), %xmm13
        vmovups   144(%rsp), %xmm14
        vmovups   96(%rsp), %xmm15
        addq      $176, %rsp
        popq      %rdi
        popq      %rsi
        movq      %rbp, %rsp
        popq      %rbp
        ret
..B1.3:
        vcmpgtsd  1984+__avx2_exp_data_internal(%rip), %xmm0, %xmm2
        vcmpltsd  2048+__avx2_exp_data_internal(%rip), %xmm0, %xmm3
        vblendvpd %xmm2, .L_2il0floatpacket.15(%rip), %xmm1, %xmm1
        vorpd     %xmm3, %xmm2, %xmm4
        vmovmskpd %xmm4, %eax
        vandnpd   %xmm1, %xmm3, %xmm1
        andn      %ecx, %eax, %eax
        andl      $1, %eax
        je        ..B1.2
..B1.4:
        vmovsd    %xmm0, (%rsp)
        vmovsd    %xmm1, 64(%rsp)
        jne       ..B1.7
..B1.5:
        vmovsd    64(%rsp), %xmm1
        jmp       ..B1.2
..B1.7:
        lea       (%rsp), %rdi
        lea       64(%rsp), %rsi
        call      __avx2_exp_cout_rare_internal
        jmp       ..B1.5
END(exp)
# -- End exp
	.section .rodata, "a"
	.align 16
	.align 16
.L_2il0floatpacket.15:
	.long	0x00000000,0x7ff00000,0x00000000,0x7ff00000
	.type	.L_2il0floatpacket.15,@object
	.size	.L_2il0floatpacket.15,16
	.data
	.hidden __avx2_exp_data_internal
	.hidden __avx2_exp_cout_rare_internal
	.section .note.GNU-stack, ""
# End
