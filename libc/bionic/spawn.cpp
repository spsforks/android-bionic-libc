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

#include <spawn.h>

#include <stdlib.h>
#include <unistd.h>

enum Action {
  kOpen,
  kClose,
  kDup2
};

struct __posix_spawn_file_action {
  __posix_spawn_file_action* next;

  Action what;
  int fd;
  int new_fd;
  const char* path;
  int flags;
  mode_t mode;
};

struct __posix_spawn_file_actions {
  __posix_spawn_file_action* head;
  __posix_spawn_file_action* last;

  void Do() {
    _exit(127);
  }
};

struct __posix_spawnattr {
  short flags;
  pid_t pgroup;
  sched_param schedparam;
  int schedpolicy;
  sigset_t sigmask;
  sigset_t sigdefault;

  void Do() {
    if (flags & POSIX_SPAWN_RESETIDS) {
      if (seteuid(getuid()) == -1 || setegid(getgid()) == -1) _exit(127);
    }
    if (flags & POSIX_SPAWN_SETPGROUP) {
      if (setpgid(0, pgroup) == -1) _exit(127);
    }
    if (flags & POSIX_SPAWN_SETSIGDEF) {
    }
    if (flags & POSIX_SPAWN_SETSIGMASK) {
    }
    if (flags & POSIX_SPAWN_SETSCHEDPARAM) {
    }
    if (flags & POSIX_SPAWN_SETSCHEDULER) {
//        if (sched_setscheduler(0, schedpolicy, schedparam) == -1) _exit(127);
    }
    if (flags & POSIX_SPAWN_SETSID) {
      if (setsid() == -1) _exit(127);
    }
  }
};

static int posix_spawn(pid_t* pid_ptr,
                       const char* path,
                       const posix_spawn_file_actions_t* actions,
                       const posix_spawnattr_t* attr,
                       char* const argv[],
                       char* const env[],
                       int exec_fn(const char* path, char* const argv[], char* const env[])) {
  // See http://man7.org/linux/man-pages/man3/posix_spawn.3.html
  // and http://pubs.opengroup.org/onlinepubs/9699919799/functions/posix_spawn.html

  short flags = attr ? (*attr)->flags : 0;
  bool use_vfork = ((flags & POSIX_SPAWN_USEVFORK) != 0) || (actions == nullptr && flags == 0);

  pid_t pid = use_vfork ? vfork() : fork();
  if (pid == -1) return errno;

  if (pid == 0) {
    // Child.
    if (attr) (*attr)->Do();
    if (actions) (*actions)->Do();
    exec_fn(path, argv, env ? env : environ);
    _exit(127);
  }

  // Parent.
  if (pid_ptr) *pid_ptr = pid;
  return 0;
}

int posix_spawn(pid_t* pid, const char* path, const posix_spawn_file_actions_t* actions,
                const posix_spawnattr_t* attr, char* const argv[], char* const env[]) {
  return posix_spawn(pid, path, actions, attr, argv, env, execve);
}

int posix_spawnp(pid_t* pid, const char* file, const posix_spawn_file_actions_t* actions,
                 const posix_spawnattr_t* attr, char* const argv[], char* const env[]) {
  return posix_spawn(pid, file, actions, attr, argv, env, execvpe);
}

int posix_spawnattr_init(posix_spawnattr_t* attr) {
  *attr = reinterpret_cast<__posix_spawnattr*>(calloc(1, sizeof(__posix_spawnattr)));
  return (*attr == nullptr) ? errno : 0;
}

int posix_spawnattr_destroy(posix_spawnattr_t* attr) {
  free(*attr);
  *attr = nullptr;
  return 0;
}

int posix_spawnattr_setflags(posix_spawnattr_t* attr, short flags) {
  if ((flags & ~(POSIX_SPAWN_RESETIDS | POSIX_SPAWN_SETPGROUP | POSIX_SPAWN_SETSIGDEF |
                 POSIX_SPAWN_SETSIGMASK | POSIX_SPAWN_SETSCHEDPARAM | POSIX_SPAWN_SETSCHEDULER |
                 POSIX_SPAWN_USEVFORK | POSIX_SPAWN_SETSID)) != 0) {
    return EINVAL;
  }
  (*attr)->flags = flags;
  return 0;
}

