/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#include <asm-generic/mman.h>
#define arch_mmap_check(addr,len,flags) (((flags) & MAP_FIXED && (addr) < FIRST_USER_ADDRESS) ? - EINVAL : 0)
