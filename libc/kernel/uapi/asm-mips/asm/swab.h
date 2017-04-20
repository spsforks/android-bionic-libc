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
#ifndef _ASM_SWAB_H
#define _ASM_SWAB_H
#include <linux/compiler.h>
#include <linux/types.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __SWAB_64_THRU_32__
#if !defined(__mips16) && (defined(__mips_isa_rev) && __mips_isa_rev >= 2 || defined(_MIPS_ARCH_LOONGSON3A))
static inline __attribute_const__ __u16 __arch_swab16(__u16 x) {
  __asm__("	.set	push			\n" "	.set	arch=mips32r2		\n" "	wsbh	%0, %1			\n" "	.set	pop			\n" : "=r" (x) : "r" (x));
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  return x;
}
#define __arch_swab16 __arch_swab16
static inline __attribute_const__ __u32 __arch_swab32(__u32 x) {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  __asm__("	.set	push			\n" "	.set	arch=mips32r2		\n" "	wsbh	%0, %1			\n" "	rotr	%0, %0, 16		\n" "	.set	pop			\n" : "=r" (x) : "r" (x));
  return x;
}
#define __arch_swab32 __arch_swab32
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifdef __mips64
static inline __attribute_const__ __u64 __arch_swab64(__u64 x) {
  __asm__("	.set	push			\n" "	.set	arch=mips64r2		\n" "	dsbh	%0, %1			\n" "	dshd	%0, %0			\n" "	.set	pop			\n" : "=r" (x) : "r" (x));
  return x;
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
}
#define __arch_swab64 __arch_swab64
#endif
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
