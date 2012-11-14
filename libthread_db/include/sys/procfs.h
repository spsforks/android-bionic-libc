/*
 * Copyright 2006 The Android Open Source Project
 */

#ifndef _SYS_PROCFS_H
#define _SYS_PROCFS_H

#include <sys/types.h>


#ifdef __cplusplus
extern "C"{
#endif

#ifdef __aarch64__
/* general-purpose register */
typedef unsigned long elf_greg_t;

#define ELF_NGREG (sizeof(struct user_pt_regs) / sizeof(elf_greg_t))
typedef elf_greg_t elf_gregset_t[ELF_NGREG];

/* floating-point register set */
typedef struct user_fpregs_struct elf_fpregset_t;
/* extended floating-point register set */
typedef struct user_fpxregs_struct elf_fpxregset_t;
#endif

typedef pid_t lwpid_t;
typedef void *psaddr_t;

#ifdef __cplusplus
}
#endif

#endif
