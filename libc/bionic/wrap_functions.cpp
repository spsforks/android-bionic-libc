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

#define execve __real_execve

#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

extern const char* __progname;

//#define SYSCALL_HOOK_IMPL
//#include <private/syscall_hook.h>

//#ifdef SYSCALL_HOOK_OPENAT
// original __openat in bionic
extern "C" int __real___openat(int, const char*, int, int);

#define DEFAULT_LOG_DEV "/data/log/system-files-accessed"
#define KMSG_LOG_DEV "/dev/kmsg_debug"

/*
  If there's a world-writable file DEFAULT_LOG_DEV, we log there.
  Otherwise, we log to "/dev/kmsg_debug", which only exists in userdebug or
  eng build, and SELinux needs to set to permissive.
*/
static const char *get_log_dev() {
  int fd = __real___openat(AT_FDCWD, DEFAULT_LOG_DEV, O_WRONLY | O_APPEND, 0);

  if (fd != -1) {
    close(fd);
    return DEFAULT_LOG_DEV;
  }
  return KMSG_LOG_DEV;
}

static void logf(const char *fmt, ...) {
  static const char *log_dev = get_log_dev();

  int fd = __real___openat(AT_FDCWD, log_dev, O_WRONLY | O_APPEND, 0);
  if (fd != -1) {
    char buf[256];
    va_list ap;
    va_start(ap, fmt);
    unsigned len = vsnprintf(buf, sizeof(buf), fmt, ap);
    if (len >= sizeof(buf)) {
      len = sizeof(buf) - 1;
    }
    va_end(ap);
    write(fd, buf, len);
    close(fd);
  }
}

static dev_t get_sys_dev_id(void) {
  dev_t did = -1;
  struct stat s;

#ifdef USE_FSTAT_ON_SYS
  int fd = __real___openat(AT_FDCWD, "/system", O_RDONLY, 0);

  if (fd != -1) {
    if (fstat(fd, &s) == 0) {
      did = s.st_dev;
    }
    close(fd);
  }
#else
  if (stat("/system", &s) == 0) {
    did = s.st_dev;
  }
#endif
  logf("_I %d (%s) /system directory is on Device ID 0x%x\n",
    static_cast<int>(getpid()), __progname, did);

  return did;
}

/* Only files under "/system" are logged here. */
extern "C" int __wrap___openat(int dir_fd, const char *path, int flag, int mode) {
  static const dev_t system_devid = get_sys_dev_id();
  int fd = __real___openat(dir_fd, path, flag, mode);
  struct stat s;

  if (fd == -1) {
//    logf("! %d: failed to open %s!\n", static_cast<int>(getpid()), path);
  } else if (fstat(fd, &s) == -1) {
//    logf("! %d: failed to fstat %s!\n", static_cast<int>(getpid()), path);
  } else if (s.st_dev == system_devid) {
    logf("_ %d (%s): %ld, %s\n", static_cast<int>(getpid()), __progname,
      static_cast<unsigned long>(s.st_ino), path);
  }

  return fd;
}
//#endif  // SYSCALL_HOOK_OPENAT

//#ifdef SYSCALL_HOOK_EXECVE
// original execve in bionic
//extern "C" int __real_execve(const char* name, char* const* argv, char* const* envp);

extern "C" int __wrap_execve(const char* name, char* const* argv, char* const* envp) {
  logf("__ %d %d: %s %s\n",
    static_cast<int>(getpid()), static_cast<int>(getppid()), name, argv[1]);

  return __real_execve(name, argv, envp);  // execve won't return on success
}
//#endif  // SYSCALL_HOOK_EXECVE

//#ifdef SYSCALL_HOOK_CLONE
// original __bionic_clone in bionic
extern "C" pid_t __real___bionic_clone(uint32_t flags, void* child_stack, int* parent_tid, void* tls, int* child_tid, int (*fn)(void*), void* arg);

extern "C" pid_t __wrap___bionic_clone(uint32_t flags, void* child_stack, int* parent_tid, void* tls, int* child_tid, int (*fn)(void*), void* arg) {
  pid_t pid = __real___bionic_clone(flags, child_stack, parent_tid, tls, child_tid, fn, arg);

  if (pid != 0) {
    // parent returns non-zero
    logf("= %d (%s) is cloned; %d is created.\n", static_cast<int>(getpid()),
      __progname, pid);
  }
  // child returns 0 and let the parent log

  return pid;
}

//#endif  // SYSCALL_HOOK_CLONE
