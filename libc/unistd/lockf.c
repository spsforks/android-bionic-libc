/*
 * Copyright (C) 2012 The Android Open Source Project
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
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

/* System call declaration - see SYSCALLS.TXT */
extern int __fcntl64(int, int, void*);

int lockf64(int fd, int cmd, off64_t len)
{
    struct flock64 fl;
    int fcmd;

    memset(&fl, 0, sizeof(fl));

    fl.l_len    = len;
    fl.l_start  = 0;
    fl.l_whence = SEEK_CUR;

    switch (cmd) {
    case F_ULOCK: /* unlock */
        fl.l_type = F_UNLCK;
        fcmd = F_SETLK64;
        break;

    case F_LOCK:  /* lock, with wait */
        fl.l_type = F_WRLCK;
        fcmd = F_SETLKW64;
        break;

    case F_TLOCK: /* try lock, no wait */
        fl.l_type = F_WRLCK;
        fcmd = F_SETLK64;
        break;

    case F_TEST:  /* test lock */
        // Note: GLibc uses F_RDLCK here for testing, while FreeBSD
        // uses F_WRLCK. Use the GLibc convention, in case some Linux
        // programs depend on this subtle difference.
        fl.l_type = F_RDLCK;
        if (__fcntl64(fd, F_GETLK64, &fl) < 0)
            return -1;
        if (fl.l_type == F_UNLCK || fl.l_pid == getpid())
            return 0;
        // Note: FreeBSD returns EAGAIN, GLibc returns EACCES.
        errno = EACCES;
        return -1;

    default:
        errno = EINVAL;
        return -1;
    }
    return __fcntl64(fd, fcmd, &fl);
}

int lockf(int fd, int cmd, off_t len)
{
    return lockf64(fd, cmd, (off64_t)len);
}
