/*
 * stat_aarch64.c
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

#include <sys/stat.h>
#include <fcntl.h>

extern int mknodat(int dfd, const char * filename, mode_t mode, dev_t dev);
/*
 * AT_FDCWD is used so that path is interpreted relative to the current
 * working directory of the calling process, as is done by chmod for a
 * relative pathname. If path is absolute then the AT_FDCWD parameter is
 * ignored.
 */
int chmod(const char *path, mode_t mode)
{
    return fchmodat(AT_FDCWD, path, mode, 0);
}

int mkdir(const char *path, mode_t mode)
{

    return mkdirat(AT_FDCWD, path, mode);
}

int mknod(const char *path, mode_t mode, dev_t dev)
{

    return mknodat(AT_FDCWD, path, mode, dev);
}
