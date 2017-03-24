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
# parameter 1: %xmm0
# parameter 2: %xmm1
# parameter 3: %xmm2
..B1.1:
..___tag_value_fma.1:
        pushq     %r12
..___tag_value_fma.3:
        pushq     %r13
..___tag_value_fma.5:
        pushq     %r14
..___tag_value_fma.7:
        pushq     %r15
..___tag_value_fma.9:
        pushq     %rbx
..___tag_value_fma.11:
        pushq     %rbp
..___tag_value_fma.13:
        subq      $88, %rsp
..___tag_value_fma.15:
        movq      $0x7fffffffffffffff, %rax
        movd      %xmm0, %r12
        movq      $0x8000000000000000, %rsi
        movd      %xmm1, %rdi
        movd      %xmm2, %r15
        movsd     %xmm0, 64(%rsp)
        movsd     %xmm1, 72(%rsp)
        movq      %r12, %r13
        movq      %rdi, %r9
        movq      %r12, %rbx
        movq      %rdi, %rbp
        movq      %r15, %r14
        movq      %rax, %rdx
        shrq      $32, %r13
        andq      %r15, %rsi
        shrq      $32, %r9
        andq      $1048575, %r13
        shrq      $52, %rbx
        andq      $1048575, %r9
        shrq      $52, %rbp
        andq      $2047, %rbx
        shrq      $52, %r14
        andq      $2047, %rbp
        andq      $2047, %r14
        andq      %rax, %r15
        orq       $1048576, %r13
        orq       $1048576, %r9
        andq      %rdi, %rdx
        movsd     %xmm2, 80(%rsp)
        andq      %r12, %rax
        movl      %r12d, %r8d
        movl      %edi, %ecx
        je        ..B1.15
..B1.2:
        testq     %rdx, %rdx
        je        ..B1.15
..B1.3:
        testq     %r15, %r15
        je        ..B1.15
..B1.4:
        cmpl      $2047, %ebx
        je        ..B1.8
..B1.5:
        movq      $0x3ff0000000000000, %r10
        cmpq      %r10, %rax
        je        ..B1.8
..B1.6:
        cmpl      $2047, %ebp
        je        ..B1.8
..B1.7:
        movq      $0x3ff0000000000000, %r10
        cmpq      %r10, %rdx
        jne       ..B1.16
..B1.8:
        movsd     64(%rsp), %xmm0
        movsd     72(%rsp), %xmm1
        movsd     80(%rsp), %xmm2
..___tag_value_fma.16:
        call      raise_denormal
..___tag_value_fma.17:
..B1.9:
        cmpl      $2047, %r14d
        jne       ..B1.15
..B1.10:
        movq      $0x7ff0000000000000, %rax
        cmpq      %rax, %r15
        ja        ..B1.14
..B1.11:
        cmpl      $2047, %ebx
        jge       ..B1.15
..B1.12:
        cmpl      $2047, %ebp
        jge       ..B1.15
..B1.13:
        movsd     80(%rsp), %xmm0
        addq      $88, %rsp
..___tag_value_fma.18:
        popq      %rbp
..___tag_value_fma.20:
        popq      %rbx
..___tag_value_fma.22:
        popq      %r15
..___tag_value_fma.24:
        popq      %r14
..___tag_value_fma.26:
        popq      %r13
..___tag_value_fma.28:
        popq      %r12
..___tag_value_fma.30:
        ret       
..___tag_value_fma.31:
..B1.14:
        lea       _one(%rip), %rax
        movsd     80(%rsp), %xmm0
        mulsd     (%rax), %xmm0
        addq      $88, %rsp
..___tag_value_fma.38:
        popq      %rbp
..___tag_value_fma.40:
        popq      %rbx
..___tag_value_fma.42:
        popq      %r15
..___tag_value_fma.44:
        popq      %r14
..___tag_value_fma.46:
        popq      %r13
..___tag_value_fma.48:
        popq      %r12
..___tag_value_fma.50:
        ret       
..___tag_value_fma.51:
..B1.15:
        movsd     64(%rsp), %xmm0
        mulsd     72(%rsp), %xmm0
        addsd     80(%rsp), %xmm0
        addq      $88, %rsp
..___tag_value_fma.58:
        popq      %rbp
..___tag_value_fma.60:
        popq      %rbx
..___tag_value_fma.62:
        popq      %r15
..___tag_value_fma.64:
        popq      %r14
..___tag_value_fma.66:
        popq      %r13
..___tag_value_fma.68:
        popq      %r12
..___tag_value_fma.70:
        ret       
..___tag_value_fma.71:
..B1.16:
        cmpl      $2047, %r14d
        je        ..B1.8
..B1.17:
        lea       (%rbx,%rbp), %r10d
        cmpl      $3071, %r10d
        jge       ..B1.109
..B1.18:
        stmxcsr   56(%rsp)
..B1.19:
        xorq      %rdi, %r12
        movq      $0x8000000000000000, %rdi
        andq      %rdi, %r12
        testl     %ebx, %ebx
        je        ..B1.22
