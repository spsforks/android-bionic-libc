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
#ifndef _ASM_X86_SWAB_H
#define _ASM_X86_SWAB_H
#include <linux/types.h>
#include <linux/compiler.h>
static inline __attribute__((__const__)) __u32 __arch_swab32(__u32 val) {
  asm("bswapl %0" : "=r" (val) : "0" (val));
  return val;
}
#define __arch_swab32 __arch_swab32
static inline __attribute__((__const__)) __u64 __arch_swab64(__u64 val) {
#ifdef __i386__
  union {
    struct {
      __u32 a;
      __u32 b;
    } s;
    __u64 u;
  } v;
  v.u = val;
  asm("bswapl %0; bswapl %1; xchgl %0,%1" : "=r" (v.s.a), "=r" (v.s.b) : "0" (v.s.a), "1" (v.s.b));
  return v.u;
#else
  asm("bswapq %0" : "=r" (val) : "0" (val));
  return val;
#endif
}
#define __arch_swab64 __arch_swab64
#endif
