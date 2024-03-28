/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_LINUX_SMC_H_
#define _UAPI_LINUX_SMC_H_
enum {
  SMC_PNETID_UNSPEC,
  SMC_PNETID_NAME,
  SMC_PNETID_ETHNAME,
  SMC_PNETID_IBNAME,
  SMC_PNETID_IBPORT,
  __SMC_PNETID_MAX,
  SMC_PNETID_MAX = __SMC_PNETID_MAX - 1
};
enum {
  SMC_PNETID_GET = 1,
  SMC_PNETID_ADD,
  SMC_PNETID_DEL,
  SMC_PNETID_FLUSH
};
#define SMCR_GENL_FAMILY_NAME "SMC_PNETID"
#define SMCR_GENL_FAMILY_VERSION 1
#define SMC_GENL_FAMILY_NAME "SMC_GEN_NETLINK"
#define SMC_GENL_FAMILY_VERSION 1
#define SMC_PCI_ID_STR_LEN 16
#define SMC_MAX_HOSTNAME_LEN 32
#define SMC_MAX_UEID 4
#define SMC_MAX_EID_LEN 32
enum {
  SMC_NETLINK_GET_SYS_INFO = 1,
  SMC_NETLINK_GET_LGR_SMCR,
  SMC_NETLINK_GET_LINK_SMCR,
  SMC_NETLINK_GET_LGR_SMCD,
  SMC_NETLINK_GET_DEV_SMCD,
  SMC_NETLINK_GET_DEV_SMCR,
  SMC_NETLINK_GET_STATS,
  SMC_NETLINK_GET_FBACK_STATS,
  SMC_NETLINK_DUMP_UEID,
  SMC_NETLINK_ADD_UEID,
  SMC_NETLINK_REMOVE_UEID,
  SMC_NETLINK_FLUSH_UEID,
  SMC_NETLINK_DUMP_SEID,
  SMC_NETLINK_ENABLE_SEID,
  SMC_NETLINK_DISABLE_SEID,
  SMC_NETLINK_DUMP_HS_LIMITATION,
  SMC_NETLINK_ENABLE_HS_LIMITATION,
  SMC_NETLINK_DISABLE_HS_LIMITATION,
};
enum {
  SMC_GEN_UNSPEC,
  SMC_GEN_SYS_INFO,
  SMC_GEN_LGR_SMCR,
  SMC_GEN_LINK_SMCR,
  SMC_GEN_LGR_SMCD,
  SMC_GEN_DEV_SMCD,
  SMC_GEN_DEV_SMCR,
  SMC_GEN_STATS,
  SMC_GEN_FBACK_STATS,
  __SMC_GEN_MAX,
  SMC_GEN_MAX = __SMC_GEN_MAX - 1
};
enum {
  SMC_NLA_SYS_UNSPEC,
  SMC_NLA_SYS_VER,
  SMC_NLA_SYS_REL,
  SMC_NLA_SYS_IS_ISM_V2,
  SMC_NLA_SYS_LOCAL_HOST,
  SMC_NLA_SYS_SEID,
  SMC_NLA_SYS_IS_SMCR_V2,
  __SMC_NLA_SYS_MAX,
  SMC_NLA_SYS_MAX = __SMC_NLA_SYS_MAX - 1
};
enum {
  SMC_NLA_LGR_V2_VER,
  SMC_NLA_LGR_V2_REL,
  SMC_NLA_LGR_V2_OS,
  SMC_NLA_LGR_V2_NEG_EID,
  SMC_NLA_LGR_V2_PEER_HOST,
  __SMC_NLA_LGR_V2_MAX,
  SMC_NLA_LGR_V2_MAX = __SMC_NLA_LGR_V2_MAX - 1
};
enum {
  SMC_NLA_LGR_R_V2_UNSPEC,
  SMC_NLA_LGR_R_V2_DIRECT,
  SMC_NLA_LGR_R_V2_MAX_CONNS,
  SMC_NLA_LGR_R_V2_MAX_LINKS,
  __SMC_NLA_LGR_R_V2_MAX,
  SMC_NLA_LGR_R_V2_MAX = __SMC_NLA_LGR_R_V2_MAX - 1
};
enum {
  SMC_NLA_LGR_R_UNSPEC,
  SMC_NLA_LGR_R_ID,
  SMC_NLA_LGR_R_ROLE,
  SMC_NLA_LGR_R_TYPE,
  SMC_NLA_LGR_R_PNETID,
  SMC_NLA_LGR_R_VLAN_ID,
  SMC_NLA_LGR_R_CONNS_NUM,
  SMC_NLA_LGR_R_V2_COMMON,
  SMC_NLA_LGR_R_V2,
  SMC_NLA_LGR_R_NET_COOKIE,
  SMC_NLA_LGR_R_PAD,
  SMC_NLA_LGR_R_BUF_TYPE,
  __SMC_NLA_LGR_R_MAX,
  SMC_NLA_LGR_R_MAX = __SMC_NLA_LGR_R_MAX - 1
};
enum {
  SMC_NLA_LINK_UNSPEC,
  SMC_NLA_LINK_ID,
  SMC_NLA_LINK_IB_DEV,
  SMC_NLA_LINK_IB_PORT,
  SMC_NLA_LINK_GID,
  SMC_NLA_LINK_PEER_GID,
  SMC_NLA_LINK_CONN_CNT,
  SMC_NLA_LINK_NET_DEV,
  SMC_NLA_LINK_UID,
  SMC_NLA_LINK_PEER_UID,
  SMC_NLA_LINK_STATE,
  __SMC_NLA_LINK_MAX,
  SMC_NLA_LINK_MAX = __SMC_NLA_LINK_MAX - 1
};
enum {
  SMC_NLA_LGR_D_UNSPEC,
  SMC_NLA_LGR_D_ID,
  SMC_NLA_LGR_D_GID,
  SMC_NLA_LGR_D_PEER_GID,
  SMC_NLA_LGR_D_VLAN_ID,
  SMC_NLA_LGR_D_CONNS_NUM,
  SMC_NLA_LGR_D_PNETID,
  SMC_NLA_LGR_D_CHID,
  SMC_NLA_LGR_D_PAD,
  SMC_NLA_LGR_D_V2_COMMON,
  SMC_NLA_LGR_D_EXT_GID,
  SMC_NLA_LGR_D_PEER_EXT_GID,
  __SMC_NLA_LGR_D_MAX,
  SMC_NLA_LGR_D_MAX = __SMC_NLA_LGR_D_MAX - 1
};
enum {
  SMC_NLA_DEV_PORT_UNSPEC,
  SMC_NLA_DEV_PORT_PNET_USR,
  SMC_NLA_DEV_PORT_PNETID,
  SMC_NLA_DEV_PORT_NETDEV,
  SMC_NLA_DEV_PORT_STATE,
  SMC_NLA_DEV_PORT_VALID,
  SMC_NLA_DEV_PORT_LNK_CNT,
  __SMC_NLA_DEV_PORT_MAX,
  SMC_NLA_DEV_PORT_MAX = __SMC_NLA_DEV_PORT_MAX - 1
};
enum {
  SMC_NLA_DEV_UNSPEC,
  SMC_NLA_DEV_USE_CNT,
  SMC_NLA_DEV_IS_CRIT,
  SMC_NLA_DEV_PCI_FID,
  SMC_NLA_DEV_PCI_CHID,
  SMC_NLA_DEV_PCI_VENDOR,
  SMC_NLA_DEV_PCI_DEVICE,
  SMC_NLA_DEV_PCI_ID,
  SMC_NLA_DEV_PORT,
  SMC_NLA_DEV_PORT2,
  SMC_NLA_DEV_IB_NAME,
  __SMC_NLA_DEV_MAX,
  SMC_NLA_DEV_MAX = __SMC_NLA_DEV_MAX - 1
};
enum {
  SMC_NLA_STATS_PLOAD_PAD,
  SMC_NLA_STATS_PLOAD_8K,
  SMC_NLA_STATS_PLOAD_16K,
  SMC_NLA_STATS_PLOAD_32K,
  SMC_NLA_STATS_PLOAD_64K,
  SMC_NLA_STATS_PLOAD_128K,
  SMC_NLA_STATS_PLOAD_256K,
  SMC_NLA_STATS_PLOAD_512K,
  SMC_NLA_STATS_PLOAD_1024K,
  SMC_NLA_STATS_PLOAD_G_1024K,
  __SMC_NLA_STATS_PLOAD_MAX,
  SMC_NLA_STATS_PLOAD_MAX = __SMC_NLA_STATS_PLOAD_MAX - 1
};
enum {
  SMC_NLA_STATS_RMB_PAD,
  SMC_NLA_STATS_RMB_SIZE_SM_PEER_CNT,
  SMC_NLA_STATS_RMB_SIZE_SM_CNT,
  SMC_NLA_STATS_RMB_FULL_PEER_CNT,
  SMC_NLA_STATS_RMB_FULL_CNT,
  SMC_NLA_STATS_RMB_REUSE_CNT,
  SMC_NLA_STATS_RMB_ALLOC_CNT,
  SMC_NLA_STATS_RMB_DGRADE_CNT,
  __SMC_NLA_STATS_RMB_MAX,
  SMC_NLA_STATS_RMB_MAX = __SMC_NLA_STATS_RMB_MAX - 1
};
enum {
  SMC_NLA_STATS_T_PAD,
  SMC_NLA_STATS_T_TX_RMB_SIZE,
  SMC_NLA_STATS_T_RX_RMB_SIZE,
  SMC_NLA_STATS_T_TXPLOAD_SIZE,
  SMC_NLA_STATS_T_RXPLOAD_SIZE,
  SMC_NLA_STATS_T_TX_RMB_STATS,
  SMC_NLA_STATS_T_RX_RMB_STATS,
  SMC_NLA_STATS_T_CLNT_V1_SUCC,
  SMC_NLA_STATS_T_CLNT_V2_SUCC,
  SMC_NLA_STATS_T_SRV_V1_SUCC,
  SMC_NLA_STATS_T_SRV_V2_SUCC,
  SMC_NLA_STATS_T_SENDPAGE_CNT,
  SMC_NLA_STATS_T_SPLICE_CNT,
  SMC_NLA_STATS_T_CORK_CNT,
  SMC_NLA_STATS_T_NDLY_CNT,
  SMC_NLA_STATS_T_URG_DATA_CNT,
  SMC_NLA_STATS_T_RX_BYTES,
  SMC_NLA_STATS_T_TX_BYTES,
  SMC_NLA_STATS_T_RX_CNT,
  SMC_NLA_STATS_T_TX_CNT,
  __SMC_NLA_STATS_T_MAX,
  SMC_NLA_STATS_T_MAX = __SMC_NLA_STATS_T_MAX - 1
};
enum {
  SMC_NLA_STATS_PAD,
  SMC_NLA_STATS_SMCD_TECH,
  SMC_NLA_STATS_SMCR_TECH,
  SMC_NLA_STATS_CLNT_HS_ERR_CNT,
  SMC_NLA_STATS_SRV_HS_ERR_CNT,
  __SMC_NLA_STATS_MAX,
  SMC_NLA_STATS_MAX = __SMC_NLA_STATS_MAX - 1
};
enum {
  SMC_NLA_FBACK_STATS_PAD,
  SMC_NLA_FBACK_STATS_TYPE,
  SMC_NLA_FBACK_STATS_SRV_CNT,
  SMC_NLA_FBACK_STATS_CLNT_CNT,
  SMC_NLA_FBACK_STATS_RSN_CODE,
  SMC_NLA_FBACK_STATS_RSN_CNT,
  __SMC_NLA_FBACK_STATS_MAX,
  SMC_NLA_FBACK_STATS_MAX = __SMC_NLA_FBACK_STATS_MAX - 1
};
enum {
  SMC_NLA_EID_TABLE_UNSPEC,
  SMC_NLA_EID_TABLE_ENTRY,
  __SMC_NLA_EID_TABLE_MAX,
  SMC_NLA_EID_TABLE_MAX = __SMC_NLA_EID_TABLE_MAX - 1
};
enum {
  SMC_NLA_SEID_UNSPEC,
  SMC_NLA_SEID_ENTRY,
  SMC_NLA_SEID_ENABLED,
  __SMC_NLA_SEID_TABLE_MAX,
  SMC_NLA_SEID_TABLE_MAX = __SMC_NLA_SEID_TABLE_MAX - 1
};
enum {
  SMC_NLA_HS_LIMITATION_UNSPEC,
  SMC_NLA_HS_LIMITATION_ENABLED,
  __SMC_NLA_HS_LIMITATION_MAX,
  SMC_NLA_HS_LIMITATION_MAX = __SMC_NLA_HS_LIMITATION_MAX - 1
};
#define SMC_LIMIT_HS 1
#endif
