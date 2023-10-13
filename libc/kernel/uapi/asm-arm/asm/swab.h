/****************************************************************************
 ****************************************************************************
 ***
 ***   This header was automatically generated from a Linux kernel header
 ***   of the same name, to make information necessary for userspace to
 ***   call into the kernel available to libc.  It contains only constants,
 ***   structures, and macros generated from the original header, and thus,
 ***   contains no copyrightable information.
 ***
 ***   To edit the content of this header, modify the corresponding
 ***   source file (e.g. under external/kernel-headers/original/) then
 ***   run bionic/libc/kernel/tools/update_all.py
 ***
 ***   Any manual change here will be lost the next time this script will
 ***   be run. You've been warned!
 ***
 ****************************************************************************
 ****************************************************************************/
#ifndef _UAPI__ASM_ARM_SWAB_H
#define _UAPI__ASM_ARM_SWAB_H
#include <linux/compiler.h>
#include <linux/types.h>
#ifndef __STRICT_ANSI__
#define __SWAB_64_THRU_32__
#endif
static inline __attribute__((__const__)) __u32 __arch_swab32(__u32 x) {
  __u32 t;
#ifndef __thumb__
  if(! __builtin_constant_p(x)) {
    asm("eor\t%0, %1, %1, ror #16" : "=r" (t) : "r" (x));
  } else
#endif
  t = x ^ ((x << 16) | (x >> 16));
  x = (x << 24) | (x >> 8);
  t &= ~0x00FF0000;
  x ^= (t >> 8);
  return x;
}
#define __arch_swab32 __arch_swab32
#endif
