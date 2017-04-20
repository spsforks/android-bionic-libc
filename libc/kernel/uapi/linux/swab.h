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
#ifndef _UAPI_LINUX_SWAB_H
#define _UAPI_LINUX_SWAB_H
#include <linux/types.h>
#include <linux/compiler.h>
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#include <asm/swab.h>
#define ___constant_swab16(x) ((__u16) ((((__u16) (x) & (__u16) 0x00ffU) << 8) | (((__u16) (x) & (__u16) 0xff00U) >> 8)))
#define ___constant_swab32(x) ((__u32) ((((__u32) (x) & (__u32) 0x000000ffUL) << 24) | (((__u32) (x) & (__u32) 0x0000ff00UL) << 8) | (((__u32) (x) & (__u32) 0x00ff0000UL) >> 8) | (((__u32) (x) & (__u32) 0xff000000UL) >> 24)))
#define ___constant_swab64(x) ((__u64) ((((__u64) (x) & (__u64) 0x00000000000000ffULL) << 56) | (((__u64) (x) & (__u64) 0x000000000000ff00ULL) << 40) | (((__u64) (x) & (__u64) 0x0000000000ff0000ULL) << 24) | (((__u64) (x) & (__u64) 0x00000000ff000000ULL) << 8) | (((__u64) (x) & (__u64) 0x000000ff00000000ULL) >> 8) | (((__u64) (x) & (__u64) 0x0000ff0000000000ULL) >> 24) | (((__u64) (x) & (__u64) 0x00ff000000000000ULL) >> 40) | (((__u64) (x) & (__u64) 0xff00000000000000ULL) >> 56)))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define ___constant_swahw32(x) ((__u32) ((((__u32) (x) & (__u32) 0x0000ffffUL) << 16) | (((__u32) (x) & (__u32) 0xffff0000UL) >> 16)))
#define ___constant_swahb32(x) ((__u32) ((((__u32) (x) & (__u32) 0x00ff00ffUL) << 8) | (((__u32) (x) & (__u32) 0xff00ff00UL) >> 8)))
static inline __attribute_const__ __u16 __fswab16(__u16 val) {
#ifdef __arch_swab16
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  return __arch_swab16(val);
#else
  return ___constant_swab16(val);
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
}
static inline __attribute_const__ __u32 __fswab32(__u32 val) {
#ifdef __arch_swab32
  return __arch_swab32(val);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#else
  return ___constant_swab32(val);
#endif
}
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
static inline __attribute_const__ __u64 __fswab64(__u64 val) {
#ifdef __arch_swab64
  return __arch_swab64(val);
#elif defined(__SWAB_64_THRU_32__)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  __u32 h = val >> 32;
  __u32 l = val & ((1ULL << 32) - 1);
  return(((__u64) __fswab32(l)) << 32) | ((__u64) (__fswab32(h)));
#else
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  return ___constant_swab64(val);
#endif
}
static inline __attribute_const__ __u32 __fswahw32(__u32 val) {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifdef __arch_swahw32
  return __arch_swahw32(val);
#else
  return ___constant_swahw32(val);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
}
static inline __attribute_const__ __u32 __fswahb32(__u32 val) {
#ifdef __arch_swahb32
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  return __arch_swahb32(val);
#else
  return ___constant_swahb32(val);
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
}
#define __swab16(x) (__u16) __builtin_bswap16((__u16) (x))
#define __swab32(x) (__u32) __builtin_bswap32((__u32) (x))
#define __swab64(x) (__u64) __builtin_bswap64((__u64) (x))
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __swahw32(x) (__builtin_constant_p((__u32) (x)) ? ___constant_swahw32(x) : __fswahw32(x))
#define __swahb32(x) (__builtin_constant_p((__u32) (x)) ? ___constant_swahb32(x) : __fswahb32(x))
#ifdef __arch_swab16p
#else
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
static __always_inline __u32 __swab32p(const __u32 * p) {
#ifdef __arch_swab32p
  return __arch_swab32p(p);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#else
  return __swab32(* p);
#endif
}
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
static __always_inline __u64 __swab64p(const __u64 * p) {
#ifdef __arch_swab64p
  return __arch_swab64p(p);
#else
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  return __swab64(* p);
#endif
}
static inline __u32 __swahw32p(const __u32 * p) {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifdef __arch_swahw32p
  return __arch_swahw32p(p);
#else
  return __swahw32(* p);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
}
static inline __u32 __swahb32p(const __u32 * p) {
#ifdef __arch_swahb32p
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  return __arch_swahb32p(p);
#else
  return __swahb32(* p);
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
}
#ifdef __arch_swab16s
#else
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifdef __arch_swab32s
#else
#endif
static __always_inline void __swab64s(__u64 * p) {
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#ifdef __arch_swab64s
  __arch_swab64s(p);
#else
  * p = __swab64p(p);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
}
static inline void __swahw32s(__u32 * p) {
#ifdef __arch_swahw32s
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
  __arch_swahw32s(p);
#else
  * p = __swahw32p(p);
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
}
static inline void __swahb32s(__u32 * p) {
#ifdef __arch_swahb32s
  __arch_swahb32s(p);
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#else
  * p = __swahb32p(p);
#endif
}
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
