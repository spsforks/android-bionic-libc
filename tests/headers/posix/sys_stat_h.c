/*
 * Copyright (C) 2017 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <sys/stat.h>

#include "header_checks.h"

static void sys_stat_h() {
  TYPE(struct stat);
  STRUCT_MEMBER(struct stat, dev_t, st_dev);
  STRUCT_MEMBER(struct stat, ino_t, st_ino);
  STRUCT_MEMBER(struct stat, mode_t, st_mode);
  STRUCT_MEMBER(struct stat, nlink_t, st_nlink);
  STRUCT_MEMBER(struct stat, uid_t, st_uid);
  STRUCT_MEMBER(struct stat, gid_t, st_gid);
  STRUCT_MEMBER(struct stat, dev_t, st_rdev);
  STRUCT_MEMBER(struct stat, off_t, st_size);
  STRUCT_MEMBER(struct stat, struct timespec, st_atim);
  STRUCT_MEMBER(struct stat, struct timespec, st_mtim);
  STRUCT_MEMBER(struct stat, struct timespec, st_ctim);
  STRUCT_MEMBER(struct stat, blksize_t, st_blksize);
  STRUCT_MEMBER(struct stat, blkcnt_t, st_blocks);

  TYPE(blkcnt_t);
  TYPE(blksize_t);
  TYPE(dev_t);
  TYPE(ino_t);
  TYPE(mode_t);
  TYPE(nlink_t);
  TYPE(uid_t);
  TYPE(gid_t);
  TYPE(off_t);
  TYPE(time_t);

  TYPE(struct timespec);

#if !defined(st_atime)
#error st_atime
#endif
#if !defined(st_ctime)
#error st_ctime
#endif
#if !defined(st_mtime)
#error st_mtime
#endif

  MACRO(S_IFMT);
  MACRO(S_IFBLK);
  MACRO(S_IFCHR);
  MACRO(S_IFIFO);
  MACRO(S_IFREG);
  MACRO(S_IFDIR);
  MACRO(S_IFLNK);
  MACRO(S_IFSOCK);

  MACRO_VALUE(S_IRWXU, 0700);
  MACRO_VALUE(S_IRUSR, 0400);
  MACRO_VALUE(S_IWUSR, 0200);
  MACRO_VALUE(S_IXUSR, 0100);

  MACRO_VALUE(S_IRWXG, 070);
  MACRO_VALUE(S_IRGRP, 040);
  MACRO_VALUE(S_IWGRP, 020);
  MACRO_VALUE(S_IXGRP, 010);

  MACRO_VALUE(S_IRWXO, 07);
  MACRO_VALUE(S_IROTH, 04);
  MACRO_VALUE(S_IWOTH, 02);
  MACRO_VALUE(S_IXOTH, 01);

  MACRO_VALUE(S_ISUID, 04000);
  MACRO_VALUE(S_ISGID, 02000);
  MACRO_VALUE(S_ISVTX, 01000);

#if !defined(S_ISBLK)
#error S_ISBLK
#endif
#if !defined(S_ISCHR)
#error S_ISCHR
#endif
#if !defined(S_ISDIR)
#error S_ISDIR
#endif
#if !defined(S_ISFIFO)
#error S_ISFIFO
#endif
#if !defined(S_ISREG)
#error S_ISREG
#endif
#if !defined(S_ISLNK)
#error S_ISLNK
#endif
#if !defined(S_ISSOCK)
#error S_ISSOCK
#endif

#if !defined(S_TYPEISMQ)
#error S_TYPEISMQ
#endif
#if !defined(S_TYPEISSEM)
#error S_TYPEISSEM
#endif
#if !defined(S_TYPEISSHM)
#error S_TYPEISSHM
#endif

#if HAVE_TYM
#if !defined(S_TYPEISTMO)
#error S_TYPEISTMO
#endif
#endif

  MACRO(UTIME_NOW);
  MACRO(UTIME_OMIT);

  FUNCTION(chmod, int (*f)(const char*, mode_t));
  FUNCTION(fchmod, int (*f)(int, mode_t));
  FUNCTION(fchmodat, int (*f)(int, const char*, mode_t, int));
  FUNCTION(fstat, int (*f)(int, struct stat*));
  FUNCTION(fstatat, int (*f)(int, const char*, struct stat*, int));
  FUNCTION(futimens, int (*f)(int, const struct timespec[2]));
  FUNCTION(lstat, int (*f)(const char*, struct stat*));
  FUNCTION(lstat, int (*f)(const char*, struct stat*));
  FUNCTION(mkdir, int (*f)(const char*, mode_t));
  FUNCTION(mkdirat, int (*f)(int, const char*, mode_t));
  FUNCTION(mkfifo, int (*f)(const char*, mode_t));
  FUNCTION(mkfifoat, int (*f)(int, const char*, mode_t));
  FUNCTION(mknod, int (*f)(const char*, mode_t, dev_t));
  FUNCTION(mknodat, int (*f)(int, const char*, mode_t, dev_t));
  FUNCTION(stat, int (*f)(const char*, struct stat*));
  FUNCTION(umask, mode_t (*f)(mode_t));
  FUNCTION(utimensat, int (*f)(int, const char*, const struct timespec[2], int));
}
