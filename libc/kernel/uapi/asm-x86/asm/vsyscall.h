/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_ASM_X86_VSYSCALL_H
#define _UAPI_ASM_X86_VSYSCALL_H
enum vsyscall_num {
  __NR_vgettimeofday,
  __NR_vtime,
  __NR_vgetcpu,
};
#define VSYSCALL_ADDR (- 10UL << 20)
#endif
