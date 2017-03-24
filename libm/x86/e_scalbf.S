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
# -- Begin  scalbf
ENTRY(scalbf)
# parameter 1: 32 + %esp
# parameter 2: 36 + %esp
..B1.1:
        pushl     %esi
        pushl     %edi
        pushl     %ebx
        pushl     %ebp
        subl      $12, %esp
        movzwl    34(%esp), %esi
        movl      %esi, %ebp
        andl      $32640, %ebp
        movzwl    38(%esp), %ebx
        andl      $32640, %ebx
        shrl      $7, %ebp
        shrl      $7, %ebx
        call      ..L2
..L2:
        popl      %edi
        lea       _GLOBAL_OFFSET_TABLE_+[. - ..L2](%edi), %edi
        cmpl      $255, %ebp
        je        ..B1.48
..B1.2:
        cmpl      $255, %ebx
        je        ..B1.31
..B1.3:
        cmpl      $127, %ebx
        jl        ..B1.16
..B1.4:
        cmpl      $150, %ebx
        jge       ..B1.7
..B1.5:
        movl      36(%esp), %edx
        movl      %ebx, %ecx
        andl      $8388607, %edx
        negl      %ecx
        movl      %edx, %eax
        addl      $22, %ecx
        orl       $8388608, %eax
        addl      $10, %ebx
        shrl      %cl, %eax
        movl      %ebx, %ecx
        shll      %cl, %edx
        testl     %edx, %edx
        je        ..B1.8
..B1.6:
        pxor      %xmm0, %xmm0
        mulss     .L_2il0floatpacket.0@GOTOFF(%edi), %xmm0
        movss     %xmm0, 4(%esp)
        flds      4(%esp)
        addl      $12, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        popl      %esi
        ret       
..B1.7:
        movl      $65536, %eax
..B1.8:
        testl     %ebp, %ebp
        jne       ..B1.11
..B1.9:
        testl     $8388607, 32(%esp)
        je        ..B1.15
..B1.10:
        movss     32(%esp), %xmm0
        mulss     .L_2il0floatpacket.1@GOTOFF(%edi), %xmm0
        movss     %xmm0, 32(%esp)
        movzwl    34(%esp), %esi
        movl      %esi, %ebp
        andl      $32640, %ebp
        shrl      $7, %ebp
        addl      $-25, %ebp
..B1.11:
        movl      $65536, %edx
        cmpl      $65536, %eax
        movzbl    39(%esp), %ecx
        cmovg     %edx, %eax
        movl      %eax, %ebx
        negl      %ebx
        testl     $128, %ecx
        cmovne    %ebx, %eax
        lea       (%ebp,%eax), %edx
        testl     %edx, %edx
        jle       ..B1.26
..B1.12:
        cmpl      $255, %edx
        jge       ..B1.25
..B1.13:
        movzbl    %dl, %edx
        andl      $-32641, %esi
        shll      $7, %edx
        orl       %edx, %esi
        movw      %si, 34(%esp)
        movss     32(%esp), %xmm0
..B1.14:
        movss     %xmm0, 4(%esp)
        flds      4(%esp)
        addl      $12, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        popl      %esi
        ret       
..B1.15:
        flds      32(%esp)
        addl      $12, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        popl      %esi
        ret       
..B1.16:
        testl     %ebx, %ebx
        jne       ..B1.19
..B1.17:
        testl     $8388607, 36(%esp)
        jne       ..B1.19
..B1.18:
        flds      32(%esp)
        addl      $12, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        popl      %esi
        ret       
..B1.19:
        cmpl      $255, %ebp
        jne       ..B1.24
..B1.20:
        testl     %ebx, %ebx
        jg        ..B1.22
..B1.21:
        pxor      %xmm0, %xmm0
        mulss     .L_2il0floatpacket.0@GOTOFF(%edi), %xmm0
        movss     %xmm0, 4(%esp)
        flds      4(%esp)
        addl      $12, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        popl      %esi
        ret       
..B1.22:
        cmpl      $1, %ebx
        je        ..B1.30
..B1.23:
        movzbl    35(%esp), %eax
        andl      $128, %eax
        shrl      $7, %eax
        flds      _infs@GOTOFF(%edi,%eax,4)
        addl      $12, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        popl      %esi
        ret       
..B1.24:
        pxor      %xmm0, %xmm0
        mulss     .L_2il0floatpacket.0@GOTOFF(%edi), %xmm0
        movss     %xmm0, 4(%esp)
        flds      4(%esp)
        addl      $12, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        popl      %esi
        ret       
..B1.25:
        movzbl    35(%esp), %eax
        andl      $128, %eax
        shrl      $7, %eax
        movss     _large_value_32@GOTOFF(%edi,%eax,4), %xmm0
        mulss     .L_2il0floatpacket.2@GOTOFF(%edi), %xmm0
        movss     %xmm0, (%esp)
        jmp       ..B1.14
..B1.26:
        cmpl      $-23, %edx
        jge       ..B1.28
