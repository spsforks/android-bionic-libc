/*
 * Copyright (C) 2008, 2009 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
#include <private/bionic_asm.h>

/*
 * Trampolines into system calls :
 * Only one of the trampoline for a N-arguments syscall is active at once.
 * Default system call trampoline : int $0x80 based.
 *
 * Upon calling, the stack is expected to be shaped by the system call
 * (ex. splice) and syscall6 call.
 * The global layout has to be as this:
 *
 * splice syscall stack layout upon
 * int80_syscall6 or kernel_syscall6 entry
 * +-------------------------+
 * | 6th syscall parameter   | %esp + 52
 * | 5th syscall parameter   | %esp + 48
 * | 4th syscall parameter   | %esp + 44
 * | 3rd syscall parameter   | %esp + 40
 * | 2nd syscall parameter   | %esp + 36
 * | 1st syscall parameter   | %esp + 32
 * | splice return address   | %esp + 28
 * |     backup of %ebx      | %esp + 24
 * |     backup of %ecx      | %esp + 20
 * |     backup of %edx      | %esp + 16
 * |     backup of %esi      | %esp + 12
 * |     backup of %edi      | %esp + 8
 * |     backup of %ebp      | %esp + 4
 * | syscall6 return address | %esp
 * +-------------------------+
 *
 */
    .macro  SYSCALL_INT80 nb_args
    .text
    .type  int80_syscall\nb_args, __bionic_asm_function_type
    .cfi_startproc
    .cfi_def_cfa_offset 4
int80_syscall\nb_args:
    .if     \nb_args > 0
    mov     4 * \nb_args + 8 (%esp), %ebx
    .endif
    int     $0x80
    ret
    .cfi_endproc
    .size   int80_syscall\nb_args, . - int80_syscall\nb_args
    .endm

    .macro  SYSCALL_KERNEL nb_args
    .text
    .type  kernel_syscall\nb_args, __bionic_asm_function_type
    .cfi_startproc
    .cfi_def_cfa_offset 4
kernel_syscall\nb_args:
    /*
     * As %ebx points to trampoline_syscall\nb_args, calculate the address of
     * kernel_vsyscall data
     */
    movl    (kernel_vsyscall - trampoline_syscall\nb_args)(%ebx), %ebx
    pushl   %ebx
    .cfi_adjust_cfa_offset 4
    .if     \nb_args > 0
    mov     4 * \nb_args + 12 (%esp), %ebx
    .endif
    /*
     * This actually jumps to the kernel mapped system call subroutine. As all
     * registers might be used for syscall parameters, push the address to
     * jump to on the stack, it cannot be an indirect load from a register
     */
    ret
    .cfi_endproc
    .size   kernel_syscall\nb_args, . - kernel_syscall\nb_args
    .endm

SYSCALL_INT80 0
SYSCALL_INT80 1
SYSCALL_INT80 2
SYSCALL_INT80 3
SYSCALL_INT80 4
SYSCALL_INT80 5
SYSCALL_INT80 6

SYSCALL_KERNEL 0
SYSCALL_KERNEL 1
SYSCALL_KERNEL 2
SYSCALL_KERNEL 3
SYSCALL_KERNEL 4
SYSCALL_KERNEL 5
SYSCALL_KERNEL 6

    .text
    .type  int80_socketcall, __bionic_asm_function_type
    .cfi_startproc
int80_socketcall:
    .cfi_def_cfa_offset 4
    movl    %ecx, %ebx
    leal    16(%esp), %ecx
    int     $0x80
    ret
    .cfi_endproc
    .size   int80_socketcall, . - int80_socketcall

    .text
    .type  kernel_socketcall, __bionic_asm_function_type
    .cfi_startproc
kernel_socketcall:
    .cfi_def_cfa_offset 4
    movl    (kernel_vsyscall - trampoline_socketcall)(%ebx), %ebx
    pushl   %ebx
    .cfi_adjust_cfa_offset 4
    mov     %ecx, %ebx
    leal    20(%esp), %ecx
    ret
    .cfi_endproc
    .size   kernel_socketcall, . - kernel_socketcall

    .text
    .type  int80_rawsyscall, __bionic_asm_function_type
    .cfi_startproc
int80_rawsyscall:
    .cfi_def_cfa_offset 4
    mov     36(%esp), %ebx
    int     $0x80
    ret
    .cfi_endproc
    .size   int80_rawsyscall, . - int80_rawsyscall

    .text
    .type  kernel_rawsyscall, __bionic_asm_function_type
    .cfi_startproc
kernel_rawsyscall:
    .cfi_def_cfa_offset 4
    movl    (kernel_vsyscall - trampoline_rawsyscall)(%ebx), %ebx
    pushl   %ebx
    .cfi_adjust_cfa_offset 4
    mov     40(%esp), %ebx
    ret
    .cfi_endproc
    .size   kernel_rawsyscall, . - kernel_rawsyscall

    .data
    .align  4

    .macro  define_trampoline name default_function
    .globl  \name
\name:
    .long   \default_function
    .endm
    define_trampoline trampoline_syscall0 int80_syscall0
    define_trampoline trampoline_syscall1 int80_syscall1
    define_trampoline trampoline_syscall2 int80_syscall2
    define_trampoline trampoline_syscall3 int80_syscall3
    define_trampoline trampoline_syscall4 int80_syscall4
    define_trampoline trampoline_syscall5 int80_syscall5
    define_trampoline trampoline_syscall6 int80_syscall6
    define_trampoline trampoline_socketcall int80_socketcall
    define_trampoline trampoline_rawsyscall int80_rawsyscall
    .set    nb_trampolines, (. - trampoline_syscall0) / 4

trampoline_kernel_optimizers:
    .long   kernel_syscall0
    .long   kernel_syscall1
    .long   kernel_syscall2
    .long   kernel_syscall3
    .long   kernel_syscall4
    .long   kernel_syscall5
    .long   kernel_syscall6
    .long   kernel_socketcall
    .long   kernel_rawsyscall
kernel_vsyscall:
    .long   0x12349876

ENTRY_PRIVATE(preinit_vsyscall)
    pushl   %ebx
    .cfi_adjust_cfa_offset 4
    pushl   %ecx
    .cfi_adjust_cfa_offset 4
    pushl   %esi
    .cfi_adjust_cfa_offset 4
    pushl   %edi
    .cfi_adjust_cfa_offset 4

    call    __x86.get_pc_thunk.bx
    addl    $_GLOBAL_OFFSET_TABLE_, %ebx
    movl    kernel_vsyscall@GOT(%ebx), %esi
    movl    %eax, (%esi)

    movl    trampoline_kernel_optimizers@GOT(%ebx), %esi
    movl    trampoline_syscall0@GOT(%ebx), %edi
    movl    $nb_trampolines, %ecx
    cld
    repnz   movsl

    popl    %edi
    .cfi_adjust_cfa_offset -4
    popl    %esi
    .cfi_adjust_cfa_offset -4
    popl    %ecx
    .cfi_adjust_cfa_offset -4
    popl    %ebx
    .cfi_adjust_cfa_offset -4
    ret
END(preinit_vsyscall)
