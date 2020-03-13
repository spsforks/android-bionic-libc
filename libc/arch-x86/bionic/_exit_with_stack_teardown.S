#include <private/bionic_asm.h>

// void _exit_with_stack_teardown(void* stackBase, size_t stackSize, void** cacheMmapBase)
ENTRY_PRIVATE(_exit_with_stack_teardown)
  // We can trash registers because this function never returns.
  mov 4(%esp), %ebx             // stackBase
  mov 8(%esp), %ecx             // stackSize
  mov 12(%esp), %edx            // cacheMmapBase
  test %edx, %edx
  jz do_unmap  // If cacheMmapBase==nullptr, unmap
  mov %ebx, (%edx)  // Make the cache entry valid: x86 needs no barrier
  // Fall through
do_exit:
  mov $0, %ebx                  // status
  movl $__NR_exit, %eax
  int $0x80  // The exit syscall does not return.
do_unmap:
  mov $__NR_munmap, %eax
  int $0x80
  // If munmap failed, we ignore the failure and exit anyway.
  jmp do_exit

  // The exit syscall does not return.
END(_exit_with_stack_teardown)
