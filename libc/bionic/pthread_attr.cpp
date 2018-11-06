/*
 * Copyright (C) 2008 The Android Open Source Project
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

#include <pthread.h>

#include <inttypes.h>
#include <stdio.h>
#include <sys/resource.h>
#include <unistd.h>
#include <fcntl.h>

#include <async_safe/log.h>

#include "private/bionic_defs.h"
#include "private/bionic_string_utils.h"
#include "private/ErrnoRestorer.h"
#include "pthread_internal.h"

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_init(pthread_attr_t* attr) {
  attr->flags = 0;
  attr->stack_base = nullptr;
  attr->stack_size = PTHREAD_STACK_SIZE_DEFAULT;
  attr->guard_size = PTHREAD_GUARD_SIZE;
  attr->sched_policy = SCHED_NORMAL;
  attr->sched_priority = 0;
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_destroy(pthread_attr_t* attr) {
  memset(attr, 0x42, sizeof(pthread_attr_t));
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_setinheritsched(pthread_attr_t* attr, int flag) {
  if (flag == PTHREAD_EXPLICIT_SCHED) {
    attr->flags &= ~PTHREAD_ATTR_FLAG_INHERIT;
    attr->flags |= PTHREAD_ATTR_FLAG_EXPLICIT;
  } else if (flag == PTHREAD_INHERIT_SCHED) {
    attr->flags |= PTHREAD_ATTR_FLAG_INHERIT;
    attr->flags &= ~PTHREAD_ATTR_FLAG_EXPLICIT;
  } else {
    return EINVAL;
  }
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_getinheritsched(const pthread_attr_t* attr, int* flag) {
  if ((attr->flags & PTHREAD_ATTR_FLAG_INHERIT) != 0) {
    *flag = PTHREAD_INHERIT_SCHED;
  } else if ((attr->flags & PTHREAD_ATTR_FLAG_EXPLICIT) != 0) {
    *flag = PTHREAD_EXPLICIT_SCHED;
  } else {
    // Historical behavior before P, when pthread_attr_setinheritsched was added.
    *flag = (attr->sched_policy != SCHED_NORMAL) ? PTHREAD_EXPLICIT_SCHED : PTHREAD_INHERIT_SCHED;
  }
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_setdetachstate(pthread_attr_t* attr, int state) {
  if (state == PTHREAD_CREATE_DETACHED) {
    attr->flags |= PTHREAD_ATTR_FLAG_DETACHED;
  } else if (state == PTHREAD_CREATE_JOINABLE) {
    attr->flags &= ~PTHREAD_ATTR_FLAG_DETACHED;
  } else {
    return EINVAL;
  }
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_getdetachstate(const pthread_attr_t* attr, int* state) {
  *state = (attr->flags & PTHREAD_ATTR_FLAG_DETACHED) ? PTHREAD_CREATE_DETACHED : PTHREAD_CREATE_JOINABLE;
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_setschedpolicy(pthread_attr_t* attr, int policy) {
  attr->sched_policy = policy;
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_getschedpolicy(const pthread_attr_t* attr, int* policy) {
  *policy = attr->sched_policy;
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_setschedparam(pthread_attr_t* attr, const sched_param* param) {
  attr->sched_priority = param->sched_priority;
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_getschedparam(const pthread_attr_t* attr, sched_param* param) {
  param->sched_priority = attr->sched_priority;
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_setstacksize(pthread_attr_t* attr, size_t stack_size) {
  if (stack_size < PTHREAD_STACK_MIN) {
    return EINVAL;
  }
  attr->stack_size = stack_size;
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_getstacksize(const pthread_attr_t* attr, size_t* stack_size) {
  void* unused;
  return pthread_attr_getstack(attr, &unused, stack_size);
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_setstack(pthread_attr_t* attr, void* stack_base, size_t stack_size) {
  if ((stack_size & (PAGE_SIZE - 1) || stack_size < PTHREAD_STACK_MIN)) {
    return EINVAL;
  }
  if (reinterpret_cast<uintptr_t>(stack_base) & (PAGE_SIZE - 1)) {
    return EINVAL;
  }
  attr->stack_base = stack_base;
  attr->stack_size = stack_size;
  return 0;
}

static uintptr_t __get_main_stack_startstack() {
  int fd = open("/proc/self/stat", O_RDONLY | O_CLOEXEC);
  if (fd == -1) {
    async_safe_fatal("couldn't open /proc/self/stat: %s", strerror(errno));
  }

  char line[BUFSIZ];
  ssize_t rd = read(fd, line, sizeof(line) - 1);
  if (rd == -1) {
    async_safe_fatal("couldn't read /proc/self/stat: %s", strerror(errno));
  }
  line[rd] = '\0';

  close(fd);

  // See man 5 proc. There's no reason comm can't contain ' ' or ')',
  // so we search backwards for the end of it. We're looking for this field:
  //
  //  startstack %lu (28) The address of the start (i.e., bottom) of the stack.
  uintptr_t startstack = 0;
  const char* end_of_comm = strrchr(line, ')');
  if (sscanf(end_of_comm + 1, " %*c "
             "%*d %*d %*d %*d %*d "
             "%*u %*u %*u %*u %*u %*u %*u "
             "%*d %*d %*d %*d %*d %*d "
             "%*u %*u %*d %*u %*u %*u %" SCNuPTR, &startstack) != 1) {
    async_safe_fatal("couldn't parse /proc/self/stat");
  }

  return startstack;
}

static bool __pthread_attr_getstack_main_thread_read_chunk(
    char** src_buf, size_t src_buf_size,
    char** dst_buf, size_t dst_buf_size,
    char sep
) {
  char* sepptr = static_cast<char*>(memchr(*src_buf, sep, src_buf_size));
  bool done = sepptr != nullptr;
  if (!done) {
    sepptr = *src_buf + src_buf_size;
  } else {
    // Also consume the separator. This will be overwritten by \0 below.
    ++sepptr;
  }

  size_t to_copy = sepptr - *src_buf;
  if (to_copy > dst_buf_size) {
    async_safe_fatal("invalid format in /proc/self/maps");
  }
  memcpy(*dst_buf, *src_buf, to_copy);
  if (done) {
    (*dst_buf)[to_copy - 1] = '\0';
  }
  *src_buf += to_copy;
  *dst_buf += to_copy;
  return done;
}

static int __pthread_attr_getstack_main_thread(void** stack_base, size_t* stack_size) {
  ErrnoRestorer errno_restorer;

  rlimit stack_limit;
  if (getrlimit(RLIMIT_STACK, &stack_limit) == -1) {
    return errno;
  }

  // If the current RLIMIT_STACK is RLIM_INFINITY, only admit to an 8MiB stack for sanity's sake.
  if (stack_limit.rlim_cur == RLIM_INFINITY) {
    stack_limit.rlim_cur = 8 * 1024 * 1024;
  }

  // Ask the kernel where our main thread's stack started.
  uintptr_t startstack = __get_main_stack_startstack();

  // Hunt for the region that contains that address.
  int fd = open("/proc/self/maps", O_RDONLY | O_CLOEXEC);
  if (fd == -1) {
    async_safe_fatal("couldn't open /proc/self/maps: %s", strerror(errno));
  }

  char buf[BUFSIZ];

  // 17 to fit 16 hexidecimal digits and a null byte.
  char lo_buf[17];
  char* const lo_buf_end = lo_buf + sizeof(lo_buf);
  char* lo_writeptr = lo_buf;

  char hi_buf[17];
  char* const hi_buf_end = hi_buf + sizeof(hi_buf);
  char* hi_writeptr = hi_buf;

  enum {
    kReadLo,
    kReadHi,
    kReadName,
  } state = kReadLo;

  ssize_t rd;
  while ((rd = read(fd, buf, sizeof(buf))) > 0) {
    char* buf_readptr = buf;
    char* const buf_end = buf + rd;

    while (buf_readptr != buf_end) {
      switch (state) {
        case kReadLo:
          if (__pthread_attr_getstack_main_thread_read_chunk(
              &buf_readptr, buf_end - buf_readptr,
              &lo_writeptr, lo_buf_end - lo_writeptr,
              '-')) {
            lo_writeptr = lo_buf;
            state = kReadHi;
          }
          break;
        case kReadHi:
          if (__pthread_attr_getstack_main_thread_read_chunk(
              &buf_readptr, buf_end - buf_readptr,
              &hi_writeptr, hi_buf_end - hi_writeptr,
              ' ')) {
            char* end;
            uintptr_t lo = strtoll(lo_buf, &end, 16);
            if (*end != '\0') {
              async_safe_fatal("invalid format in /proc/self/maps");
            }

            uintptr_t hi = strtoll(hi_buf, &end, 16);
            if (*end != '\0') {
              async_safe_fatal("invalid format in /proc/self/maps");
            }

            if (lo <= startstack && startstack <= hi) {
              *stack_size = stack_limit.rlim_cur;
              *stack_base = reinterpret_cast<void*>(hi - *stack_size);
              close(fd);
              return 0;
            }
            hi_writeptr = hi_buf;
            state = kReadName;
          }
          break;
        case kReadName: {
          char* newline = static_cast<char*>(
              memchr(buf_readptr, '\n', buf_end - buf_readptr));
          if (newline == nullptr) {
            buf_readptr = buf_end;
          } else {
            // Also consume newline.
            buf_readptr = newline + 1;
            state = kReadLo;
          }
        }
        break;
      }
    }
  }
  async_safe_fatal("Stack not found in /proc/self/maps");
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_getstack(const pthread_attr_t* attr, void** stack_base, size_t* stack_size) {
  *stack_base = attr->stack_base;
  *stack_size = attr->stack_size;
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_setguardsize(pthread_attr_t* attr, size_t guard_size) {
  attr->guard_size = guard_size;
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_getguardsize(const pthread_attr_t* attr, size_t* guard_size) {
  *guard_size = attr->guard_size;
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_getattr_np(pthread_t t, pthread_attr_t* attr) {
  pthread_internal_t* thread = reinterpret_cast<pthread_internal_t*>(t);
  *attr = thread->attr;
  // We prefer reading join_state here to setting thread->attr.flags in pthread_detach.
  // Because data race exists in the latter case.
  if (atomic_load(&thread->join_state) == THREAD_DETACHED) {
    attr->flags |= PTHREAD_ATTR_FLAG_DETACHED;
  }
  // The main thread's stack information is not stored in thread->attr, and we need to
  // collect that at runtime.
  if (thread->tid == getpid()) {
    return __pthread_attr_getstack_main_thread(&attr->stack_base, &attr->stack_size);
  }
  return 0;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_setscope(pthread_attr_t*, int scope) {
  if (scope == PTHREAD_SCOPE_SYSTEM) {
    return 0;
  }
  if (scope == PTHREAD_SCOPE_PROCESS) {
    return ENOTSUP;
  }
  return EINVAL;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_attr_getscope(const pthread_attr_t*, int* scope) {
  *scope = PTHREAD_SCOPE_SYSTEM;
  return 0;
}
