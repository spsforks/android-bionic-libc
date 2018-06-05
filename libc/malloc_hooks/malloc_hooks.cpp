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
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/uio.h>
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
//
std::string GetName(int fd) {
  std::string path = "/proc/self/fd/";
  path += std::to_string(fd);
  char buf[512];
  readlink(path.c_str(), buf, sizeof(buf));
  return std::string(buf, sizeof(buf));
}

using TimeSeconds = std::chrono::seconds;
using TimeMillis = std::chrono::milliseconds;
using TimeNanos = std::chrono::nanoseconds;

inline TimeNanos FromPosixTimespec(const struct timespec& ts) {
  return TimeNanos(ts.tv_sec * 1000000000LL + ts.tv_nsec);
}

constexpr clockid_t kWallTimeClockSource = CLOCK_MONOTONIC;

inline TimeNanos GetTimeInternalNs(clockid_t clk_id) {
  struct timespec ts = {};
  clock_gettime(clk_id, &ts);
  return FromPosixTimespec(ts);
}

inline TimeNanos GetWallTimeNs() {
  return GetTimeInternalNs(kWallTimeClockSource);
}

using TimeMicros = std::chrono::microseconds;
inline TimeMicros GetWallTimeUs() {
  return std::chrono::duration_cast<TimeMicros>(GetWallTimeNs());
}

__BEGIN_DECLS

bool hooks_initialize(const MallocDispatch* malloc_dispatch, int* malloc_zygote_child,
                      const char* options);
void hooks_finalize();
void hooks_get_malloc_leak_info(uint8_t** info, size_t* overall_size, size_t* info_size,
                                size_t* total_memory, size_t* backtrace_size);
ssize_t hooks_malloc_backtrace(void* pointer, uintptr_t* frames, size_t frame_count);
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
                  void (*callback)(uintptr_t base, size_t size, void* arg), void* arg);
void hooks_malloc_disable();
void hooks_malloc_enable();
bool hooks_write_malloc_leak_info(FILE*);

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
void* hooks_pvalloc(size_t bytes);
void* hooks_valloc(size_t size);
#endif
__END_DECLS

int GetPipe() {
  int sock = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock == -1) {
    error_log("%s: profhd socket failed %d", getprogname(), errno);
    return sock;
  }
  struct sockaddr_un name;
  name.sun_family = AF_UNIX;
  strncpy(name.sun_path, "/data/local/tmp/sockfd", sizeof(name.sun_path) - 1);
  if (connect(sock, reinterpret_cast<const struct sockaddr*>(&name), sizeof(name)) == -1) {
    error_log("%s: profhd connect failed %d", getprogname(), errno);
    close(sock);
    return -1;
  }
  return sock;
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
uint64_t* free_page;
size_t free_page_offset;
size_t free_page_size;
pid_t init_pid;
bool firstinit = true;

constexpr size_t kFreePageBytes = 4096;
constexpr uint8_t kAlloc = 1;
constexpr uint8_t kFree = 2;
struct MetadataHeader {
  uint8_t type;
  uint64_t pid;
};

struct AllocMetadata {
  MetadataHeader header;
  unwindstack::ArchEnum arch;
  uint8_t regs[264];
  uint64_t size;
  // These are pointers, but to support mixed 32 and 64 bit mode,
  // these have to be uint64_t.
  uint64_t sp;
  uint64_t sp_offset;
  uint64_t addr;
  uint64_t last_timing;
};

std::mutex mtx;

void CloseFD() {
  error_log("%s: profhd closing socket %d", getprogname(), errno);
  errno = 0;
  if (close(pipefd) == -1) errno = 0;
  pipefd = -1;
}

inline void AddToFreePage(const void* addr) {
  if (pipefd == -1) {
    error_log("%s: profhd free pipe problem", getprogname());
    return;
  }
  std::lock_guard<std::mutex> l(mtx);
  if (free_page_offset == free_page_size) {
    size_t written = 0;
    do {
      ssize_t wr = send(pipefd, free_page + written, kFreePageBytes - written, MSG_NOSIGNAL);
      if (wr == -1 && errno != EINTR) {
        CloseFD();
        return;
      }
      written += wr;
    } while (written < kFreePageBytes);

    free_page_offset = 4;
  }
  free_page[free_page_offset++] = reinterpret_cast<uint64_t>(addr);
}

uint64_t last_timing = 0;
std::atomic<uint64_t> accum(0);

