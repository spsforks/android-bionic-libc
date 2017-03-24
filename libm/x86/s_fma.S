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
# -- Begin  fma
ENTRY(fma)
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
..B1.2:
        fnstcw    76(%esp)
..B1.3:
        movl      12(%ebp), %esi
        movl      16(%ebp), %edx
        movl      24(%ebp), %eax
        movzwl    76(%esp), %ecx
        movl      %esi, 60(%esp)
        andl      $2147483647, %esi
        andl      $3072, %ecx
        movl      %ecx, 56(%esp)
        movl      20(%ebp), %ecx
        movl      %ecx, %edi
        movl      %edx, 52(%esp)
        andl      $2147483647, %edi
        movl      28(%ebp), %edx
        movl      %eax, 72(%esp)
        movl      %esi, %eax
        movl      8(%ebp), %ebx
        movl      %edx, 68(%esp)
        andl      $2147483647, %edx
        orl       %ebx, %eax
        call      ..L2
..L2:
        popl      %eax
        lea       _GLOBAL_OFFSET_TABLE_+[. - ..L2](%eax), %eax
        movl      %eax, 64(%esp)
        jne       ..B1.4
..B1.130:
        cmpl      $2146435072, %esi
        ja        ..B1.8
        jmp       ..B1.6
..B1.4:
        cmpl      $2146435072, %esi
        jb        ..B1.9
..B1.5:
        ja        ..B1.8
..B1.6:
        jne       ..B1.134
..B1.7:
        testl     %ebx, %ebx
        jne       ..B1.8
..B1.133:
        cmpl      $2146435072, %edi
        ja        ..B1.15
        jmp       ..B1.13
..B1.8:
        movsd     8(%ebp), %xmm0
        addsd     16(%ebp), %xmm0
        mulsd     24(%ebp), %xmm0
        movsd     %xmm0, 8(%esp)
        fldl      8(%esp)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.9:
        lea       -1072693248(%esi), %eax
        orl       %ebx, %eax
        je        ..B1.133
..B1.10:
        movl      %edi, %eax
        orl       52(%esp), %eax
        je        ..B1.133
..B1.11:
        cmpl      $2146435072, %edi
        jae       ..B1.12
        jmp       ..B1.16
..B1.134:
        cmpl      $2146435072, %edi
..B1.12:
        ja        ..B1.15
..B1.13:
        jne       ..B1.138
..B1.14:
        cmpl      $0, 52(%esp)
        jne       ..B1.15
..B1.137:
        cmpl      $2146435072, %edx
        ja        ..B1.22
        jmp       ..B1.20
..B1.15:
        movsd     16(%ebp), %xmm0
        addsd     24(%ebp), %xmm0
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
        lea       -1072693248(%edi), %eax
        orl       52(%esp), %eax
        je        ..B1.137
..B1.17:
        movl      %edx, %eax
        orl       72(%esp), %eax
        je        ..B1.137
..B1.18:
        cmpl      $2146435072, %edx
        jae       ..B1.19
        jmp       ..B1.28
..B1.138:
        cmpl      $2146435072, %edx
..B1.19:
        ja        ..B1.22
..B1.20:
        jne       ..B1.23
..B1.21:
        cmpl      $0, 72(%esp)
        je        ..B1.23
..B1.22:
        movl      64(%esp), %eax
        movsd     24(%ebp), %xmm0
        mulsd     _ones@GOTOFF(%eax), %xmm0
        movsd     %xmm0, 8(%esp)
        fldl      8(%esp)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.23:
        cmpl      $2146435072, %esi
        jae       ..B1.27
..B1.24:
        cmpl      $2146435072, %edi
        jae       ..B1.27
..B1.25:
        cmpl      $2146435072, %edx
        jb        ..B1.27
..B1.26:
        fldl      24(%ebp)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.27:
        movsd     8(%ebp), %xmm0
        mulsd     16(%ebp), %xmm0
        addsd     24(%ebp), %xmm0
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
        cmpl      $1048576, %esi
        jae       ..B1.30
..B1.29:
        movl      60(%esp), %ebx
        movl      %ebx, %eax
        shrl      $31, %eax
        orl       $1072693248, %ebx
        movl      64(%esp), %esi
        movl      %ebx, 12(%ebp)
        movsd     8(%ebp), %xmm0
        subsd     _ones@GOTOFF(%esi,%eax,8), %xmm0
        movsd     %xmm0, 8(%ebp)
        movl      12(%ebp), %esi
        movl      %esi, 60(%esp)
        andl      $2147483647, %esi
        movl      %esi, %eax
        shrl      $20, %eax
        addl      $-1022, %eax
        movl      8(%ebp), %ebx
        movl      %eax, 4(%esp)
        jmp       ..B1.31
