/*
 * Copyright (C) 2015 The Android Open Source Project
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/auxv.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/system_properties.h>
#include <unistd.h>

#include "private/bionic_safestack.h"

#include "private/bionic_tls.h"
#include "private/bionic_prctl.h"
#include "private/libc_logging.h"
#include "private/ScopedPthreadMutexLocker.h"

// Default size of the unsafe stack. This value is only used if the stack
// size rlimit is set to infinity.
static constexpr size_t kDefaultUnsafeStackSize = 0x2800000;

#ifdef __LP64__
static constexpr size_t kTopGuardPageSize = PAGE_SIZE;
#else
// Without address randomization, top guard page of the unsafe stack will be
// always adjacent to the guard page of the normal stack, which makes it
// unnecessary.
static constexpr size_t kTopGuardPageSize = 0;
#endif


extern "C" void** __safestack_pointer_address() {
  return &(__get_tls()[TLS_SLOT_SAFESTACK]);
}

#ifdef __LP64__
static pthread_mutex_t random_mu = PTHREAD_MUTEX_INITIALIZER;
static uintptr_t random_state;
static uintptr_t random_mask;

static void safestack_random_init_locked() {
  random_state = *reinterpret_cast<uintptr_t*>(getauxval(AT_RANDOM));
  unsigned zero_bits = __builtin_clzl(reinterpret_cast<uintptr_t>(&zero_bits));
  random_mask = (static_cast<uintptr_t>(-1L) >> zero_bits) & ~(PAGE_SIZE - 1);
}

static uintptr_t safestack_random() {
  ScopedPthreadMutexLocker lock(&random_mu);
  if (!random_mask) safestack_random_init_locked();
  random_state = random_state * 6364136223846793005UL + 1442695040888963407UL;
  return random_state & random_mask;
}

static void* randomized_stack_mmap(size_t mmap_size, int prot, int flags) {
  const int kAttempts = 100;
  void* space = nullptr;
  for (int i = 0; i < kAttempts; ++i) {
    if (space) munmap(space, mmap_size);
    uintptr_t addr = safestack_random();
    space = mmap(reinterpret_cast<void*>(addr), mmap_size, prot, flags, -1, 0);
    if (space == MAP_FAILED) break;
    if (reinterpret_cast<uintptr_t>(space) == addr) return space;
  }
  if (space == MAP_FAILED) {
    __libc_fatal(
      "randomized_stack_mmap failed: couldn't allocate %zu-bytes mapped space: "
      "%s",
      mmap_size, strerror(errno));
  }
  __libc_format_log(ANDROID_LOG_WARN, "safestack",
    "randomized_stack_mmap failed: couldn't allocate %zu-bytes mapped space at "
    "a preferred address in %d attempts, falling back to the regular mmap",
    mmap_size, kAttempts);
  return space;
}
#else // __LP64__
// Not random at all on 32-bit.
static void* randomized_stack_mmap(size_t mmap_size, int prot, int flags) {
  void* space = mmap(nullptr, mmap_size, prot, flags, -1, 0);
  if (space == MAP_FAILED) {
    __libc_fatal(
      "unsafe_stack_alloc failed: couldn't allocate %zu-bytes mapped space: %s",
      mmap_size, strerror(errno));
  }
  return space;
}
#endif // __LP64__

void __unsafe_stack_alloc(pthread_internal_t* thr, size_t mmap_size,
                          size_t stack_guard_size) {
  const unsigned kNameBufferSize = 32;
  mmap_size = BIONIC_ALIGN(mmap_size + kTopGuardPageSize, PAGE_SIZE);

  // Create a new private anonymous map.
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE;
  char* space =
    reinterpret_cast<char*>(randomized_stack_mmap(mmap_size, prot, flags));
  char* stack_top = space + mmap_size - kTopGuardPageSize;
  char* stack_bottom = space + stack_guard_size;

  // Stack is at the lower end of mapped space, stack guard region is at the
  // lower end of stack.
  // Set the stack guard region to PROT_NONE, so we can detect thread stack
  // overflow.
  if (mprotect(space, stack_guard_size, PROT_NONE) == -1) {
    __libc_fatal(
      "unsafe_stack_alloc failed: couldn't mprotect PROT_NONE "
      "%zu-byte stack guard region: %s",
      stack_guard_size, strerror(errno));
  }
  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, space, stack_guard_size,
        "unsafe stack guard page");

  if (kTopGuardPageSize) {
    if (mprotect(stack_top, kTopGuardPageSize, PROT_NONE) == -1) {
      __libc_fatal(
        "unsafe_stack_alloc failed: couldn't mprotect PROT_NONE "
        "%zu-byte stack guard region: %s",
        stack_guard_size, strerror(errno));
    }
    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, stack_top, kTopGuardPageSize,
          "unsafe stack top guard page");
  }

  char* stack_name = stack_top - kNameBufferSize;
  snprintf(stack_name, kNameBufferSize, "unsafe stack:%d",
           static_cast<int>(thr->tid));
  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, stack_bottom,
        stack_top - stack_bottom, stack_name);

  thr->unsafe_stack_start = space;
  thr->unsafe_stack_size = mmap_size;

  __get_tls()[TLS_SLOT_SAFESTACK] = stack_top;
}

void __unsafe_stack_free(pthread_internal_t* thr) {
  munmap(thr->unsafe_stack_start, thr->unsafe_stack_size);
  // Just in case...
  thr->unsafe_stack_start = nullptr;
  thr->unsafe_stack_size = 0;
}

void __unsafe_stack_main_thread_init() {
  size_t size = kDefaultUnsafeStackSize;
  size_t guard = PAGE_SIZE;
  struct rlimit limit;
  if (getrlimit(RLIMIT_STACK, &limit) == 0 && limit.rlim_cur != RLIM_INFINITY) {
    size = limit.rlim_cur;
  }

  __unsafe_stack_alloc(__get_thread(), size, guard);
}
