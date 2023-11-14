/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_XT_PHYSDEV_H
#define _UAPI_XT_PHYSDEV_H
#include <linux/types.h>
#include <linux/if.h>
#define XT_PHYSDEV_OP_IN 0x01
#define XT_PHYSDEV_OP_OUT 0x02
#define XT_PHYSDEV_OP_BRIDGED 0x04
#define XT_PHYSDEV_OP_ISIN 0x08
#define XT_PHYSDEV_OP_ISOUT 0x10
#define XT_PHYSDEV_OP_MASK (0x20 - 1)
struct xt_physdev_info {
  char physindev[IFNAMSIZ];
  char in_mask[IFNAMSIZ];
  char physoutdev[IFNAMSIZ];
  char out_mask[IFNAMSIZ];
  __u8 invert;
  __u8 bitmask;
};
#endif
