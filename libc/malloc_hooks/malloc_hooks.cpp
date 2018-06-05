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

#include <async_safe/log.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <malloc.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <unwindstack/Regs.h>
#include <unwindstack/RegsGetLocal.h>

#include <mutex>  // std::mutex

#include <private/bionic_malloc_dispatch.h>

#define error_log(format, ...) \
  async_safe_format_log(ANDROID_LOG_ERROR, "libc", (format), ##__VA_ARGS__)

// ------------------------------------------------------------------------
// Global Data
// ------------------------------------------------------------------------
const MallocDispatch* g_dispatch;
// ------------------------------------------------------------------------

// ------------------------------------------------------------------------
// Use C style prototypes for all exported functions. This makes it easy
// to do dlsym lookups during libc initialization when hooks are enabled.
// ------------------------------------------------------------------------
__BEGIN_DECLS

bool hooks_initialize(const MallocDispatch* malloc_dispatch,
                      int* malloc_zygote_child, const char* options);
void hooks_finalize();
void hooks_get_malloc_leak_info(uint8_t** info, size_t* overall_size,
                                size_t* info_size, size_t* total_memory,
                                size_t* backtrace_size);
ssize_t hooks_malloc_backtrace(void* pointer, uintptr_t* frames,
                               size_t frame_count);
void hooks_free_malloc_leak_info(uint8_t* info);
size_t hooks_malloc_usable_size(void* pointer);
void* hooks_malloc(size_t size);
void hooks_free(void* pointer);
void* hooks_memalign(size_t alignment, size_t bytes);
void* hooks_aligned_alloc(size_t alignment, size_t bytes);
void* hooks_realloc(void* pointer, size_t bytes);
void* hooks_calloc(size_t nmemb, size_t bytes);
struct mallinfo hooks_mallinfo();
int hooks_mallopt(int param, int value);
int hooks_posix_memalign(void** memptr, size_t alignment, size_t size);
int hooks_iterate(uintptr_t base, size_t size,
                  void (*callback)(uintptr_t base, size_t size, void* arg),
                  void* arg);
void hooks_malloc_disable();
void hooks_malloc_enable();

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
void* hooks_pvalloc(size_t bytes);
void* hooks_valloc(size_t size);
#endif

size_t Receive(int sock, void* msg, size_t len, int* recv_fd) {
  msghdr msg_hdr = {};
  iovec iov = {msg, len};
  msg_hdr.msg_iov = &iov;
  msg_hdr.msg_iovlen = 1;
  alignas(cmsghdr) char control_buf[256];

  if (recv_fd) {
    msg_hdr.msg_control = control_buf;
    msg_hdr.msg_controllen = static_cast<int>(CMSG_SPACE(sizeof(int)));
  }
  const ssize_t sz = recvmsg(sock, &msg_hdr, 0);
  if (sz < 0) {
    error_log("%s: profhd recvmsg failed %d", getprogname(), errno);
    return 0;
  }

  int* fds = nullptr;
  uint32_t fds_len = 0;

  if (msg_hdr.msg_controllen > 0) {
    for (cmsghdr* cmsg = CMSG_FIRSTHDR(&msg_hdr); cmsg;
         cmsg = CMSG_NXTHDR(&msg_hdr, cmsg)) {
      const size_t payload_len = cmsg->cmsg_len - CMSG_LEN(0);
      if (cmsg->cmsg_level == SOL_SOCKET && cmsg->cmsg_type == SCM_RIGHTS) {
        fds = reinterpret_cast<int*>(CMSG_DATA(cmsg));
        fds_len = static_cast<uint32_t>(payload_len / sizeof(int));
      }
    }
  }

  if (msg_hdr.msg_flags & MSG_TRUNC || msg_hdr.msg_flags & MSG_CTRUNC) {
    for (size_t i = 0; fds && i < fds_len; ++i) close(fds[i]);
    return 0;
  }

  for (size_t i = 0; fds && i < fds_len; ++i) {
    if (recv_fd && i == 0) {
      *recv_fd = fds[i];
    } else {
      close(fds[i]);
    }
  }

  return static_cast<size_t>(sz);
}

int GetPipe() {
  int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock == -1) {
    error_log("%s: profhd socket failed %d", getprogname(), errno);
    return sock;
  }
  /*
  if (fcntl(sock, F_SETFL, fcntl(sock, F_GETFL) | O_NONBLOCK) == -1) {
    error_log("%s: profhd nonblock failed %d", getprogname(), errno);
    return -1;
  }
  */
  struct sockaddr_un name;
  name.sun_family = AF_UNIX;
  strncpy(name.sun_path, "/data/local/tmp/sockfd", sizeof(name.sun_path) - 1);
  if (connect(sock, reinterpret_cast<const struct sockaddr*>(&name),
              sizeof(name)) == -1) {
    error_log("%s: profhd connect failed %d", getprogname(), errno);
    close(sock);
    return -1;
  }
  int tbfd = -1;
  char buf[256];
  Receive(sock, buf, sizeof(buf), &tbfd);
  close(sock);
  return tbfd;
}

