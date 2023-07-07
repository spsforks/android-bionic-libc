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
#ifndef _UAPI_LINUX_USBIP_H
#define _UAPI_LINUX_USBIP_H
enum usbip_device_status {
  SDEV_ST_AVAILABLE = 0x01,
  SDEV_ST_USED,
  SDEV_ST_ERROR,
  VDEV_ST_NULL,
  VDEV_ST_NOTASSIGNED,
  VDEV_ST_USED,
  VDEV_ST_ERROR
};
#define USBIP_URB_SHORT_NOT_OK 0x0001
#define USBIP_URB_ISO_ASAP 0x0002
#define USBIP_URB_NO_TRANSFER_DMA_MAP 0x0004
#define USBIP_URB_ZERO_PACKET 0x0040
#define USBIP_URB_NO_INTERRUPT 0x0080
#define USBIP_URB_FREE_BUFFER 0x0100
#define USBIP_URB_DIR_IN 0x0200
#define USBIP_URB_DIR_OUT 0
#define USBIP_URB_DIR_MASK USBIP_URB_DIR_IN
#define USBIP_URB_DMA_MAP_SINGLE 0x00010000
#define USBIP_URB_DMA_MAP_PAGE 0x00020000
#define USBIP_URB_DMA_MAP_SG 0x00040000
#define USBIP_URB_MAP_LOCAL 0x00080000
#define USBIP_URB_SETUP_MAP_SINGLE 0x00100000
#define USBIP_URB_SETUP_MAP_LOCAL 0x00200000
#define USBIP_URB_DMA_SG_COMBINED 0x00400000
#define USBIP_URB_ALIGNED_TEMP_BUFFER 0x00800000
#endif
