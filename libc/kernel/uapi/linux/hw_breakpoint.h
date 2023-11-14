/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_LINUX_HW_BREAKPOINT_H
#define _UAPI_LINUX_HW_BREAKPOINT_H
enum {
  HW_BREAKPOINT_LEN_1 = 1,
  HW_BREAKPOINT_LEN_2 = 2,
  HW_BREAKPOINT_LEN_3 = 3,
  HW_BREAKPOINT_LEN_4 = 4,
  HW_BREAKPOINT_LEN_5 = 5,
  HW_BREAKPOINT_LEN_6 = 6,
  HW_BREAKPOINT_LEN_7 = 7,
  HW_BREAKPOINT_LEN_8 = 8,
};
enum {
  HW_BREAKPOINT_EMPTY = 0,
  HW_BREAKPOINT_R = 1,
  HW_BREAKPOINT_W = 2,
  HW_BREAKPOINT_RW = HW_BREAKPOINT_R | HW_BREAKPOINT_W,
  HW_BREAKPOINT_X = 4,
  HW_BREAKPOINT_INVALID = HW_BREAKPOINT_RW | HW_BREAKPOINT_X,
};
#endif
