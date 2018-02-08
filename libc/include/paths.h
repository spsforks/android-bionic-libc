/*
 * Copyright (c) 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)paths.h	8.1 (Berkeley) 6/2/93
 */

#ifndef _PATHS_H_
#define _PATHS_H_

#include <sys/cdefs.h>

__BEGIN_DECLS

/*
 * Code such as popen(3) and system(3) needs to choose between
 * /system/bin/sh and /vendor/bin/sh at runtime, based on the caller.
 */
#define __BIONIC_PATH_BSHELL_SYSTEM "/system/bin/sh"
#define __BIONIC_PATH_BSHELL_VENDOR "/vendor/bin/sh"
#if defined(__ANDROID_NDK__)
#define _PATH_BSHELL __BIONIC_PATH_BSHELL_SYSTEM
#else
const char* __bionic_get_shell_path(void);
#define _PATH_BSHELL __bionic_get_shell_path()
#endif

/*
 * Some code built for /vendor (such as /vendor/bin/sh) needs to override
 * the default path because SELinux restricts what the vendor partition
 * shell is allowed to execute.
 */
#define __BIONIC_PATH_DEFPATH_SYSTEM "/sbin:/system/sbin:/system/bin:/system/xbin:/odm/bin:/vendor/bin:/vendor/xbin"
#define __BIONIC_PATH_DEFPATH_VENDOR "/vendor/bin:/vendor/xbin"
#if defined(__ANDROID_NDK__)
#define _PATH_DEFPATH __BIONIC_PATH_DEFPATH_SYSTEM
#else
const char* __bionic_get_default_path(void);
#define _PATH_DEFPATH __bionic_get_default_path()
#endif

#define	_PATH_CONSOLE	"/dev/console"
#define	_PATH_DEV	"/dev/"
#define	_PATH_DEVNULL	"/dev/null"
#define	_PATH_KLOG	"/proc/kmsg"
#define	_PATH_MOUNTED	"/proc/mounts"
#define	_PATH_TTY	"/dev/tty"

__END_DECLS

#endif