..B1.20:
        testl     %ebp, %ebp
        je        ..B1.22
..B1.21:
        testl     %r14d, %r14d
        jne       ..B1.29
..B1.22:
        movsd     64(%rsp), %xmm0
        movsd     72(%rsp), %xmm1
        movsd     80(%rsp), %xmm2
        movq      %rax, 8(%rsp)
        movq      %rdx, 16(%rsp)
        movq      %r9, 24(%rsp)
        movq      %r8, 32(%rsp)
        movq      %rcx, 40(%rsp)
        movq      %rsi, 48(%rsp)
..___tag_value_fma.78:
        call      raise_denormal
..___tag_value_fma.79:
..B1.23:
        movq      48(%rsp), %rsi
        testl     %ebx, %ebx
        movq      40(%rsp), %rcx
        movq      32(%rsp), %r8
        movq      24(%rsp), %r9
        movq      16(%rsp), %rdx
        movq      8(%rsp), %rax
        jne       ..B1.25
..B1.24:
        lea       _one(%rip), %r13
        movq      $0x3ff0000000000000, %r8
        orq       %r8, %rax
        movq      %rax, 8(%rsp)
        movsd     8(%rsp), %xmm0
        subsd     (%r13), %xmm0
        movd      %xmm0, %rbx
        movsd     %xmm0, 8(%rsp)
        movq      %rbx, %r13
        shrq      $32, %r13
        movl      %ebx, %r8d
        andq      $1048575, %r13
        shrq      $52, %rbx
        orq       $1048576, %r13
        addq      $-1022, %rbx
..B1.25:
        testl     %ebp, %ebp
        jne       ..B1.27
..B1.26:
        lea       _one(%rip), %r9
        movq      $0x3ff0000000000000, %rcx
        orq       %rcx, %rdx
        movq      %rdx, 8(%rsp)
        movsd     8(%rsp), %xmm0
        subsd     (%r9), %xmm0
        movd      %xmm0, %rbp
        movsd     %xmm0, 8(%rsp)
        movq      %rbp, %r9
        shrq      $32, %r9
        movl      %ebp, %ecx
        andq      $1048575, %r9
        shrq      $52, %rbp
        orq       $1048576, %r9
        addq      $-1022, %rbp
..B1.27:
        testl     %r14d, %r14d
        jne       ..B1.29
..B1.28:
        movq      $0x7fffffffffffffff, %rax
        lea       _one(%rip), %rdx
        andq      %r15, %rax
        movq      $0x3ff0000000000000, %r15
        orq       %r15, %rax
        movq      %rax, 8(%rsp)
        movsd     8(%rsp), %xmm0
        subsd     (%rdx), %xmm0
        movd      %xmm0, %r15
        movsd     %xmm0, 8(%rsp)
        movq      %r15, %r14
        shrq      $52, %r14
        addq      $-1022, %r14
..B1.29:
        movq      $0xfffffffffffff, %rax
        movq      $0x3fffffffffffff, %rdi
        andq      %rax, %r15
        movq      %r13, %rax
        imulq     %r9, %rax
        imulq     %rcx, %r13
        imulq     %r8, %r9
        imulq     %r8, %rcx
        movslq    %ebx, %rbx
        addq      %r9, %r13
        movslq    %ebp, %rbp
        movq      %r13, %r8
        andq      $4194303, %r13
        movq      %rcx, %r9
        shlq      $10, %rax
        andq      %rdi, %rcx
        shrq      $22, %r8
        movq      $0x8000000000000000, %r10
        shlq      $32, %r13
        lea       -1023(%rbx,%rbp), %rdx
        movslq    %r14d, %rbx
        addq      %r8, %rax
        movq      %rdx, %r8
        addq      %r13, %rcx
        subq      %rbx, %r8
        andq      %rcx, %rdi
        shrq      $54, %r9
        btsq      $52, %r15
        shrq      $54, %rcx
        addq      %rcx, %r9
        movq      %r12, %rcx
        xorq      %rsi, %rcx
        addq      %r9, %rax
        testq     %r10, %rcx
        lea       2(%r8), %rbp
        jne       ..B1.36
..B1.30:
        testq     %rbp, %rbp
        jle       ..B1.94
..B1.31:
        cmpq      $54, %rbp
        ja        ..B1.33
..B1.32:
        movl      %ebp, %ecx
        movq      %r15, %r14
        negq      %rbp
        shrq      %cl, %r14
        movl      %ebp, %ecx
        shlq      %cl, %r15
        movq      $0x3fffffffffffff, %rbp
        shrq      $10, %r15
        addq      %r15, %rdi
        andq      %rdi, %rbp
        shrq      $54, %rdi
        addq      %rdi, %rax
        addq      %r14, %rax
        movq      %rax, %rcx
        movq      %rax, %rsi
        shrq      $52, %rcx
        shrq      $51, %rsi
        andq      $1, %rcx
        andq      $1, %rsi
        orq       %rcx, %rsi
        addq      %rsi, %rcx
        addq      %rcx, %rdx
        negq      %rcx
        addq      $2, %rcx
        movl      %edx, %r14d
        shlq      %cl, %rbp
        shlq      %cl, %rax
        movq      %rbp, %rsi
        shrq      $54, %rbp
        shlq      $10, %rsi
        addq      %rax, %rbp
        jmp       ..B1.58
