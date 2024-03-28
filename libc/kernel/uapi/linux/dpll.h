/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_LINUX_DPLL_H
#define _UAPI_LINUX_DPLL_H
#define DPLL_FAMILY_NAME "dpll"
#define DPLL_FAMILY_VERSION 1
enum dpll_mode {
  DPLL_MODE_MANUAL = 1,
  DPLL_MODE_AUTOMATIC,
  __DPLL_MODE_MAX,
  DPLL_MODE_MAX = (__DPLL_MODE_MAX - 1)
};
enum dpll_lock_status {
  DPLL_LOCK_STATUS_UNLOCKED = 1,
  DPLL_LOCK_STATUS_LOCKED,
  DPLL_LOCK_STATUS_LOCKED_HO_ACQ,
  DPLL_LOCK_STATUS_HOLDOVER,
  __DPLL_LOCK_STATUS_MAX,
  DPLL_LOCK_STATUS_MAX = (__DPLL_LOCK_STATUS_MAX - 1)
};
#define DPLL_TEMP_DIVIDER 1000
enum dpll_type {
  DPLL_TYPE_PPS = 1,
  DPLL_TYPE_EEC,
  __DPLL_TYPE_MAX,
  DPLL_TYPE_MAX = (__DPLL_TYPE_MAX - 1)
};
enum dpll_pin_type {
  DPLL_PIN_TYPE_MUX = 1,
  DPLL_PIN_TYPE_EXT,
  DPLL_PIN_TYPE_SYNCE_ETH_PORT,
  DPLL_PIN_TYPE_INT_OSCILLATOR,
  DPLL_PIN_TYPE_GNSS,
  __DPLL_PIN_TYPE_MAX,
  DPLL_PIN_TYPE_MAX = (__DPLL_PIN_TYPE_MAX - 1)
};
enum dpll_pin_direction {
  DPLL_PIN_DIRECTION_INPUT = 1,
  DPLL_PIN_DIRECTION_OUTPUT,
  __DPLL_PIN_DIRECTION_MAX,
  DPLL_PIN_DIRECTION_MAX = (__DPLL_PIN_DIRECTION_MAX - 1)
};
#define DPLL_PIN_FREQUENCY_1_HZ 1
#define DPLL_PIN_FREQUENCY_10_KHZ 10000
#define DPLL_PIN_FREQUENCY_77_5_KHZ 77500
#define DPLL_PIN_FREQUENCY_10_MHZ 10000000
enum dpll_pin_state {
  DPLL_PIN_STATE_CONNECTED = 1,
  DPLL_PIN_STATE_DISCONNECTED,
  DPLL_PIN_STATE_SELECTABLE,
  __DPLL_PIN_STATE_MAX,
  DPLL_PIN_STATE_MAX = (__DPLL_PIN_STATE_MAX - 1)
};
enum dpll_pin_capabilities {
  DPLL_PIN_CAPABILITIES_DIRECTION_CAN_CHANGE = 1,
  DPLL_PIN_CAPABILITIES_PRIORITY_CAN_CHANGE = 2,
  DPLL_PIN_CAPABILITIES_STATE_CAN_CHANGE = 4,
};
#define DPLL_PHASE_OFFSET_DIVIDER 1000
enum dpll_a {
  DPLL_A_ID = 1,
  DPLL_A_MODULE_NAME,
  DPLL_A_PAD,
  DPLL_A_CLOCK_ID,
  DPLL_A_MODE,
  DPLL_A_MODE_SUPPORTED,
  DPLL_A_LOCK_STATUS,
  DPLL_A_TEMP,
  DPLL_A_TYPE,
  __DPLL_A_MAX,
  DPLL_A_MAX = (__DPLL_A_MAX - 1)
};
enum dpll_a_pin {
  DPLL_A_PIN_ID = 1,
  DPLL_A_PIN_PARENT_ID,
  DPLL_A_PIN_MODULE_NAME,
  DPLL_A_PIN_PAD,
  DPLL_A_PIN_CLOCK_ID,
  DPLL_A_PIN_BOARD_LABEL,
  DPLL_A_PIN_PANEL_LABEL,
  DPLL_A_PIN_PACKAGE_LABEL,
  DPLL_A_PIN_TYPE,
  DPLL_A_PIN_DIRECTION,
  DPLL_A_PIN_FREQUENCY,
  DPLL_A_PIN_FREQUENCY_SUPPORTED,
  DPLL_A_PIN_FREQUENCY_MIN,
  DPLL_A_PIN_FREQUENCY_MAX,
  DPLL_A_PIN_PRIO,
  DPLL_A_PIN_STATE,
  DPLL_A_PIN_CAPABILITIES,
  DPLL_A_PIN_PARENT_DEVICE,
  DPLL_A_PIN_PARENT_PIN,
  DPLL_A_PIN_PHASE_ADJUST_MIN,
  DPLL_A_PIN_PHASE_ADJUST_MAX,
  DPLL_A_PIN_PHASE_ADJUST,
  DPLL_A_PIN_PHASE_OFFSET,
  DPLL_A_PIN_FRACTIONAL_FREQUENCY_OFFSET,
  __DPLL_A_PIN_MAX,
  DPLL_A_PIN_MAX = (__DPLL_A_PIN_MAX - 1)
};
enum dpll_cmd {
  DPLL_CMD_DEVICE_ID_GET = 1,
  DPLL_CMD_DEVICE_GET,
  DPLL_CMD_DEVICE_SET,
  DPLL_CMD_DEVICE_CREATE_NTF,
  DPLL_CMD_DEVICE_DELETE_NTF,
  DPLL_CMD_DEVICE_CHANGE_NTF,
  DPLL_CMD_PIN_ID_GET,
  DPLL_CMD_PIN_GET,
  DPLL_CMD_PIN_SET,
  DPLL_CMD_PIN_CREATE_NTF,
  DPLL_CMD_PIN_DELETE_NTF,
  DPLL_CMD_PIN_CHANGE_NTF,
  __DPLL_CMD_MAX,
  DPLL_CMD_MAX = (__DPLL_CMD_MAX - 1)
};
#define DPLL_MCGRP_MONITOR "monitor"
#endif
