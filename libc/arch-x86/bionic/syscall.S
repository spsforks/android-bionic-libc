/*
 * Generic syscall call.
 * Upon entry:
 *	%eax: system call number  - caller save
 *	%ebx: arg0 to system call -   callee save
 *	%ecx: arg1                - caller save
 *	%edx: arg2                - caller save
 *	%esi: arg3                -   callee save
 *	%edi: arg4                -   callee save
 *	%ebp: arg5                -   callee save
 */

#include <private/bionic_asm.h>

ENTRY(syscall)
    .cfi_def_cfa_offset 4

    pushl    %ebx
    .cfi_adjust_cfa_offset 4
    .cfi_rel_offset ebx, 0
    movl     12(%esp), %ebx

    pushl    %ecx
    .cfi_adjust_cfa_offset 4
    .cfi_rel_offset ecx, 0
    movl     20(%esp), %ecx

    pushl    %edx
    .cfi_adjust_cfa_offset 4
    .cfi_rel_offset edx, 0
    movl     28(%esp), %edx

    pushl    %esi
    .cfi_adjust_cfa_offset 4
    .cfi_rel_offset esi, 0
    movl     36(%esp), %esi

    pushl    %edi
    .cfi_adjust_cfa_offset 4
    .cfi_rel_offset edi, 0
    movl     44(%esp), %edi

    pushl    %ebp
    .cfi_adjust_cfa_offset 4
    .cfi_rel_offset ebp, 0
    movl     52(%esp), %ebp

    movl     28(%esp), %eax
    call    __x86.get_pc_thunk.bx
    addl    $_GLOBAL_OFFSET_TABLE_, %ebx
    movl    trampoline_rawsyscall@GOT(%ebx), %ebx
    call    *(%ebx)

    cmpl    $-MAX_ERRNO, %eax
    jb      1f

    negl    %eax
    pushl   %eax
    .cfi_adjust_cfa_offset 4
    call    __set_errno_internal
    addl    $4, %esp
    .cfi_adjust_cfa_offset -4
1:

    popl    %ebp
    .cfi_adjust_cfa_offset -4

    popl    %edi
    .cfi_adjust_cfa_offset -4

    popl    %esi
    .cfi_adjust_cfa_offset -4

    popl    %edx
    .cfi_adjust_cfa_offset -4

    popl    %ecx
    .cfi_adjust_cfa_offset -4

    popl    %ebx
    .cfi_adjust_cfa_offset -4
    ret
END(syscall)