..B1.33:
        cmpq      $107, %rbp
        ja        ..B1.35
..B1.34:
        addq      $12, %r8
        negq      %rbp
        movl      %r8d, %ecx
        movq      %r15, %r14
        addq      $54, %rbp
        shrq      %cl, %r14
        movl      %ebp, %ecx
        addq      %r14, %rdi
        movq      $0x3fffffffffffff, %rbp
        andq      %rdi, %rbp
        shrq      $54, %rdi
        addq      %rdi, %rax
        shlq      %cl, %r15
        movq      %rax, %rcx
        shrq      $51, %rcx
        addq      %rcx, %rdx
        negq      %rcx
        addq      $2, %rcx
        movl      %edx, %r14d
        shrq      $1, %r15
        shlq      %cl, %rbp
        movq      %rbp, %rsi
        shlq      $10, %rsi
        shrq      $54, %rbp
        orq       %r15, %rsi
        shlq      %cl, %rax
        addq      %rax, %rbp
        jmp       ..B1.58
..B1.35:
        movq      %rax, %rbx
        shrq      $51, %rbx
        addq      %rbx, %rdx
        negq      %rbx
        movl      %edx, %r14d
        lea       1(%rbx), %rcx
        addq      $2, %rbx
        shlq      %cl, %rdi
        movl      %ebx, %ecx
        movq      %rdi, %rsi
        shrq      $53, %rdi
        shlq      %cl, %rax
        shlq      $11, %rsi
        orq       %r15, %rsi
        lea       (%rdi,%rax), %rbp
        jmp       ..B1.58
..B1.36:
        testq     %rbp, %rbp
        jl        ..B1.105
..B1.37:
        cmpq      $3, %rbp
        ja        ..B1.53
..B1.38:
        movl      %ebp, %ecx
        movq      %r15, %rbx
        negq      %rbp
        addq      $63, %rbp
        shrq      %cl, %rbx
        movl      %ebp, %ecx
        shlq      %cl, %r15
        subq      %rbx, %rax
        addq      %r15, %r15
        movq      %rax, %rsi
        shrq      $10, %r15
        subq      %r15, %rdi
        orq       %rdi, %rsi
        je        ..B1.52
..B1.39:
        movq      %rdi, %rbx
        movq      $0x3fffffffffffff, %rsi
        sarq      $54, %rbx
        andq      %rdi, %rsi
        addq      %rax, %rbx
        jns       ..B1.41
..B1.40:
        negq      %rdi
        movq      $0x3fffffffffffff, %rsi
        negq      %rax
        andq      %rdi, %rsi
        sarq      $54, %rdi
        movq      $0x8000000000000000, %rcx
        xorq      %rcx, %r12
        lea       (%rax,%rdi), %rbx
..B1.41:
        testq     %rbx, %rbx
        jne       ..B1.49
..B1.42:
        movq      $0x20000000000000, %rax
        testq     %rsi, %rax
        je        ..B1.44
..B1.43:
        movq      %rsi, %rbp
        movl      %edx, %r14d
        shrq      $1, %rbp
        addl      $-51, %r14d
        shlq      $63, %rsi
        jmp       ..B1.58
..B1.44:
        xorl      %ecx, %ecx
        movq      %rsi, %rax
        testq     %rsi, %rsi
        je        ..B1.48
..B1.46:
        shrq      $1, %rax
        incq      %rcx
        testq     %rax, %rax
        jne       ..B1.46
..B1.48:
        negq      %rcx
        movq      %rsi, %rbp
        addq      $53, %rcx
        xorl      %esi, %esi
        subq      %rcx, %rdx
        addq      $-52, %rdx
        shlq      %cl, %rbp
        movl      %edx, %r14d
        jmp       ..B1.58
..B1.49:
        xorl      %edi, %edi
        movq      %rbx, %rax
..B1.50:
        shrq      $1, %rax
        incq      %rdi
        testq     %rax, %rax
        jne       ..B1.50
..B1.51:
        negq      %rdi
        movq      %rsi, %rbp
        lea       53(%rdi), %rax
        addq      $63, %rdi
        movq      %rax, %rcx
        subq      %rax, %rdx
        negq      %rcx
        addq      $2, %rdx
        addq      $54, %rcx
        movl      %edx, %r14d
        shrq      %cl, %rbp
        movl      %edi, %ecx
        shlq      %cl, %rsi
        movl      %eax, %ecx
        shlq      %cl, %rbx
        addq      %rbx, %rbp
        jmp       ..B1.58
