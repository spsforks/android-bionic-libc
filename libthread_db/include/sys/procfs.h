/*
 * Copyright 2006 The Android Open Source Project
 */

#ifndef _SYS_PROCFS_H
#define _SYS_PROCFS_H

#include <sys/types.h>


#ifdef __cplusplus
extern "C"{
#endif


/* TODO: Temporary pending completion of <elf.h> based on the new kernel headers
 *
 * Some of these structures are defined in uapi/linux/elfcore.h, however this
 * header makes reference to asm/elf.h which is not exported among the uapi
 * headers for AArch64. A common resolution needs to be implemented that will
 * work for all architectures.
 */
#if defined (__aarch64__)
#define NT_PRSTATUS 1
#define ELF_NGREG (sizeof(struct user_pt_regs) / sizeof(elf_greg_t))

typedef unsigned long elf_greg_t;
typedef elf_greg_t elf_gregset_t[ELF_NGREG];

/* Signal info.  */
struct elf_siginfo
  {
    int si_signo;           /* Signal number.  */
    int si_code;            /* Extra code.  */
    int si_errno;           /* Errno.  */
  };

/* Definitions to generate Intel SVR4-like core files.  These mostly
   have the same names as the SVR4 types with "elf_" tacked on the
   front to prevent clashes with Linux definitions, and the typedef
   forms have been avoided.  This is mostly like the SVR4 structure,
   but more Linuxy, with things that Linux does not support and which
   GDB doesn't really use excluded.  */

struct elf_prstatus
  {
    struct elf_siginfo pr_info;     /* Info associated with signal.  */
    short int pr_cursig;        /* Current signal.  */
    unsigned long int pr_sigpend;   /* Set of pending signals.  */
    unsigned long int pr_sighold;   /* Set of held signals.  */
    __pid_t pr_pid;
    __pid_t pr_ppid;
    __pid_t pr_pgrp;
    __pid_t pr_sid;
    struct timeval pr_utime;        /* User time.  */
    struct timeval pr_stime;        /* System time.  */
    struct timeval pr_cutime;       /* Cumulative user time.  */
    struct timeval pr_cstime;       /* Cumulative system time.  */
    elf_gregset_t pr_reg;       /* GP registers.  */
    int pr_fpvalid;         /* True if math copro being used.  */
  };
#endif

typedef pid_t lwpid_t;
typedef void *psaddr_t;

#ifdef __cplusplus
}
#endif

#endif
