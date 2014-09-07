#include <private/bionic_asm.h>

ENTRY_PRIVATE(__sigreturn)
  popl %eax
  movl $__NR_sigreturn, %eax
  int $0x80
END(__sigreturn)
