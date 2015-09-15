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

#include "bionic/pthread_internal.h"
#include "private/bionic_tls.h"
#include "private/bionic_prctl.h"
#include "private/libc_logging.h"
#include "private/ScopedPthreadMutexLocker.h"

extern "C" void** __safestack_pointer_address() {
  return &(__get_tls()[TLS_SLOT_SAFESTACK]);
}

extern "C" int __safestack_getstack(pthread_t* thread, void** stackaddr, size_t* stacksize,
                                    size_t* stackguard) {
  reinterpret_cast<pthread_internal_t*>(thread)->unsafe_stack_get(stackaddr, stacksize);
#if defined(BIONIC_SAFESTACK)
  *stackguard = PAGE_SIZE;
#else
  *stackguard = 0;
#endif
  return 0;
}

extern "C" void *__get_unsafe_stack_start() {
#if defined(BIONIC_SAFESTACK)
  void *stackaddr;
  size_t stacksize;
  __get_thread()->unsafe_stack_get(&stackaddr, &stacksize);
  return reinterpret_cast<void *>(reinterpret_cast<char *>(stackaddr) + PAGE_SIZE);
#else
  return nullptr;
#endif
}

extern "C" void *__get_unsafe_stack_ptr() {
  return *__safestack_pointer_address();
}

#if defined(BIONIC_SAFESTACK)

// Default size of the unsafe stack. This value is only used if the stack
// size rlimit is set to infinity.
static constexpr size_t kDefaultUnsafeStackSize = 40 * 1024 * 1024;

#if defined(__LP64__)
static constexpr float kRandomGapSize = 4.;
#else
static constexpr float kRandomGapSize = 0.1;
#endif

int pthread_internal_t::unsafe_stack_alloc(size_t stack_size, size_t stack_guard_size,
                                           void** stack_top_ptr) {
  stack_size = BIONIC_ALIGN(stack_size, PAGE_SIZE);

  size_t max_gap_size = static_cast<size_t>(stack_size * kRandomGapSize);
  size_t gap_size = BIONIC_ALIGN(arc4random_uniform(max_gap_size), PAGE_SIZE);
  size_t mmap_size = stack_size + gap_size;

  // Create a new private anonymous map.
  int prot = PROT_READ | PROT_WRITE;
  int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE;
  char* space = reinterpret_cast<char*>(mmap(nullptr, mmap_size, prot, flags, -1, 0));
  if (space == nullptr) return errno;

  char* stack_top = space + stack_size;

  // Stack is at the lower end of mapped space, stack guard region is at the
  // lower end of stack.
  // Set the stack guard region to PROT_NONE, so we can detect thread stack
  // overflow.
  if (mprotect(space, stack_guard_size, PROT_NONE) == -1) {
    int rc = errno;
    __libc_format_log(ANDROID_LOG_WARN, "safestack",
                      "unsafe_stack_alloc failed: couldn't mprotect PROT_NONE "
                      "%zu-byte stack guard region: %s",
                      stack_guard_size, strerror(errno));
    munmap(space, mmap_size);
    return rc;
  }

  if (mprotect(stack_top, gap_size, PROT_NONE) == -1) {
    int rc = errno;
    __libc_format_log(ANDROID_LOG_WARN, "safestack",
                      "unsafe_stack_alloc failed: couldn't mprotect PROT_NONE "
                      "%zu-byte stack guard region: %s",
                      stack_guard_size, strerror(errno));
    munmap(space, mmap_size);
    return rc;
  }

  // Randomize the stack top.
  size_t max_top_offset = stack_size / 100;
  if (max_top_offset > PAGE_SIZE - 1) {
    max_top_offset = PAGE_SIZE - 1;
  }
  size_t top_offset = arc4random_uniform(max_top_offset);
  top_offset &= ~0xFUL;
  stack_top -= top_offset;

  unsafe_stack_start = space;
  unsafe_stack_size = stack_size;
  unsafe_stack_gap_size = gap_size;

  *stack_top_ptr = stack_top;

  return 0;
}

void pthread_internal_t::unsafe_stack_free() {
  munmap(unsafe_stack_start, unsafe_stack_size + unsafe_stack_gap_size);
  // Just in case...
  unsafe_stack_start = nullptr;
  unsafe_stack_size = 0;
  unsafe_stack_gap_size = 0;
}

void pthread_internal_t::unsafe_stack_set_vma_name(size_t guard, char* buf, size_t buf_size) {
  char* space = reinterpret_cast<char*>(unsafe_stack_start);
  size_t stack_size = unsafe_stack_size;
  size_t gap_size = unsafe_stack_gap_size;

  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, space, guard, "unsafe stack guard page");
  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, space + stack_size, gap_size, "unsafe stack random gap");

  const char* name;
  if (buf) {
    snprintf(buf, buf_size, "unsafe stack:%d", static_cast<int>(tid));
    name = buf;
  } else {
    // Main thread.
    name = "unsafe stack";
  }
  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, space + guard, stack_size - guard, name);
}

void pthread_internal_t::unsafe_stack_get(void **start, size_t *size) {
  *start = unsafe_stack_start;
  *size = unsafe_stack_size;
}

void* __init_main_thread_unsafe_stack() {
  size_t size = kDefaultUnsafeStackSize;
  size_t guard = PAGE_SIZE;
  struct rlimit limit;
  if (getrlimit(RLIMIT_STACK, &limit) == 0 && limit.rlim_cur != RLIM_INFINITY) {
    size = limit.rlim_cur;
  }

  pthread_internal_t* thr = __get_thread();
  void* stack_top;
  int rc = thr->unsafe_stack_alloc(size, guard, &stack_top);
  if (rc != 0) {
    __libc_fatal("Failed to allocate the unsafe stack for the main thread: %s\n", strerror(rc));
  }

  thr->unsafe_stack_set_vma_name(guard, nullptr, 0);
  return stack_top;
}

#else  // defined(BIONIC_SAFESTACK)

int pthread_internal_t::unsafe_stack_alloc(size_t, size_t, void** stack_top_ptr) {
  *stack_top_ptr = nullptr;
  return 0;
}

void pthread_internal_t::unsafe_stack_free() {
}

void pthread_internal_t::unsafe_stack_set_vma_name(size_t, char*, size_t) {
}

void pthread_internal_t::unsafe_stack_get(void **start, size_t *size) {
  *start = nullptr;
  *size = 0;
}

void* __init_main_thread_unsafe_stack() {
  return nullptr;
}


#endif   // defined(BIONIC_SAFESTACK)
