#include <private/bionic_asm.h>

// void _exit_with_stack_teardown(void* stackBase, size_t stackSize, int madvCommand, void** cacheMmapBase)
ENTRY_PRIVATE(_exit_with_stack_teardown)
  // We can trash registers because this function never returns.
  mov 4(%esp), %ebx             // stackBase
  mov 8(%esp), %ecx             // stackSize
  mov 12(%esp), %edx            // madvCommand
  mov 16(%esp), %esi            // cacheMmapBase
  test %esi, %esi               // If cacheMmapBase==nullptr...
  jz .Ldo_unmap                 // ...just unmap the stack
  test %edx, %edx               // If madvCommand==MADV_NORMAL...
  jz .Ldo_validate_cache_entry  // ...skip the madvise
  mov $__NR_madvise, %eax
  int $0x80
  // Fall through
.Ldo_validate_cache_entry:
  mov %ebx, (%esi)              // Make the cache entry valid: x86 needs no barrier
  // Fall through
.Ldo_exit:
  mov $0, %ebx                  // status
  movl $__NR_exit, %eax
  int $0x80                     // The exit syscall does not return
.Ldo_unmap:
  mov $__NR_munmap, %eax
  int $0x80
  jmp .Ldo_exit                 // If munmap failed, we ignore the failure and exit anyway
END(_exit_with_stack_teardown)
