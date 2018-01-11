/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <stdio.h>

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

extern "C"
int __openat0(int, const char*, int, int);

extern "C"
int __openat(int dir_fd, const char *path, int flag, int mode) {
  int fd = __openat0(dir_fd, path, flag, mode);

  if (fd >= 0) {  // Let's only log every successful open!
    int kmsg_fd = __openat0(AT_FDCWD, "/dev/kmsg_debug", O_WRONLY | O_APPEND | O_CLOEXEC, 0);
    struct stat s;
    if (fstat(fd, &s) == 0 && s.st_dev == 0xfd01) { // system partition
      char buf[256];

      unsigned len = snprintf(buf, sizeof(buf), "_ %d: 0x%lx %ld\n",
        static_cast<int>(getpid()),
        static_cast<unsigned long>(s.st_dev),
        static_cast<unsigned long>(s.st_ino));
        if (len >= sizeof(buf)) {
          len = sizeof(buf) - 1;
        }
      write(kmsg_fd, buf, len);
    }
    close(kmsg_fd);
  }
  return fd;
}

// original execve in bionic
extern "C"
int execve0(const char* name, char* const* argv, char* const* envp);

extern "C"
int execve(const char* name, char* const* argv, char* const* envp) {
  int kmsg_fd = __openat0(AT_FDCWD, "/dev/kmsg_debug", O_WRONLY | O_APPEND | O_CLOEXEC, 0);

  if (kmsg_fd != -1) {
    char buf[256];
      unsigned len = snprintf(buf, sizeof(buf), "= %d %d: %s\n",
        static_cast<int>(getpid()), static_cast<int>(getppid()), name);
      if (len >= sizeof(buf)) {
        len = sizeof(buf) - 1;
      }
    write(kmsg_fd, buf, len);
    close(kmsg_fd);
  }
  return execve0(name, argv, envp); // execve won't return on success
}
