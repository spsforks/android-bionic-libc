/*
 * This file is auto-generated. Modifications will be lost.
 *
 * See https://android.googlesource.com/platform/bionic/+/master/libc/kernel/
 * for more information.
 */
#ifndef _UAPI_LINUX_AUDIT_H_
#define _UAPI_LINUX_AUDIT_H_
#include <linux/types.h>
#include <linux/elf-em.h>
#define AUDIT_GET 1000
#define AUDIT_SET 1001
#define AUDIT_LIST 1002
#define AUDIT_ADD 1003
#define AUDIT_DEL 1004
#define AUDIT_USER 1005
#define AUDIT_LOGIN 1006
#define AUDIT_WATCH_INS 1007
#define AUDIT_WATCH_REM 1008
#define AUDIT_WATCH_LIST 1009
#define AUDIT_SIGNAL_INFO 1010
#define AUDIT_ADD_RULE 1011
#define AUDIT_DEL_RULE 1012
#define AUDIT_LIST_RULES 1013
#define AUDIT_TRIM 1014
#define AUDIT_MAKE_EQUIV 1015
#define AUDIT_TTY_GET 1016
#define AUDIT_TTY_SET 1017
#define AUDIT_SET_FEATURE 1018
#define AUDIT_GET_FEATURE 1019
#define AUDIT_FIRST_USER_MSG 1100
#define AUDIT_USER_AVC 1107
#define AUDIT_USER_TTY 1124
#define AUDIT_LAST_USER_MSG 1199
#define AUDIT_FIRST_USER_MSG2 2100
#define AUDIT_LAST_USER_MSG2 2999
#define AUDIT_DAEMON_START 1200
#define AUDIT_DAEMON_END 1201
#define AUDIT_DAEMON_ABORT 1202
#define AUDIT_DAEMON_CONFIG 1203
#define AUDIT_SYSCALL 1300
#define AUDIT_PATH 1302
#define AUDIT_IPC 1303
#define AUDIT_SOCKETCALL 1304
#define AUDIT_CONFIG_CHANGE 1305
#define AUDIT_SOCKADDR 1306
#define AUDIT_CWD 1307
#define AUDIT_EXECVE 1309
#define AUDIT_IPC_SET_PERM 1311
#define AUDIT_MQ_OPEN 1312
#define AUDIT_MQ_SENDRECV 1313
#define AUDIT_MQ_NOTIFY 1314
#define AUDIT_MQ_GETSETATTR 1315
#define AUDIT_KERNEL_OTHER 1316
#define AUDIT_FD_PAIR 1317
#define AUDIT_OBJ_PID 1318
#define AUDIT_TTY 1319
#define AUDIT_EOE 1320
#define AUDIT_BPRM_FCAPS 1321
#define AUDIT_CAPSET 1322
#define AUDIT_MMAP 1323
#define AUDIT_NETFILTER_PKT 1324
#define AUDIT_NETFILTER_CFG 1325
#define AUDIT_SECCOMP 1326
#define AUDIT_PROCTITLE 1327
#define AUDIT_FEATURE_CHANGE 1328
#define AUDIT_REPLACE 1329
#define AUDIT_KERN_MODULE 1330
#define AUDIT_FANOTIFY 1331
#define AUDIT_TIME_INJOFFSET 1332
#define AUDIT_TIME_ADJNTPVAL 1333
#define AUDIT_BPF 1334
#define AUDIT_EVENT_LISTENER 1335
#define AUDIT_URINGOP 1336
#define AUDIT_OPENAT2 1337
#define AUDIT_DM_CTRL 1338
#define AUDIT_DM_EVENT 1339
#define AUDIT_AVC 1400
#define AUDIT_SELINUX_ERR 1401
#define AUDIT_AVC_PATH 1402
#define AUDIT_MAC_POLICY_LOAD 1403
#define AUDIT_MAC_STATUS 1404
#define AUDIT_MAC_CONFIG_CHANGE 1405
#define AUDIT_MAC_UNLBL_ALLOW 1406
#define AUDIT_MAC_CIPSOV4_ADD 1407
#define AUDIT_MAC_CIPSOV4_DEL 1408
#define AUDIT_MAC_MAP_ADD 1409
#define AUDIT_MAC_MAP_DEL 1410
#define AUDIT_MAC_IPSEC_ADDSA 1411
#define AUDIT_MAC_IPSEC_DELSA 1412
#define AUDIT_MAC_IPSEC_ADDSPD 1413
#define AUDIT_MAC_IPSEC_DELSPD 1414
#define AUDIT_MAC_IPSEC_EVENT 1415
#define AUDIT_MAC_UNLBL_STCADD 1416
#define AUDIT_MAC_UNLBL_STCDEL 1417
#define AUDIT_MAC_CALIPSO_ADD 1418
#define AUDIT_MAC_CALIPSO_DEL 1419
#define AUDIT_FIRST_KERN_ANOM_MSG 1700
#define AUDIT_LAST_KERN_ANOM_MSG 1799
#define AUDIT_ANOM_PROMISCUOUS 1700
#define AUDIT_ANOM_ABEND 1701
#define AUDIT_ANOM_LINK 1702
#define AUDIT_ANOM_CREAT 1703
#define AUDIT_INTEGRITY_DATA 1800
#define AUDIT_INTEGRITY_METADATA 1801
#define AUDIT_INTEGRITY_STATUS 1802
#define AUDIT_INTEGRITY_HASH 1803
#define AUDIT_INTEGRITY_PCR 1804
#define AUDIT_INTEGRITY_RULE 1805
#define AUDIT_INTEGRITY_EVM_XATTR 1806
#define AUDIT_INTEGRITY_POLICY_RULE 1807
#define AUDIT_KERNEL 2000
#define AUDIT_FILTER_USER 0x00
#define AUDIT_FILTER_TASK 0x01
#define AUDIT_FILTER_ENTRY 0x02
#define AUDIT_FILTER_WATCH 0x03
#define AUDIT_FILTER_EXIT 0x04
#define AUDIT_FILTER_EXCLUDE 0x05
#define AUDIT_FILTER_TYPE AUDIT_FILTER_EXCLUDE
#define AUDIT_FILTER_FS 0x06
#define AUDIT_FILTER_URING_EXIT 0x07
#define AUDIT_NR_FILTERS 8
#define AUDIT_FILTER_PREPEND 0x10
#define AUDIT_NEVER 0
#define AUDIT_POSSIBLE 1
#define AUDIT_ALWAYS 2
#define AUDIT_MAX_FIELDS 64
#define AUDIT_MAX_KEY_LEN 256
#define AUDIT_BITMASK_SIZE 64
#define AUDIT_WORD(nr) ((__u32) ((nr) / 32))
#define AUDIT_BIT(nr) (1U << ((nr) - AUDIT_WORD(nr) * 32))
#define AUDIT_SYSCALL_CLASSES 16
#define AUDIT_CLASS_DIR_WRITE 0
#define AUDIT_CLASS_DIR_WRITE_32 1
#define AUDIT_CLASS_CHATTR 2
#define AUDIT_CLASS_CHATTR_32 3
#define AUDIT_CLASS_READ 4
#define AUDIT_CLASS_READ_32 5
#define AUDIT_CLASS_WRITE 6
#define AUDIT_CLASS_WRITE_32 7
#define AUDIT_CLASS_SIGNAL 8
#define AUDIT_CLASS_SIGNAL_32 9
#define AUDIT_UNUSED_BITS 0x07FFFC00
#define AUDIT_COMPARE_UID_TO_OBJ_UID 1
#define AUDIT_COMPARE_GID_TO_OBJ_GID 2
#define AUDIT_COMPARE_EUID_TO_OBJ_UID 3
#define AUDIT_COMPARE_EGID_TO_OBJ_GID 4
#define AUDIT_COMPARE_AUID_TO_OBJ_UID 5
#define AUDIT_COMPARE_SUID_TO_OBJ_UID 6
#define AUDIT_COMPARE_SGID_TO_OBJ_GID 7
#define AUDIT_COMPARE_FSUID_TO_OBJ_UID 8
#define AUDIT_COMPARE_FSGID_TO_OBJ_GID 9
#define AUDIT_COMPARE_UID_TO_AUID 10
#define AUDIT_COMPARE_UID_TO_EUID 11
#define AUDIT_COMPARE_UID_TO_FSUID 12
#define AUDIT_COMPARE_UID_TO_SUID 13
#define AUDIT_COMPARE_AUID_TO_FSUID 14
#define AUDIT_COMPARE_AUID_TO_SUID 15
#define AUDIT_COMPARE_AUID_TO_EUID 16
#define AUDIT_COMPARE_EUID_TO_SUID 17
#define AUDIT_COMPARE_EUID_TO_FSUID 18
#define AUDIT_COMPARE_SUID_TO_FSUID 19
#define AUDIT_COMPARE_GID_TO_EGID 20
#define AUDIT_COMPARE_GID_TO_FSGID 21
#define AUDIT_COMPARE_GID_TO_SGID 22
#define AUDIT_COMPARE_EGID_TO_FSGID 23
#define AUDIT_COMPARE_EGID_TO_SGID 24
#define AUDIT_COMPARE_SGID_TO_FSGID 25
#define AUDIT_MAX_FIELD_COMPARE AUDIT_COMPARE_SGID_TO_FSGID
#define AUDIT_PID 0
#define AUDIT_UID 1
#define AUDIT_EUID 2
#define AUDIT_SUID 3
#define AUDIT_FSUID 4
#define AUDIT_GID 5
#define AUDIT_EGID 6
#define AUDIT_SGID 7
#define AUDIT_FSGID 8
#define AUDIT_LOGINUID 9
#define AUDIT_PERS 10
#define AUDIT_ARCH 11
#define AUDIT_MSGTYPE 12
#define AUDIT_SUBJ_USER 13
#define AUDIT_SUBJ_ROLE 14
#define AUDIT_SUBJ_TYPE 15
#define AUDIT_SUBJ_SEN 16
#define AUDIT_SUBJ_CLR 17
#define AUDIT_PPID 18
#define AUDIT_OBJ_USER 19
#define AUDIT_OBJ_ROLE 20
#define AUDIT_OBJ_TYPE 21
#define AUDIT_OBJ_LEV_LOW 22
#define AUDIT_OBJ_LEV_HIGH 23
#define AUDIT_LOGINUID_SET 24
#define AUDIT_SESSIONID 25
#define AUDIT_FSTYPE 26
#define AUDIT_DEVMAJOR 100
#define AUDIT_DEVMINOR 101
#define AUDIT_INODE 102
#define AUDIT_EXIT 103
#define AUDIT_SUCCESS 104
#define AUDIT_WATCH 105
#define AUDIT_PERM 106
#define AUDIT_DIR 107
#define AUDIT_FILETYPE 108
#define AUDIT_OBJ_UID 109
#define AUDIT_OBJ_GID 110
#define AUDIT_FIELD_COMPARE 111
#define AUDIT_EXE 112
#define AUDIT_SADDR_FAM 113
#define AUDIT_ARG0 200
#define AUDIT_ARG1 (AUDIT_ARG0 + 1)
#define AUDIT_ARG2 (AUDIT_ARG0 + 2)
#define AUDIT_ARG3 (AUDIT_ARG0 + 3)
#define AUDIT_FILTERKEY 210
#define AUDIT_NEGATE 0x80000000
#define AUDIT_BIT_MASK 0x08000000
#define AUDIT_LESS_THAN 0x10000000
#define AUDIT_GREATER_THAN 0x20000000
#define AUDIT_NOT_EQUAL 0x30000000
#define AUDIT_EQUAL 0x40000000
#define AUDIT_BIT_TEST (AUDIT_BIT_MASK | AUDIT_EQUAL)
#define AUDIT_LESS_THAN_OR_EQUAL (AUDIT_LESS_THAN | AUDIT_EQUAL)
#define AUDIT_GREATER_THAN_OR_EQUAL (AUDIT_GREATER_THAN | AUDIT_EQUAL)
#define AUDIT_OPERATORS (AUDIT_EQUAL | AUDIT_NOT_EQUAL | AUDIT_BIT_MASK)
enum {
  Audit_equal,
  Audit_not_equal,
  Audit_bitmask,
  Audit_bittest,
  Audit_lt,
  Audit_gt,
  Audit_le,
  Audit_ge,
  Audit_bad
};
#define AUDIT_STATUS_ENABLED 0x0001
#define AUDIT_STATUS_FAILURE 0x0002
#define AUDIT_STATUS_PID 0x0004
#define AUDIT_STATUS_RATE_LIMIT 0x0008
#define AUDIT_STATUS_BACKLOG_LIMIT 0x0010
#define AUDIT_STATUS_BACKLOG_WAIT_TIME 0x0020
#define AUDIT_STATUS_LOST 0x0040
#define AUDIT_STATUS_BACKLOG_WAIT_TIME_ACTUAL 0x0080
#define AUDIT_FEATURE_BITMAP_BACKLOG_LIMIT 0x00000001
#define AUDIT_FEATURE_BITMAP_BACKLOG_WAIT_TIME 0x00000002
#define AUDIT_FEATURE_BITMAP_EXECUTABLE_PATH 0x00000004
#define AUDIT_FEATURE_BITMAP_EXCLUDE_EXTEND 0x00000008
#define AUDIT_FEATURE_BITMAP_SESSIONID_FILTER 0x00000010
#define AUDIT_FEATURE_BITMAP_LOST_RESET 0x00000020
#define AUDIT_FEATURE_BITMAP_FILTER_FS 0x00000040
#define AUDIT_FEATURE_BITMAP_ALL (AUDIT_FEATURE_BITMAP_BACKLOG_LIMIT | AUDIT_FEATURE_BITMAP_BACKLOG_WAIT_TIME | AUDIT_FEATURE_BITMAP_EXECUTABLE_PATH | AUDIT_FEATURE_BITMAP_EXCLUDE_EXTEND | AUDIT_FEATURE_BITMAP_SESSIONID_FILTER | AUDIT_FEATURE_BITMAP_LOST_RESET | AUDIT_FEATURE_BITMAP_FILTER_FS)
#define AUDIT_VERSION_LATEST AUDIT_FEATURE_BITMAP_ALL
#define AUDIT_VERSION_BACKLOG_LIMIT AUDIT_FEATURE_BITMAP_BACKLOG_LIMIT
#define AUDIT_VERSION_BACKLOG_WAIT_TIME AUDIT_FEATURE_BITMAP_BACKLOG_WAIT_TIME
#define AUDIT_FAIL_SILENT 0
#define AUDIT_FAIL_PRINTK 1
#define AUDIT_FAIL_PANIC 2
#define __AUDIT_ARCH_CONVENTION_MASK 0x30000000
#define __AUDIT_ARCH_CONVENTION_MIPS64_N32 0x20000000
#define __AUDIT_ARCH_64BIT 0x80000000
#define __AUDIT_ARCH_LE 0x40000000
#define AUDIT_ARCH_AARCH64 (EM_AARCH64 | __AUDIT_ARCH_64BIT | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_ALPHA (EM_ALPHA | __AUDIT_ARCH_64BIT | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_ARCOMPACT (EM_ARCOMPACT | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_ARCOMPACTBE (EM_ARCOMPACT)
#define AUDIT_ARCH_ARCV2 (EM_ARCV2 | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_ARCV2BE (EM_ARCV2)
#define AUDIT_ARCH_ARM (EM_ARM | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_ARMEB (EM_ARM)
#define AUDIT_ARCH_C6X (EM_TI_C6000 | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_C6XBE (EM_TI_C6000)
#define AUDIT_ARCH_CRIS (EM_CRIS | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_CSKY (EM_CSKY | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_FRV (EM_FRV)
#define AUDIT_ARCH_H8300 (EM_H8_300)
#define AUDIT_ARCH_HEXAGON (EM_HEXAGON)
#define AUDIT_ARCH_I386 (EM_386 | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_IA64 (EM_IA_64 | __AUDIT_ARCH_64BIT | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_M32R (EM_M32R)
#define AUDIT_ARCH_M68K (EM_68K)
#define AUDIT_ARCH_MICROBLAZE (EM_MICROBLAZE)
#define AUDIT_ARCH_MIPS (EM_MIPS)
#define AUDIT_ARCH_MIPSEL (EM_MIPS | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_MIPS64 (EM_MIPS | __AUDIT_ARCH_64BIT)
#define AUDIT_ARCH_MIPS64N32 (EM_MIPS | __AUDIT_ARCH_64BIT | __AUDIT_ARCH_CONVENTION_MIPS64_N32)
#define AUDIT_ARCH_MIPSEL64 (EM_MIPS | __AUDIT_ARCH_64BIT | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_MIPSEL64N32 (EM_MIPS | __AUDIT_ARCH_64BIT | __AUDIT_ARCH_LE | __AUDIT_ARCH_CONVENTION_MIPS64_N32)
#define AUDIT_ARCH_NDS32 (EM_NDS32 | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_NDS32BE (EM_NDS32)
#define AUDIT_ARCH_NIOS2 (EM_ALTERA_NIOS2 | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_OPENRISC (EM_OPENRISC)
#define AUDIT_ARCH_PARISC (EM_PARISC)
#define AUDIT_ARCH_PARISC64 (EM_PARISC | __AUDIT_ARCH_64BIT)
#define AUDIT_ARCH_PPC (EM_PPC)
#define AUDIT_ARCH_PPC64 (EM_PPC64 | __AUDIT_ARCH_64BIT)
#define AUDIT_ARCH_PPC64LE (EM_PPC64 | __AUDIT_ARCH_64BIT | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_RISCV32 (EM_RISCV | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_RISCV64 (EM_RISCV | __AUDIT_ARCH_64BIT | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_S390 (EM_S390)
#define AUDIT_ARCH_S390X (EM_S390 | __AUDIT_ARCH_64BIT)
#define AUDIT_ARCH_SH (EM_SH)
#define AUDIT_ARCH_SHEL (EM_SH | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_SH64 (EM_SH | __AUDIT_ARCH_64BIT)
#define AUDIT_ARCH_SHEL64 (EM_SH | __AUDIT_ARCH_64BIT | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_SPARC (EM_SPARC)
#define AUDIT_ARCH_SPARC64 (EM_SPARCV9 | __AUDIT_ARCH_64BIT)
#define AUDIT_ARCH_TILEGX (EM_TILEGX | __AUDIT_ARCH_64BIT | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_TILEGX32 (EM_TILEGX | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_TILEPRO (EM_TILEPRO | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_UNICORE (EM_UNICORE | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_X86_64 (EM_X86_64 | __AUDIT_ARCH_64BIT | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_XTENSA (EM_XTENSA)
#define AUDIT_ARCH_LOONGARCH32 (EM_LOONGARCH | __AUDIT_ARCH_LE)
#define AUDIT_ARCH_LOONGARCH64 (EM_LOONGARCH | __AUDIT_ARCH_64BIT | __AUDIT_ARCH_LE)
#define AUDIT_PERM_EXEC 1
#define AUDIT_PERM_WRITE 2
#define AUDIT_PERM_READ 4
#define AUDIT_PERM_ATTR 8
#define AUDIT_MESSAGE_TEXT_MAX 8560
enum audit_nlgrps {
  AUDIT_NLGRP_NONE,
  AUDIT_NLGRP_READLOG,
  __AUDIT_NLGRP_MAX
};
#define AUDIT_NLGRP_MAX (__AUDIT_NLGRP_MAX - 1)
struct audit_status {
  __u32 mask;
  __u32 enabled;
  __u32 failure;
  __u32 pid;
  __u32 rate_limit;
  __u32 backlog_limit;
  __u32 lost;
  __u32 backlog;
  union {
    __u32 version;
    __u32 feature_bitmap;
  };
  __u32 backlog_wait_time;
  __u32 backlog_wait_time_actual;
};
struct audit_features {
#define AUDIT_FEATURE_VERSION 1
  __u32 vers;
  __u32 mask;
  __u32 features;
  __u32 lock;
};
#define AUDIT_FEATURE_ONLY_UNSET_LOGINUID 0
#define AUDIT_FEATURE_LOGINUID_IMMUTABLE 1
#define AUDIT_LAST_FEATURE AUDIT_FEATURE_LOGINUID_IMMUTABLE
#define audit_feature_valid(x) ((x) >= 0 && (x) <= AUDIT_LAST_FEATURE)
#define AUDIT_FEATURE_TO_MASK(x) (1 << ((x) & 31))
struct audit_tty_status {
  __u32 enabled;
  __u32 log_passwd;
};
#define AUDIT_UID_UNSET (unsigned int) - 1
#define AUDIT_SID_UNSET ((unsigned int) - 1)
struct audit_rule_data {
  __u32 flags;
  __u32 action;
  __u32 field_count;
  __u32 mask[AUDIT_BITMASK_SIZE];
  __u32 fields[AUDIT_MAX_FIELDS];
  __u32 values[AUDIT_MAX_FIELDS];
  __u32 fieldflags[AUDIT_MAX_FIELDS];
  __u32 buflen;
  char buf[];
};
#endif