..B1.30:
        movl      %esi, %eax
        shrl      $20, %eax
        movl      %eax, 4(%esp)
..B1.31:
        cmpl      $1048576, %edi
        jae       ..B1.33
..B1.32:
        movl      %ecx, %edi
        orl       $1072693248, %ecx
        shrl      $31, %edi
        movl      %ecx, 20(%ebp)
        movl      64(%esp), %ecx
        movsd     16(%ebp), %xmm0
        subsd     _ones@GOTOFF(%ecx,%edi,8), %xmm0
        movsd     %xmm0, 16(%ebp)
        movl      20(%ebp), %ecx
        movl      %ecx, %edi
        movl      16(%ebp), %eax
        andl      $2147483647, %edi
        movl      %eax, 52(%esp)
        movl      %edi, %eax
        shrl      $20, %eax
        addl      $-1022, %eax
        movl      %eax, 8(%esp)
        jmp       ..B1.34
..B1.33:
        movl      %edi, %eax
        shrl      $20, %eax
        movl      %eax, 8(%esp)
..B1.34:
        cmpl      $1048576, %edx
        jae       ..B1.36
..B1.35:
        movl      68(%esp), %eax
        movl      %eax, %edx
        shrl      $31, %edx
        orl       $1072693248, %eax
        movl      %eax, 28(%ebp)
        movl      64(%esp), %eax
        movsd     24(%ebp), %xmm0
        subsd     _ones@GOTOFF(%eax,%edx,8), %xmm0
        movsd     %xmm0, 24(%ebp)
        movl      28(%ebp), %edx
        movl      24(%ebp), %eax
        movl      %edx, 68(%esp)
        andl      $2147483647, %edx
        movl      %eax, 72(%esp)
        movl      %edx, %eax
        shrl      $20, %eax
        addl      $-1022, %eax
        movl      %eax, (%esp)
        jmp       ..B1.37
..B1.36:
        movl      %edx, %eax
        shrl      $20, %eax
        movl      %eax, (%esp)
..B1.37:
        andl      $1048575, %edx
        movl      %ebx, %eax
        orl       $1048576, %edx
        andl      $1048575, %esi
        movl      %edx, 12(%esp)
        orl       $1048576, %esi
        movl      60(%esp), %edx
        andl      $1048575, %edi
        xorl      %ecx, %edx
        orl       $1048576, %edi
        andl      $-2147483648, %edx
        movl      %edx, 60(%esp)
        movl      4(%esp), %ecx
        movl      8(%esp), %edx
        lea       -1023(%ecx,%edx), %edx
        movl      52(%esp), %ecx
        movl      %edx, 28(%esp)
        mull      %ecx
        movl      %eax, 36(%esp)
        movl      %ecx, %eax
        movl      %edx, 24(%esp)
        mull      %esi
        movl      %eax, %ecx
        movl      %ebx, %eax
        movl      %edx, 44(%esp)
        mull      %edi
        movl      %eax, %ebx
        movl      %esi, %eax
        movl      %edx, 48(%esp)
        addl      %ebx, %ecx
        movl      $0, %ebx
        adcl      $0, %ebx
        mull      %edi
        addl      24(%esp), %ecx
        movl      44(%esp), %esi
        adcl      $0, %ebx
        xorl      %edi, %edi
        addl      %eax, %ebx
        movl      $0, %eax
        movl      %ecx, 40(%esp)
        adcl      $0, %eax
        addl      48(%esp), %esi
        adcl      $0, %edi
        addl      %esi, %ebx
        adcl      %edi, %eax
        addl      %eax, %edx
        movl      %edx, 32(%esp)
        testl     $512, %edx
        movl      12(%esp), %edx
        je        ..B1.39
