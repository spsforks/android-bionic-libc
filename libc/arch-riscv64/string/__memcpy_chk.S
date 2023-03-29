#include <private/bionic_asm.h>

ENTRY(__memcpy_chk)
  bleu a2, a3, 1f
  // Preserve for accurate backtrace.
   addi sp, sp, -16
   sd fp, -8(sp)
   sd ra, -16(sp)
.cfi_def_cfa_offset 16
.cfi_rel_offset fp, 0
.cfi_rel_offset ra, 8
   addi sp, sp, 16
   tail __memcpy_chk_fail

1:
   tail memcpy
END(__memcpy_chk)
