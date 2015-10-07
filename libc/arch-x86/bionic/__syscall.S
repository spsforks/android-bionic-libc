#include <private/bionic_asm.h>

ENTRY_PRIVATE(__syscall)
    pushl   %eax
    .cfi_adjust_cfa_offset 4
    .cfi_rel_offset eax, 0
    pushl   %ecx
    .cfi_adjust_cfa_offset 4
    .cfi_rel_offset ecx, 0
    pushl   %edx
    .cfi_adjust_cfa_offset 4
    .cfi_rel_offset edx, 0

    call    __kernel_syscall
    pushl   %eax
    addl    $4, %esp

    popl    %edx
    .cfi_adjust_cfa_offset -4
    .cfi_restore edx
    popl    %ecx
    .cfi_adjust_cfa_offset -4
    .cfi_restore ecx
    popl    %eax
    .cfi_adjust_cfa_offset -4
    .cfi_restore eax
    jmp     -0x10(%esp)
END(__syscall)

ENTRY_PRIVATE(__legacy_syscall)
    int $0x80
    ret
END(__legacy_syscall)


