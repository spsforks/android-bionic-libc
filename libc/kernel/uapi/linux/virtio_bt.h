/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_LINUX_VIRTIO_BT_H
#define _UAPI_LINUX_VIRTIO_BT_H
#include <linux/virtio_types.h>
#define VIRTIO_BT_F_VND_HCI 0
#define VIRTIO_BT_F_MSFT_EXT 1
#define VIRTIO_BT_F_AOSP_EXT 2
#define VIRTIO_BT_F_CONFIG_V2 3
enum virtio_bt_config_type {
  VIRTIO_BT_CONFIG_TYPE_PRIMARY = 0,
  VIRTIO_BT_CONFIG_TYPE_AMP = 1,
};
enum virtio_bt_config_vendor {
  VIRTIO_BT_CONFIG_VENDOR_NONE = 0,
  VIRTIO_BT_CONFIG_VENDOR_ZEPHYR = 1,
  VIRTIO_BT_CONFIG_VENDOR_INTEL = 2,
  VIRTIO_BT_CONFIG_VENDOR_REALTEK = 3,
};
struct virtio_bt_config {
  __u8 type;
  __u16 vendor;
  __u16 msft_opcode;
} __attribute__((packed));
struct virtio_bt_config_v2 {
  __u8 type;
  __u8 alignment;
  __u16 vendor;
  __u16 msft_opcode;
};
#endif
