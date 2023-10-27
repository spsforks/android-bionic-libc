/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef __BNXT_RE_UVERBS_ABI_H__
#define __BNXT_RE_UVERBS_ABI_H__
#include <linux/types.h>
#include <rdma/ib_user_ioctl_cmds.h>
#define BNXT_RE_ABI_VERSION 1
#define BNXT_RE_CHIP_ID0_CHIP_NUM_SFT 0x00
#define BNXT_RE_CHIP_ID0_CHIP_REV_SFT 0x10
#define BNXT_RE_CHIP_ID0_CHIP_MET_SFT 0x18
enum {
  BNXT_RE_UCNTX_CMASK_HAVE_CCTX = 0x1ULL,
  BNXT_RE_UCNTX_CMASK_HAVE_MODE = 0x02ULL,
  BNXT_RE_UCNTX_CMASK_WC_DPI_ENABLED = 0x04ULL,
};
enum bnxt_re_wqe_mode {
  BNXT_QPLIB_WQE_MODE_STATIC = 0x00,
  BNXT_QPLIB_WQE_MODE_VARIABLE = 0x01,
  BNXT_QPLIB_WQE_MODE_INVALID = 0x02,
};
struct bnxt_re_uctx_resp {
  __u32 dev_id;
  __u32 max_qp;
  __u32 pg_size;
  __u32 cqe_sz;
  __u32 max_cqd;
  __u32 rsvd;
  __aligned_u64 comp_mask;
  __u32 chip_id0;
  __u32 chip_id1;
  __u32 mode;
  __u32 rsvd1;
};
struct bnxt_re_pd_resp {
  __u32 pdid;
  __u32 dpi;
  __u64 dbr;
} __attribute__((packed, aligned(4)));
struct bnxt_re_cq_req {
  __aligned_u64 cq_va;
  __aligned_u64 cq_handle;
};
struct bnxt_re_cq_resp {
  __u32 cqid;
  __u32 tail;
  __u32 phase;
  __u32 rsvd;
};
struct bnxt_re_resize_cq_req {
  __aligned_u64 cq_va;
};
struct bnxt_re_qp_req {
  __aligned_u64 qpsva;
  __aligned_u64 qprva;
  __aligned_u64 qp_handle;
};
struct bnxt_re_qp_resp {
  __u32 qpid;
  __u32 rsvd;
};
struct bnxt_re_srq_req {
  __aligned_u64 srqva;
  __aligned_u64 srq_handle;
};
struct bnxt_re_srq_resp {
  __u32 srqid;
};
enum bnxt_re_shpg_offt {
  BNXT_RE_BEG_RESV_OFFT = 0x00,
  BNXT_RE_AVID_OFFT = 0x10,
  BNXT_RE_AVID_SIZE = 0x04,
  BNXT_RE_END_RESV_OFFT = 0xFF0
};
enum bnxt_re_objects {
  BNXT_RE_OBJECT_ALLOC_PAGE = (1U << UVERBS_ID_NS_SHIFT),
};
enum bnxt_re_alloc_page_type {
  BNXT_RE_ALLOC_WC_PAGE = 0,
};
enum bnxt_re_var_alloc_page_attrs {
  BNXT_RE_ALLOC_PAGE_HANDLE = (1U << UVERBS_ID_NS_SHIFT),
  BNXT_RE_ALLOC_PAGE_TYPE,
  BNXT_RE_ALLOC_PAGE_DPI,
  BNXT_RE_ALLOC_PAGE_MMAP_OFFSET,
  BNXT_RE_ALLOC_PAGE_MMAP_LENGTH,
};
enum bnxt_re_alloc_page_attrs {
  BNXT_RE_DESTROY_PAGE_HANDLE = (1U << UVERBS_ID_NS_SHIFT),
};
enum bnxt_re_alloc_page_methods {
  BNXT_RE_METHOD_ALLOC_PAGE = (1U << UVERBS_ID_NS_SHIFT),
  BNXT_RE_METHOD_DESTROY_PAGE,
};
#endif
