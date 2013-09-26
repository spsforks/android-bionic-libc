/*
 * unistd_aarch64.c
 *
 * Copyright (C) 2013 ARM Ltd.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * *Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * *Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in
 * the documentation and/or other materials provided with the
 * distribution.
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

#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

extern int __sys_clone (unsigned long, void*, int*, void*, int*);
extern int linkat (int, const char*, int, const char*, int);
extern int symlinkat(const char *oldname, int newdfd, const char *newname);
extern int readlinkat(int dfd, const char *path, char *buf,int bufsiz);
extern int dup3(int, int, int);
extern int pipe2(int*, int);

/*
 * AT_FDCWD is used so that the path is interpreted relative to the current
 * working directory of the calling process. If path is absolute then
 * AT_FDCWD parameter is ignored.
 */
int access(const char *pathname, int mode)
{
    return faccessat(AT_FDCWD, pathname, mode, 0);
}

int chown(const char *path, uid_t owner, gid_t group)
{
    return fchownat(AT_FDCWD, path, owner, group, 0);
}

/*
 * AT_SYMLINK_NOFOLLOW flag specifies that the symbolic link should not be
 * dereferenced.
 */
int lchown(const char *path, uid_t owner, gid_t group)
{
    return fchownat(AT_FDCWD, path, owner, group, AT_SYMLINK_NOFOLLOW);
}

int dup2(int oldfd, int newfd)
{
    return dup3(oldfd, newfd, 0);
}

/*
 * When SIGCHLD with all the other parameters set to NULL are passed to
 * sys_clone() - the call will behave as a fork(), with copy-on-write
 * semantics.
 */
int __fork()
{
    return __sys_clone(SIGCHLD, NULL, NULL, NULL, NULL);
}

int link(const char *oldpath, const char *newpath)
{
    return linkat(AT_FDCWD, oldpath, AT_FDCWD, newpath, 0);
}

int stat(const char *path, struct stat *buf)
{
    return fstatat(AT_FDCWD, path, buf, 0);
}

int lstat(const char *path, struct stat *buf)
{
    return fstatat(AT_FDCWD, path, buf, AT_SYMLINK_NOFOLLOW);
}

int symlink(const char *oldname, const char *newname)
{

    return symlinkat(oldname, AT_FDCWD, newname);
}

int readlink(const char *path, char *buf, size_t bufsiz)
{

    return readlinkat(AT_FDCWD, path, buf, bufsiz);
}

int unlink(const char *pathname)
{
    return unlinkat(AT_FDCWD, pathname, 0);
}

int rmdir(const char *pathname)
{
    return unlinkat(AT_FDCWD, pathname, AT_REMOVEDIR);
}

int pipe(int pipefd[2])
{
    return pipe2(pipefd, 0);
}

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
           struct timeval *timeout)
{
    struct timespec tv, *tv_timeout = NULL;

    if (timeout != NULL) {
        tv_timeout = &tv;
        tv.tv_sec = timeout->tv_sec;
        tv.tv_nsec = timeout->tv_usec * 1000;
        /* check for overflow */
        if (tv.tv_nsec/1000 != timeout->tv_usec)
            tv.tv_nsec = LONG_MAX;
    }
    return pselect(nfds, readfds, writefds, exceptfds, tv_timeout, NULL);
}