..B1.38:
        movl      %ebx, %eax
        movl      32(%esp), %ecx
        shll      $11, %ecx
        shrl      $21, %eax
        orl       %eax, %ecx
        movl      %ecx, 32(%esp)
        movl      36(%esp), %ecx
        movl      %ecx, %edi
        movl      40(%esp), %eax
        movl      %eax, %esi
        shll      $11, %eax
        shrl      $21, %edi
        shll      $11, %ebx
        orl       %edi, %eax
        shrl      $21, %esi
        shll      $11, %ecx
        orl       %esi, %ebx
        incl      28(%esp)
        movl      %eax, 40(%esp)
        movl      %ecx, 36(%esp)
        jmp       ..B1.40
..B1.39:
        movl      %ebx, %eax
        movl      32(%esp), %ecx
        shll      $12, %ecx
        shrl      $20, %eax
        orl       %eax, %ecx
        movl      %ecx, 32(%esp)
        movl      36(%esp), %ecx
        movl      %ecx, %edi
        movl      40(%esp), %eax
        movl      %eax, %esi
        shll      $12, %eax
        shrl      $20, %edi
        shll      $12, %ebx
        orl       %edi, %eax
        shrl      $20, %esi
        shll      $12, %ecx
        orl       %esi, %ebx
        movl      %eax, 40(%esp)
        movl      %ecx, 36(%esp)
..B1.40:
        movl      28(%esp), %eax
        cmpl      (%esp), %eax
        jg        ..B1.45
..B1.41:
        cmpl      (%esp), %eax
        jne       ..B1.46
..B1.42:
        cmpl      32(%esp), %edx
        jb        ..B1.45
..B1.43:
        jne       ..B1.46
..B1.44:
        cmpl      72(%esp), %ebx
        jb        ..B1.46
..B1.45:
        subl      (%esp), %eax
        movl      %edx, 12(%esp)
        movl      %eax, 52(%esp)
        xorl      %eax, %eax
        movl      72(%esp), %edx
        xorl      %esi, %esi
        movl      68(%esp), %ecx
        andl      $-2147483648, %ecx
        movl      %edx, 24(%esp)
        movl      %ecx, 48(%esp)
        jmp       ..B1.47
..B1.46:
        movl      (%esp), %esi
        movl      %esi, %eax
        movl      32(%esp), %ecx
        subl      28(%esp), %eax
        movl      %edx, 32(%esp)
        movl      60(%esp), %edx
        movl      68(%esp), %edi
        movl      %eax, 52(%esp)
        andl      $-2147483648, %edi
        movl      %esi, 28(%esp)
        movl      %ebx, 24(%esp)
        movl      40(%esp), %esi
        movl      36(%esp), %eax
        movl      %edx, 48(%esp)
        xorl      %edx, %edx
        movl      %ecx, 12(%esp)
        movl      72(%esp), %ebx
        movl      %edi, 60(%esp)
        movl      %edx, 36(%esp)
        movl      %edx, 40(%esp)
..B1.47:
        cmpl      $0, 52(%esp)
        je        ..B1.57
..B1.48:
        cmpl      $32, 52(%esp)
        jge       ..B1.50
..B1.49:
        movl      %ebx, 4(%esp)
        movl      $-1, %edi
        movl      52(%esp), %ebx
        movl      %ebx, %ecx
        movl      %ebx, %edx
        shrl      %cl, %edi
        negl      %edx
        notl      %edi
        movl      %edx, %ecx
        movl      %edi, (%esp)
        movl      %eax, %edi
        shll      %cl, %edi
        andl      (%esp), %edi
        movl      %edi, 44(%esp)
        movl      %esi, %edi
        shll      %cl, %edi
        movl      %ebx, %ecx
        andl      (%esp), %edi
        shrl      %cl, %eax
        movl      %edx, %ecx
        orl       %edi, %eax
        movl      %eax, 8(%esp)
        movl      24(%esp), %eax
        movl      %eax, %edi
        shll      %cl, %edi
        movl      %ebx, %ecx
        andl      (%esp), %edi
        shrl      %cl, %esi
        movl      %edx, %ecx
        movl      12(%esp), %edx
        orl       %edi, %esi
        movl      %edx, %edi
        shll      %cl, %edi
        movl      %ebx, %ecx
        andl      (%esp), %edi
        shrl      %cl, %eax
        orl       %edi, %eax
        movl      %eax, 24(%esp)
        shrl      %cl, %edx
        movl      %edx, 12(%esp)
        movl      4(%esp), %ebx
        movl      8(%esp), %eax
        jmp       ..B1.58
..B1.50:
        cmpl      $64, 52(%esp)
        jge       ..B1.52