void SendStack(size_t bytes, void* addr, const void* stackptr) {
  if (getpid() != init_pid) {
    error_log("%s: profhd fork problem", getprogname());
    close(pipefd);
    pipefd = GetPipe();
    init_pid = getpid();
  }

  if (pipefd == -1 || stackbound == nullptr) {
    error_log("%s: profhd pipe problem", getprogname());
    return;
  }

  pthread_attr_t attr;
  pthread_t t = pthread_self();
  ssize_t size = 12345;

  void* x = alloca(sizeof(uint64_t) + sizeof(AllocMetadata));
  uint64_t* preamble = reinterpret_cast<uint64_t*>(x);
  uint8_t* sp = reinterpret_cast<uint8_t*>(preamble);

  if (gettid() == getpid()) {
    size = reinterpret_cast<uintptr_t>(stackbound) - reinterpret_cast<uintptr_t>(sp);
    if (size > limit.rlim_cur) {
      error_log("%s: profhd main out of bounds sp [%p, %p]: %p", getprogname(), stackbound,
                reinterpret_cast<uint8_t*>(stackbound) + limit.rlim_cur, sp);
      return;
    }
  } else {
    if (pthread_getattr_np(t, &attr) != 0) {
      error_log("%s: profhd getattr failed %d", getprogname(), errno);
      return;
    }
    uint8_t* x;
    size_t s;
    if (pthread_attr_getstack(&attr, reinterpret_cast<void**>(&x), &s) != 0) {
      error_log("%s: profhd getstack failed %d", getprogname(), errno);
      return;
    }
    if (sp < x || sp > x + s) {
      error_log("%s: profhd out of bounds sp [%p, %p]: %p", getprogname(), x, x + size, sp);
      return;
    }
    size = s - (sp - x);
  }

  if (size <= 0) error_log("profhd: negative size");

  *preamble = size - sizeof(uint64_t);
  reinterpret_cast<AllocMetadata*>(sp + sizeof(uint64_t))->header.type = kFree;
  reinterpret_cast<AllocMetadata*>(sp + sizeof(uint64_t))->header.pid = getpid();
  reinterpret_cast<AllocMetadata*>(sp + sizeof(uint64_t))->header.type = kAlloc;
  reinterpret_cast<AllocMetadata*>(sp + sizeof(uint64_t))->size = bytes;
  reinterpret_cast<AllocMetadata*>(sp + sizeof(uint64_t))->header.pid = getpid();
  reinterpret_cast<AllocMetadata*>(sp + sizeof(uint64_t))->arch = regs->CurrentArch();
  reinterpret_cast<AllocMetadata*>(sp + sizeof(uint64_t))->sp = reinterpret_cast<uint64_t>(stackptr);
  reinterpret_cast<AllocMetadata*>(sp + sizeof(uint64_t))->sp_offset =
      (reinterpret_cast<const uint8_t*>(stackptr) - reinterpret_cast<uint8_t*>(sp)) -
      sizeof(uint64_t);
  reinterpret_cast<AllocMetadata*>(sp + sizeof(uint64_t))->addr = reinterpret_cast<uint64_t>(addr);
  reinterpret_cast<AllocMetadata*>(sp + sizeof(uint64_t))->last_timing = last_timing;

  memcpy(&reinterpret_cast<AllocMetadata*>(&preamble[1])->regs, regs->RawData(), stack_offset);

  auto start = GetWallTimeUs();
  size_t written = 0;
  do {
    ssize_t wr = send(pipefd, sp + written, size - written, MSG_NOSIGNAL);
    if (wr == -1) {
      error_log("%s: profhd send to %d=%s failed %p %zd=%zd %d", getprogname(), pipefd,
                GetName(pipefd).c_str(), sp, size, wr, errno);
      CloseFD();
      return;
    }
    written += wr;
  } while (written < size);

  if (*reinterpret_cast<uint64_t*>(sp) != written - sizeof(uint64_t)) {
    error_log("profhd: Invalid preamble.");
    CloseFD();
  }
  auto timing = GetWallTimeUs() - start;
  last_timing = timing.count();
}

inline void MaybeSendStack(size_t bytes, void* addr) {
  if (itr++ % 100 == 0) {
    uint64_t a = accum.exchange(0);
    std::lock_guard<std::mutex> l(mtx);
    unwindstack::RegsGetLocal(regs);
    const void* sp = __builtin_frame_address(0);
    SendStack(bytes + a, addr, sp);
  } else {
    accum += bytes;
  }
}

static void* perf_malloc_hook(size_t bytes, const void*) {
  void* addr = g_dispatch->malloc(bytes);
  MaybeSendStack(bytes, addr);
  return addr;
}

