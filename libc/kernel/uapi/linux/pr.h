/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_PR_H
#define _UAPI_PR_H
#include <linux/types.h>
enum pr_status {
  PR_STS_SUCCESS = 0x0,
  PR_STS_IOERR = 0x2,
  PR_STS_RESERVATION_CONFLICT = 0x18,
  PR_STS_RETRY_PATH_FAILURE = 0xe0000,
  PR_STS_PATH_FAST_FAILED = 0xf0000,
  PR_STS_PATH_FAILED = 0x10000,
};
enum pr_type {
  PR_WRITE_EXCLUSIVE = 1,
  PR_EXCLUSIVE_ACCESS = 2,
  PR_WRITE_EXCLUSIVE_REG_ONLY = 3,
  PR_EXCLUSIVE_ACCESS_REG_ONLY = 4,
  PR_WRITE_EXCLUSIVE_ALL_REGS = 5,
  PR_EXCLUSIVE_ACCESS_ALL_REGS = 6,
};
struct pr_reservation {
  __u64 key;
  __u32 type;
  __u32 flags;
};
struct pr_registration {
  __u64 old_key;
  __u64 new_key;
  __u32 flags;
  __u32 __pad;
};
struct pr_preempt {
  __u64 old_key;
  __u64 new_key;
  __u32 type;
  __u32 flags;
};
struct pr_clear {
  __u64 key;
  __u32 flags;
  __u32 __pad;
};
#define PR_FL_IGNORE_KEY (1 << 0)
#define IOC_PR_REGISTER _IOW('p', 200, struct pr_registration)
#define IOC_PR_RESERVE _IOW('p', 201, struct pr_reservation)
#define IOC_PR_RELEASE _IOW('p', 202, struct pr_reservation)
#define IOC_PR_PREEMPT _IOW('p', 203, struct pr_preempt)
#define IOC_PR_PREEMPT_ABORT _IOW('p', 204, struct pr_preempt)
#define IOC_PR_CLEAR _IOW('p', 205, struct pr_clear)
#endif
