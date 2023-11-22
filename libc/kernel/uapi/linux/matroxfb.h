/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __LINUX_MATROXFB_H__
#define __LINUX_MATROXFB_H__
#include <asm/ioctl.h>
#include <linux/types.h>
#include <linux/videodev2.h>
#include <linux/fb.h>
struct matroxioc_output_mode {
  __u32 output;
#define MATROXFB_OUTPUT_PRIMARY 0x0000
#define MATROXFB_OUTPUT_SECONDARY 0x0001
#define MATROXFB_OUTPUT_DFP 0x0002
  __u32 mode;
#define MATROXFB_OUTPUT_MODE_PAL 0x0001
#define MATROXFB_OUTPUT_MODE_NTSC 0x0002
#define MATROXFB_OUTPUT_MODE_MONITOR 0x0080
};
#define MATROXFB_SET_OUTPUT_MODE _IOW('n', 0xFA, size_t)
#define MATROXFB_GET_OUTPUT_MODE _IOWR('n', 0xFA, size_t)
#define MATROXFB_OUTPUT_CONN_PRIMARY (1 << MATROXFB_OUTPUT_PRIMARY)
#define MATROXFB_OUTPUT_CONN_SECONDARY (1 << MATROXFB_OUTPUT_SECONDARY)
#define MATROXFB_OUTPUT_CONN_DFP (1 << MATROXFB_OUTPUT_DFP)
#define MATROXFB_SET_OUTPUT_CONNECTION _IOW('n', 0xF8, size_t)
#define MATROXFB_GET_OUTPUT_CONNECTION _IOR('n', 0xF8, size_t)
#define MATROXFB_GET_AVAILABLE_OUTPUTS _IOR('n', 0xF9, size_t)
#define MATROXFB_GET_ALL_OUTPUTS _IOR('n', 0xFB, size_t)
enum matroxfb_ctrl_id {
  MATROXFB_CID_TESTOUT = V4L2_CID_PRIVATE_BASE,
  MATROXFB_CID_DEFLICKER,
  MATROXFB_CID_LAST
};
#endif
