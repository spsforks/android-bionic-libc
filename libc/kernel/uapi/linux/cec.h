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
#ifndef _CEC_UAPI_H
#define _CEC_UAPI_H
#include <linux/types.h>
#include <linux/string.h>
#define CEC_MAX_MSG_SIZE 16
struct cec_msg {
  __u64 tx_ts;
  __u64 rx_ts;
  __u32 len;
  __u32 timeout;
  __u32 sequence;
  __u32 flags;
  __u8 msg[CEC_MAX_MSG_SIZE];
  __u8 reply;
  __u8 rx_status;
  __u8 tx_status;
  __u8 tx_arb_lost_cnt;
  __u8 tx_nack_cnt;
  __u8 tx_low_drive_cnt;
  __u8 tx_error_cnt;
};
#define CEC_MSG_FL_REPLY_TO_FOLLOWERS (1 << 0)
#define CEC_MSG_FL_RAW (1 << 1)
#define CEC_TX_STATUS_OK (1 << 0)
#define CEC_TX_STATUS_ARB_LOST (1 << 1)
#define CEC_TX_STATUS_NACK (1 << 2)
#define CEC_TX_STATUS_LOW_DRIVE (1 << 3)
#define CEC_TX_STATUS_ERROR (1 << 4)
#define CEC_TX_STATUS_MAX_RETRIES (1 << 5)
#define CEC_TX_STATUS_ABORTED (1 << 6)
#define CEC_TX_STATUS_TIMEOUT (1 << 7)
#define CEC_RX_STATUS_OK (1 << 0)
#define CEC_RX_STATUS_TIMEOUT (1 << 1)
#define CEC_RX_STATUS_FEATURE_ABORT (1 << 2)
#define CEC_RX_STATUS_ABORTED (1 << 3)
#define CEC_LOG_ADDR_INVALID 0xff
#define CEC_PHYS_ADDR_INVALID 0xffff
#define CEC_MAX_LOG_ADDRS 4
#define CEC_LOG_ADDR_TV 0
#define CEC_LOG_ADDR_RECORD_1 1
#define CEC_LOG_ADDR_RECORD_2 2
#define CEC_LOG_ADDR_TUNER_1 3
#define CEC_LOG_ADDR_PLAYBACK_1 4
#define CEC_LOG_ADDR_AUDIOSYSTEM 5
#define CEC_LOG_ADDR_TUNER_2 6
#define CEC_LOG_ADDR_TUNER_3 7
#define CEC_LOG_ADDR_PLAYBACK_2 8
#define CEC_LOG_ADDR_RECORD_3 9
#define CEC_LOG_ADDR_TUNER_4 10
#define CEC_LOG_ADDR_PLAYBACK_3 11
#define CEC_LOG_ADDR_BACKUP_1 12
#define CEC_LOG_ADDR_BACKUP_2 13
#define CEC_LOG_ADDR_SPECIFIC 14
#define CEC_LOG_ADDR_UNREGISTERED 15
#define CEC_LOG_ADDR_BROADCAST 15
#define CEC_LOG_ADDR_TYPE_TV 0
#define CEC_LOG_ADDR_TYPE_RECORD 1
#define CEC_LOG_ADDR_TYPE_TUNER 2
#define CEC_LOG_ADDR_TYPE_PLAYBACK 3
#define CEC_LOG_ADDR_TYPE_AUDIOSYSTEM 4
#define CEC_LOG_ADDR_TYPE_SPECIFIC 5
#define CEC_LOG_ADDR_TYPE_UNREGISTERED 6
#define CEC_LOG_ADDR_MASK_TV (1 << CEC_LOG_ADDR_TV)
#define CEC_LOG_ADDR_MASK_RECORD ((1 << CEC_LOG_ADDR_RECORD_1) | (1 << CEC_LOG_ADDR_RECORD_2) | (1 << CEC_LOG_ADDR_RECORD_3))
#define CEC_LOG_ADDR_MASK_TUNER ((1 << CEC_LOG_ADDR_TUNER_1) | (1 << CEC_LOG_ADDR_TUNER_2) | (1 << CEC_LOG_ADDR_TUNER_3) | (1 << CEC_LOG_ADDR_TUNER_4))
#define CEC_LOG_ADDR_MASK_PLAYBACK ((1 << CEC_LOG_ADDR_PLAYBACK_1) | (1 << CEC_LOG_ADDR_PLAYBACK_2) | (1 << CEC_LOG_ADDR_PLAYBACK_3))
#define CEC_LOG_ADDR_MASK_AUDIOSYSTEM (1 << CEC_LOG_ADDR_AUDIOSYSTEM)
#define CEC_LOG_ADDR_MASK_BACKUP ((1 << CEC_LOG_ADDR_BACKUP_1) | (1 << CEC_LOG_ADDR_BACKUP_2))
#define CEC_LOG_ADDR_MASK_SPECIFIC (1 << CEC_LOG_ADDR_SPECIFIC)
#define CEC_LOG_ADDR_MASK_UNREGISTERED (1 << CEC_LOG_ADDR_UNREGISTERED)
#define CEC_VENDOR_ID_NONE 0xffffffff
#define CEC_MODE_NO_INITIATOR (0x0 << 0)
#define CEC_MODE_INITIATOR (0x1 << 0)
#define CEC_MODE_EXCL_INITIATOR (0x2 << 0)
#define CEC_MODE_INITIATOR_MSK 0x0f
#define CEC_MODE_NO_FOLLOWER (0x0 << 4)
#define CEC_MODE_FOLLOWER (0x1 << 4)
#define CEC_MODE_EXCL_FOLLOWER (0x2 << 4)
#define CEC_MODE_EXCL_FOLLOWER_PASSTHRU (0x3 << 4)
#define CEC_MODE_MONITOR_PIN (0xd << 4)
#define CEC_MODE_MONITOR (0xe << 4)
#define CEC_MODE_MONITOR_ALL (0xf << 4)
#define CEC_MODE_FOLLOWER_MSK 0xf0
#define CEC_CAP_PHYS_ADDR (1 << 0)
#define CEC_CAP_LOG_ADDRS (1 << 1)
#define CEC_CAP_TRANSMIT (1 << 2)
#define CEC_CAP_PASSTHROUGH (1 << 3)
#define CEC_CAP_RC (1 << 4)
#define CEC_CAP_MONITOR_ALL (1 << 5)
#define CEC_CAP_NEEDS_HPD (1 << 6)
#define CEC_CAP_MONITOR_PIN (1 << 7)
#define CEC_CAP_CONNECTOR_INFO (1 << 8)
struct cec_caps {
  char driver[32];
  char name[32];
  __u32 available_log_addrs;
  __u32 capabilities;
  __u32 version;
};
struct cec_log_addrs {
  __u8 log_addr[CEC_MAX_LOG_ADDRS];
  __u16 log_addr_mask;
  __u8 cec_version;
  __u8 num_log_addrs;
  __u32 vendor_id;
  __u32 flags;
  char osd_name[15];
  __u8 primary_device_type[CEC_MAX_LOG_ADDRS];
  __u8 log_addr_type[CEC_MAX_LOG_ADDRS];
  __u8 all_device_types[CEC_MAX_LOG_ADDRS];
  __u8 features[CEC_MAX_LOG_ADDRS][12];
};
#define CEC_LOG_ADDRS_FL_ALLOW_UNREG_FALLBACK (1 << 0)
#define CEC_LOG_ADDRS_FL_ALLOW_RC_PASSTHRU (1 << 1)
#define CEC_LOG_ADDRS_FL_CDC_ONLY (1 << 2)
struct cec_drm_connector_info {
  __u32 card_no;
  __u32 connector_id;
};
#define CEC_CONNECTOR_TYPE_NO_CONNECTOR 0
#define CEC_CONNECTOR_TYPE_DRM 1
struct cec_connector_info {
  __u32 type;
  union {
    struct cec_drm_connector_info drm;
    __u32 raw[16];
  };
};
#define CEC_EVENT_STATE_CHANGE 1
#define CEC_EVENT_LOST_MSGS 2
#define CEC_EVENT_PIN_CEC_LOW 3
#define CEC_EVENT_PIN_CEC_HIGH 4
#define CEC_EVENT_PIN_HPD_LOW 5
#define CEC_EVENT_PIN_HPD_HIGH 6
#define CEC_EVENT_PIN_5V_LOW 7
#define CEC_EVENT_PIN_5V_HIGH 8
#define CEC_EVENT_FL_INITIAL_STATE (1 << 0)
#define CEC_EVENT_FL_DROPPED_EVENTS (1 << 1)
struct cec_event_state_change {
  __u16 phys_addr;
  __u16 log_addr_mask;
  __u16 have_conn_info;
};
struct cec_event_lost_msgs {
  __u32 lost_msgs;
};
struct cec_event {
  __u64 ts;
  __u32 event;
  __u32 flags;
  union {
    struct cec_event_state_change state_change;
    struct cec_event_lost_msgs lost_msgs;
    __u32 raw[16];
  };
};
#define CEC_ADAP_G_CAPS _IOWR('a', 0, struct cec_caps)
#define CEC_ADAP_G_PHYS_ADDR _IOR('a', 1, __u16)
#define CEC_ADAP_S_PHYS_ADDR _IOW('a', 2, __u16)
#define CEC_ADAP_G_LOG_ADDRS _IOR('a', 3, struct cec_log_addrs)
#define CEC_ADAP_S_LOG_ADDRS _IOWR('a', 4, struct cec_log_addrs)
#define CEC_TRANSMIT _IOWR('a', 5, struct cec_msg)
#define CEC_RECEIVE _IOWR('a', 6, struct cec_msg)
#define CEC_DQEVENT _IOWR('a', 7, struct cec_event)
#define CEC_G_MODE _IOR('a', 8, __u32)
#define CEC_S_MODE _IOW('a', 9, __u32)
#define CEC_ADAP_G_CONNECTOR_INFO _IOR('a', 10, struct cec_connector_info)
#define CEC_MSG_ACTIVE_SOURCE 0x82
#define CEC_MSG_IMAGE_VIEW_ON 0x04
#define CEC_MSG_TEXT_VIEW_ON 0x0d
#define CEC_MSG_INACTIVE_SOURCE 0x9d
#define CEC_MSG_REQUEST_ACTIVE_SOURCE 0x85
#define CEC_MSG_ROUTING_CHANGE 0x80
#define CEC_MSG_ROUTING_INFORMATION 0x81
#define CEC_MSG_SET_STREAM_PATH 0x86
#define CEC_MSG_STANDBY 0x36
#define CEC_MSG_RECORD_OFF 0x0b
#define CEC_MSG_RECORD_ON 0x09
#define CEC_OP_RECORD_SRC_OWN 1
#define CEC_OP_RECORD_SRC_DIGITAL 2
#define CEC_OP_RECORD_SRC_ANALOG 3
#define CEC_OP_RECORD_SRC_EXT_PLUG 4
#define CEC_OP_RECORD_SRC_EXT_PHYS_ADDR 5
#define CEC_OP_SERVICE_ID_METHOD_BY_DIG_ID 0
#define CEC_OP_SERVICE_ID_METHOD_BY_CHANNEL 1
#define CEC_OP_DIG_SERVICE_BCAST_SYSTEM_ARIB_GEN 0x00
#define CEC_OP_DIG_SERVICE_BCAST_SYSTEM_ATSC_GEN 0x01
#define CEC_OP_DIG_SERVICE_BCAST_SYSTEM_DVB_GEN 0x02
#define CEC_OP_DIG_SERVICE_BCAST_SYSTEM_ARIB_BS 0x08
#define CEC_OP_DIG_SERVICE_BCAST_SYSTEM_ARIB_CS 0x09
#define CEC_OP_DIG_SERVICE_BCAST_SYSTEM_ARIB_T 0x0a
#define CEC_OP_DIG_SERVICE_BCAST_SYSTEM_ATSC_CABLE 0x10
#define CEC_OP_DIG_SERVICE_BCAST_SYSTEM_ATSC_SAT 0x11
#define CEC_OP_DIG_SERVICE_BCAST_SYSTEM_ATSC_T 0x12
#define CEC_OP_DIG_SERVICE_BCAST_SYSTEM_DVB_C 0x18
#define CEC_OP_DIG_SERVICE_BCAST_SYSTEM_DVB_S 0x19
#define CEC_OP_DIG_SERVICE_BCAST_SYSTEM_DVB_S2 0x1a
#define CEC_OP_DIG_SERVICE_BCAST_SYSTEM_DVB_T 0x1b
#define CEC_OP_ANA_BCAST_TYPE_CABLE 0
#define CEC_OP_ANA_BCAST_TYPE_SATELLITE 1
#define CEC_OP_ANA_BCAST_TYPE_TERRESTRIAL 2
#define CEC_OP_BCAST_SYSTEM_PAL_BG 0x00
#define CEC_OP_BCAST_SYSTEM_SECAM_LQ 0x01
#define CEC_OP_BCAST_SYSTEM_PAL_M 0x02
#define CEC_OP_BCAST_SYSTEM_NTSC_M 0x03
#define CEC_OP_BCAST_SYSTEM_PAL_I 0x04
#define CEC_OP_BCAST_SYSTEM_SECAM_DK 0x05
#define CEC_OP_BCAST_SYSTEM_SECAM_BG 0x06
#define CEC_OP_BCAST_SYSTEM_SECAM_L 0x07
#define CEC_OP_BCAST_SYSTEM_PAL_DK 0x08
#define CEC_OP_BCAST_SYSTEM_OTHER 0x1f
#define CEC_OP_CHANNEL_NUMBER_FMT_1_PART 0x01
#define CEC_OP_CHANNEL_NUMBER_FMT_2_PART 0x02
#define CEC_MSG_RECORD_STATUS 0x0a
#define CEC_OP_RECORD_STATUS_CUR_SRC 0x01
#define CEC_OP_RECORD_STATUS_DIG_SERVICE 0x02
#define CEC_OP_RECORD_STATUS_ANA_SERVICE 0x03
#define CEC_OP_RECORD_STATUS_EXT_INPUT 0x04
#define CEC_OP_RECORD_STATUS_NO_DIG_SERVICE 0x05
#define CEC_OP_RECORD_STATUS_NO_ANA_SERVICE 0x06
#define CEC_OP_RECORD_STATUS_NO_SERVICE 0x07
#define CEC_OP_RECORD_STATUS_INVALID_EXT_PLUG 0x09
#define CEC_OP_RECORD_STATUS_INVALID_EXT_PHYS_ADDR 0x0a
#define CEC_OP_RECORD_STATUS_UNSUP_CA 0x0b
#define CEC_OP_RECORD_STATUS_NO_CA_ENTITLEMENTS 0x0c
#define CEC_OP_RECORD_STATUS_CANT_COPY_SRC 0x0d
#define CEC_OP_RECORD_STATUS_NO_MORE_COPIES 0x0e
#define CEC_OP_RECORD_STATUS_NO_MEDIA 0x10
#define CEC_OP_RECORD_STATUS_PLAYING 0x11
#define CEC_OP_RECORD_STATUS_ALREADY_RECORDING 0x12
#define CEC_OP_RECORD_STATUS_MEDIA_PROT 0x13
#define CEC_OP_RECORD_STATUS_NO_SIGNAL 0x14
#define CEC_OP_RECORD_STATUS_MEDIA_PROBLEM 0x15
#define CEC_OP_RECORD_STATUS_NO_SPACE 0x16
#define CEC_OP_RECORD_STATUS_PARENTAL_LOCK 0x17
#define CEC_OP_RECORD_STATUS_TERMINATED_OK 0x1a
#define CEC_OP_RECORD_STATUS_ALREADY_TERM 0x1b
#define CEC_OP_RECORD_STATUS_OTHER 0x1f
#define CEC_MSG_RECORD_TV_SCREEN 0x0f
#define CEC_MSG_CLEAR_ANALOGUE_TIMER 0x33
#define CEC_OP_REC_SEQ_SUNDAY 0x01
#define CEC_OP_REC_SEQ_MONDAY 0x02
#define CEC_OP_REC_SEQ_TUESDAY 0x04
#define CEC_OP_REC_SEQ_WEDNESDAY 0x08
#define CEC_OP_REC_SEQ_THURSDAY 0x10
#define CEC_OP_REC_SEQ_FRIDAY 0x20
#define CEC_OP_REC_SEQ_SATURDAY 0x40
#define CEC_OP_REC_SEQ_ONCE_ONLY 0x00
#define CEC_MSG_CLEAR_DIGITAL_TIMER 0x99
#define CEC_MSG_CLEAR_EXT_TIMER 0xa1
#define CEC_OP_EXT_SRC_PLUG 0x04
#define CEC_OP_EXT_SRC_PHYS_ADDR 0x05
#define CEC_MSG_SET_ANALOGUE_TIMER 0x34
#define CEC_MSG_SET_DIGITAL_TIMER 0x97
#define CEC_MSG_SET_EXT_TIMER 0xa2
#define CEC_MSG_SET_TIMER_PROGRAM_TITLE 0x67
#define CEC_MSG_TIMER_CLEARED_STATUS 0x43
#define CEC_OP_TIMER_CLR_STAT_RECORDING 0x00
#define CEC_OP_TIMER_CLR_STAT_NO_MATCHING 0x01
#define CEC_OP_TIMER_CLR_STAT_NO_INFO 0x02
#define CEC_OP_TIMER_CLR_STAT_CLEARED 0x80
#define CEC_MSG_TIMER_STATUS 0x35
#define CEC_OP_TIMER_OVERLAP_WARNING_NO_OVERLAP 0
#define CEC_OP_TIMER_OVERLAP_WARNING_OVERLAP 1
#define CEC_OP_MEDIA_INFO_UNPROT_MEDIA 0
#define CEC_OP_MEDIA_INFO_PROT_MEDIA 1
#define CEC_OP_MEDIA_INFO_NO_MEDIA 2
#define CEC_OP_PROG_IND_NOT_PROGRAMMED 0
#define CEC_OP_PROG_IND_PROGRAMMED 1
#define CEC_OP_PROG_INFO_ENOUGH_SPACE 0x08
#define CEC_OP_PROG_INFO_NOT_ENOUGH_SPACE 0x09
#define CEC_OP_PROG_INFO_MIGHT_NOT_BE_ENOUGH_SPACE 0x0b
#define CEC_OP_PROG_INFO_NONE_AVAILABLE 0x0a
#define CEC_OP_PROG_ERROR_NO_FREE_TIMER 0x01
#define CEC_OP_PROG_ERROR_DATE_OUT_OF_RANGE 0x02
#define CEC_OP_PROG_ERROR_REC_SEQ_ERROR 0x03
#define CEC_OP_PROG_ERROR_INV_EXT_PLUG 0x04
#define CEC_OP_PROG_ERROR_INV_EXT_PHYS_ADDR 0x05
#define CEC_OP_PROG_ERROR_CA_UNSUPP 0x06
#define CEC_OP_PROG_ERROR_INSUF_CA_ENTITLEMENTS 0x07
#define CEC_OP_PROG_ERROR_RESOLUTION_UNSUPP 0x08
#define CEC_OP_PROG_ERROR_PARENTAL_LOCK 0x09
#define CEC_OP_PROG_ERROR_CLOCK_FAILURE 0x0a
#define CEC_OP_PROG_ERROR_DUPLICATE 0x0e
#define CEC_MSG_CEC_VERSION 0x9e
#define CEC_OP_CEC_VERSION_1_3A 4
#define CEC_OP_CEC_VERSION_1_4 5
#define CEC_OP_CEC_VERSION_2_0 6
#define CEC_MSG_GET_CEC_VERSION 0x9f
#define CEC_MSG_GIVE_PHYSICAL_ADDR 0x83
#define CEC_MSG_GET_MENU_LANGUAGE 0x91
#define CEC_MSG_REPORT_PHYSICAL_ADDR 0x84
#define CEC_OP_PRIM_DEVTYPE_TV 0
#define CEC_OP_PRIM_DEVTYPE_RECORD 1
#define CEC_OP_PRIM_DEVTYPE_TUNER 3
#define CEC_OP_PRIM_DEVTYPE_PLAYBACK 4
#define CEC_OP_PRIM_DEVTYPE_AUDIOSYSTEM 5
#define CEC_OP_PRIM_DEVTYPE_SWITCH 6
#define CEC_OP_PRIM_DEVTYPE_PROCESSOR 7
#define CEC_MSG_SET_MENU_LANGUAGE 0x32
#define CEC_MSG_REPORT_FEATURES 0xa6
#define CEC_OP_ALL_DEVTYPE_TV 0x80
#define CEC_OP_ALL_DEVTYPE_RECORD 0x40
#define CEC_OP_ALL_DEVTYPE_TUNER 0x20
#define CEC_OP_ALL_DEVTYPE_PLAYBACK 0x10
#define CEC_OP_ALL_DEVTYPE_AUDIOSYSTEM 0x08
#define CEC_OP_ALL_DEVTYPE_SWITCH 0x04
#define CEC_OP_FEAT_EXT 0x80
#define CEC_OP_FEAT_RC_TV_PROFILE_NONE 0x00
#define CEC_OP_FEAT_RC_TV_PROFILE_1 0x02
#define CEC_OP_FEAT_RC_TV_PROFILE_2 0x06
#define CEC_OP_FEAT_RC_TV_PROFILE_3 0x0a
#define CEC_OP_FEAT_RC_TV_PROFILE_4 0x0e
#define CEC_OP_FEAT_RC_SRC_HAS_DEV_ROOT_MENU 0x50
#define CEC_OP_FEAT_RC_SRC_HAS_DEV_SETUP_MENU 0x48
#define CEC_OP_FEAT_RC_SRC_HAS_CONTENTS_MENU 0x44
#define CEC_OP_FEAT_RC_SRC_HAS_MEDIA_TOP_MENU 0x42
#define CEC_OP_FEAT_RC_SRC_HAS_MEDIA_CONTEXT_MENU 0x41
#define CEC_OP_FEAT_DEV_HAS_RECORD_TV_SCREEN 0x40
#define CEC_OP_FEAT_DEV_HAS_SET_OSD_STRING 0x20
#define CEC_OP_FEAT_DEV_HAS_DECK_CONTROL 0x10
#define CEC_OP_FEAT_DEV_HAS_SET_AUDIO_RATE 0x08
#define CEC_OP_FEAT_DEV_SINK_HAS_ARC_TX 0x04
#define CEC_OP_FEAT_DEV_SOURCE_HAS_ARC_RX 0x02
#define CEC_OP_FEAT_DEV_HAS_SET_AUDIO_VOLUME_LEVEL 0x01
#define CEC_MSG_GIVE_FEATURES 0xa5
#define CEC_MSG_DECK_CONTROL 0x42
#define CEC_OP_DECK_CTL_MODE_SKIP_FWD 1
#define CEC_OP_DECK_CTL_MODE_SKIP_REV 2
#define CEC_OP_DECK_CTL_MODE_STOP 3
#define CEC_OP_DECK_CTL_MODE_EJECT 4
#define CEC_MSG_DECK_STATUS 0x1b
#define CEC_OP_DECK_INFO_PLAY 0x11
#define CEC_OP_DECK_INFO_RECORD 0x12
#define CEC_OP_DECK_INFO_PLAY_REV 0x13
#define CEC_OP_DECK_INFO_STILL 0x14
#define CEC_OP_DECK_INFO_SLOW 0x15
#define CEC_OP_DECK_INFO_SLOW_REV 0x16
#define CEC_OP_DECK_INFO_FAST_FWD 0x17
#define CEC_OP_DECK_INFO_FAST_REV 0x18
#define CEC_OP_DECK_INFO_NO_MEDIA 0x19
#define CEC_OP_DECK_INFO_STOP 0x1a
#define CEC_OP_DECK_INFO_SKIP_FWD 0x1b
#define CEC_OP_DECK_INFO_SKIP_REV 0x1c
#define CEC_OP_DECK_INFO_INDEX_SEARCH_FWD 0x1d
#define CEC_OP_DECK_INFO_INDEX_SEARCH_REV 0x1e
#define CEC_OP_DECK_INFO_OTHER 0x1f
#define CEC_MSG_GIVE_DECK_STATUS 0x1a
#define CEC_OP_STATUS_REQ_ON 1
#define CEC_OP_STATUS_REQ_OFF 2
#define CEC_OP_STATUS_REQ_ONCE 3
#define CEC_MSG_PLAY 0x41
#define CEC_OP_PLAY_MODE_PLAY_FWD 0x24
#define CEC_OP_PLAY_MODE_PLAY_REV 0x20
#define CEC_OP_PLAY_MODE_PLAY_STILL 0x25
#define CEC_OP_PLAY_MODE_PLAY_FAST_FWD_MIN 0x05
#define CEC_OP_PLAY_MODE_PLAY_FAST_FWD_MED 0x06
#define CEC_OP_PLAY_MODE_PLAY_FAST_FWD_MAX 0x07
#define CEC_OP_PLAY_MODE_PLAY_FAST_REV_MIN 0x09
#define CEC_OP_PLAY_MODE_PLAY_FAST_REV_MED 0x0a
#define CEC_OP_PLAY_MODE_PLAY_FAST_REV_MAX 0x0b
#define CEC_OP_PLAY_MODE_PLAY_SLOW_FWD_MIN 0x15
#define CEC_OP_PLAY_MODE_PLAY_SLOW_FWD_MED 0x16
#define CEC_OP_PLAY_MODE_PLAY_SLOW_FWD_MAX 0x17
#define CEC_OP_PLAY_MODE_PLAY_SLOW_REV_MIN 0x19
#define CEC_OP_PLAY_MODE_PLAY_SLOW_REV_MED 0x1a
#define CEC_OP_PLAY_MODE_PLAY_SLOW_REV_MAX 0x1b
#define CEC_MSG_GIVE_TUNER_DEVICE_STATUS 0x08
#define CEC_MSG_SELECT_ANALOGUE_SERVICE 0x92
#define CEC_MSG_SELECT_DIGITAL_SERVICE 0x93
#define CEC_MSG_TUNER_DEVICE_STATUS 0x07
#define CEC_OP_REC_FLAG_NOT_USED 0
#define CEC_OP_REC_FLAG_USED 1
#define CEC_OP_TUNER_DISPLAY_INFO_DIGITAL 0
#define CEC_OP_TUNER_DISPLAY_INFO_NONE 1
#define CEC_OP_TUNER_DISPLAY_INFO_ANALOGUE 2
#define CEC_MSG_TUNER_STEP_DECREMENT 0x06
#define CEC_MSG_TUNER_STEP_INCREMENT 0x05
#define CEC_MSG_DEVICE_VENDOR_ID 0x87
#define CEC_MSG_GIVE_DEVICE_VENDOR_ID 0x8c
#define CEC_MSG_VENDOR_COMMAND 0x89
#define CEC_MSG_VENDOR_COMMAND_WITH_ID 0xa0
#define CEC_MSG_VENDOR_REMOTE_BUTTON_DOWN 0x8a
#define CEC_MSG_VENDOR_REMOTE_BUTTON_UP 0x8b
#define CEC_MSG_SET_OSD_STRING 0x64
#define CEC_OP_DISP_CTL_DEFAULT 0x00
#define CEC_OP_DISP_CTL_UNTIL_CLEARED 0x40
#define CEC_OP_DISP_CTL_CLEAR 0x80
#define CEC_MSG_GIVE_OSD_NAME 0x46
#define CEC_MSG_SET_OSD_NAME 0x47
#define CEC_MSG_MENU_REQUEST 0x8d
#define CEC_OP_MENU_REQUEST_ACTIVATE 0x00
#define CEC_OP_MENU_REQUEST_DEACTIVATE 0x01
#define CEC_OP_MENU_REQUEST_QUERY 0x02
#define CEC_MSG_MENU_STATUS 0x8e
#define CEC_OP_MENU_STATE_ACTIVATED 0x00
#define CEC_OP_MENU_STATE_DEACTIVATED 0x01
#define CEC_MSG_USER_CONTROL_PRESSED 0x44
#define CEC_OP_UI_CMD_SELECT 0x00
#define CEC_OP_UI_CMD_UP 0x01
#define CEC_OP_UI_CMD_DOWN 0x02
#define CEC_OP_UI_CMD_LEFT 0x03
#define CEC_OP_UI_CMD_RIGHT 0x04
#define CEC_OP_UI_CMD_RIGHT_UP 0x05
#define CEC_OP_UI_CMD_RIGHT_DOWN 0x06
#define CEC_OP_UI_CMD_LEFT_UP 0x07
#define CEC_OP_UI_CMD_LEFT_DOWN 0x08
#define CEC_OP_UI_CMD_DEVICE_ROOT_MENU 0x09
#define CEC_OP_UI_CMD_DEVICE_SETUP_MENU 0x0a
#define CEC_OP_UI_CMD_CONTENTS_MENU 0x0b
#define CEC_OP_UI_CMD_FAVORITE_MENU 0x0c
#define CEC_OP_UI_CMD_BACK 0x0d
#define CEC_OP_UI_CMD_MEDIA_TOP_MENU 0x10
#define CEC_OP_UI_CMD_MEDIA_CONTEXT_SENSITIVE_MENU 0x11
#define CEC_OP_UI_CMD_NUMBER_ENTRY_MODE 0x1d
#define CEC_OP_UI_CMD_NUMBER_11 0x1e
#define CEC_OP_UI_CMD_NUMBER_12 0x1f
#define CEC_OP_UI_CMD_NUMBER_0_OR_NUMBER_10 0x20
#define CEC_OP_UI_CMD_NUMBER_1 0x21
#define CEC_OP_UI_CMD_NUMBER_2 0x22
#define CEC_OP_UI_CMD_NUMBER_3 0x23
#define CEC_OP_UI_CMD_NUMBER_4 0x24
#define CEC_OP_UI_CMD_NUMBER_5 0x25
#define CEC_OP_UI_CMD_NUMBER_6 0x26
#define CEC_OP_UI_CMD_NUMBER_7 0x27
#define CEC_OP_UI_CMD_NUMBER_8 0x28
#define CEC_OP_UI_CMD_NUMBER_9 0x29
#define CEC_OP_UI_CMD_DOT 0x2a
#define CEC_OP_UI_CMD_ENTER 0x2b
#define CEC_OP_UI_CMD_CLEAR 0x2c
#define CEC_OP_UI_CMD_NEXT_FAVORITE 0x2f
#define CEC_OP_UI_CMD_CHANNEL_UP 0x30
#define CEC_OP_UI_CMD_CHANNEL_DOWN 0x31
#define CEC_OP_UI_CMD_PREVIOUS_CHANNEL 0x32
#define CEC_OP_UI_CMD_SOUND_SELECT 0x33
#define CEC_OP_UI_CMD_INPUT_SELECT 0x34
#define CEC_OP_UI_CMD_DISPLAY_INFORMATION 0x35
#define CEC_OP_UI_CMD_HELP 0x36
#define CEC_OP_UI_CMD_PAGE_UP 0x37
#define CEC_OP_UI_CMD_PAGE_DOWN 0x38
#define CEC_OP_UI_CMD_POWER 0x40
#define CEC_OP_UI_CMD_VOLUME_UP 0x41
#define CEC_OP_UI_CMD_VOLUME_DOWN 0x42
#define CEC_OP_UI_CMD_MUTE 0x43
#define CEC_OP_UI_CMD_PLAY 0x44
#define CEC_OP_UI_CMD_STOP 0x45
#define CEC_OP_UI_CMD_PAUSE 0x46
#define CEC_OP_UI_CMD_RECORD 0x47
#define CEC_OP_UI_CMD_REWIND 0x48
#define CEC_OP_UI_CMD_FAST_FORWARD 0x49
#define CEC_OP_UI_CMD_EJECT 0x4a
#define CEC_OP_UI_CMD_SKIP_FORWARD 0x4b
#define CEC_OP_UI_CMD_SKIP_BACKWARD 0x4c
#define CEC_OP_UI_CMD_STOP_RECORD 0x4d
#define CEC_OP_UI_CMD_PAUSE_RECORD 0x4e
#define CEC_OP_UI_CMD_ANGLE 0x50
#define CEC_OP_UI_CMD_SUB_PICTURE 0x51
#define CEC_OP_UI_CMD_VIDEO_ON_DEMAND 0x52
#define CEC_OP_UI_CMD_ELECTRONIC_PROGRAM_GUIDE 0x53
#define CEC_OP_UI_CMD_TIMER_PROGRAMMING 0x54
#define CEC_OP_UI_CMD_INITIAL_CONFIGURATION 0x55
#define CEC_OP_UI_CMD_SELECT_BROADCAST_TYPE 0x56
#define CEC_OP_UI_CMD_SELECT_SOUND_PRESENTATION 0x57
#define CEC_OP_UI_CMD_AUDIO_DESCRIPTION 0x58
#define CEC_OP_UI_CMD_INTERNET 0x59
#define CEC_OP_UI_CMD_3D_MODE 0x5a
#define CEC_OP_UI_CMD_PLAY_FUNCTION 0x60
#define CEC_OP_UI_CMD_PAUSE_PLAY_FUNCTION 0x61
#define CEC_OP_UI_CMD_RECORD_FUNCTION 0x62
#define CEC_OP_UI_CMD_PAUSE_RECORD_FUNCTION 0x63
#define CEC_OP_UI_CMD_STOP_FUNCTION 0x64
#define CEC_OP_UI_CMD_MUTE_FUNCTION 0x65
#define CEC_OP_UI_CMD_RESTORE_VOLUME_FUNCTION 0x66
#define CEC_OP_UI_CMD_TUNE_FUNCTION 0x67
#define CEC_OP_UI_CMD_SELECT_MEDIA_FUNCTION 0x68
#define CEC_OP_UI_CMD_SELECT_AV_INPUT_FUNCTION 0x69
#define CEC_OP_UI_CMD_SELECT_AUDIO_INPUT_FUNCTION 0x6a
#define CEC_OP_UI_CMD_POWER_TOGGLE_FUNCTION 0x6b
#define CEC_OP_UI_CMD_POWER_OFF_FUNCTION 0x6c
#define CEC_OP_UI_CMD_POWER_ON_FUNCTION 0x6d
#define CEC_OP_UI_CMD_F1_BLUE 0x71
#define CEC_OP_UI_CMD_F2_RED 0x72
#define CEC_OP_UI_CMD_F3_GREEN 0x73
#define CEC_OP_UI_CMD_F4_YELLOW 0x74
#define CEC_OP_UI_CMD_F5 0x75
#define CEC_OP_UI_CMD_DATA 0x76
#define CEC_OP_UI_BCAST_TYPE_TOGGLE_ALL 0x00
#define CEC_OP_UI_BCAST_TYPE_TOGGLE_DIG_ANA 0x01
#define CEC_OP_UI_BCAST_TYPE_ANALOGUE 0x10
#define CEC_OP_UI_BCAST_TYPE_ANALOGUE_T 0x20
#define CEC_OP_UI_BCAST_TYPE_ANALOGUE_CABLE 0x30
#define CEC_OP_UI_BCAST_TYPE_ANALOGUE_SAT 0x40
#define CEC_OP_UI_BCAST_TYPE_DIGITAL 0x50
#define CEC_OP_UI_BCAST_TYPE_DIGITAL_T 0x60
#define CEC_OP_UI_BCAST_TYPE_DIGITAL_CABLE 0x70
#define CEC_OP_UI_BCAST_TYPE_DIGITAL_SAT 0x80
#define CEC_OP_UI_BCAST_TYPE_DIGITAL_COM_SAT 0x90
#define CEC_OP_UI_BCAST_TYPE_DIGITAL_COM_SAT2 0x91
#define CEC_OP_UI_BCAST_TYPE_IP 0xa0
#define CEC_OP_UI_SND_PRES_CTL_DUAL_MONO 0x10
#define CEC_OP_UI_SND_PRES_CTL_KARAOKE 0x20
#define CEC_OP_UI_SND_PRES_CTL_DOWNMIX 0x80
#define CEC_OP_UI_SND_PRES_CTL_REVERB 0x90
#define CEC_OP_UI_SND_PRES_CTL_EQUALIZER 0xa0
#define CEC_OP_UI_SND_PRES_CTL_BASS_UP 0xb1
#define CEC_OP_UI_SND_PRES_CTL_BASS_NEUTRAL 0xb2
#define CEC_OP_UI_SND_PRES_CTL_BASS_DOWN 0xb3
#define CEC_OP_UI_SND_PRES_CTL_TREBLE_UP 0xc1
#define CEC_OP_UI_SND_PRES_CTL_TREBLE_NEUTRAL 0xc2
#define CEC_OP_UI_SND_PRES_CTL_TREBLE_DOWN 0xc3
#define CEC_MSG_USER_CONTROL_RELEASED 0x45
#define CEC_MSG_GIVE_DEVICE_POWER_STATUS 0x8f
#define CEC_MSG_REPORT_POWER_STATUS 0x90
#define CEC_OP_POWER_STATUS_ON 0
#define CEC_OP_POWER_STATUS_STANDBY 1
#define CEC_OP_POWER_STATUS_TO_ON 2
#define CEC_OP_POWER_STATUS_TO_STANDBY 3
#define CEC_MSG_FEATURE_ABORT 0x00
#define CEC_OP_ABORT_UNRECOGNIZED_OP 0
#define CEC_OP_ABORT_INCORRECT_MODE 1
#define CEC_OP_ABORT_NO_SOURCE 2
#define CEC_OP_ABORT_INVALID_OP 3
#define CEC_OP_ABORT_REFUSED 4
#define CEC_OP_ABORT_UNDETERMINED 5
#define CEC_MSG_ABORT 0xff
#define CEC_MSG_GIVE_AUDIO_STATUS 0x71
#define CEC_MSG_GIVE_SYSTEM_AUDIO_MODE_STATUS 0x7d
#define CEC_MSG_REPORT_AUDIO_STATUS 0x7a
#define CEC_OP_AUD_MUTE_STATUS_OFF 0
#define CEC_OP_AUD_MUTE_STATUS_ON 1
#define CEC_MSG_REPORT_SHORT_AUDIO_DESCRIPTOR 0xa3
#define CEC_MSG_REQUEST_SHORT_AUDIO_DESCRIPTOR 0xa4
#define CEC_MSG_SET_SYSTEM_AUDIO_MODE 0x72
#define CEC_OP_SYS_AUD_STATUS_OFF 0
#define CEC_OP_SYS_AUD_STATUS_ON 1
#define CEC_MSG_SYSTEM_AUDIO_MODE_REQUEST 0x70
#define CEC_MSG_SYSTEM_AUDIO_MODE_STATUS 0x7e
#define CEC_OP_AUD_FMT_ID_CEA861 0
#define CEC_OP_AUD_FMT_ID_CEA861_CXT 1
#define CEC_MSG_SET_AUDIO_VOLUME_LEVEL 0x73
#define CEC_MSG_SET_AUDIO_RATE 0x9a
#define CEC_OP_AUD_RATE_OFF 0
#define CEC_OP_AUD_RATE_WIDE_STD 1
#define CEC_OP_AUD_RATE_WIDE_FAST 2
#define CEC_OP_AUD_RATE_WIDE_SLOW 3
#define CEC_OP_AUD_RATE_NARROW_STD 4
#define CEC_OP_AUD_RATE_NARROW_FAST 5
#define CEC_OP_AUD_RATE_NARROW_SLOW 6
#define CEC_MSG_INITIATE_ARC 0xc0
#define CEC_MSG_REPORT_ARC_INITIATED 0xc1
#define CEC_MSG_REPORT_ARC_TERMINATED 0xc2
#define CEC_MSG_REQUEST_ARC_INITIATION 0xc3
#define CEC_MSG_REQUEST_ARC_TERMINATION 0xc4
#define CEC_MSG_TERMINATE_ARC 0xc5
#define CEC_MSG_REQUEST_CURRENT_LATENCY 0xa7
#define CEC_MSG_REPORT_CURRENT_LATENCY 0xa8
#define CEC_OP_LOW_LATENCY_MODE_OFF 0
#define CEC_OP_LOW_LATENCY_MODE_ON 1
#define CEC_OP_AUD_OUT_COMPENSATED_NA 0
#define CEC_OP_AUD_OUT_COMPENSATED_DELAY 1
#define CEC_OP_AUD_OUT_COMPENSATED_NO_DELAY 2
#define CEC_OP_AUD_OUT_COMPENSATED_PARTIAL_DELAY 3
#define CEC_MSG_CDC_MESSAGE 0xf8
#define CEC_MSG_CDC_HEC_INQUIRE_STATE 0x00
#define CEC_MSG_CDC_HEC_REPORT_STATE 0x01
#define CEC_OP_HEC_FUNC_STATE_NOT_SUPPORTED 0
#define CEC_OP_HEC_FUNC_STATE_INACTIVE 1
#define CEC_OP_HEC_FUNC_STATE_ACTIVE 2
#define CEC_OP_HEC_FUNC_STATE_ACTIVATION_FIELD 3
#define CEC_OP_HOST_FUNC_STATE_NOT_SUPPORTED 0
#define CEC_OP_HOST_FUNC_STATE_INACTIVE 1
#define CEC_OP_HOST_FUNC_STATE_ACTIVE 2
#define CEC_OP_ENC_FUNC_STATE_EXT_CON_NOT_SUPPORTED 0
#define CEC_OP_ENC_FUNC_STATE_EXT_CON_INACTIVE 1
#define CEC_OP_ENC_FUNC_STATE_EXT_CON_ACTIVE 2
#define CEC_OP_CDC_ERROR_CODE_NONE 0
#define CEC_OP_CDC_ERROR_CODE_CAP_UNSUPPORTED 1
#define CEC_OP_CDC_ERROR_CODE_WRONG_STATE 2
#define CEC_OP_CDC_ERROR_CODE_OTHER 3
#define CEC_OP_HEC_SUPPORT_NO 0
#define CEC_OP_HEC_SUPPORT_YES 1
#define CEC_OP_HEC_ACTIVATION_ON 0
#define CEC_OP_HEC_ACTIVATION_OFF 1
#define CEC_MSG_CDC_HEC_SET_STATE_ADJACENT 0x02
#define CEC_MSG_CDC_HEC_SET_STATE 0x03
#define CEC_OP_HEC_SET_STATE_DEACTIVATE 0
#define CEC_OP_HEC_SET_STATE_ACTIVATE 1
#define CEC_MSG_CDC_HEC_REQUEST_DEACTIVATION 0x04
#define CEC_MSG_CDC_HEC_NOTIFY_ALIVE 0x05
#define CEC_MSG_CDC_HEC_DISCOVER 0x06
#define CEC_MSG_CDC_HPD_SET_STATE 0x10
#define CEC_OP_HPD_STATE_CP_EDID_DISABLE 0
#define CEC_OP_HPD_STATE_CP_EDID_ENABLE 1
#define CEC_OP_HPD_STATE_CP_EDID_DISABLE_ENABLE 2
#define CEC_OP_HPD_STATE_EDID_DISABLE 3
#define CEC_OP_HPD_STATE_EDID_ENABLE 4
#define CEC_OP_HPD_STATE_EDID_DISABLE_ENABLE 5
#define CEC_MSG_CDC_HPD_REPORT_STATE 0x11
#define CEC_OP_HPD_ERROR_NONE 0
#define CEC_OP_HPD_ERROR_INITIATOR_NOT_CAPABLE 1
#define CEC_OP_HPD_ERROR_INITIATOR_WRONG_STATE 2
#define CEC_OP_HPD_ERROR_OTHER 3
#define CEC_OP_HPD_ERROR_NONE_NO_VIDEO 4
#endif