..B1.51:
        movl      52(%esp), %ecx
        movl      %ecx, %edx
        movl      %ebx, 4(%esp)
        negl      %edx
        movl      $-1, %ebx
        movl      %esi, %edi
        shrl      %cl, %ebx
        movl      %edx, %ecx
        shll      %cl, %edi
        movl      $1, %ecx
        testl     %eax, %eax
        notl      %ebx
        cmovne    %ecx, %eax
        andl      %ebx, %edi
        orl       %eax, %edi
        movl      %edx, %ecx
        movl      %edi, 44(%esp)
        movl      24(%esp), %edi
        movl      %edi, %eax
        shll      %cl, %eax
        movl      52(%esp), %ecx
        andl      %ebx, %eax
        shrl      %cl, %esi
        movl      %edx, %ecx
        movl      12(%esp), %edx
        orl       %esi, %eax
        movl      %edx, %esi
        shll      %cl, %esi
        movl      52(%esp), %ecx
        andl      %ebx, %esi
        shrl      %cl, %edi
        shrl      %cl, %edx
        orl       %edi, %esi
        movl      4(%esp), %ebx
        movl      %edx, 24(%esp)
        movl      $0, 12(%esp)
        jmp       ..B1.58
..B1.52:
        cmpl      $96, 52(%esp)
        jge       ..B1.54
..B1.53:
        movl      52(%esp), %edi
        movl      %edi, %ecx
        movl      %ebx, 4(%esp)
        movl      %edi, %ebx
        negl      %ebx
        orl       %esi, %eax
        movl      %ebx, (%esp)
        movl      24(%esp), %edx
        movl      $1, %eax
        movl      $0, %esi
        cmovne    %eax, %esi
        movl      $-1, %ebx
        shrl      %cl, %ebx
        movl      (%esp), %ecx
        notl      %ebx
        shll      %cl, %edx
        andl      %ebx, %edx
        orl       %esi, %edx
        movl      12(%esp), %esi
        movl      %esi, %eax
        shll      %cl, %eax
        movl      %edi, %ecx
        andl      %ebx, %eax
        movl      24(%esp), %ebx
        shrl      %cl, %ebx
        movl      %edx, 44(%esp)
        xorl      %edx, %edx
        shrl      %cl, %esi
        orl       %ebx, %eax
        movl      %edx, 24(%esp)
        movl      %edx, 12(%esp)
        movl      4(%esp), %ebx
        jmp       ..B1.58
..B1.54:
        cmpl      $128, 52(%esp)
        jge       ..B1.56
..B1.55:
        movl      %ebx, 4(%esp)
        movl      $-1, %edi
        movl      52(%esp), %ebx
        movl      %ebx, %edx
        negl      %edx
        movl      %ebx, %ecx
        shrl      %cl, %edi
        movl      %edx, %ecx
        movl      12(%esp), %edx
        notl      %edi
        shll      %cl, %edx
        movl      %ebx, %ecx
        andl      %edi, %edx
        movl      24(%esp), %edi
        orl       %esi, %edi
        xorl      %esi, %esi
        orl       %eax, %edi
        movl      $1, %eax
        movl      4(%esp), %ebx
        cmovne    %eax, %esi
        movl      12(%esp), %eax
        orl       %esi, %edx
        xorl      %esi, %esi
        movl      %edx, 44(%esp)
        shrl      %cl, %eax
        movl      %esi, 24(%esp)
        movl      %esi, 12(%esp)
        jmp       ..B1.58
..B1.56:
        movl      12(%esp), %edx
        xorl      %ecx, %ecx
        orl       24(%esp), %edx
        orl       %esi, %edx
        orl       %eax, %edx
        movl      $1, %eax
        cmovne    %eax, %ecx
        xorl      %eax, %eax
        xorl      %esi, %esi
        movl      %ecx, 44(%esp)
        movl      %esi, 24(%esp)
        movl      %esi, 12(%esp)
        jmp       ..B1.58
..B1.57:
        movl      $0, 44(%esp)
..B1.58:
        movl      60(%esp), %edx
        cmpl      48(%esp), %edx
        jne       ..B1.62
