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
#ifndef _UAPI_LINUX_ETHTOOL_NETLINK_H_
#define _UAPI_LINUX_ETHTOOL_NETLINK_H_
#include <linux/ethtool.h>
enum {
  ETHTOOL_MSG_USER_NONE,
  ETHTOOL_MSG_STRSET_GET,
  ETHTOOL_MSG_LINKINFO_GET,
  ETHTOOL_MSG_LINKINFO_SET,
  ETHTOOL_MSG_LINKMODES_GET,
  ETHTOOL_MSG_LINKMODES_SET,
  ETHTOOL_MSG_LINKSTATE_GET,
  ETHTOOL_MSG_DEBUG_GET,
  ETHTOOL_MSG_DEBUG_SET,
  ETHTOOL_MSG_WOL_GET,
  ETHTOOL_MSG_WOL_SET,
  ETHTOOL_MSG_FEATURES_GET,
  ETHTOOL_MSG_FEATURES_SET,
  ETHTOOL_MSG_PRIVFLAGS_GET,
  ETHTOOL_MSG_PRIVFLAGS_SET,
  ETHTOOL_MSG_RINGS_GET,
  ETHTOOL_MSG_RINGS_SET,
  ETHTOOL_MSG_CHANNELS_GET,
  ETHTOOL_MSG_CHANNELS_SET,
  ETHTOOL_MSG_COALESCE_GET,
  ETHTOOL_MSG_COALESCE_SET,
  ETHTOOL_MSG_PAUSE_GET,
  ETHTOOL_MSG_PAUSE_SET,
  ETHTOOL_MSG_EEE_GET,
  ETHTOOL_MSG_EEE_SET,
  ETHTOOL_MSG_TSINFO_GET,
  ETHTOOL_MSG_CABLE_TEST_ACT,
  ETHTOOL_MSG_CABLE_TEST_TDR_ACT,
  ETHTOOL_MSG_TUNNEL_INFO_GET,
  ETHTOOL_MSG_FEC_GET,
  ETHTOOL_MSG_FEC_SET,
  ETHTOOL_MSG_MODULE_EEPROM_GET,
  ETHTOOL_MSG_STATS_GET,
  ETHTOOL_MSG_PHC_VCLOCKS_GET,
  ETHTOOL_MSG_MODULE_GET,
  ETHTOOL_MSG_MODULE_SET,
  ETHTOOL_MSG_PSE_GET,
  ETHTOOL_MSG_PSE_SET,
  ETHTOOL_MSG_RSS_GET,
  ETHTOOL_MSG_PLCA_GET_CFG,
  ETHTOOL_MSG_PLCA_SET_CFG,
  ETHTOOL_MSG_PLCA_GET_STATUS,
  ETHTOOL_MSG_MM_GET,
  ETHTOOL_MSG_MM_SET,
  __ETHTOOL_MSG_USER_CNT,
  ETHTOOL_MSG_USER_MAX = __ETHTOOL_MSG_USER_CNT - 1
};
enum {
  ETHTOOL_MSG_KERNEL_NONE,
  ETHTOOL_MSG_STRSET_GET_REPLY,
  ETHTOOL_MSG_LINKINFO_GET_REPLY,
  ETHTOOL_MSG_LINKINFO_NTF,
  ETHTOOL_MSG_LINKMODES_GET_REPLY,
  ETHTOOL_MSG_LINKMODES_NTF,
  ETHTOOL_MSG_LINKSTATE_GET_REPLY,
  ETHTOOL_MSG_DEBUG_GET_REPLY,
  ETHTOOL_MSG_DEBUG_NTF,
  ETHTOOL_MSG_WOL_GET_REPLY,
  ETHTOOL_MSG_WOL_NTF,
  ETHTOOL_MSG_FEATURES_GET_REPLY,
  ETHTOOL_MSG_FEATURES_SET_REPLY,
  ETHTOOL_MSG_FEATURES_NTF,
  ETHTOOL_MSG_PRIVFLAGS_GET_REPLY,
  ETHTOOL_MSG_PRIVFLAGS_NTF,
  ETHTOOL_MSG_RINGS_GET_REPLY,
  ETHTOOL_MSG_RINGS_NTF,
  ETHTOOL_MSG_CHANNELS_GET_REPLY,
  ETHTOOL_MSG_CHANNELS_NTF,
  ETHTOOL_MSG_COALESCE_GET_REPLY,
  ETHTOOL_MSG_COALESCE_NTF,
  ETHTOOL_MSG_PAUSE_GET_REPLY,
  ETHTOOL_MSG_PAUSE_NTF,
  ETHTOOL_MSG_EEE_GET_REPLY,
  ETHTOOL_MSG_EEE_NTF,
  ETHTOOL_MSG_TSINFO_GET_REPLY,
  ETHTOOL_MSG_CABLE_TEST_NTF,
  ETHTOOL_MSG_CABLE_TEST_TDR_NTF,
  ETHTOOL_MSG_TUNNEL_INFO_GET_REPLY,
  ETHTOOL_MSG_FEC_GET_REPLY,
  ETHTOOL_MSG_FEC_NTF,
  ETHTOOL_MSG_MODULE_EEPROM_GET_REPLY,
  ETHTOOL_MSG_STATS_GET_REPLY,
  ETHTOOL_MSG_PHC_VCLOCKS_GET_REPLY,
  ETHTOOL_MSG_MODULE_GET_REPLY,
  ETHTOOL_MSG_MODULE_NTF,
  ETHTOOL_MSG_PSE_GET_REPLY,
  ETHTOOL_MSG_RSS_GET_REPLY,
  ETHTOOL_MSG_PLCA_GET_CFG_REPLY,
  ETHTOOL_MSG_PLCA_GET_STATUS_REPLY,
  ETHTOOL_MSG_PLCA_NTF,
  ETHTOOL_MSG_MM_GET_REPLY,
  ETHTOOL_MSG_MM_NTF,
  __ETHTOOL_MSG_KERNEL_CNT,
  ETHTOOL_MSG_KERNEL_MAX = __ETHTOOL_MSG_KERNEL_CNT - 1
};
#define ETHTOOL_FLAG_COMPACT_BITSETS (1 << 0)
#define ETHTOOL_FLAG_OMIT_REPLY (1 << 1)
#define ETHTOOL_FLAG_STATS (1 << 2)
#define ETHTOOL_FLAG_ALL (ETHTOOL_FLAG_COMPACT_BITSETS | ETHTOOL_FLAG_OMIT_REPLY | ETHTOOL_FLAG_STATS)
enum {
  ETHTOOL_A_HEADER_UNSPEC,
  ETHTOOL_A_HEADER_DEV_INDEX,
  ETHTOOL_A_HEADER_DEV_NAME,
  ETHTOOL_A_HEADER_FLAGS,
  __ETHTOOL_A_HEADER_CNT,
  ETHTOOL_A_HEADER_MAX = __ETHTOOL_A_HEADER_CNT - 1
};
enum {
  ETHTOOL_A_BITSET_BIT_UNSPEC,
  ETHTOOL_A_BITSET_BIT_INDEX,
  ETHTOOL_A_BITSET_BIT_NAME,
  ETHTOOL_A_BITSET_BIT_VALUE,
  __ETHTOOL_A_BITSET_BIT_CNT,
  ETHTOOL_A_BITSET_BIT_MAX = __ETHTOOL_A_BITSET_BIT_CNT - 1
};
enum {
  ETHTOOL_A_BITSET_BITS_UNSPEC,
  ETHTOOL_A_BITSET_BITS_BIT,
  __ETHTOOL_A_BITSET_BITS_CNT,
  ETHTOOL_A_BITSET_BITS_MAX = __ETHTOOL_A_BITSET_BITS_CNT - 1
};
enum {
  ETHTOOL_A_BITSET_UNSPEC,
  ETHTOOL_A_BITSET_NOMASK,
  ETHTOOL_A_BITSET_SIZE,
  ETHTOOL_A_BITSET_BITS,
  ETHTOOL_A_BITSET_VALUE,
  ETHTOOL_A_BITSET_MASK,
  __ETHTOOL_A_BITSET_CNT,
  ETHTOOL_A_BITSET_MAX = __ETHTOOL_A_BITSET_CNT - 1
};
enum {
  ETHTOOL_A_STRING_UNSPEC,
  ETHTOOL_A_STRING_INDEX,
  ETHTOOL_A_STRING_VALUE,
  __ETHTOOL_A_STRING_CNT,
  ETHTOOL_A_STRING_MAX = __ETHTOOL_A_STRING_CNT - 1
};
enum {
  ETHTOOL_A_STRINGS_UNSPEC,
  ETHTOOL_A_STRINGS_STRING,
  __ETHTOOL_A_STRINGS_CNT,
  ETHTOOL_A_STRINGS_MAX = __ETHTOOL_A_STRINGS_CNT - 1
};
enum {
  ETHTOOL_A_STRINGSET_UNSPEC,
  ETHTOOL_A_STRINGSET_ID,
  ETHTOOL_A_STRINGSET_COUNT,
  ETHTOOL_A_STRINGSET_STRINGS,
  __ETHTOOL_A_STRINGSET_CNT,
  ETHTOOL_A_STRINGSET_MAX = __ETHTOOL_A_STRINGSET_CNT - 1
};
enum {
  ETHTOOL_A_STRINGSETS_UNSPEC,
  ETHTOOL_A_STRINGSETS_STRINGSET,
  __ETHTOOL_A_STRINGSETS_CNT,
  ETHTOOL_A_STRINGSETS_MAX = __ETHTOOL_A_STRINGSETS_CNT - 1
};
enum {
  ETHTOOL_A_STRSET_UNSPEC,
  ETHTOOL_A_STRSET_HEADER,
  ETHTOOL_A_STRSET_STRINGSETS,
  ETHTOOL_A_STRSET_COUNTS_ONLY,
  __ETHTOOL_A_STRSET_CNT,
  ETHTOOL_A_STRSET_MAX = __ETHTOOL_A_STRSET_CNT - 1
};
enum {
  ETHTOOL_A_LINKINFO_UNSPEC,
  ETHTOOL_A_LINKINFO_HEADER,
  ETHTOOL_A_LINKINFO_PORT,
  ETHTOOL_A_LINKINFO_PHYADDR,
  ETHTOOL_A_LINKINFO_TP_MDIX,
  ETHTOOL_A_LINKINFO_TP_MDIX_CTRL,
  ETHTOOL_A_LINKINFO_TRANSCEIVER,
  __ETHTOOL_A_LINKINFO_CNT,
  ETHTOOL_A_LINKINFO_MAX = __ETHTOOL_A_LINKINFO_CNT - 1
};
enum {
  ETHTOOL_A_LINKMODES_UNSPEC,
  ETHTOOL_A_LINKMODES_HEADER,
  ETHTOOL_A_LINKMODES_AUTONEG,
  ETHTOOL_A_LINKMODES_OURS,
  ETHTOOL_A_LINKMODES_PEER,
  ETHTOOL_A_LINKMODES_SPEED,
  ETHTOOL_A_LINKMODES_DUPLEX,
  ETHTOOL_A_LINKMODES_MASTER_SLAVE_CFG,
  ETHTOOL_A_LINKMODES_MASTER_SLAVE_STATE,
  ETHTOOL_A_LINKMODES_LANES,
  ETHTOOL_A_LINKMODES_RATE_MATCHING,
  __ETHTOOL_A_LINKMODES_CNT,
  ETHTOOL_A_LINKMODES_MAX = __ETHTOOL_A_LINKMODES_CNT - 1
};
enum {
  ETHTOOL_A_LINKSTATE_UNSPEC,
  ETHTOOL_A_LINKSTATE_HEADER,
  ETHTOOL_A_LINKSTATE_LINK,
  ETHTOOL_A_LINKSTATE_SQI,
  ETHTOOL_A_LINKSTATE_SQI_MAX,
  ETHTOOL_A_LINKSTATE_EXT_STATE,
  ETHTOOL_A_LINKSTATE_EXT_SUBSTATE,
  ETHTOOL_A_LINKSTATE_EXT_DOWN_CNT,
  __ETHTOOL_A_LINKSTATE_CNT,
  ETHTOOL_A_LINKSTATE_MAX = __ETHTOOL_A_LINKSTATE_CNT - 1
};
enum {
  ETHTOOL_A_DEBUG_UNSPEC,
  ETHTOOL_A_DEBUG_HEADER,
  ETHTOOL_A_DEBUG_MSGMASK,
  __ETHTOOL_A_DEBUG_CNT,
  ETHTOOL_A_DEBUG_MAX = __ETHTOOL_A_DEBUG_CNT - 1
};
enum {
  ETHTOOL_A_WOL_UNSPEC,
  ETHTOOL_A_WOL_HEADER,
  ETHTOOL_A_WOL_MODES,
  ETHTOOL_A_WOL_SOPASS,
  __ETHTOOL_A_WOL_CNT,
  ETHTOOL_A_WOL_MAX = __ETHTOOL_A_WOL_CNT - 1
};
enum {
  ETHTOOL_A_FEATURES_UNSPEC,
  ETHTOOL_A_FEATURES_HEADER,
  ETHTOOL_A_FEATURES_HW,
  ETHTOOL_A_FEATURES_WANTED,
  ETHTOOL_A_FEATURES_ACTIVE,
  ETHTOOL_A_FEATURES_NOCHANGE,
  __ETHTOOL_A_FEATURES_CNT,
  ETHTOOL_A_FEATURES_MAX = __ETHTOOL_A_FEATURES_CNT - 1
};
enum {
  ETHTOOL_A_PRIVFLAGS_UNSPEC,
  ETHTOOL_A_PRIVFLAGS_HEADER,
  ETHTOOL_A_PRIVFLAGS_FLAGS,
  __ETHTOOL_A_PRIVFLAGS_CNT,
  ETHTOOL_A_PRIVFLAGS_MAX = __ETHTOOL_A_PRIVFLAGS_CNT - 1
};
enum {
  ETHTOOL_TCP_DATA_SPLIT_UNKNOWN = 0,
  ETHTOOL_TCP_DATA_SPLIT_DISABLED,
  ETHTOOL_TCP_DATA_SPLIT_ENABLED,
};
enum {
  ETHTOOL_A_RINGS_UNSPEC,
  ETHTOOL_A_RINGS_HEADER,
  ETHTOOL_A_RINGS_RX_MAX,
  ETHTOOL_A_RINGS_RX_MINI_MAX,
  ETHTOOL_A_RINGS_RX_JUMBO_MAX,
  ETHTOOL_A_RINGS_TX_MAX,
  ETHTOOL_A_RINGS_RX,
  ETHTOOL_A_RINGS_RX_MINI,
  ETHTOOL_A_RINGS_RX_JUMBO,
  ETHTOOL_A_RINGS_TX,
  ETHTOOL_A_RINGS_RX_BUF_LEN,
  ETHTOOL_A_RINGS_TCP_DATA_SPLIT,
  ETHTOOL_A_RINGS_CQE_SIZE,
  ETHTOOL_A_RINGS_TX_PUSH,
  ETHTOOL_A_RINGS_RX_PUSH,
  __ETHTOOL_A_RINGS_CNT,
  ETHTOOL_A_RINGS_MAX = (__ETHTOOL_A_RINGS_CNT - 1)
};
enum {
  ETHTOOL_A_CHANNELS_UNSPEC,
  ETHTOOL_A_CHANNELS_HEADER,
  ETHTOOL_A_CHANNELS_RX_MAX,
  ETHTOOL_A_CHANNELS_TX_MAX,
  ETHTOOL_A_CHANNELS_OTHER_MAX,
  ETHTOOL_A_CHANNELS_COMBINED_MAX,
  ETHTOOL_A_CHANNELS_RX_COUNT,
  ETHTOOL_A_CHANNELS_TX_COUNT,
  ETHTOOL_A_CHANNELS_OTHER_COUNT,
  ETHTOOL_A_CHANNELS_COMBINED_COUNT,
  __ETHTOOL_A_CHANNELS_CNT,
  ETHTOOL_A_CHANNELS_MAX = (__ETHTOOL_A_CHANNELS_CNT - 1)
};
enum {
  ETHTOOL_A_COALESCE_UNSPEC,
  ETHTOOL_A_COALESCE_HEADER,
  ETHTOOL_A_COALESCE_RX_USECS,
  ETHTOOL_A_COALESCE_RX_MAX_FRAMES,
  ETHTOOL_A_COALESCE_RX_USECS_IRQ,
  ETHTOOL_A_COALESCE_RX_MAX_FRAMES_IRQ,
  ETHTOOL_A_COALESCE_TX_USECS,
  ETHTOOL_A_COALESCE_TX_MAX_FRAMES,
  ETHTOOL_A_COALESCE_TX_USECS_IRQ,
  ETHTOOL_A_COALESCE_TX_MAX_FRAMES_IRQ,
  ETHTOOL_A_COALESCE_STATS_BLOCK_USECS,
  ETHTOOL_A_COALESCE_USE_ADAPTIVE_RX,
  ETHTOOL_A_COALESCE_USE_ADAPTIVE_TX,
  ETHTOOL_A_COALESCE_PKT_RATE_LOW,
  ETHTOOL_A_COALESCE_RX_USECS_LOW,
  ETHTOOL_A_COALESCE_RX_MAX_FRAMES_LOW,
  ETHTOOL_A_COALESCE_TX_USECS_LOW,
  ETHTOOL_A_COALESCE_TX_MAX_FRAMES_LOW,
  ETHTOOL_A_COALESCE_PKT_RATE_HIGH,
  ETHTOOL_A_COALESCE_RX_USECS_HIGH,
  ETHTOOL_A_COALESCE_RX_MAX_FRAMES_HIGH,
  ETHTOOL_A_COALESCE_TX_USECS_HIGH,
  ETHTOOL_A_COALESCE_TX_MAX_FRAMES_HIGH,
  ETHTOOL_A_COALESCE_RATE_SAMPLE_INTERVAL,
  ETHTOOL_A_COALESCE_USE_CQE_MODE_TX,
  ETHTOOL_A_COALESCE_USE_CQE_MODE_RX,
  ETHTOOL_A_COALESCE_TX_AGGR_MAX_BYTES,
  ETHTOOL_A_COALESCE_TX_AGGR_MAX_FRAMES,
  ETHTOOL_A_COALESCE_TX_AGGR_TIME_USECS,
  __ETHTOOL_A_COALESCE_CNT,
  ETHTOOL_A_COALESCE_MAX = (__ETHTOOL_A_COALESCE_CNT - 1)
};
enum {
  ETHTOOL_A_PAUSE_UNSPEC,
  ETHTOOL_A_PAUSE_HEADER,
  ETHTOOL_A_PAUSE_AUTONEG,
  ETHTOOL_A_PAUSE_RX,
  ETHTOOL_A_PAUSE_TX,
  ETHTOOL_A_PAUSE_STATS,
  ETHTOOL_A_PAUSE_STATS_SRC,
  __ETHTOOL_A_PAUSE_CNT,
  ETHTOOL_A_PAUSE_MAX = (__ETHTOOL_A_PAUSE_CNT - 1)
};
enum {
  ETHTOOL_A_PAUSE_STAT_UNSPEC,
  ETHTOOL_A_PAUSE_STAT_PAD,
  ETHTOOL_A_PAUSE_STAT_TX_FRAMES,
  ETHTOOL_A_PAUSE_STAT_RX_FRAMES,
  __ETHTOOL_A_PAUSE_STAT_CNT,
  ETHTOOL_A_PAUSE_STAT_MAX = (__ETHTOOL_A_PAUSE_STAT_CNT - 1)
};
enum {
  ETHTOOL_A_EEE_UNSPEC,
  ETHTOOL_A_EEE_HEADER,
  ETHTOOL_A_EEE_MODES_OURS,
  ETHTOOL_A_EEE_MODES_PEER,
  ETHTOOL_A_EEE_ACTIVE,
  ETHTOOL_A_EEE_ENABLED,
  ETHTOOL_A_EEE_TX_LPI_ENABLED,
  ETHTOOL_A_EEE_TX_LPI_TIMER,
  __ETHTOOL_A_EEE_CNT,
  ETHTOOL_A_EEE_MAX = (__ETHTOOL_A_EEE_CNT - 1)
};
enum {
  ETHTOOL_A_TSINFO_UNSPEC,
  ETHTOOL_A_TSINFO_HEADER,
  ETHTOOL_A_TSINFO_TIMESTAMPING,
  ETHTOOL_A_TSINFO_TX_TYPES,
  ETHTOOL_A_TSINFO_RX_FILTERS,
  ETHTOOL_A_TSINFO_PHC_INDEX,
  __ETHTOOL_A_TSINFO_CNT,
  ETHTOOL_A_TSINFO_MAX = (__ETHTOOL_A_TSINFO_CNT - 1)
};
enum {
  ETHTOOL_A_PHC_VCLOCKS_UNSPEC,
  ETHTOOL_A_PHC_VCLOCKS_HEADER,
  ETHTOOL_A_PHC_VCLOCKS_NUM,
  ETHTOOL_A_PHC_VCLOCKS_INDEX,
  __ETHTOOL_A_PHC_VCLOCKS_CNT,
  ETHTOOL_A_PHC_VCLOCKS_MAX = (__ETHTOOL_A_PHC_VCLOCKS_CNT - 1)
};
enum {
  ETHTOOL_A_CABLE_TEST_UNSPEC,
  ETHTOOL_A_CABLE_TEST_HEADER,
  __ETHTOOL_A_CABLE_TEST_CNT,
  ETHTOOL_A_CABLE_TEST_MAX = __ETHTOOL_A_CABLE_TEST_CNT - 1
};
enum {
  ETHTOOL_A_CABLE_RESULT_CODE_UNSPEC,
  ETHTOOL_A_CABLE_RESULT_CODE_OK,
  ETHTOOL_A_CABLE_RESULT_CODE_OPEN,
  ETHTOOL_A_CABLE_RESULT_CODE_SAME_SHORT,
  ETHTOOL_A_CABLE_RESULT_CODE_CROSS_SHORT,
};
enum {
  ETHTOOL_A_CABLE_PAIR_A,
  ETHTOOL_A_CABLE_PAIR_B,
  ETHTOOL_A_CABLE_PAIR_C,
  ETHTOOL_A_CABLE_PAIR_D,
};
enum {
  ETHTOOL_A_CABLE_RESULT_UNSPEC,
  ETHTOOL_A_CABLE_RESULT_PAIR,
  ETHTOOL_A_CABLE_RESULT_CODE,
  __ETHTOOL_A_CABLE_RESULT_CNT,
  ETHTOOL_A_CABLE_RESULT_MAX = (__ETHTOOL_A_CABLE_RESULT_CNT - 1)
};
enum {
  ETHTOOL_A_CABLE_FAULT_LENGTH_UNSPEC,
  ETHTOOL_A_CABLE_FAULT_LENGTH_PAIR,
  ETHTOOL_A_CABLE_FAULT_LENGTH_CM,
  __ETHTOOL_A_CABLE_FAULT_LENGTH_CNT,
  ETHTOOL_A_CABLE_FAULT_LENGTH_MAX = (__ETHTOOL_A_CABLE_FAULT_LENGTH_CNT - 1)
};
enum {
  ETHTOOL_A_CABLE_TEST_NTF_STATUS_UNSPEC,
  ETHTOOL_A_CABLE_TEST_NTF_STATUS_STARTED,
  ETHTOOL_A_CABLE_TEST_NTF_STATUS_COMPLETED
};
enum {
  ETHTOOL_A_CABLE_NEST_UNSPEC,
  ETHTOOL_A_CABLE_NEST_RESULT,
  ETHTOOL_A_CABLE_NEST_FAULT_LENGTH,
  __ETHTOOL_A_CABLE_NEST_CNT,
  ETHTOOL_A_CABLE_NEST_MAX = (__ETHTOOL_A_CABLE_NEST_CNT - 1)
};
enum {
  ETHTOOL_A_CABLE_TEST_NTF_UNSPEC,
  ETHTOOL_A_CABLE_TEST_NTF_HEADER,
  ETHTOOL_A_CABLE_TEST_NTF_STATUS,
  ETHTOOL_A_CABLE_TEST_NTF_NEST,
  __ETHTOOL_A_CABLE_TEST_NTF_CNT,
  ETHTOOL_A_CABLE_TEST_NTF_MAX = (__ETHTOOL_A_CABLE_TEST_NTF_CNT - 1)
};
enum {
  ETHTOOL_A_CABLE_TEST_TDR_CFG_UNSPEC,
  ETHTOOL_A_CABLE_TEST_TDR_CFG_FIRST,
  ETHTOOL_A_CABLE_TEST_TDR_CFG_LAST,
  ETHTOOL_A_CABLE_TEST_TDR_CFG_STEP,
  ETHTOOL_A_CABLE_TEST_TDR_CFG_PAIR,
  __ETHTOOL_A_CABLE_TEST_TDR_CFG_CNT,
  ETHTOOL_A_CABLE_TEST_TDR_CFG_MAX = __ETHTOOL_A_CABLE_TEST_TDR_CFG_CNT - 1
};
enum {
  ETHTOOL_A_CABLE_TEST_TDR_UNSPEC,
  ETHTOOL_A_CABLE_TEST_TDR_HEADER,
  ETHTOOL_A_CABLE_TEST_TDR_CFG,
  __ETHTOOL_A_CABLE_TEST_TDR_CNT,
  ETHTOOL_A_CABLE_TEST_TDR_MAX = __ETHTOOL_A_CABLE_TEST_TDR_CNT - 1
};
enum {
  ETHTOOL_A_CABLE_AMPLITUDE_UNSPEC,
  ETHTOOL_A_CABLE_AMPLITUDE_PAIR,
  ETHTOOL_A_CABLE_AMPLITUDE_mV,
  __ETHTOOL_A_CABLE_AMPLITUDE_CNT,
  ETHTOOL_A_CABLE_AMPLITUDE_MAX = (__ETHTOOL_A_CABLE_AMPLITUDE_CNT - 1)
};
enum {
  ETHTOOL_A_CABLE_PULSE_UNSPEC,
  ETHTOOL_A_CABLE_PULSE_mV,
  __ETHTOOL_A_CABLE_PULSE_CNT,
  ETHTOOL_A_CABLE_PULSE_MAX = (__ETHTOOL_A_CABLE_PULSE_CNT - 1)
};
enum {
  ETHTOOL_A_CABLE_STEP_UNSPEC,
  ETHTOOL_A_CABLE_STEP_FIRST_DISTANCE,
  ETHTOOL_A_CABLE_STEP_LAST_DISTANCE,
  ETHTOOL_A_CABLE_STEP_STEP_DISTANCE,
  __ETHTOOL_A_CABLE_STEP_CNT,
  ETHTOOL_A_CABLE_STEP_MAX = (__ETHTOOL_A_CABLE_STEP_CNT - 1)
};
enum {
  ETHTOOL_A_CABLE_TDR_NEST_UNSPEC,
  ETHTOOL_A_CABLE_TDR_NEST_STEP,
  ETHTOOL_A_CABLE_TDR_NEST_AMPLITUDE,
  ETHTOOL_A_CABLE_TDR_NEST_PULSE,
  __ETHTOOL_A_CABLE_TDR_NEST_CNT,
  ETHTOOL_A_CABLE_TDR_NEST_MAX = (__ETHTOOL_A_CABLE_TDR_NEST_CNT - 1)
};
enum {
  ETHTOOL_A_CABLE_TEST_TDR_NTF_UNSPEC,
  ETHTOOL_A_CABLE_TEST_TDR_NTF_HEADER,
  ETHTOOL_A_CABLE_TEST_TDR_NTF_STATUS,
  ETHTOOL_A_CABLE_TEST_TDR_NTF_NEST,
  __ETHTOOL_A_CABLE_TEST_TDR_NTF_CNT,
  ETHTOOL_A_CABLE_TEST_TDR_NTF_MAX = __ETHTOOL_A_CABLE_TEST_TDR_NTF_CNT - 1
};
enum {
  ETHTOOL_UDP_TUNNEL_TYPE_VXLAN,
  ETHTOOL_UDP_TUNNEL_TYPE_GENEVE,
  ETHTOOL_UDP_TUNNEL_TYPE_VXLAN_GPE,
  __ETHTOOL_UDP_TUNNEL_TYPE_CNT
};
enum {
  ETHTOOL_A_TUNNEL_UDP_ENTRY_UNSPEC,
  ETHTOOL_A_TUNNEL_UDP_ENTRY_PORT,
  ETHTOOL_A_TUNNEL_UDP_ENTRY_TYPE,
  __ETHTOOL_A_TUNNEL_UDP_ENTRY_CNT,
  ETHTOOL_A_TUNNEL_UDP_ENTRY_MAX = (__ETHTOOL_A_TUNNEL_UDP_ENTRY_CNT - 1)
};
enum {
  ETHTOOL_A_TUNNEL_UDP_TABLE_UNSPEC,
  ETHTOOL_A_TUNNEL_UDP_TABLE_SIZE,
  ETHTOOL_A_TUNNEL_UDP_TABLE_TYPES,
  ETHTOOL_A_TUNNEL_UDP_TABLE_ENTRY,
  __ETHTOOL_A_TUNNEL_UDP_TABLE_CNT,
  ETHTOOL_A_TUNNEL_UDP_TABLE_MAX = (__ETHTOOL_A_TUNNEL_UDP_TABLE_CNT - 1)
};
enum {
  ETHTOOL_A_TUNNEL_UDP_UNSPEC,
  ETHTOOL_A_TUNNEL_UDP_TABLE,
  __ETHTOOL_A_TUNNEL_UDP_CNT,
  ETHTOOL_A_TUNNEL_UDP_MAX = (__ETHTOOL_A_TUNNEL_UDP_CNT - 1)
};
enum {
  ETHTOOL_A_TUNNEL_INFO_UNSPEC,
  ETHTOOL_A_TUNNEL_INFO_HEADER,
  ETHTOOL_A_TUNNEL_INFO_UDP_PORTS,
  __ETHTOOL_A_TUNNEL_INFO_CNT,
  ETHTOOL_A_TUNNEL_INFO_MAX = (__ETHTOOL_A_TUNNEL_INFO_CNT - 1)
};
enum {
  ETHTOOL_A_FEC_UNSPEC,
  ETHTOOL_A_FEC_HEADER,
  ETHTOOL_A_FEC_MODES,
  ETHTOOL_A_FEC_AUTO,
  ETHTOOL_A_FEC_ACTIVE,
  ETHTOOL_A_FEC_STATS,
  __ETHTOOL_A_FEC_CNT,
  ETHTOOL_A_FEC_MAX = (__ETHTOOL_A_FEC_CNT - 1)
};
enum {
  ETHTOOL_A_FEC_STAT_UNSPEC,
  ETHTOOL_A_FEC_STAT_PAD,
  ETHTOOL_A_FEC_STAT_CORRECTED,
  ETHTOOL_A_FEC_STAT_UNCORR,
  ETHTOOL_A_FEC_STAT_CORR_BITS,
  __ETHTOOL_A_FEC_STAT_CNT,
  ETHTOOL_A_FEC_STAT_MAX = (__ETHTOOL_A_FEC_STAT_CNT - 1)
};
enum {
  ETHTOOL_A_MODULE_EEPROM_UNSPEC,
  ETHTOOL_A_MODULE_EEPROM_HEADER,
  ETHTOOL_A_MODULE_EEPROM_OFFSET,
  ETHTOOL_A_MODULE_EEPROM_LENGTH,
  ETHTOOL_A_MODULE_EEPROM_PAGE,
  ETHTOOL_A_MODULE_EEPROM_BANK,
  ETHTOOL_A_MODULE_EEPROM_I2C_ADDRESS,
  ETHTOOL_A_MODULE_EEPROM_DATA,
  __ETHTOOL_A_MODULE_EEPROM_CNT,
  ETHTOOL_A_MODULE_EEPROM_MAX = (__ETHTOOL_A_MODULE_EEPROM_CNT - 1)
};
enum {
  ETHTOOL_A_STATS_UNSPEC,
  ETHTOOL_A_STATS_PAD,
  ETHTOOL_A_STATS_HEADER,
  ETHTOOL_A_STATS_GROUPS,
  ETHTOOL_A_STATS_GRP,
  ETHTOOL_A_STATS_SRC,
  __ETHTOOL_A_STATS_CNT,
  ETHTOOL_A_STATS_MAX = (__ETHTOOL_A_STATS_CNT - 1)
};
enum {
  ETHTOOL_STATS_ETH_PHY,
  ETHTOOL_STATS_ETH_MAC,
  ETHTOOL_STATS_ETH_CTRL,
  ETHTOOL_STATS_RMON,
  __ETHTOOL_STATS_CNT
};
enum {
  ETHTOOL_A_STATS_GRP_UNSPEC,
  ETHTOOL_A_STATS_GRP_PAD,
  ETHTOOL_A_STATS_GRP_ID,
  ETHTOOL_A_STATS_GRP_SS_ID,
  ETHTOOL_A_STATS_GRP_STAT,
  ETHTOOL_A_STATS_GRP_HIST_RX,
  ETHTOOL_A_STATS_GRP_HIST_TX,
  ETHTOOL_A_STATS_GRP_HIST_BKT_LOW,
  ETHTOOL_A_STATS_GRP_HIST_BKT_HI,
  ETHTOOL_A_STATS_GRP_HIST_VAL,
  __ETHTOOL_A_STATS_GRP_CNT,
  ETHTOOL_A_STATS_GRP_MAX = (__ETHTOOL_A_STATS_CNT - 1)
};
enum {
  ETHTOOL_A_STATS_ETH_PHY_5_SYM_ERR,
  __ETHTOOL_A_STATS_ETH_PHY_CNT,
  ETHTOOL_A_STATS_ETH_PHY_MAX = (__ETHTOOL_A_STATS_ETH_PHY_CNT - 1)
};
enum {
  ETHTOOL_A_STATS_ETH_MAC_2_TX_PKT,
  ETHTOOL_A_STATS_ETH_MAC_3_SINGLE_COL,
  ETHTOOL_A_STATS_ETH_MAC_4_MULTI_COL,
  ETHTOOL_A_STATS_ETH_MAC_5_RX_PKT,
  ETHTOOL_A_STATS_ETH_MAC_6_FCS_ERR,
  ETHTOOL_A_STATS_ETH_MAC_7_ALIGN_ERR,
  ETHTOOL_A_STATS_ETH_MAC_8_TX_BYTES,
  ETHTOOL_A_STATS_ETH_MAC_9_TX_DEFER,
  ETHTOOL_A_STATS_ETH_MAC_10_LATE_COL,
  ETHTOOL_A_STATS_ETH_MAC_11_XS_COL,
  ETHTOOL_A_STATS_ETH_MAC_12_TX_INT_ERR,
  ETHTOOL_A_STATS_ETH_MAC_13_CS_ERR,
  ETHTOOL_A_STATS_ETH_MAC_14_RX_BYTES,
  ETHTOOL_A_STATS_ETH_MAC_15_RX_INT_ERR,
  ETHTOOL_A_STATS_ETH_MAC_18_TX_MCAST,
  ETHTOOL_A_STATS_ETH_MAC_19_TX_BCAST,
  ETHTOOL_A_STATS_ETH_MAC_20_XS_DEFER,
  ETHTOOL_A_STATS_ETH_MAC_21_RX_MCAST,
  ETHTOOL_A_STATS_ETH_MAC_22_RX_BCAST,
  ETHTOOL_A_STATS_ETH_MAC_23_IR_LEN_ERR,
  ETHTOOL_A_STATS_ETH_MAC_24_OOR_LEN,
  ETHTOOL_A_STATS_ETH_MAC_25_TOO_LONG_ERR,
  __ETHTOOL_A_STATS_ETH_MAC_CNT,
  ETHTOOL_A_STATS_ETH_MAC_MAX = (__ETHTOOL_A_STATS_ETH_MAC_CNT - 1)
};
enum {
  ETHTOOL_A_STATS_ETH_CTRL_3_TX,
  ETHTOOL_A_STATS_ETH_CTRL_4_RX,
  ETHTOOL_A_STATS_ETH_CTRL_5_RX_UNSUP,
  __ETHTOOL_A_STATS_ETH_CTRL_CNT,
  ETHTOOL_A_STATS_ETH_CTRL_MAX = (__ETHTOOL_A_STATS_ETH_CTRL_CNT - 1)
};
enum {
  ETHTOOL_A_STATS_RMON_UNDERSIZE,
  ETHTOOL_A_STATS_RMON_OVERSIZE,
  ETHTOOL_A_STATS_RMON_FRAG,
  ETHTOOL_A_STATS_RMON_JABBER,
  __ETHTOOL_A_STATS_RMON_CNT,
  ETHTOOL_A_STATS_RMON_MAX = (__ETHTOOL_A_STATS_RMON_CNT - 1)
};
enum {
  ETHTOOL_A_MODULE_UNSPEC,
  ETHTOOL_A_MODULE_HEADER,
  ETHTOOL_A_MODULE_POWER_MODE_POLICY,
  ETHTOOL_A_MODULE_POWER_MODE,
  __ETHTOOL_A_MODULE_CNT,
  ETHTOOL_A_MODULE_MAX = (__ETHTOOL_A_MODULE_CNT - 1)
};
enum {
  ETHTOOL_A_PSE_UNSPEC,
  ETHTOOL_A_PSE_HEADER,
  ETHTOOL_A_PODL_PSE_ADMIN_STATE,
  ETHTOOL_A_PODL_PSE_ADMIN_CONTROL,
  ETHTOOL_A_PODL_PSE_PW_D_STATUS,
  __ETHTOOL_A_PSE_CNT,
  ETHTOOL_A_PSE_MAX = (__ETHTOOL_A_PSE_CNT - 1)
};
enum {
  ETHTOOL_A_RSS_UNSPEC,
  ETHTOOL_A_RSS_HEADER,
  ETHTOOL_A_RSS_CONTEXT,
  ETHTOOL_A_RSS_HFUNC,
  ETHTOOL_A_RSS_INDIR,
  ETHTOOL_A_RSS_HKEY,
  __ETHTOOL_A_RSS_CNT,
  ETHTOOL_A_RSS_MAX = (__ETHTOOL_A_RSS_CNT - 1),
};
enum {
  ETHTOOL_A_PLCA_UNSPEC,
  ETHTOOL_A_PLCA_HEADER,
  ETHTOOL_A_PLCA_VERSION,
  ETHTOOL_A_PLCA_ENABLED,
  ETHTOOL_A_PLCA_STATUS,
  ETHTOOL_A_PLCA_NODE_CNT,
  ETHTOOL_A_PLCA_NODE_ID,
  ETHTOOL_A_PLCA_TO_TMR,
  ETHTOOL_A_PLCA_BURST_CNT,
  ETHTOOL_A_PLCA_BURST_TMR,
  __ETHTOOL_A_PLCA_CNT,
  ETHTOOL_A_PLCA_MAX = (__ETHTOOL_A_PLCA_CNT - 1)
};
enum {
  ETHTOOL_A_MM_STAT_UNSPEC,
  ETHTOOL_A_MM_STAT_PAD,
  ETHTOOL_A_MM_STAT_REASSEMBLY_ERRORS,
  ETHTOOL_A_MM_STAT_SMD_ERRORS,
  ETHTOOL_A_MM_STAT_REASSEMBLY_OK,
  ETHTOOL_A_MM_STAT_RX_FRAG_COUNT,
  ETHTOOL_A_MM_STAT_TX_FRAG_COUNT,
  ETHTOOL_A_MM_STAT_HOLD_COUNT,
  __ETHTOOL_A_MM_STAT_CNT,
  ETHTOOL_A_MM_STAT_MAX = (__ETHTOOL_A_MM_STAT_CNT - 1)
};
enum {
  ETHTOOL_A_MM_UNSPEC,
  ETHTOOL_A_MM_HEADER,
  ETHTOOL_A_MM_PMAC_ENABLED,
  ETHTOOL_A_MM_TX_ENABLED,
  ETHTOOL_A_MM_TX_ACTIVE,
  ETHTOOL_A_MM_TX_MIN_FRAG_SIZE,
  ETHTOOL_A_MM_RX_MIN_FRAG_SIZE,
  ETHTOOL_A_MM_VERIFY_ENABLED,
  ETHTOOL_A_MM_VERIFY_STATUS,
  ETHTOOL_A_MM_VERIFY_TIME,
  ETHTOOL_A_MM_MAX_VERIFY_TIME,
  ETHTOOL_A_MM_STATS,
  __ETHTOOL_A_MM_CNT,
  ETHTOOL_A_MM_MAX = (__ETHTOOL_A_MM_CNT - 1)
};
#define ETHTOOL_GENL_NAME "ethtool"
#define ETHTOOL_GENL_VERSION 1
#define ETHTOOL_MCGRP_MONITOR_NAME "monitor"
#endif
