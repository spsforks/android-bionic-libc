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
# -- Begin  remquo
ENTRY(remquo)
# parameter 1: 8 + %ebp
# parameter 2: 16 + %ebp
# parameter 3: 24 + %ebp
..B1.1:
        pushl     %ebp
        movl      %esp, %ebp
        andl      $-16, %esp
        pushl     %esi
        pushl     %edi
        pushl     %ebx
        subl      $84, %esp
        movl      $1, %ebx
        movl      24(%ebp), %ecx
        movl      %ecx, 56(%esp)
        movl      12(%ebp), %ecx
        movl      %ecx, %edx
        movl      20(%ebp), %esi
        movl      %esi, %eax
        andl      $-2147483648, %edx
        andl      $-2147483648, %eax
        movl      %edx, 48(%esp)
        cmpl      %edx, %eax
        movl      16(%ebp), %edx
        movl      $-1, %eax
        movl      %edx, 68(%esp)
        cmove     %ebx, %eax
        andl      $2147483647, %ecx
        andl      $2147483647, %esi
        movl      %edx, 40(%esp)
        movl      %ecx, %edx
        movl      %esi, %edi
        andl      $2146435072, %edx
        andl      $2146435072, %edi
        movl      %eax, 52(%esp)
        shrl      $20, %edx
        movl      8(%ebp), %eax
        shrl      $20, %edi
        movl      %eax, 8(%esp)
        cmpl      $2047, %edx
        movl      %ecx, 12(%esp)
        movl      %esi, 44(%esp)
        movl      %edx, 64(%esp)
        movl      %edi, 60(%esp)
        je        ..B1.5
..B1.2:
        cmpl      $2047, 60(%esp)
        je        ..B1.5
..B1.3:
        movl      %eax, %edx
        orl       %ecx, %edx
        je        ..B1.5
..B1.4:
        movl      68(%esp), %edx
        orl       %esi, %edx
        jne       ..B1.6
..B1.5:
        movl      $1, %edx
        jmp       ..B1.7
..B1.6:
        xorl      %edx, %edx
..B1.7:
        movl      %ecx, %ebx
        movl      %esi, %edi
        andl      $1048575, %ebx
        andl      $1048575, %edi
        movsd     8(%esp), %xmm1
        testl     %edx, %edx
        movsd     40(%esp), %xmm0
        je        ..B1.16
..B1.8:
        movl      56(%esp), %edx
        movl      %ecx, %ebx
        movl      $0, (%edx)
        movl      $2146435072, %edx
        subl      %edx, %ebx
        jb        ..B1.9
..B1.76:
        orl       %eax, %ebx
        jne       ..B1.10
..B1.9:
        movl      %esi, %ebx
        subl      %edx, %ebx
        jb        ..B1.11
..B1.77:
        orl       68(%esp), %ebx
        je        ..B1.11
..B1.10:
        movsd     8(%ebp), %xmm0
        addsd     16(%ebp), %xmm0
        jmp       ..B1.15
..B1.11:
        movl      68(%esp), %ebx
        orl       %esi, %ebx
        je        ..B1.13
..B1.12:
        subl      %edx, %ecx
        orl       %ecx, %eax
        jne       ..B1.14
..B1.13:
        pxor      %xmm0, %xmm0
        divsd     %xmm0, %xmm0
        jmp       ..B1.15
..B1.14:
        movsd     8(%ebp), %xmm0
..B1.15:
        movsd     %xmm0, 8(%esp)
        fldl      8(%esp)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.16:
        movl      68(%esp), %edx
        movl      %edx, 16(%esp)
        movl      64(%esp), %edx
        subl      60(%esp), %edx
        movl      %edi, 20(%esp)
        cmpl      $31, %edx
        call      ..L2
..L2:
        popl      %edi
        lea       _GLOBAL_OFFSET_TABLE_+[. - ..L2](%edi), %edi
        movsd     %xmm1, 24(%esp)
        movl      %edi, 32(%esp)
        jle       ..B1.21