int posix_spawnattr_getflags(const posix_spawnattr_t* attr, short* flags) {
  *flags = (*attr)->flags;
  return 0;
}

int posix_spawnattr_setpgroup(posix_spawnattr_t* attr, pid_t pgroup) {
  (*attr)->pgroup = pgroup;
  return 0;
}

int posix_spawnattr_getpgroup(const posix_spawnattr_t* attr, pid_t* pgroup) {
  *pgroup = (*attr)->pgroup;
  return 0;
}

int posix_spawnattr_setsigmask(posix_spawnattr_t* attr, const sigset_t* mask) {
  (*attr)->sigmask = *mask;
  return 0;
}

int posix_spawnattr_getsigmask(const posix_spawnattr_t* attr, sigset_t* mask) {
  *mask = (*attr)->sigmask;
  return 0;
}

int posix_spawnattr_setsigdefault(posix_spawnattr_t* attr, const sigset_t* mask) {
  (*attr)->sigdefault = *mask;
  return 0;
}

int posix_spawnattr_getsigdefault(const posix_spawnattr_t* attr, sigset_t* mask) {
  *mask = (*attr)->sigdefault;
  return 0;
}

int posix_spawnattr_setschedparam(posix_spawnattr_t* attr, const struct sched_param* param) {
  (*attr)->schedparam = *param;
  return 0;
}

int posix_spawnattr_getschedparam(const posix_spawnattr_t* attr, struct sched_param* param) {
  *param = (*attr)->schedparam;
  return 0;
}

int posix_spawnattr_setschedpolicy(posix_spawnattr_t* attr, int policy) {
  (*attr)->schedpolicy = policy;
  return 0;
}

int posix_spawnattr_getschedpolicy(const posix_spawnattr_t* attr, int* policy) {
  *policy = (*attr)->schedpolicy;
  return 0;
}

int posix_spawn_file_actions_init(posix_spawn_file_actions_t* actions) {
  *actions = reinterpret_cast<__posix_spawn_file_actions*>(calloc(1, sizeof(**actions)));
  return (*actions == nullptr) ? errno : 0;
}

int posix_spawn_file_actions_destroy(posix_spawn_file_actions_t* actions) {
  __posix_spawn_file_action* a = (*actions)->head;
  while (a) {
    __posix_spawn_file_action* last = a;
    a = a->next;
    free(last);
  }
  free(*actions);
  *actions = nullptr;
  return 0;
}

static int posix_spawn_add_file_action(posix_spawn_file_actions_t* actions,
                                       Action what,
                                       int fd,
                                       int new_fd,
                                       const char* path,
                                       int flags,
                                       mode_t mode) {
  if (fd < 0 || new_fd < 0) return EBADF;

  __posix_spawn_file_action* action = new __posix_spawn_file_action;
  if (action == nullptr) return errno;

  // TODO: instead allocate a char[] at the end?
  if (path != nullptr) {
    action->path = strdup(path);
    if (action->path == nullptr) {
      free(action);
      return errno;
    }
  }
  action->what = what;
  action->fd = fd;
  action->new_fd = new_fd;
  action->flags = flags;
  action->mode = mode;

  if ((*actions)->head == nullptr) {
    (*actions)->head = (*actions)->last = action;
  } else {
    (*actions)->last->next = action;
    (*actions)->last = action;
  }

  return 0;
}

int posix_spawn_file_actions_addopen(posix_spawn_file_actions_t* actions,
                                     int fd, const char* path, int flags, mode_t mode) {
  return posix_spawn_add_file_action(actions, kOpen, fd, 0, path, flags, mode);
}

int posix_spawn_file_actions_addclose(posix_spawn_file_actions_t* actions, int fd) {
  return posix_spawn_add_file_action(actions, kClose, fd, 0, nullptr, 0, 0);
}

int posix_spawn_file_actions_adddup2(posix_spawn_file_actions_t* actions, int fd, int new_fd) {
  return posix_spawn_add_file_action(actions, kDup2, fd, new_fd, nullptr, 0, 0);
}