..B1.59:
        movl      36(%esp), %ecx
        addl      %ecx, %eax
        xorl      %edx, %edx
        cmpl      %ecx, %eax
        movl      40(%esp), %edi
        setb      %dl
        addl      %esi, %edi
        movl      $0, %esi
        adcl      $0, %esi
        addl      %edx, %edi
        adcl      $0, %esi
        xorl      %edx, %edx
        addl      24(%esp), %ebx
        movl      32(%esp), %ecx
        adcl      $0, %edx
        addl      %esi, %ebx
        movl      %edi, 40(%esp)
        adcl      $0, %edx
        addl      12(%esp), %ecx
        addl      %edx, %ecx
        movl      %ecx, 32(%esp)
        testl     $2097152, %ecx
        je        ..B1.61
..B1.60:
        orl       40(%esp), %eax
        movl      %ebx, %ecx
        xorl      %edx, %edx
        orl       44(%esp), %eax
        movl      32(%esp), %edi
        movl      $1, %eax
        movl      %edi, %esi
        cmovne    %eax, %edx
        shll      $31, %ecx
        shrl      $1, %edi
        orl       %edx, %ecx
        shll      $31, %esi
        andl      $1048575, %edi
        shrl      $1, %ebx
        incl      28(%esp)
        orl       %esi, %ebx
        movl      %ecx, 12(%esp)
        movl      %edi, 32(%esp)
        jmp       ..B1.84
..B1.61:
        xorl      %edx, %edx
        orl       44(%esp), %eax
        movl      $1, %eax
        cmovne    %eax, %edx
        orl       40(%esp), %edx
        andl      $1048575, 32(%esp)
        movl      %edx, 12(%esp)
        jmp       ..B1.84
..B1.62:
        xorl      %ecx, %ecx
        movl      $1, %edi
        negl      44(%esp)
        movl      36(%esp), %edx
        cmovne    %edi, %ecx
        subl      %eax, %edx
        movl      $0, %eax
        sbbl      $0, %eax
        xorl      %edi, %edi
        subl      %ecx, %edx
        movl      40(%esp), %ecx
        sbbl      $0, %eax
        subl      %esi, %ecx
        movl      $0, %esi
        movl      %edx, 36(%esp)
        sbbl      $0, %esi
        cltd      
        addl      %eax, %ecx
        movl      %ecx, 40(%esp)
        adcl      %edx, %esi
        subl      24(%esp), %ebx
        movl      %esi, %eax
        cltd      
        sbbl      $0, %edi
        addl      %ebx, %esi
        movl      32(%esp), %ebx
        adcl      %edx, %edi
        subl      12(%esp), %ebx
        addl      %edi, %ebx
        movl      %ebx, 32(%esp)
        je        ..B1.64
..B1.63:
        movl      %ebx, %edx
        shll      $11, %edx
        movl      $0, 8(%esp)
        jmp       ..B1.72
..B1.64:
        testl     %esi, %esi
        je        ..B1.66
..B1.65:
        movl      $21, 8(%esp)
        movl      %esi, %edx
        jmp       ..B1.72
..B1.66:
        cmpl      $0, 40(%esp)
        je        ..B1.68
..B1.67:
        movl      %ecx, %edx
        movl      $53, 8(%esp)
        jmp       ..B1.72
..B1.68:
        cmpl      $0, 36(%esp)
        je        ..B1.70
..B1.69:
        movl      36(%esp), %edx
        movl      $85, 8(%esp)
        jmp       ..B1.72
..B1.70:
        cmpl      $0, 44(%esp)
        je        ..B1.123
..B1.71:
        movl      44(%esp), %edx
        movl      $117, 8(%esp)
..B1.72:
        testl     $-2147483648, %edx
        jne       ..B1.76
..B1.73:
        movl      8(%esp), %ecx
..B1.74:
        addl      %edx, %edx
        incl      %ecx
        testl     $-2147483648, %edx
        je        ..B1.74
..B1.75:
        movl      %ecx, 8(%esp)
..B1.76:
        cmpl      $32, 8(%esp)
        jge       ..B1.78
..B1.77:
        movl      $1, %eax
        movl      8(%esp), %edi
        movl      %edi, %ecx
        movl      %eax, %ebx
        movl      %edi, %edx
        shll      %cl, %ebx
        negl      %edx
        decl      %ebx
        movl      %ebx, (%esp)
        movl      40(%esp), %ebx
        shll      %cl, %ebx
        movl      36(%esp), %ecx
        orl       44(%esp), %ecx
        movl      $0, %ecx
        cmovne    %eax, %ecx
        orl       %ecx, %ebx
        movl      %edi, %ecx
        movl      %ebx, 12(%esp)
        movl      %esi, %ebx
        shll      %cl, %ebx
        movl      %edx, %ecx
        movl      40(%esp), %eax
        shrl      %cl, %eax
        movl      %edi, %ecx
        movl      32(%esp), %edi
        shll      %cl, %edi
        movl      %edx, %ecx
        shrl      %cl, %esi
        andl      (%esp), %esi
        orl       %esi, %edi
        andl      (%esp), %eax
        andl      $1048575, %edi
        movl      %edi, 32(%esp)
        orl       %eax, %ebx
        jmp       ..B1.83
