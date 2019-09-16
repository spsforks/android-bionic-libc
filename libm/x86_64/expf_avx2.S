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
# -- Begin  
ENTRY(expf)
# parameter 1: %xmm0
..B1.1:
..L1:

        pushq     %rbp
	.cfi_def_cfa_offset 16
        movq      %rsp, %rbp
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
        andq      $-64, %rsp
        pushq     %rsi
        pushq     %rdi
        subq      $176, %rsp
        vmovaps   %xmm0, %xmm1
        vmovups   %xmm8, 80(%rsp)
        vmovaps   %xmm1, %xmm8
        vmovss    64+__svml_sexp_ha_data_internal(%rip), %xmm3
        vfmadd132ss __svml_sexp_ha_data_internal(%rip), %xmm3, %xmm8
        vmovups   %xmm11, 16(%rsp)
        vmovss    128+__svml_sexp_ha_data_internal(%rip), %xmm2
        vandps    %xmm2, %xmm1, %xmm4
        vmovups   384+__svml_sexp_ha_data_internal(%rip), %xmm7
        vpermilps %xmm8, %xmm7, %xmm7
        vsubss    %xmm3, %xmm8, %xmm0
        vcmpnless 192+__svml_sexp_ha_data_internal(%rip), %xmm4, %xmm5
        vmovaps   %xmm0, %xmm6
        vpslld    $21, %xmm8, %xmm4
        vfnmadd132ss 256+__svml_sexp_ha_data_internal(%rip), %xmm1, %xmm6
        vmovss    512+__svml_sexp_ha_data_internal(%rip), %xmm3
        vmovups   448+__svml_sexp_ha_data_internal(%rip), %xmm2
        vmovmskps %xmm5, %edx
        vmovss    704+__svml_sexp_ha_data_internal(%rip), %xmm5
        vmovups   %xmm10, 32(%rsp)
        vandps    %xmm5, %xmm4, %xmm10
        vfnmadd132ss 320+__svml_sexp_ha_data_internal(%rip), %xmm6, %xmm0
        vmovups   %xmm9, 48(%rsp)
        vmovups   %xmm12, 112(%rsp)
        vmovups   %xmm15, 144(%rsp)
        vmovups   %xmm14, 128(%rsp)
        vmovaps   %xmm0, %xmm11
        vfmadd213ss 576+__svml_sexp_ha_data_internal(%rip), %xmm3, %xmm11
        vmulss    %xmm0, %xmm0, %xmm6
        vfmadd213ss 640+__svml_sexp_ha_data_internal(%rip), %xmm0, %xmm11
        vmovups   %xmm13, 96(%rsp)
        vfmadd213ss %xmm7, %xmm6, %xmm11
        vpermilps %xmm8, %xmm2, %xmm9
        vaddss    %xmm0, %xmm11, %xmm0
        vmulss    %xmm10, %xmm9, %xmm12
        vfmadd213ss %xmm12, %xmm12, %xmm0
        testb     $1, %dl
        jne       ..B1.3
..B1.2:
        vmovups   80(%rsp), %xmm8
	.cfi_restore 25
        vmovups   48(%rsp), %xmm9
	.cfi_restore 26
        vmovups   32(%rsp), %xmm10
	.cfi_restore 27
        vmovups   16(%rsp), %xmm11
	.cfi_restore 28
        vmovups   112(%rsp), %xmm12
        .cfi_restore 29
        vmovups   96(%rsp), %xmm13
        .cfi_restore 30
        vmovups   128(%rsp), %xmm14
        .cfi_restore 31
        vmovups   144(%rsp), %xmm15
        .cfi_restore 32
        addq      $176, %rsp
	.cfi_restore 5
        popq      %rdi
	.cfi_restore 4
        popq      %rsi
        movq      %rbp, %rsp
        popq      %rbp
	.cfi_def_cfa 7, 8
	.cfi_restore 6
        ret
	.cfi_def_cfa 6, 16
	.cfi_offset 6, -16
..B1.3:
        vcmpgtss  2560+__svml_sexp_ha_data_internal(%rip), %xmm1, %xmm2
        vcmpltss  2624+__svml_sexp_ha_data_internal(%rip), %xmm1, %xmm3
        vblendvps %xmm2, .L_2il0floatpacket.14(%rip), %xmm0, %xmm0
        vorps     %xmm3, %xmm2, %xmm4
        vmovmskps %xmm4, %eax
        vandnps   %xmm0, %xmm3, %xmm0
        andn      %edx, %eax, %eax
        andl      $1, %eax
        je        ..B1.2
..B1.4:
        vmovss    %xmm1, (%rsp)
        vmovss    %xmm0, 64(%rsp)
        jne       ..B1.7
..B1.5:
        vmovss    64(%rsp), %xmm0
        jmp       ..B1.2
..B1.7:
        lea       (%rsp), %rdi
        lea       64(%rsp), %rsi
        call      __svml_sexp_ha_cout_rare_internal
        jmp       ..B1.5
END(expf)
	.section .rodata, "a"
	.align 16
	.align 16
.L_2il0floatpacket.14:
	.long	0x7f800000,0x7f800000,0x7f800000,0x7f800000
	.type	.L_2il0floatpacket.14,@object
	.size	.L_2il0floatpacket.14,16
	.data
	.hidden __svml_sexp_ha_data_internal
	.hidden __svml_sexp_ha_cout_rare_internal
	.section .note.GNU-stack, ""
# End