..B1.27:
        movzbl    35(%esp), %eax
        andl      $128, %eax
        shrl      $7, %eax
        movss     _small_value_32@GOTOFF(%edi,%eax,4), %xmm0
        mulss     .L_2il0floatpacket.3@GOTOFF(%edi), %xmm0
        jmp       ..B1.29
..B1.28:
        movl      32(%esp), %edx
        lea       25(%ebp,%eax), %eax
        movl      %edx, (%esp)
        shrl      $16, %edx
        movzbl    %al, %eax
        andl      $-32641, %edx
        shll      $7, %eax
        orl       %eax, %edx
        movw      %dx, 2(%esp)
        movss     (%esp), %xmm0
        mulss     .L_2il0floatpacket.4@GOTOFF(%edi), %xmm0
..B1.29:
        movss     %xmm0, 4(%esp)
        flds      4(%esp)
        movss     %xmm0, (%esp)
        addl      $12, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        popl      %esi
        ret       
..B1.30:
        testl     $8388607, 36(%esp)
        jle       ..B1.21
        jmp       ..B1.23
..B1.31:
        testl     $8388607, 36(%esp)
        jne       ..B1.43
..B1.32:
        testl     %ebp, %ebp
        jne       ..B1.38
..B1.33:
        testl     $8388607, 32(%esp)
        jne       ..B1.39
..B1.34:
        movzbl    39(%esp), %eax
        testl     $128, %eax
        je        ..B1.36
..B1.35:
        movss     32(%esp), %xmm0
        jmp       ..B1.37
..B1.36:
        pxor      %xmm0, %xmm0
        mulss     .L_2il0floatpacket.0@GOTOFF(%edi), %xmm0
..B1.37:
        movss     %xmm0, 4(%esp)
        flds      4(%esp)
        addl      $12, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        popl      %esi
        ret       
..B1.38:
        cmpl      $255, %ebp
        je        ..B1.44
..B1.39:
        movzbl    35(%esp), %edx
        andl      $128, %edx
        movzbl    39(%esp), %eax
        shrl      $7, %edx
        testl     $128, %eax
        je        ..B1.41
..B1.40:
        flds      _zeros@GOTOFF(%edi,%edx,4)
        addl      $12, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        popl      %esi
        ret       
..B1.41:
        flds      _infs@GOTOFF(%edi,%edx,4)
..B1.42:
        addl      $12, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        popl      %esi
        ret       
..B1.43:
        movss     32(%esp), %xmm0
        addss     36(%esp), %xmm0
        movss     %xmm0, 4(%esp)
        flds      4(%esp)
        addl      $12, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        popl      %esi
        ret       
..B1.44:
        movzbl    39(%esp), %eax
        testl     $128, %eax
        jne       ..B1.46
..B1.45:
        movss     32(%esp), %xmm0
        jmp       ..B1.47
..B1.46:
        pxor      %xmm0, %xmm0
        mulss     .L_2il0floatpacket.0@GOTOFF(%edi), %xmm0
..B1.47:
        movss     %xmm0, 4(%esp)
        flds      4(%esp)
        addl      $12, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        popl      %esi
        ret       
..B1.48:
        testl     $8388607, 32(%esp)
        jne       ..B1.43
..B1.49:
        cmpl      $255, %ebx
        je        ..B1.31
        jmp       ..B1.16
END(scalbf)
# -- End  scalbf
	.section .rodata, "a"
	.align 4
	.align 4
.L_2il0floatpacket.0:
	.long	0x7f800000
	.type	.L_2il0floatpacket.0,@object
	.size	.L_2il0floatpacket.0,4
	.align 4
.L_2il0floatpacket.1:
	.long	0x4c000000
	.type	.L_2il0floatpacket.1,@object
	.size	.L_2il0floatpacket.1,4
	.align 4
.L_2il0floatpacket.2:
	.long	0x71800000
	.type	.L_2il0floatpacket.2,@object
	.size	.L_2il0floatpacket.2,4
	.align 4
.L_2il0floatpacket.3:
	.long	0x0d800000
	.type	.L_2il0floatpacket.3,@object
	.size	.L_2il0floatpacket.3,4
	.align 4
.L_2il0floatpacket.4:
	.long	0x33000000
	.type	.L_2il0floatpacket.4,@object
	.size	.L_2il0floatpacket.4,4
	.align 4
_infs:
	.long	2139095040
	.long	4286578688
	.type	_infs,@object
	.size	_infs,8
	.align 4
_large_value_32:
	.long	1904214016
	.long	4051697664
	.type	_large_value_32,@object
	.size	_large_value_32,8
	.align 4
_small_value_32:
	.long	226492416
	.long	2373976064
	.type	_small_value_32,@object
	.size	_small_value_32,8
	.align 4
_zeros:
	.long	0
	.long	2147483648
	.type	_zeros,@object
	.size	_zeros,8
	.data
	.section .note.GNU-stack, ""
# End