..B1.52:
        lea       _zero(%rip), %rax
        movsd     (%rax), %xmm0
        addq      $88, %rsp
..___tag_value_fma.80:
        popq      %rbp
..___tag_value_fma.82:
        popq      %rbx
..___tag_value_fma.84:
        popq      %r15
..___tag_value_fma.86:
        popq      %r14
..___tag_value_fma.88:
        popq      %r13
..___tag_value_fma.90:
        popq      %r12
..___tag_value_fma.92:
        ret       
..___tag_value_fma.93:
..B1.53:
        cmpq      $54, %rbp
        ja        ..B1.55
..B1.54:
        movl      %ebp, %ecx
        movq      %r15, %r14
        negq      %rbp
        shrq      %cl, %r14
        movl      %ebp, %ecx
        shlq      %cl, %r15
        movq      $0x3fffffffffffff, %rbp
        shrq      $10, %r15
        subq      %r14, %rax
        subq      %r15, %rdi
        andq      %rdi, %rbp
        sarq      $54, %rdi
        addq      %rax, %rdi
        movq      %rdi, %rcx
        movq      %rdi, %rsi
        shrq      $51, %rcx
        shrq      $50, %rsi
        andq      $1, %rcx
        andq      $1, %rsi
        orq       %rcx, %rsi
        addq      %rsi, %rcx
        lea       -1(%rdx,%rcx), %r14
        negq      %rcx
        addq      $3, %rcx
        shlq      %cl, %rbp
        shlq      %cl, %rdi
        movq      %rbp, %rsi
        shrq      $54, %rbp
        shlq      $10, %rsi
        addq      %rdi, %rbp
        jmp       ..B1.58
..B1.55:
        cmpq      $108, %rbp
        ja        ..B1.57
..B1.56:
        addq      $12, %r8
        negq      %rbp
        addq      $54, %rbp
        movl      %r8d, %ecx
        movq      %r15, %r14
        shrq      %cl, %r14
        movl      %ebp, %ecx
        shlq      %cl, %r15
        movq      $0x3fffffffffffff, %rbp
        shrq      $10, %r15
        movq      %rbp, %rbx
        negq      %r15
        andq      %r15, %rbx
        sarq      $54, %r15
        addq      %r15, %rdi
        subq      %r14, %rdi
        andq      %rdi, %rbp
        sarq      $54, %rdi
        addq      %rdi, %rax
        movq      %rax, %rcx
        movq      %rax, %rsi
        shrq      $51, %rcx
        shrq      $50, %rsi
        andq      $1, %rcx
        andq      $1, %rsi
        orq       %rcx, %rsi
        addq      %rsi, %rcx
        lea       -1(%rdx,%rcx), %r14
        negq      %rcx
        addq      $3, %rcx
        shlq      %cl, %rbp
        movq      %rbp, %rsi
        shlq      $10, %rsi
        shrq      $54, %rbp
        orq       %rbx, %rsi
        shlq      %cl, %rax
        addq      %rax, %rbp
        jmp       ..B1.58
..B1.57:
        negq      %r15
        movq      $0x3fffffffffffff, %rbp
        decq      %rdi
        andq      %rbp, %r15
        andq      %rdi, %rbp
        sarq      $54, %rdi
        addq      %rdi, %rax
        movq      %rax, %rcx
        movq      %rax, %r14
        shrq      $51, %rcx
        shrq      $50, %r14
        andq      $1, %rcx
        andq      $1, %r14
        orq       %rcx, %r14
        addq      %r14, %rcx
        lea       -1(%rdx,%rcx), %r14
        negq      %rcx
        addq      $3, %rcx
        shlq      %cl, %rbp
        movq      %rbp, %rsi
        shlq      $10, %rsi
        shrq      $54, %rbp
        orq       %r15, %rsi
        shlq      %cl, %rax
        addq      %rax, %rbp
..B1.58:
        cmpl      $2047, %r14d
        jge       ..B1.90
..B1.59:
        testl     %r14d, %r14d
        jg        ..B1.63
..B1.60:
        cmpl      $-52, %r14d
        jge       ..B1.62
..B1.61:
        movq      %rsi, %rdx
        movq      %rsi, %rax
        shrq      $1, %rdx
        andq      $1, %rax
        orq       %rax, %rdx
        xorl      %ebx, %ebx
        orq       %rbp, %rdx
        jmp       ..B1.64
..B1.62:
        movl      %r14d, %ecx
        movq      %rbp, %rbx
        negl      %ecx
        movq      %rsi, %rdx
        incl      %ecx
        movq      %rsi, %rax
        shrq      %cl, %rbx
        andq      $1, %rax
        negl      %ecx
        movq      %rbp, %rdi
        shrq      $1, %rdx
        shlq      %cl, %rdi
        orq       %rax, %rdx
        orq       %rdi, %rdx
        jmp       ..B1.64
..B1.63:
        movslq    %r14d, %rbx
        movq      $0xfffffffffffff, %rax
        shlq      $52, %rbx
        andq      %rbp, %rax
        orq       %rax, %rbx
        movq      %rsi, %rdx
