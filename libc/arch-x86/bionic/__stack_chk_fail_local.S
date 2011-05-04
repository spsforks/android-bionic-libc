        .text
        .align 4
        .type __stack_chk_fail_local, @function
        .globl __stack_chk_fail_local

__stack_chk_fail_local:
	jmp   __stack_chk_fail
