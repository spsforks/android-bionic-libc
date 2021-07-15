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
#ifndef __LINUX_IF_PACKET_H
#define __LINUX_IF_PACKET_H
#include <asm/byteorder.h>
#include <linux/types.h>
struct sockaddr_pkt {
  unsigned short spkt_family;
  unsigned char spkt_device[14];
  __be16 spkt_protocol;
};
struct sockaddr_ll {
  unsigned short sll_family;
  __be16 sll_protocol;
  int sll_ifindex;
  unsigned short sll_hatype;
  unsigned char sll_pkttype;
  unsigned char sll_halen;
  unsigned char sll_addr[8];
};
#define PACKET_HOST 0
#define PACKET_BROADCAST 1
#define PACKET_MULTICAST 2
#define PACKET_OTHERHOST 3
#define PACKET_OUTGOING 4
#define PACKET_LOOPBACK 5
#define PACKET_USER 6
#define PACKET_KERNEL 7
#define PACKET_FASTROUTE 6
#define PACKET_ADD_MEMBERSHIP 1
#define PACKET_DROP_MEMBERSHIP 2
#define PACKET_RECV_OUTPUT 3
#define PACKET_RX_RING 5
#define PACKET_STATISTICS 6
#define PACKET_COPY_THRESH 7
#define PACKET_AUXDATA 8
#define PACKET_ORIGDEV 9
#define PACKET_VERSION 10
#define PACKET_HDRLEN 11
#define PACKET_RESERVE 12
#define PACKET_TX_RING 13
#define PACKET_LOSS 14
#define PACKET_VNET_HDR 15
#define PACKET_TX_TIMESTAMP 16
#define PACKET_TIMESTAMP 17
#define PACKET_FANOUT 18
#define PACKET_TX_HAS_OFF 19
#define PACKET_QDISC_BYPASS 20
#define PACKET_ROLLOVER_STATS 21
#define PACKET_FANOUT_DATA 22
#define PACKET_IGNORE_OUTGOING 23
#define PACKET_FANOUT_HASH 0
#define PACKET_FANOUT_LB 1
#define PACKET_FANOUT_CPU 2
#define PACKET_FANOUT_ROLLOVER 3
#define PACKET_FANOUT_RND 4
#define PACKET_FANOUT_QM 5
#define PACKET_FANOUT_CBPF 6
#define PACKET_FANOUT_EBPF 7
#define PACKET_FANOUT_FLAG_ROLLOVER 0x1000
#define PACKET_FANOUT_FLAG_UNIQUEID 0x2000
#define PACKET_FANOUT_FLAG_DEFRAG 0x8000
struct tpacket_stats {
  unsigned int tp_packets;
  unsigned int tp_drops;
};
struct tpacket_stats_v3 {
  unsigned int tp_packets;
  unsigned int tp_drops;
  unsigned int tp_freeze_q_cnt;
};
struct tpacket_rollover_stats {
  __aligned_u64 tp_all;
  __aligned_u64 tp_huge;
  __aligned_u64 tp_failed;
};
union tpacket_stats_u {
  struct tpacket_stats stats1;
  struct tpacket_stats_v3 stats3;
};
struct tpacket_auxdata {
  __u32 tp_status;
  __u32 tp_len;
  __u32 tp_snaplen;
  __u16 tp_mac;
  __u16 tp_net;
  __u16 tp_vlan_tci;
  __u16 tp_vlan_tpid;
};
#define TP_STATUS_KERNEL 0
#define TP_STATUS_USER (1 << 0)
#define TP_STATUS_COPY (1 << 1)
#define TP_STATUS_LOSING (1 << 2)
#define TP_STATUS_CSUMNOTREADY (1 << 3)
#define TP_STATUS_VLAN_VALID (1 << 4)
#define TP_STATUS_BLK_TMO (1 << 5)
#define TP_STATUS_VLAN_TPID_VALID (1 << 6)
#define TP_STATUS_CSUM_VALID (1 << 7)
#define TP_STATUS_AVAILABLE 0
#define TP_STATUS_SEND_REQUEST (1 << 0)
#define TP_STATUS_SENDING (1 << 1)
#define TP_STATUS_WRONG_FORMAT (1 << 2)
#define TP_STATUS_TS_SOFTWARE (1 << 29)
#define TP_STATUS_TS_SYS_HARDWARE (1 << 30)
#define TP_STATUS_TS_RAW_HARDWARE (1U << 31)
#define TP_FT_REQ_FILL_RXHASH 0x1
struct tpacket_hdr {
  unsigned long tp_status;
  unsigned int tp_len;
  unsigned int tp_snaplen;
  unsigned short tp_mac;
  unsigned short tp_net;
  unsigned int tp_sec;
  unsigned int tp_usec;
};
#define TPACKET_ALIGNMENT 16
#define TPACKET_ALIGN(x) (((x) + TPACKET_ALIGNMENT - 1) & ~(TPACKET_ALIGNMENT - 1))
#define TPACKET_HDRLEN (TPACKET_ALIGN(sizeof(struct tpacket_hdr)) + sizeof(struct sockaddr_ll))
struct tpacket2_hdr {
  __u32 tp_status;
  __u32 tp_len;
  __u32 tp_snaplen;
  __u16 tp_mac;
  __u16 tp_net;
  __u32 tp_sec;
  __u32 tp_nsec;
  __u16 tp_vlan_tci;
  __u16 tp_vlan_tpid;
  __u8 tp_padding[4];
};
struct tpacket_hdr_variant1 {
  __u32 tp_rxhash;
  __u32 tp_vlan_tci;
  __u16 tp_vlan_tpid;
  __u16 tp_padding;
};
struct tpacket3_hdr {
  __u32 tp_next_offset;
  __u32 tp_sec;
  __u32 tp_nsec;
  __u32 tp_snaplen;
  __u32 tp_len;
  __u32 tp_status;
  __u16 tp_mac;
  __u16 tp_net;
  union {
    struct tpacket_hdr_variant1 hv1;
  };
  __u8 tp_padding[8];
};
struct tpacket_bd_ts {
  unsigned int ts_sec;
  union {
    unsigned int ts_usec;
    unsigned int ts_nsec;
  };
};
struct tpacket_hdr_v1 {
  __u32 block_status;
  __u32 num_pkts;
  __u32 offset_to_first_pkt;
  __u32 blk_len;
  __aligned_u64 seq_num;
  struct tpacket_bd_ts ts_first_pkt, ts_last_pkt;
};
union tpacket_bd_header_u {
  struct tpacket_hdr_v1 bh1;
};
struct tpacket_block_desc {
  __u32 version;
  __u32 offset_to_priv;
  union tpacket_bd_header_u hdr;
};
#define TPACKET2_HDRLEN (TPACKET_ALIGN(sizeof(struct tpacket2_hdr)) + sizeof(struct sockaddr_ll))
#define TPACKET3_HDRLEN (TPACKET_ALIGN(sizeof(struct tpacket3_hdr)) + sizeof(struct sockaddr_ll))
enum tpacket_versions {
  TPACKET_V1,
  TPACKET_V2,
  TPACKET_V3
};
struct tpacket_req {
  unsigned int tp_block_size;
  unsigned int tp_block_nr;
  unsigned int tp_frame_size;
  unsigned int tp_frame_nr;
};
struct tpacket_req3 {
  unsigned int tp_block_size;
  unsigned int tp_block_nr;
  unsigned int tp_frame_size;
  unsigned int tp_frame_nr;
  unsigned int tp_retire_blk_tov;
  unsigned int tp_sizeof_priv;
  unsigned int tp_feature_req_word;
};
union tpacket_req_u {
  struct tpacket_req req;
  struct tpacket_req3 req3;
};
struct packet_mreq {
  int mr_ifindex;
  unsigned short mr_type;
  unsigned short mr_alen;
  unsigned char mr_address[8];
};
struct fanout_args {
#ifdef __LITTLE_ENDIAN_BITFIELD
  __u16 id;
  __u16 type_flags;
#else
  __u16 type_flags;
  __u16 id;
#endif
  __u32 max_num_members;
};
#define PACKET_MR_MULTICAST 0
#define PACKET_MR_PROMISC 1
#define PACKET_MR_ALLMULTI 2
#define PACKET_MR_UNICAST 3
#endif