..B1.64:
        movl      56(%rsp), %r9d
        andl      $24576, %r9d
        testq     %rdx, %rdx
        je        ..B1.91
..B1.65:
        lea       _one(%rip), %rax
        testl     %r14d, %r14d
        movq      (%rax), %rcx
        lea       _small_value_64(%rip), %rax
        movq      %rcx, (%rsp)
        movsd     (%rsp), %xmm0
        addsd     (%rax), %xmm0
        movsd     %xmm0, (%rsp)
        jge       ..B1.67
..B1.66:
        movq      %r12, %rdi
        lea       _small_values_64(%rip), %rcx
        shrq      $63, %rdi
        movq      (%rcx,%rdi,8), %r8
        movq      %r8, (%rsp)
        movsd     (%rsp), %xmm0
        mulsd     (%rax), %xmm0
        movsd     %xmm0, (%rsp)
..B1.67:
        testl     %r9d, %r9d
        je        ..B1.97
..B1.68:
        cmpl      $24576, %r9d
        je        ..B1.87
..B1.69:
        cmpl      $16384, %r9d
        jne       ..B1.78
..B1.70:
        testl     %r14d, %r14d
        jne       ..B1.76
..B1.71:
        testq     %r12, %r12
        jne       ..B1.75
..B1.72:
        movq      $0x1fffffffffffff, %rdx
        cmpq      %rdx, %rbp
        jb        ..B1.75
..B1.73:
        jne       ..B1.77
..B1.74:
        movq      $0x8000000000000000, %rdx
        cmpq      %rdx, %rsi
        jae       ..B1.77
..B1.75:
        movq      %r12, %rcx
        lea       _small_values_64(%rip), %rdx
        shrq      $63, %rcx
        movq      (%rdx,%rcx,8), %rbp
        movq      %rbp, (%rsp)
        movsd     (%rsp), %xmm0
        mulsd     (%rax), %xmm0
        movsd     %xmm0, (%rsp)
..B1.76:
        testq     %r12, %r12
        jne       ..B1.89
..B1.77:
        incq      %rbx
        jmp       ..B1.89
..B1.78:
        cmpl      $8192, %r9d
        jne       ..B1.89
..B1.79:
        testl     %r14d, %r14d
        jne       ..B1.85
..B1.80:
        testq     %r12, %r12
        je        ..B1.84
..B1.81:
        movq      $0x1fffffffffffff, %rdx
        cmpq      %rdx, %rbp
        jb        ..B1.84
..B1.82:
        jne       ..B1.86
..B1.83:
        movq      $0x8000000000000000, %rdx
        cmpq      %rdx, %rsi
        jae       ..B1.86
..B1.84:
        movq      %r12, %rcx
        lea       _small_values_64(%rip), %rdx
        shrq      $63, %rcx
        movq      (%rdx,%rcx,8), %rbp
        movq      %rbp, (%rsp)
        movsd     (%rsp), %xmm0
        mulsd     (%rax), %xmm0
        movsd     %xmm0, (%rsp)
..B1.85:
        testq     %r12, %r12
        je        ..B1.89
..B1.86:
        incq      %rbx
        jmp       ..B1.89
..B1.87:
        testl     %r14d, %r14d
        jg        ..B1.89
..B1.88:
        movq      %r12, %rcx
        lea       _small_values_64(%rip), %rdx
        shrq      $63, %rcx
        movq      (%rdx,%rcx,8), %rbp
        movq      %rbp, (%rsp)
        movsd     (%rsp), %xmm0
        mulsd     (%rax), %xmm0
        movsd     %xmm0, (%rsp)
..B1.89:
        movq      $0x7ff0000000000000, %rax
        cmpq      %rax, %rbx
        jb        ..B1.91
..B1.90:
        shrq      $63, %r12
        lea       _large_values_64(%rip), %rax
        lea       _large_value_64(%rip), %rcx
        movq      (%rax,%r12,8), %rdx
        movq      %rdx, (%rsp)
        movsd     (%rsp), %xmm0
        mulsd     (%rcx), %xmm0
        movsd     %xmm0, (%rsp)
        movsd     (%rsp), %xmm0
        addq      $88, %rsp
..___tag_value_fma.100:
        popq      %rbp
..___tag_value_fma.102:
        popq      %rbx
..___tag_value_fma.104:
        popq      %r15
..___tag_value_fma.106:
        popq      %r14
..___tag_value_fma.108:
        popq      %r13
..___tag_value_fma.110:
        popq      %r12
..___tag_value_fma.112:
        ret       
..___tag_value_fma.113:
..B1.91:
        orq       %r12, %rbx
        movq      %rbx, 8(%rsp)
        movsd     8(%rsp), %xmm0
        addq      $88, %rsp
..___tag_value_fma.120:
        popq      %rbp
..___tag_value_fma.122:
        popq      %rbx
..___tag_value_fma.124:
        popq      %r15