..B1.78:
        cmpl      $64, 8(%esp)
        jge       ..B1.80
..B1.79:
        movl      %esi, (%esp)
        movl      $1, %edi
        movl      8(%esp), %esi
        movl      %esi, %ebx
        negl      %ebx
        movl      %esi, %ecx
        movl      %ebx, 4(%esp)
        movl      %edi, %edx
        movl      44(%esp), %ebx
        testl     %ebx, %ebx
        movl      36(%esp), %eax
        cmovne    %edi, %ebx
        shll      %cl, %eax
        orl       %ebx, %eax
        movl      %eax, 12(%esp)
        movl      40(%esp), %eax
        movl      %eax, %ebx
        shll      %cl, %edx
        movl      36(%esp), %edi
        decl      %edx
        shll      %cl, %ebx
        movl      4(%esp), %ecx
        shrl      %cl, %edi
        movl      %esi, %ecx
        andl      %edx, %edi
        orl       %edi, %ebx
        movl      (%esp), %edi
        shll      %cl, %edi
        movl      4(%esp), %ecx
        shrl      %cl, %eax
        andl      %edx, %eax
        orl       %eax, %edi
        andl      $1048575, %edi
        movl      %edi, 32(%esp)
        jmp       ..B1.83
..B1.80:
        cmpl      $96, 8(%esp)
        jge       ..B1.82
..B1.81:
        movl      8(%esp), %esi
        movl      %esi, %ebx
        movl      %esi, %ecx
        movl      $1, %eax
        movl      36(%esp), %edx
        negl      %ebx
        movl      44(%esp), %edi
        movl      %ebx, (%esp)
        movl      %edx, %ebx
        shll      %cl, %eax
        shll      %cl, %edi
        decl      %eax
        shll      %cl, %ebx
        movl      (%esp), %ecx
        movl      %edi, 12(%esp)
        shrl      %cl, %edi
        movl      %esi, %ecx
        andl      %eax, %edi
        orl       %edi, %ebx
        movl      40(%esp), %edi
        shll      %cl, %edi
        movl      (%esp), %ecx
        shrl      %cl, %edx
        andl      %eax, %edx
        orl       %edx, %edi
        andl      $1048575, %edi
        movl      %edi, 32(%esp)
        jmp       ..B1.83
..B1.82:
        movl      8(%esp), %ecx
        xorl      %ebx, %ebx
        movl      36(%esp), %eax
        xorl      %edx, %edx
        shll      %cl, %eax
        andl      $1048575, %eax
        cmpl      $128, %ecx
        movl      %ebx, 12(%esp)
        cmovl     %eax, %edx
        movl      %edx, 32(%esp)
..B1.83:
        movl      28(%esp), %eax
        subl      8(%esp), %eax
        movl      %eax, 28(%esp)
..B1.84:
        movl      28(%esp), %eax
        lea       -1(%eax), %edx
        cmpl      $2046, %edx
        jb        ..B1.92
..B1.85:
        cmpl      $2047, 28(%esp)
        jge       ..B1.91
..B1.86:
        negl      %eax
        incl      %eax
        orl       $1048576, 32(%esp)
        movl      %eax, 28(%esp)
        cmpl      $32, %eax
        jge       ..B1.88
..B1.87:
        movl      %eax, %edi
        movl      %edi, %esi
        movl      %edi, %ecx
        movl      $-1, %edx
        negl      %esi
        movl      %ebx, %eax
        movl      %esi, (%esp)
        shrl      %cl, %edx
        movl      %esi, %ecx
        notl      %edx
        shll      %cl, %eax
        movl      %edi, %ecx
        movl      12(%esp), %edi
        movl      %edi, %esi
        shrl      %cl, %esi
        andl      %edx, %eax
        movl      (%esp), %ecx
        orl       %esi, %eax
        shll      %cl, %edi
        xorl      %esi, %esi
        movl      $1, %ecx
        testl     %edx, %edi
        cmovne    %ecx, %esi
        orl       %eax, %esi
        movl      32(%esp), %eax
        movl      %esi, 12(%esp)
        movl      %eax, %esi
        movl      (%esp), %ecx
        shll      %cl, %esi
        movl      28(%esp), %ecx
        andl      %edx, %esi
        shrl      %cl, %ebx
        shrl      %cl, %eax
        orl       %esi, %ebx
        jmp       ..B1.93