void* FindStack() {
  FILE* maps = fopen("/proc/self/maps", "r");
  if (maps == nullptr) {
    error_log("%s: profhd fopen failed %d", getprogname(), errno);
    return nullptr;
  }
  while (!feof(maps)) {
    char line[1024];
    char* data = fgets(line, sizeof(line), maps);
    if (data != nullptr && strstr(data, "[stack]")) {
      char* sep = strstr(data, "-");
      if (sep == nullptr) continue;
      sep++;
      fclose(maps);
      return reinterpret_cast<void*>(strtoll(sep, nullptr, 16));
    }
  }
  fclose(maps);
  error_log("%s: profhd reading stack failed", getprogname());
  return nullptr;
}

void* stackbound;
int pipefd;
int itr;
struct rlimit limit;
unwindstack::Regs* regs;
size_t stack_offset;

struct Metadata {
  unwindstack::ArchEnum arch;
  uint8_t regs[66];
  int64_t pid;
  uint64_t size;
  void* sp;
};

std::mutex mtx;

void SendStack(size_t bytes) {
  if ((++itr % 100) != 0) return;

  if (pipefd == -1 || stackbound == nullptr) {
    return;
  }
  unwindstack::RegsGetLocal(regs);

  pthread_attr_t attr;
  pthread_t t = pthread_self();
  size_t size = 12345;
  uint8_t* sp = reinterpret_cast<uint8_t*>(__builtin_frame_address(0));

  if (gettid() == getpid()) {
    size = reinterpret_cast<uintptr_t>(stackbound) -
           reinterpret_cast<uintptr_t>(sp);
    if (size > limit.rlim_cur) {
      error_log("%s: profhd main out of bounds sp [%p, %p]: %p", getprogname(),
                stackbound,
                reinterpret_cast<uint8_t*>(stackbound) + limit.rlim_cur, sp);
      return;
    }
  } else {
    if (pthread_getattr_np(t, &attr) != 0) {
      error_log("%s: profhd getattr failed %d", getprogname(), errno);
      return;
    }
    uint8_t* x;
    if (pthread_attr_getstack(&attr, reinterpret_cast<void**>(&x), &size) !=
        0) {
      error_log("%s: profhd getstack failed %d", getprogname(), errno);
      return;
    }
    if (sp < x || sp > x + size) {
      error_log("%s: profhd out of bounds sp [%p, %p]: %p", getprogname(), x,
                x + size, sp);
      return;
    }
    size -= sp - x;
  }

  Metadata m;
  m.size = bytes;
  m.pid = getpid();
  m.arch = regs->CurrentArch();
  m.sp = sp;
  memcpy(&m.regs, regs->RawData(), stack_offset);
  struct iovec v[1];
  v[0].iov_base = sp;
  v[0].iov_len = size;
  uint64_t total_size = sizeof(m) + size;
  error_log("perfhd.cc total_size %" PRIu64, total_size);
  std::lock_guard<std::mutex> l(mtx);
  if (write(pipefd, &total_size, sizeof(total_size)) == -1) {
    error_log("%s: profhd size write failed %d", getprogname(), errno);
    return;
  }
  if (write(pipefd, &m, sizeof(m)) == -1) {
    error_log("%s: profhd write failed %d", getprogname(), errno);
    return;
  }
  if (vmsplice(pipefd, v, 1, 0 /* SPLICE_F_NONBLOCK */) == -1) {
    error_log("%s: profhd vmsplice failed %p %zd %d", getprogname(), sp, size,
              errno);
    return;
  }
}

static void* perf_malloc_hook(size_t bytes, const void*) {
  SendStack(bytes);
  return g_dispatch->malloc(bytes);
}

static void* default_malloc_hook(size_t bytes, const void*) {
  return g_dispatch->malloc(bytes);
}

static void* default_realloc_hook(void* pointer, size_t bytes, const void*) {
  return g_dispatch->realloc(pointer, bytes);
}

static void default_free_hook(void* pointer, const void*) {
  g_dispatch->free(pointer);
}

static void* default_memalign_hook(size_t alignment, size_t bytes,
                                   const void*) {
  return g_dispatch->memalign(alignment, bytes);
}

__END_DECLS
// ------------------------------------------------------------------------

