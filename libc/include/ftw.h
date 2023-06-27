/* $NetBSD: ftw.h,v 1.1 2005/12/30 23:07:33 agc Exp $ */

/*	From OpenBSD: ftw.h,v 1.1 2003/07/21 21:13:18 millert Exp 	*/

/*
 * Copyright (c) 2003 Todd C. Miller <Todd.Miller@courtesan.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Sponsored in part by the Defense Advanced Research Projects
 * Agency (DARPA) and Air Force Research Laboratory, Air Force
 * Materiel Command, USAF, under agreement number F39502-99-1-0512.
 */

#ifndef	_FTW_H
#define	_FTW_H

#include <sys/cdefs.h>
#include <sys/stat.h>
#include <sys/types.h>

/*
 * Valid flags for the 3rd argument to the function that is passed as the
 * second argument to ftw(3) and nftw(3).  Say it three times fast!
 */
#define	FTW_F		0	/* File.  */
#define	FTW_D		1	/* Directory.  */
#define	FTW_DNR		2	/* Directory without read permission.  */
#define	FTW_DP		3	/* Directory with subdirectories visited.  */
#define	FTW_NS		4	/* Unknown type; stat() failed.  */
#define	FTW_SL		5	/* Symbolic link.  */
#define	FTW_SLN		6	/* Sym link that names a nonexistent file.  */

/*
 * Flags for use as the 4th argument to nftw(3).  These may be ORed together.
 */
#define	FTW_PHYS	0x01	/* Physical walk, don't follow sym links.  */
#define	FTW_MOUNT	0x02	/* The walk does not cross a mount point.  */
#define	FTW_DEPTH	0x04	/* Subdirs visited before the dir itself. */
#define	FTW_CHDIR	0x08	/* Change to a directory before reading it. */

struct FTW {
	int base;
	int level;
};

__BEGIN_DECLS
int ftw(const char* _Nonnull __dir_path, int (* _Nonnull __callback)(const char* _Nonnull, const struct stat* _Nonnull, int), int __max_fd_count);
int nftw(const char* _Nonnull __dir_path, int (* _Nonnull __callback)(const char* _Nonnull, const struct stat* _Nonnull, int, struct FTW* _Nonnull), int __max_fd_count, int __flags);
int ftw64(const char* _Nonnull __dir_path, int (* _Nonnull __callback)(const char* _Nonnull, const struct stat64* _Nonnull, int), int __max_fd_count);
int nftw64(const char* _Nonnull __dir_path, int (* _Nonnull __callback)(const char* _Nonnull, const struct stat64* _Nonnull, int, struct FTW* _Nonnull), int __max_fd_count, int __flags);
__END_DECLS

#endif