..B1.17:
        cmpl      $2016, 60(%esp)
        jge       ..B1.21
..B1.18:
        movl      %eax, 8(%esp)
        movl      %edi, %eax
        movl      %ebx, 12(%esp)
        movsd     .L_2il0floatpacket.0@GOTOFF(%eax), %xmm1
        mulsd     %xmm0, %xmm1
        movsd     %xmm1, (%esp)
..B1.19:
        fldl      (%esp)
        fldl      24(%esp)
.L_2TAG_PACKET_0.0.2:
        fprem     
        fstsw     %ax
        sahf      
        jp        .L_2TAG_PACKET_0.0.2
        fstp      %st(1)
        fstpl     24(%esp)
..B1.20:
        movl      28(%esp), %ecx
        movl      %ecx, %ebx
        andl      $2146435072, %ebx
        shrl      $20, %ebx
        movl      40(%esp), %edx
        movl      %ebx, 64(%esp)
        movl      %ecx, %ebx
        movl      24(%esp), %eax
        andl      $1048575, %ebx
        movl      44(%esp), %esi
        movl      %edx, 68(%esp)
..B1.21:
        movl      %eax, %edi
        movl      %ecx, %edx
        subl      68(%esp), %edi
        sbbl      %esi, %edx
        movl      %edx, 36(%esp)
        jb        ..B1.22
..B1.78:
        orl       36(%esp), %edi
        jne       ..B1.29
..B1.22:
        subl      68(%esp), %eax
        subl      %esi, %ecx
        orl       %ecx, %eax
        je        ..B1.28
..B1.23:
        movsd     24(%esp), %xmm2
        cmpl      $2046, 64(%esp)
        je        ..B1.26
..B1.24:
        movaps    %xmm2, %xmm1
        addsd     %xmm2, %xmm1
        comisd    %xmm1, %xmm0
        jb        ..B1.26
..B1.25:
        movl      32(%esp), %edx
        movaps    %xmm2, %xmm0
        cmpl      $0, 48(%esp)
        movl      56(%esp), %eax
        xorps     .L_2il0floatpacket.1@GOTOFF(%edx), %xmm0
        jne       ..L3
        movaps    %xmm2, %xmm0
..L3:
        movsd     %xmm0, 8(%esp)
        fldl      8(%esp)
        movl      $0, (%eax)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.26:
        movl      56(%esp), %edx
        movl      52(%esp), %eax
        movl      %eax, (%edx)
..B1.27:
        movl      32(%esp), %eax
        subsd     %xmm0, %xmm2
        movaps    %xmm2, %xmm0
        cmpl      $0, 48(%esp)
        xorps     .L_2il0floatpacket.1@GOTOFF(%eax), %xmm0
        jne       ..L4
        movaps    %xmm2, %xmm0
..L4:
        movsd     %xmm0, 8(%esp)
        fldl      8(%esp)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.28:
        movl      56(%esp), %edx
        pxor      %xmm0, %xmm0
        mulsd     8(%ebp), %xmm0
        movl      52(%esp), %eax
        movsd     %xmm0, 8(%esp)
        fldl      8(%esp)
        movl      %eax, (%edx)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.29:
        cmpl      $0, 64(%esp)
        jne       ..B1.35
..B1.30:
        movl      %eax, %edx
        orl       %ebx, %edx
        je        ..B1.35
..B1.31:
        movl      %eax, 8(%esp)
        testl     %ebx, %ebx
        movl      %ebx, 12(%esp)
        je        ..B1.33
..B1.32:
        bsr       %ebx, %edx
        negl      %edx
        addl      $20, %edx
        movl      %edx, 64(%esp)
        jmp       ..B1.34
..B1.33:
        bsr       8(%esp), %edx
        negl      %edx
        addl      $52, %edx
        movl      %edx, 64(%esp)