bool hooks_initialize(const MallocDispatch* malloc_dispatch, int*,
                      const char*) {
  getrlimit(RLIMIT_STACK, &limit);
  pipefd = GetPipe();
  // fcntl(pipefd, F_SETFL, fcntl(pipefd, F_GETFL) | O_NONBLOCK);
  stackbound = FindStack();
  itr = 0;
  regs = unwindstack::Regs::CreateFromLocal();
  stack_offset = (regs->Is32Bit() ? 1 : 2) * regs->total_regs();
  error_log("profhd: stack offset %zd", stack_offset);
  g_dispatch = malloc_dispatch;
  __malloc_hook = perf_malloc_hook;
  __realloc_hook = default_realloc_hook;
  __free_hook = default_free_hook;
  __memalign_hook = default_memalign_hook;
  return true;
}

void hooks_finalize() {}

void hooks_get_malloc_leak_info(uint8_t** info, size_t* overall_size,
                                size_t* info_size, size_t* total_memory,
                                size_t* backtrace_size) {
  *info = nullptr;
  *overall_size = 0;
  *info_size = 0;
  *total_memory = 0;
  *backtrace_size = 0;
}

void hooks_free_malloc_leak_info(uint8_t*) {}

size_t hooks_malloc_usable_size(void* pointer) {
  return g_dispatch->malloc_usable_size(pointer);
}

void* hooks_malloc(size_t size) {
  if (__malloc_hook != nullptr && __malloc_hook != default_malloc_hook) {
    return __malloc_hook(size, __builtin_return_address(0));
  }
  return g_dispatch->malloc(size);
}

void hooks_free(void* pointer) {
  if (__free_hook != nullptr && __free_hook != default_free_hook) {
    return __free_hook(pointer, __builtin_return_address(0));
  }
  return g_dispatch->free(pointer);
}

void* hooks_memalign(size_t alignment, size_t bytes) {
  if (__memalign_hook != nullptr && __memalign_hook != default_memalign_hook) {
    return __memalign_hook(alignment, bytes, __builtin_return_address(0));
  }
  return g_dispatch->memalign(alignment, bytes);
}

void* hooks_realloc(void* pointer, size_t bytes) {
  if (__realloc_hook != nullptr && __realloc_hook != default_realloc_hook) {
    return __realloc_hook(pointer, bytes, __builtin_return_address(0));
  }
  return g_dispatch->realloc(pointer, bytes);
}

void* hooks_calloc(size_t nmemb, size_t bytes) {
  if (__malloc_hook != nullptr && __malloc_hook != default_malloc_hook) {
    size_t size;
    if (__builtin_mul_overflow(nmemb, bytes, &size)) {
      return nullptr;
    }
    void* ptr = __malloc_hook(size, __builtin_return_address(0));
    if (ptr != nullptr) {
      memset(ptr, 0, size);
    }
    return ptr;
  }
  return g_dispatch->calloc(nmemb, bytes);
}

struct mallinfo hooks_mallinfo() {
  return g_dispatch->mallinfo();
}

int hooks_mallopt(int param, int value) {
  return g_dispatch->mallopt(param, value);
}

void* hooks_aligned_alloc(size_t alignment, size_t size) {
  if (__memalign_hook != nullptr && __memalign_hook != default_memalign_hook) {
    if (!powerof2(alignment)) {
      errno = EINVAL;
      return nullptr;
    }
    void* ptr = __memalign_hook(alignment, size, __builtin_return_address(0));
    if (ptr == nullptr) {
      errno = ENOMEM;
    }
    return ptr;
  }
  return g_dispatch->aligned_alloc(alignment, size);
}

int hooks_posix_memalign(void** memptr, size_t alignment, size_t size) {
  if (__memalign_hook != nullptr && __memalign_hook != default_memalign_hook) {
    if (!powerof2(alignment)) {
      return EINVAL;
    }
    *memptr = __memalign_hook(alignment, size, __builtin_return_address(0));
    if (*memptr == nullptr) {
      return ENOMEM;
    }
    return 0;
  }
  return g_dispatch->posix_memalign(memptr, alignment, size);
}

int hooks_iterate(uintptr_t, size_t, void (*)(uintptr_t, size_t, void*),
                  void*) {
  return 0;
}

void hooks_malloc_disable() {}

void hooks_malloc_enable() {}

ssize_t hooks_malloc_backtrace(void*, uintptr_t*, size_t) { return 0; }

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
void* hooks_pvalloc(size_t bytes) {
  size_t pagesize = getpagesize();
  size_t size = __BIONIC_ALIGN(bytes, pagesize);
  if (size < bytes) {
    // Overflow
    errno = ENOMEM;
    return nullptr;
  }
  return hooks_memalign(pagesize, size);
}

void* hooks_valloc(size_t size) { return hooks_memalign(getpagesize(), size); }
#endif