..___tag_value_fma.126:
        popq      %r14
..___tag_value_fma.128:
        popq      %r13
..___tag_value_fma.130:
        popq      %r12
..___tag_value_fma.132:
        ret       
..___tag_value_fma.133:
..B1.94:
        negq      %rbp
        cmpq      $53, %rbp
        ja        ..B1.96
..B1.95:
        movl      %ebp, %esi
        negq      %rbp
        addq      $63, %rbp
        movl      %esi, %ecx
        movq      %rax, %r14
        addq      %rax, %rax
        shrq      %cl, %r14
        movl      %ebp, %ecx
        shlq      %cl, %rax
        movl      %esi, %ecx
        shlq      $10, %rdi
        addq      %r14, %r15
        shrq      %cl, %rdi
        movq      %r15, %rcx
        shrq      $53, %rcx
        movq      %r15, %rsi
        orq       %rdi, %rax
        andq      %rcx, %rsi
        addq      %rcx, %rbx
        movq      %r15, %rbp
        shlq      $63, %rsi
        movl      %ebx, %r14d
        shrq      %cl, %rax
        shrq      %cl, %rbp
        orq       %rax, %rsi
        jmp       ..B1.58
..B1.96:
        movq      %rax, %rsi
        movq      %r15, %rbp
        orq       %rdi, %rsi
        jmp       ..B1.58
..B1.97:
        testl     %r14d, %r14d
        jne       ..B1.101
..B1.98:
        movq      $0x1fffffffffffff, %rcx
        cmpq      %rcx, %rbp
        jb        ..B1.100
..B1.99:
        movq      $0x8000000000000000, %rcx
        cmpq      %rcx, %rsi
        jae       ..B1.101
..B1.100:
        movq      %r12, %rbp
        lea       _small_values_64(%rip), %rcx
        shrq      $63, %rbp
        movq      (%rcx,%rbp,8), %rsi
        movq      %rsi, (%rsp)
        movsd     (%rsp), %xmm0
        mulsd     (%rax), %xmm0
        movsd     %xmm0, (%rsp)
..B1.101:
        movq      $0x8000000000000000, %rax
        cmpq      %rax, %rdx
        ja        ..B1.104
..B1.102:
        movq      $0x8000000000000000, %rax
        cmpq      %rax, %rdx
        jne       ..B1.89
..B1.103:
        testq     $1, %rbx
        je        ..B1.89
..B1.104:
        incq      %rbx
        jmp       ..B1.89
..B1.105:
        negq      %rbp
        movq      %rsi, %r12
        cmpq      $54, %rbp
        ja        ..B1.107
..B1.106:
        movl      %ebp, %r14d
        negq      %rbp
        movl      %r14d, %ecx
        movq      %rax, %r8
        shrq      %cl, %r8
        movl      %ebp, %ecx
        shlq      %cl, %rax
        movl      %r14d, %ecx
        movq      %rdi, %rsi
        shrq      %cl, %rsi
        movl      %ebp, %ecx
        shlq      %cl, %rdi
        movq      $0x3fffffffffffff, %rbp
        shrq      $10, %rdi
        movq      %rdi, %rdx
        shrq      $10, %rax
        negq      %rdx
        sarq      $54, %rdx
        orq       %rsi, %rax
        subq      %rax, %rdx
        andq      %rdx, %rbp
        sarq      $54, %rdx
        addq      %rdx, %r15
        subq      %r8, %r15
        movq      %r15, %rcx
        shrq      $52, %rcx
        lea       -1(%rbx,%rcx), %r14
        negq      %rcx
        incq      %rcx
        shlq      %cl, %rbp
        movq      %rbp, %rsi
        shlq      $10, %rsi
        shrq      $54, %rbp
        orq       %rdi, %rsi
        shlq      %cl, %r15
        addq      %r15, %rbp
        jmp       ..B1.58
..B1.107:
        movq      $0x10000000000000, %rax
        lea       -1(%r15), %rbp
        movq      $-2, %rsi
        cmpq      %rax, %rbp
        jae       ..B1.58
..B1.108:
        decl      %r14d
        lea       -1(,%r15,2), %rbp
        jmp       ..B1.58
..B1.109:
        movsd     64(%rsp), %xmm0
        movsd     72(%rsp), %xmm1
        movsd     80(%rsp), %xmm2
..___tag_value_fma.140:
        call      raise_denormal
..___tag_value_fma.141:
..B1.110:
        movsd     64(%rsp), %xmm0
        mulsd     72(%rsp), %xmm0
        addq      $88, %rsp
..___tag_value_fma.142:
        popq      %rbp
..___tag_value_fma.144:
        popq      %rbx
..___tag_value_fma.146:
        popq      %r15
..___tag_value_fma.148:
        popq      %r14
..___tag_value_fma.150:
        popq      %r13
..___tag_value_fma.152:
        popq      %r12
..___tag_value_fma.154:
        ret       
..___tag_value_fma.155:
END(fma)
# -- End  fma
	.text
