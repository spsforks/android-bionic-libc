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
# -- Begin  roundf
ENTRY(roundf)
# parameter 1: 32 + %esp
..B1.1:
        pushl     %edi
        pushl     %ebx
        pushl     %ebp
        subl      $16, %esp
        movl      32(%esp), %eax
        movl      %eax, %edx
        andl      $2147483647, %edx
        movl      %eax, %ebx
        addl      $-1065353216, %edx
        shrl      $31, %ebx
        call      ..L2
..L2:
        popl      %ebp
        lea       _GLOBAL_OFFSET_TABLE_+[. - ..L2](%ebp), %ebp
        cmpl      $192937984, %edx
        jb        ..B1.8
..B1.2:
        testl     %edx, %edx
        jge       ..B1.7
..B1.3:
        cmpl      $-8388608, %edx
        jge       ..B1.5
..B1.4:
        movss     zeros@GOTOFF(%ebp,%ebx,4), %xmm0
        movss     %xmm0, (%esp)
        jmp       ..B1.6
..B1.5:
        movss     ones@GOTOFF(%ebp,%ebx,4), %xmm0
        movss     %xmm0, (%esp)
..B1.6:
        movss     %xmm0, (%esp)
        flds      (%esp)
        addl      $16, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        ret       
..B1.7:
        flds      32(%esp)
        movss     32(%esp), %xmm0
        movss     %xmm0, (%esp)
        addl      $16, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        ret       
..B1.8:
        movl      %edx, %ecx
        movl      $-1, %edi
        shrl      $23, %ecx
        addl      $9, %ecx
        shll      %cl, %edx
        negl      %ecx
        shll      %cl, %edi
        andl      %edi, %eax
        movl      %eax, (%esp)
        testl     $-2147483648, %edx
        jne       ..B1.10
..B1.9:
        movss     (%esp), %xmm0
        jmp       ..B1.11
..B1.10:
        movss     (%esp), %xmm0
        addss     ones@GOTOFF(%ebp,%ebx,4), %xmm0
        movss     %xmm0, (%esp)
..B1.11:
        movss     %xmm0, (%esp)
        flds      (%esp)
        addl      $16, %esp
        popl      %ebp
        popl      %ebx
        popl      %edi
        ret       
END(roundf)
# -- End  roundf
	.section .rodata, "a"
	.align 4
	.align 4
zeros:
	.long	0x00000000
	.long	0x80000000
	.type	zeros,@object
	.size	zeros,8
	.align 4
ones:
	.long	0x3f800000
	.long	0xbf800000
	.type	ones,@object
	.size	ones,8
	.data
	.section .note.GNU-stack, ""
# End