static void* default_malloc_hook(size_t bytes, const void*) {
  return g_dispatch->malloc(bytes);
}

static void* default_realloc_hook(void* pointer, size_t bytes, const void*) {
  return g_dispatch->realloc(pointer, bytes);
}

static void* perf_realloc_hook(void* pointer, size_t bytes, const void*) {
  AddToFreePage(pointer);
  void* addr = g_dispatch->realloc(pointer, bytes);
  MaybeSendStack(bytes, addr);
  return addr;
}

static void perf_free_hook(void* pointer, const void*) {
  AddToFreePage(pointer);
  g_dispatch->free(pointer);
}

static void default_free_hook(void* pointer, const void*) {
  g_dispatch->free(pointer);
}

static void* default_memalign_hook(size_t alignment, size_t bytes, const void*) {
  void* addr = g_dispatch->memalign(alignment, bytes);
  return addr;
}

void* perf_memalign_hook(size_t alignment, size_t bytes, const void*) {
  void* addr = g_dispatch->memalign(alignment, bytes);
  MaybeSendStack(bytes, addr);
  return addr;
}

// ------------------------------------------------------------------------

void after_fork() {
  close(pipefd);
  free(free_page);
  pipefd = -1;
  //  error_log("%s %d: profhd resetting malloc hooks after fork", getprogname(),
  //            getpid());
  __malloc_hook = default_malloc_hook;
  __realloc_hook = default_realloc_hook;
  __free_hook = default_free_hook;
  __memalign_hook = default_memalign_hook;
  hooks_initialize(g_dispatch, nullptr, nullptr);
  errno = 0;
}

bool hooks_initialize(const MallocDispatch* malloc_dispatch, int*, const char*) {
  if (firstinit) {
    pthread_atfork(nullptr, nullptr, after_fork);
  }
  firstinit = false;
  init_pid = getpid();
  if (posix_memalign(reinterpret_cast<void**>(&free_page), 4096, kFreePageBytes) != 0) {
    error_log("%s: profhd failed to allocate free page.", getprogname());
    return false;
  }
  free_page_offset = 0;
  static_assert(sizeof(uint64_t) == sizeof(*free_page), "too large metadata");
  *reinterpret_cast<uint64_t*>(free_page) = kFreePageBytes - sizeof(uint64_t);
  free_page_offset++;
  reinterpret_cast<MetadataHeader*>(&free_page[free_page_offset])->type = kFree;
  reinterpret_cast<MetadataHeader*>(&free_page[free_page_offset])->pid = getpid();
  static_assert(sizeof(MetadataHeader) <= 3 * sizeof(*free_page), "too large metadata");
  free_page_offset += 3;

  free_page_size = kFreePageBytes / sizeof(*free_page);

  getrlimit(RLIMIT_STACK, &limit);
  pipefd = GetPipe();
  if (pipefd == -1) return false;
  // fcntl(pipefd, F_SETFL, fcntl(pipefd, F_GETFL) | O_NONBLOCK);
  stackbound = FindStack();
  itr = 0;
  regs = unwindstack::Regs::CreateFromLocal();
  stack_offset = (regs->Is32Bit() ? 4 : 8) * regs->total_regs();
  g_dispatch = malloc_dispatch;
  __malloc_hook = perf_malloc_hook;
  __realloc_hook = perf_realloc_hook;
  __free_hook = perf_free_hook;
  __memalign_hook = perf_memalign_hook;
  error_log("%s: profhd enabled.", getprogname());
  return true;
}

void hooks_finalize() {
}

void hooks_get_malloc_leak_info(uint8_t** info, size_t* overall_size, size_t* info_size,
                                size_t* total_memory, size_t* backtrace_size) {
  *info = nullptr;
  *overall_size = 0;
  *info_size = 0;
  *total_memory = 0;
  *backtrace_size = 0;
}

void hooks_free_malloc_leak_info(uint8_t*) {
}

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

int hooks_iterate(uintptr_t, size_t, void (*)(uintptr_t, size_t, void*), void*) {
  return 0;
}

void hooks_malloc_disable() {
}

void hooks_malloc_enable() {
}

ssize_t hooks_malloc_backtrace(void*, uintptr_t*, size_t) {
  return 0;
}

bool hooks_write_malloc_leak_info(FILE*) {
  return true;
}

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

void* hooks_valloc(size_t size) {
  return hooks_memalign(getpagesize(), size);
}
#endif