# -- Begin  raise_denormal
	.text
       .align    16,0x90
raise_denormal:
# parameter 1: %xmm0
# parameter 2: %xmm1
# parameter 3: %xmm2
..B2.1:
..___tag_value_raise_denormal.156:
..___tag_value_raise_denormal.158:
..B2.2:
        ucomisd   %xmm1, %xmm0
        ucomisd   %xmm2, %xmm2
        ret       
..B2.3:
        .align    16,0x90
..___tag_value_raise_denormal.159:
	.type	raise_denormal,@function
	.size	raise_denormal,.-raise_denormal
	.data
# -- End  raise_denormal
	.section .rodata, "a"
	.align 16
	.align 16
_one:
	.long	0
	.long	1072693248
	.type	_one,@object
	.size	_one,8
	.space 8, 0x00 	# pad
	.align 16
_zero:
	.long	0
	.long	0
	.type	_zero,@object
	.size	_zero,8
	.space 8, 0x00 	# pad
	.align 16
_small_value_64:
	.long	0
	.long	24117248
	.type	_small_value_64,@object
	.size	_small_value_64,8
	.space 8, 0x00 	# pad
	.align 16
_small_values_64:
	.long	0
	.long	24117248
	.long	0
	.long	2171600896
	.type	_small_values_64,@object
	.size	_small_values_64,16
	.align 16
_large_values_64:
	.long	0
	.long	2121269248
	.long	0
	.long	4268752896
	.type	_large_values_64,@object
	.size	_large_values_64,16
	.align 16
_large_value_64:
	.long	0
	.long	2121269248
	.type	_large_value_64,@object
	.size	_large_value_64,8
	.data
	.section .note.GNU-stack, ""
// -- Begin DWARF2 SEGMENT .eh_frame
	.section .eh_frame,"a",@progbits
