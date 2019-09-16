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
.L_2__routine_start___svml_scos_ha_cout_rare_internal_0:
# -- Begin  __svml_scos_ha_cout_rare_internal
	.text
       .align    16,0x90
	.hidden __svml_scos_ha_cout_rare_internal
	.globl __svml_scos_ha_cout_rare_internal
__svml_scos_ha_cout_rare_internal:
# parameter 1: %rdi
# parameter 2: %rsi
..B1.1:
	.cfi_startproc
..L1:

        movl      (%rdi), %edx
        movzwl    2(%rdi), %eax
        movl      %edx, -8(%rsp)
        andl      $32640, %eax
        shrl      $24, %edx
        andl      $127, %edx
        cmpl      $32640, %eax
        jne       ..B1.6
..B1.2:
        movb      %dl, -5(%rsp)
        cmpl      $2139095040, -8(%rsp)
        jne       ..B1.4
..B1.3:
        movss     (%rdi), %xmm0
        movl      $1, %eax
        mulss     _vmlsCosHATab(%rip), %xmm0
        movss     %xmm0, (%rsi)
        ret
..B1.4:
        movss     (%rdi), %xmm0
        xorl      %eax, %eax
        mulss     (%rdi), %xmm0
        movss     %xmm0, (%rsi)
..B1.5:
        ret
..B1.6:
        xorl      %eax, %eax
        ret
        .align    16,0x90
	.cfi_endproc
	.type	__svml_scos_ha_cout_rare_internal,@function
	.size	__svml_scos_ha_cout_rare_internal,.-__svml_scos_ha_cout_rare_internal
..LN__svml_scos_ha_cout_rare_internal.0:
	.data
# -- End  __svml_scos_ha_cout_rare_internal
	.section .rodata, "a"
	.align 8
	.align 8
_vmlsCosHATab:
	.long	0
	.long	2139095040
	.long	0
	.long	0
	.type	_vmlsCosHATab,@object
	.size	_vmlsCosHATab,16
	.data
	.section .note.GNU-stack, ""
# End