..B1.34:
        cmpl      $31, 64(%esp)
        ja        ..B1.64
..B1.66:
        movl      %eax, %edx
        movl      64(%esp), %ecx
        shldl     %cl, %eax, %ebx
        shll      %cl, %edx
        jmp       ..B1.65
..B1.64:
        movl      %eax, %ebx
        xorl      %edx, %edx
        movl      64(%esp), %ecx
        shll      %cl, %ebx
..B1.65:
        movl      %edx, %eax
        movl      %ecx, %edx
        negl      %edx
        incl      %edx
        movl      %edx, 64(%esp)
        jmp       ..B1.36
..B1.35:
        orl       $1048576, %ebx
..B1.36:
        movl      16(%esp), %edx
        movl      20(%esp), %ecx
        movl      %edx, 36(%esp)
        movl      %ecx, 40(%esp)
        cmpl      $0, 60(%esp)
        jne       ..B1.42
..B1.37:
        orl       40(%esp), %edx
        je        ..B1.42
..B1.38:
        cmpl      $0, 40(%esp)
        je        ..B1.40
..B1.39:
        bsr       40(%esp), %edx
        negl      %edx
        addl      $20, %edx
        movl      %edx, 60(%esp)
        jmp       ..B1.41
..B1.40:
        bsr       16(%esp), %edx
        negl      %edx
        addl      $52, %edx
        movl      %edx, 60(%esp)
..B1.41:
        cmpl      $31, 60(%esp)
        ja        ..B1.67
..B1.69:
        movl      %edx, %ecx
        movl      36(%esp), %esi
        movl      40(%esp), %edx
        shldl     %cl, %esi, %edx
        movl      %edx, 40(%esp)
        shll      %cl, %esi
        jmp       ..B1.68
..B1.67:
        movl      %edx, %ecx
        xorl      %esi, %esi
        movl      36(%esp), %edx
        shll      %cl, %edx
        movl      %edx, 40(%esp)
..B1.68:
        movl      %ecx, %edx
        negl      %edx
        incl      %edx
        movl      %esi, 36(%esp)
        movl      %edx, 60(%esp)
        jmp       ..B1.43
..B1.42:
        orl       $1048576, 40(%esp)
..B1.43:
        movl      64(%esp), %ecx
        xorl      %edx, %edx
        subl      60(%esp), %ecx
        xorl      %esi, %esi
        movl      %ecx, 64(%esp)
        cmpl      $-1, %ecx
        je        ..B1.49
..B1.45:
        movl      %edx, %edi
        addl      %esi, %esi
        shrl      $31, %edi
        addl      %edx, %edx
        orl       %edi, %esi
        movl      %eax, %edi
        subl      36(%esp), %edi
        movl      %ebx, %edi
        sbbl      40(%esp), %edi
        jb        ..B1.47
..B1.46:
        subl      36(%esp), %eax
        sbbl      40(%esp), %ebx
        addl      $1, %edx
        adcl      $0, %esi
..B1.47:
        movl      %eax, %edi
        addl      %ebx, %ebx
        shrl      $31, %edi
        decl      %ecx
        addl      %eax, %eax
        orl       %edi, %ebx
        cmpl      $-1, %ecx
        jne       ..B1.45
..B1.49:
        movl      %eax, %ecx
        orl       %ebx, %ecx
        je        ..B1.61
..B1.50:
        movl      %ebx, %esi
        shll      $31, %esi
        shrl      $1, %eax
        shrl      $1, %ebx
        orl       %eax, %esi
        movl      %esi, 8(%esp)
        testl     %ebx, %ebx
        movl      %ebx, 12(%esp)
        je        ..B1.52
..B1.51:
        bsr       %ebx, %ecx
        negl      %ecx
        addl      $20, %ecx
        jmp       ..B1.53
..B1.52:
        bsr       8(%esp), %ecx
        negl      %ecx
        addl      $52, %ecx