.eh_frame_seg:
	.align 1
	.4byte 0x00000014
	.8byte 0x00527a0100000000
	.8byte 0x08070c1b01107801
	.4byte 0x00000190
	.4byte 0x00000244
	.4byte 0x0000001c
	.4byte ..___tag_value_fma.1-.
	.4byte ..___tag_value_fma.155-..___tag_value_fma.1
	.2byte 0x0400
	.4byte ..___tag_value_fma.3-..___tag_value_fma.1
	.4byte 0x028c100e
	.byte 0x04
	.4byte ..___tag_value_fma.5-..___tag_value_fma.3
	.4byte 0x038d180e
	.byte 0x04
	.4byte ..___tag_value_fma.7-..___tag_value_fma.5
	.4byte 0x048e200e
	.byte 0x04
	.4byte ..___tag_value_fma.9-..___tag_value_fma.7
	.4byte 0x058f280e
	.byte 0x04
	.4byte ..___tag_value_fma.11-..___tag_value_fma.9
	.4byte 0x0683300e
	.byte 0x04
	.4byte ..___tag_value_fma.13-..___tag_value_fma.11
	.4byte 0x0786380e
	.byte 0x04
	.4byte ..___tag_value_fma.15-..___tag_value_fma.13
	.4byte 0x0401900e
	.4byte ..___tag_value_fma.18-..___tag_value_fma.15
	.4byte 0x04c6380e
	.4byte ..___tag_value_fma.20-..___tag_value_fma.18
	.4byte 0x04c3300e
	.4byte ..___tag_value_fma.22-..___tag_value_fma.20
	.4byte 0x04cf280e
	.4byte ..___tag_value_fma.24-..___tag_value_fma.22
	.4byte 0x04ce200e
	.4byte ..___tag_value_fma.26-..___tag_value_fma.24
	.4byte 0x04cd180e
	.4byte ..___tag_value_fma.28-..___tag_value_fma.26
	.4byte 0x04cc100e
	.4byte ..___tag_value_fma.30-..___tag_value_fma.28
	.2byte 0x080e
	.byte 0x04
	.4byte ..___tag_value_fma.31-..___tag_value_fma.30
	.8byte 0x8c0786068301900e
	.8byte 0x04058f048e038d02
	.4byte ..___tag_value_fma.38-..___tag_value_fma.31
	.4byte 0x04c6380e
	.4byte ..___tag_value_fma.40-..___tag_value_fma.38
	.4byte 0x04c3300e
	.4byte ..___tag_value_fma.42-..___tag_value_fma.40
	.4byte 0x04cf280e
	.4byte ..___tag_value_fma.44-..___tag_value_fma.42
	.4byte 0x04ce200e
	.4byte ..___tag_value_fma.46-..___tag_value_fma.44
	.4byte 0x04cd180e
	.4byte ..___tag_value_fma.48-..___tag_value_fma.46
	.4byte 0x04cc100e
	.4byte ..___tag_value_fma.50-..___tag_value_fma.48
	.2byte 0x080e
	.byte 0x04
	.4byte ..___tag_value_fma.51-..___tag_value_fma.50
	.8byte 0x8c0786068301900e
	.8byte 0x04058f048e038d02
	.4byte ..___tag_value_fma.58-..___tag_value_fma.51
	.4byte 0x04c6380e
	.4byte ..___tag_value_fma.60-..___tag_value_fma.58
	.4byte 0x04c3300e
	.4byte ..___tag_value_fma.62-..___tag_value_fma.60
	.4byte 0x04cf280e
	.4byte ..___tag_value_fma.64-..___tag_value_fma.62
	.4byte 0x04ce200e
	.4byte ..___tag_value_fma.66-..___tag_value_fma.64
	.4byte 0x04cd180e
	.4byte ..___tag_value_fma.68-..___tag_value_fma.66
	.4byte 0x04cc100e
	.4byte ..___tag_value_fma.70-..___tag_value_fma.68
	.2byte 0x080e
	.byte 0x04
	.4byte ..___tag_value_fma.71-..___tag_value_fma.70
	.8byte 0x8c0786068301900e
	.8byte 0x04058f048e038d02
	.4byte ..___tag_value_fma.80-..___tag_value_fma.71
	.4byte 0x04c6380e
	.4byte ..___tag_value_fma.82-..___tag_value_fma.80
	.4byte 0x04c3300e
	.4byte ..___tag_value_fma.84-..___tag_value_fma.82
	.4byte 0x04cf280e
	.4byte ..___tag_value_fma.86-..___tag_value_fma.84
	.4byte 0x04ce200e
	.4byte ..___tag_value_fma.88-..___tag_value_fma.86
	.4byte 0x04cd180e
	.4byte ..___tag_value_fma.90-..___tag_value_fma.88
	.4byte 0x04cc100e
	.4byte ..___tag_value_fma.92-..___tag_value_fma.90
	.2byte 0x080e
	.byte 0x04
	.4byte ..___tag_value_fma.93-..___tag_value_fma.92
	.8byte 0x8c0786068301900e
	.8byte 0x04058f048e038d02
	.4byte ..___tag_value_fma.100-..___tag_value_fma.93
	.4byte 0x04c6380e
	.4byte ..___tag_value_fma.102-..___tag_value_fma.100
	.4byte 0x04c3300e
	.4byte ..___tag_value_fma.104-..___tag_value_fma.102
	.4byte 0x04cf280e
	.4byte ..___tag_value_fma.106-..___tag_value_fma.104
	.4byte 0x04ce200e
	.4byte ..___tag_value_fma.108-..___tag_value_fma.106
	.4byte 0x04cd180e
	.4byte ..___tag_value_fma.110-..___tag_value_fma.108
	.4byte 0x04cc100e
	.4byte ..___tag_value_fma.112-..___tag_value_fma.110
	.2byte 0x080e
	.byte 0x04
	.4byte ..___tag_value_fma.113-..___tag_value_fma.112
	.8byte 0x8c0786068301900e
	.8byte 0x04058f048e038d02
	.4byte ..___tag_value_fma.120-..___tag_value_fma.113
	.4byte 0x04c6380e
	.4byte ..___tag_value_fma.122-..___tag_value_fma.120
	.4byte 0x04c3300e
	.4byte ..___tag_value_fma.124-..___tag_value_fma.122
	.4byte 0x04cf280e
	.4byte ..___tag_value_fma.126-..___tag_value_fma.124
	.4byte 0x04ce200e
	.4byte ..___tag_value_fma.128-..___tag_value_fma.126
	.4byte 0x04cd180e
	.4byte ..___tag_value_fma.130-..___tag_value_fma.128
	.4byte 0x04cc100e
	.4byte ..___tag_value_fma.132-..___tag_value_fma.130
	.2byte 0x080e
	.byte 0x04
	.4byte ..___tag_value_fma.133-..___tag_value_fma.132
	.8byte 0x8c0786068301900e
	.8byte 0x04058f048e038d02
	.4byte ..___tag_value_fma.142-..___tag_value_fma.133
	.4byte 0x04c6380e
	.4byte ..___tag_value_fma.144-..___tag_value_fma.142
	.4byte 0x04c3300e
	.4byte ..___tag_value_fma.146-..___tag_value_fma.144
	.4byte 0x04cf280e
	.4byte ..___tag_value_fma.148-..___tag_value_fma.146
	.4byte 0x04ce200e
	.4byte ..___tag_value_fma.150-..___tag_value_fma.148
	.4byte 0x04cd180e
	.4byte ..___tag_value_fma.152-..___tag_value_fma.150
	.4byte 0x04cc100e
	.4byte ..___tag_value_fma.154-..___tag_value_fma.152
	.2byte 0x080e
	.4byte 0x00000014
	.4byte 0x00000264
	.4byte ..___tag_value_raise_denormal.156-.
	.4byte ..___tag_value_raise_denormal.159-..___tag_value_raise_denormal.156
	.2byte 0x0400
	.4byte ..___tag_value_raise_denormal.158-..___tag_value_raise_denormal.156
	.2byte 0x100e
# End