..B1.88:
        xorl      %eax, %eax
        cmpl      $53, 28(%esp)
        jg        ..B1.90
..B1.89:
        movl      28(%esp), %ecx
        movl      %ecx, %edx
        negl      %edx
        movl      $-1, %edi
        shrl      %cl, %edi
        movl      %edx, %ecx
        movl      %ebx, %esi
        notl      %edi
        movl      %eax, (%esp)
        movl      32(%esp), %eax
        shll      %cl, %eax
        movl      28(%esp), %ecx
        andl      %edi, %eax
        shrl      %cl, %esi
        movl      %edx, %ecx
        shll      %cl, %ebx
        xorl      %edx, %edx
        andl      %edi, %ebx
        orl       %esi, %eax
        orl       12(%esp), %ebx
        movl      $1, %ebx
        cmovne    %ebx, %edx
        movl      28(%esp), %ecx
        orl       %eax, %edx
        movl      32(%esp), %ebx
        movl      (%esp), %eax
        movl      %edx, 12(%esp)
        shrl      %cl, %ebx
        jmp       ..B1.93
..B1.90:
        xorl      %ebx, %ebx
        movl      $1, 12(%esp)
        xorl      %eax, %eax
        jmp       ..B1.94
..B1.91:
        movl      60(%esp), %eax
        shrl      $31, %eax
        movl      64(%esp), %edx
        movsd     _large_value_64@GOTOFF(%edx,%eax,8), %xmm0
        mulsd     _large_value_64@GOTOFF(%edx), %xmm0
        movsd     %xmm0, 8(%esp)
        fldl      8(%esp)
        movsd     %xmm0, 16(%esp)
        movsd     %xmm0, (%esp)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.92:
        movl      %eax, %edx
        shll      $20, %edx
        movl      32(%esp), %eax
        orl       %edx, %eax
..B1.93:
        cmpl      $0, 12(%esp)
        je        ..B1.122
..B1.94:
        movl      64(%esp), %edx
        cmpl      $0, 56(%esp)
        movsd     _small_value_64@GOTOFF(%edx), %xmm1
        movsd     _ones@GOTOFF(%edx), %xmm0
        addsd     %xmm1, %xmm0
        movsd     %xmm0, 16(%esp)
        je        ..B1.124
..B1.95:
        cmpl      $3072, 56(%esp)
        je        ..B1.118
..B1.96:
        cmpl      $2048, 56(%esp)
        jne       ..B1.107
..B1.97:
        cmpl      $0, 60(%esp)
        je        ..B1.99
..B1.98:
        cmpl      $1048576, %eax
        jb        ..B1.104
        jmp       ..B1.122
..B1.99:
        cmpl      $1048575, %eax
        jb        ..B1.104
..B1.100:
        jne       ..B1.105
..B1.101:
        cmpl      $-1, %ebx
        jb        ..B1.104
..B1.102:
        jne       ..B1.105
..B1.103:
        cmpl      $-2147483648, 12(%esp)
        jae       ..B1.105
..B1.104:
        mulsd     %xmm1, %xmm1
        movsd     %xmm1, 16(%esp)
        cmpl      $0, 60(%esp)
        jne       ..B1.122
..B1.105:
        xorl      %ecx, %ecx
        movl      $1, %edx
        incl      %ebx
        cmove     %edx, %ecx
        addl      %ecx, %eax
        cmpl      $2146435072, %eax
        jb        ..B1.122
..B1.106:
        movl      60(%esp), %eax
        shrl      $31, %eax
        movl      64(%esp), %edx
        movsd     _large_value_64@GOTOFF(%edx,%eax,8), %xmm0
        mulsd     _large_value_64@GOTOFF(%edx), %xmm0
        movsd     %xmm0, 8(%esp)
        fldl      8(%esp)
        movsd     %xmm0, 16(%esp)
        movsd     %xmm0, (%esp)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.107:
        cmpl      $1024, 56(%esp)
        jne       ..B1.122