..B1.53:
        cmpl      $31, %ecx
        ja        ..B1.70
..B1.72:
        shldl     %cl, %esi, %ebx
        shll      %cl, %esi
        movl      %esi, 16(%esp)
        jmp       ..B1.71
..B1.70:
        movl      %esi, %ebx
        movl      $0, 16(%esp)
        shll      %cl, %ebx
..B1.71:
        movl      60(%esp), %edi
        subl      %ecx, %edi
        movl      %ebx, %ecx
        movl      %edi, 60(%esp)
        negl      %edi
        incl      %edi
        andl      $1048575, %ecx
        movl      %ecx, 20(%esp)
        cmpl      $31, %edi
        ja        ..B1.73
..B1.75:
        movl      %edi, %ecx
        movl      %ebx, %eax
        movl      16(%esp), %esi
        shrdl     %cl, %ebx, %esi
        shrl      %cl, %eax
        jmp       ..B1.74
..B1.73:
        movl      %edi, %ecx
        movl      %ebx, %esi
        shrl      %cl, %esi
        xorl      %eax, %eax
..B1.74:
        movl      60(%esp), %ebx
        xorl      %ecx, %ecx
        testl     %ebx, %ebx
        cmovle    %ecx, %ebx
        cmovg     20(%esp), %eax
        cmovg     16(%esp), %esi
        shll      $20, %ebx
        orl       %ebx, %eax
        movl      %esi, 8(%esp)
        movl      %eax, 12(%esp)
        movsd     8(%esp), %xmm2
        movaps    %xmm2, %xmm1
        addsd     %xmm2, %xmm1
        comisd    %xmm0, %xmm1
        jb        ..B1.56
..B1.54:
        ucomisd   %xmm1, %xmm0
        jp        ..B1.55
        je        ..B1.59
..B1.55:
        incl      %edx
        subsd     %xmm0, %xmm2
..B1.56:
        movl      32(%esp), %ecx
        andl      $2147483647, %edx
        movl      52(%esp), %eax
        movaps    %xmm2, %xmm0
        imull     %edx, %eax
        xorps     .L_2il0floatpacket.1@GOTOFF(%ecx), %xmm0
        cmpl      $0, 48(%esp)
        movl      56(%esp), %edx
        jne       ..L5
        movaps    %xmm2, %xmm0
..L5:
        movsd     %xmm0, 8(%esp)
        fldl      8(%esp)
        movl      %eax, (%edx)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.59:
        movl      %edx, %ecx
        xorl      %eax, %eax
        andl      $1, %ecx
        orl       %eax, %ecx
        je        ..B1.56
..B1.60:
        movl      32(%esp), %eax
        incl      %edx
        xorps     .L_2il0floatpacket.1@GOTOFF(%eax), %xmm2
        jmp       ..B1.56
..B1.61:
        andl      $2147483647, %edx
        pxor      %xmm0, %xmm0
        mulsd     8(%ebp), %xmm0
        movl      52(%esp), %eax
        imull     %edx, %eax
        movsd     %xmm0, 8(%esp)
        fldl      8(%esp)
        movl      56(%esp), %edx
        movl      %eax, (%edx)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
END(remquo)
# -- End  remquo
	.text
# Start file scope ASM
ALIAS_SYMBOL(remquol, remquo);
# End file scope ASM
	.section .rodata, "a"
	.align 16
	.align 16
.L_2il0floatpacket.1:
	.long	0x00000000,0x80000000,0x00000000,0x00000000
	.type	.L_2il0floatpacket.1,@object
	.size	.L_2il0floatpacket.1,16
	.align 8
.L_2il0floatpacket.0:
	.long	0x00000000,0xc1e00000
	.type	.L_2il0floatpacket.0,@object
	.size	.L_2il0floatpacket.0,8
	.data
	.section .note.GNU-stack, ""
# End
