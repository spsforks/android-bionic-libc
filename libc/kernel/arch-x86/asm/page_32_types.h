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
#ifndef _ASM_X86_PAGE_32_DEFS_H
#define _ASM_X86_PAGE_32_DEFS_H
#include <linux/const.h>
#define __PAGE_OFFSET _AC(CONFIG_PAGE_OFFSET, UL)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define THREAD_ORDER 1
#define THREAD_SIZE (PAGE_SIZE << THREAD_ORDER)
#define STACKFAULT_STACK 0
#define DOUBLEFAULT_STACK 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define NMI_STACK 0
#define DEBUG_STACK 0
#define MCE_STACK 0
#define N_EXCEPTION_STACKS 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __PHYSICAL_MASK_SHIFT 32
#define __VIRTUAL_MASK_SHIFT 32
#define KERNEL_IMAGE_SIZE (512 * 1024 * 1024)
#ifndef __ASSEMBLY__
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#endif
#endif