..B1.108:
        cmpl      $0, 60(%esp)
        jne       ..B1.110
..B1.109:
        cmpl      $1048576, %eax
        jb        ..B1.115
        jmp       ..B1.122
..B1.110:
        cmpl      $1048575, %eax
        jb        ..B1.115
..B1.111:
        jne       ..B1.116
..B1.112:
        cmpl      $-1, %ebx
        jb        ..B1.115
..B1.113:
        jne       ..B1.116
..B1.114:
        cmpl      $-2147483648, 12(%esp)
        jae       ..B1.116
..B1.115:
        mulsd     %xmm1, %xmm1
        movsd     %xmm1, 16(%esp)
        cmpl      $0, 60(%esp)
        je        ..B1.122
..B1.116:
        xorl      %ecx, %ecx
        movl      $1, %edx
        incl      %ebx
        cmove     %edx, %ecx
        addl      %ecx, %eax
        cmpl      $2146435072, %eax
        jb        ..B1.122
..B1.117:
        movl      60(%esp), %eax
        shrl      $31, %eax
        movl      64(%esp), %edx
        movsd     _large_value_64@GOTOFF(%edx,%eax,8), %xmm0
        mulsd     _large_value_64@GOTOFF(%edx), %xmm0
        movsd     %xmm0, 8(%esp)
        fldl      8(%esp)
        movsd     %xmm0, 16(%esp)
        movsd     %xmm0, (%esp)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.118:
        cmpl      $1048576, %eax
        jb        ..B1.121
..B1.119:
        lea       -1048576(%eax), %edx
        orl       %ebx, %edx
        jne       ..B1.122
..B1.120:
        cmpl      $-2147483648, 12(%esp)
        jne       ..B1.122
..B1.121:
        mulsd     %xmm1, %xmm1
        movsd     %xmm1, 16(%esp)
..B1.122:
        movl      60(%esp), %edx
        orl       %eax, %edx
        movl      %edx, 4(%esp)
        movl      %ebx, (%esp)
        fldl      (%esp)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.123:
        movl      $1, %eax
        cmpl      $1024, 56(%esp)
        movl      64(%esp), %edx
        je        ..L3
        movl      $0, %eax
..L3:
        fldl      _zeros@GOTOFF(%edx,%eax,8)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
..B1.124:
        testl     $-2147483648, 12(%esp)
        je        ..B1.118
..B1.125:
        movl      %ebx, %ecx
        movl      12(%esp), %edx
        andl      $1, %ecx
        andl      $2147483647, %edx
        orl       %edx, %ecx
        je        ..B1.118
..B1.126:
        xorl      %ecx, %ecx
        movl      $1, %edx
        incl      %ebx
        cmove     %edx, %ecx
        addl      %ecx, %eax
        cmpl      $2146435072, %eax
        jb        ..B1.118
..B1.127:
        movl      60(%esp), %eax
        shrl      $31, %eax
        movl      64(%esp), %edx
        movsd     _large_value_64@GOTOFF(%edx,%eax,8), %xmm0
        mulsd     _large_value_64@GOTOFF(%edx), %xmm0
        movsd     %xmm0, 8(%esp)
        fldl      8(%esp)
        movsd     %xmm0, 16(%esp)
        movsd     %xmm0, (%esp)
        addl      $84, %esp
        popl      %ebx
        popl      %edi
        popl      %esi
        movl      %ebp, %esp
        popl      %ebp
        ret       
END(fma)
# -- End  fma
	.text
# Start file scope ASM
ALIAS_SYMBOL(fmal, fma);
# End file scope ASM
	.section .rodata, "a"
	.align 4
	.align 4
_ones:
	.long	0
	.long	1072693248
	.long	0
	.long	3220176896
	.type	_ones,@object
	.size	_ones,16
	.align 4
_large_value_64:
	.long	0
	.long	2121269248
	.long	0
	.long	4268752896
	.type	_large_value_64,@object
	.size	_large_value_64,16
	.align 4
_small_value_64:
	.long	0
	.long	24117248
	.long	0
	.long	2171600896
	.type	_small_value_64,@object
	.size	_small_value_64,16
	.align 4
_zeros:
	.long	0
	.long	0
	.long	0
	.long	2147483648
	.type	_zeros,@object
	.size	_zeros,16
	.data
	.section .note.GNU-stack, ""
# End
