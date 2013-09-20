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
#ifndef _ASM_X86_PAGE_64_DEFS_H
#define _ASM_X86_PAGE_64_DEFS_H
#define THREAD_ORDER 1
#define THREAD_SIZE (PAGE_SIZE << THREAD_ORDER)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define CURRENT_MASK (~(THREAD_SIZE - 1))
#define EXCEPTION_STACK_ORDER 0
#define EXCEPTION_STKSZ (PAGE_SIZE << EXCEPTION_STACK_ORDER)
#define DEBUG_STACK_ORDER (EXCEPTION_STACK_ORDER + 1)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DEBUG_STKSZ (PAGE_SIZE << DEBUG_STACK_ORDER)
#define IRQ_STACK_ORDER 2
#define IRQ_STACK_SIZE (PAGE_SIZE << IRQ_STACK_ORDER)
#define STACKFAULT_STACK 1
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define DOUBLEFAULT_STACK 2
#define NMI_STACK 3
#define DEBUG_STACK 4
#define MCE_STACK 5
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define N_EXCEPTION_STACKS 5
#define PUD_PAGE_SIZE (_AC(1, UL) << PUD_SHIFT)
#define PUD_PAGE_MASK (~(PUD_PAGE_SIZE-1))
#define __PAGE_OFFSET _AC(0xffff880000000000, UL)
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __PHYSICAL_START ((CONFIG_PHYSICAL_START +   (CONFIG_PHYSICAL_ALIGN - 1)) &   ~(CONFIG_PHYSICAL_ALIGN - 1))
#define __START_KERNEL (__START_KERNEL_map + __PHYSICAL_START)
#define __START_KERNEL_map _AC(0xffffffff80000000, UL)
#define __PHYSICAL_MASK_SHIFT 46
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __VIRTUAL_MASK_SHIFT 47
#define KERNEL_IMAGE_SIZE (512 * 1024 * 1024)
#define KERNEL_IMAGE_START _AC(0xffffffff80000000, UL)
#ifndef __ASSEMBLY__
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
#define __phys_reloc_hide(x) (x)
#define vmemmap ((struct page *)VMEMMAP_START)
#endif
#endif
/* WARNING: DO NOT EDIT, AUTO-GENERATED CODE - SEE TOP FOR INSTRUCTIONS */
