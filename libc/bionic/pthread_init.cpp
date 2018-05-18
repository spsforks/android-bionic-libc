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

#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <unistd.h>

#include "pthread_internal.h"

#include <async_safe/log.h>

#include "elf_tls.h"
#include "private/bionic_prctl.h"
#include "private/bionic_ssp.h"
#include "private/bionic_tls.h"

// Statically assert that there aren't too many Bionic TLS slots.
static_assert(BIONIC_TLS_SLOTS <= kMaxTlsSlots, "More slots interferes with variant 1 TLS");

// Allocate and return a bionic_tls using mmap. Surround it with guard pages.
bionic_tls* __allocate_bionic_tls() {
  // Add a guard before and after.
  size_t allocation_size = BIONIC_TLS_SIZE + (2 * PTHREAD_GUARD_SIZE);
  void* allocation = mmap(nullptr, allocation_size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (allocation == MAP_FAILED) {
    async_safe_format_log(ANDROID_LOG_WARN, "libc",
                          "pthread_create failed: couldn't allocate TLS: %s", strerror(errno));
    return nullptr;
  }

  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, allocation, allocation_size, "bionic TLS guard");

  // Carve out the writable TLS section.
  bionic_tls* btls = reinterpret_cast<bionic_tls*>(static_cast<char*>(allocation) +
                                                   PTHREAD_GUARD_SIZE);
  if (mprotect(btls, BIONIC_TLS_SIZE, PROT_READ | PROT_WRITE) != 0) {
    async_safe_format_log(ANDROID_LOG_WARN, "libc",
                          "pthread_create failed: couldn't mprotect TLS: %s", strerror(errno));
    munmap(allocation, allocation_size);
    return nullptr;
  }

  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, btls, BIONIC_TLS_SIZE, "bionic TLS");
  return btls;
}

// Unmap the bionic TLS, including guard pages.
void __free_bionic_tls(bionic_tls* tls) {
  void* allocation = reinterpret_cast<char*>(tls) - PTHREAD_GUARD_SIZE;
  munmap(allocation, BIONIC_TLS_SIZE + 2 * PTHREAD_GUARD_SIZE);
}

// Allocate a TCB containing an Initial TLS Block.
bionic_tcb* __allocate_tcb(TlsModules& modules, void* memalign_func(size_t alignment, size_t bytes)) {
  const size_t max_align = MAX(modules.initset_alignment, alignof(bionic_tcb));

  // tpoff == offset from the start of the memalign'ed heap object to the thread pointer (TP).
#if defined(BIONIC_TLS_VARIANT1)
  const size_t tpoff = __BIONIC_ALIGN(kBionicTcbSlotsOffset, max_align);
  size_t size = tpoff + MAX(modules.initset_size, sizeof(bionic_tcb) - kBionicTcbSlotsOffset);
#elif defined(BIONIC_TLS_VARIANT2)
  const size_t tpoff = __BIONIC_ALIGN(modules.initset_size, max_align);
  size_t size = tpoff + sizeof(bionic_tcb);
#endif

  size = __BIONIC_ALIGN(size, max_align);
  void* ptr = memalign_func(max_align, size);
  if (!ptr) return nullptr;
  memset(ptr, 0, size);
  bionic_tcb* tcb = reinterpret_cast<bionic_tcb*>(static_cast<char*>(ptr) + tpoff - kBionicTcbSlotsOffset);
  if (modules.initset_data != nullptr) {
    memcpy(reinterpret_cast<char*>(&tcb->slots) + modules.initset_data_offset,
           modules.initset_data, modules.initset_data_size);
  }

  return tcb;
}

void __free_tcb(TlsModules& modules, bionic_tcb* tcb, void free_func(void* ptr)) {
  // XXX: Look for a better way to deal with variant1-vs-variant2 layout?

  const size_t max_align = MAX(modules.initset_alignment, alignof(bionic_tcb));
#if defined(BIONIC_TLS_VARIANT1)
  const size_t tpoff = __BIONIC_ALIGN(kBionicTcbSlotsOffset, max_align);
  void* ptr = reinterpret_cast<char*>(&tcb->slots) - tpoff;
#elif defined(BIONIC_TLS_VARIANT2)
  const size_t tpoff = __BIONIC_ALIGN(modules.initset_size, max_align);
  void* ptr = reinterpret_cast<char*>(tcb) - tpoff;
#endif

  free_func(ptr);
}

void __init_tcb(bionic_tcb* tcb, pthread_internal_t* thread) {
#if defined(__i386__) || defined(__x86_64__)
  // Slot 0 must point to itself. The x86 Linux kernel reads the TLS from %fs:0.
  // XXX: Does the kernel really do that? I think there are other reasons x86 needs this...
  tcb->slots[TLS_SLOT_SELF] = tcb->slots;
#endif
  tcb->slots[TLS_SLOT_THREAD_ID] = thread;

  // __tls_get_addr assumes that the DTV pointer is never NULL. It must check
  // the DTV generation on every access, so initialize the DTV to one with a 0
  // generation.
  static TlsDtv zero_dtv;
  tcb->slots[TLS_SLOT_DTV] = &zero_dtv;
}

void __init_tcb_stack_guard(bionic_tcb* tcb) {
  // XXX: __init_tcb_stack_guard is compiled with -fstack-protector. Move it somehow (or inline it)?
  tcb->slots[TLS_SLOT_STACK_GUARD] = reinterpret_cast<void*>(__stack_chk_guard);
}

void __init_alternate_signal_stack(pthread_internal_t* thread) {
  // Create and set an alternate signal stack.
  void* stack_base = mmap(NULL, SIGNAL_STACK_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if (stack_base != MAP_FAILED) {
    // Create a guard to catch stack overflows in signal handlers.
    if (mprotect(stack_base, PTHREAD_GUARD_SIZE, PROT_NONE) == -1) {
      munmap(stack_base, SIGNAL_STACK_SIZE);
      return;
    }
    stack_t ss;
    ss.ss_sp = reinterpret_cast<uint8_t*>(stack_base) + PTHREAD_GUARD_SIZE;
    ss.ss_size = SIGNAL_STACK_SIZE - PTHREAD_GUARD_SIZE;
    ss.ss_flags = 0;
    sigaltstack(&ss, NULL);
    thread->alternate_signal_stack = stack_base;

    // We can only use const static allocated string for mapped region name, as Android kernel
    // uses the string pointer directly when dumping /proc/pid/maps.
    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, ss.ss_sp, ss.ss_size, "thread signal stack");
    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, stack_base, PTHREAD_GUARD_SIZE, "thread signal stack guard");
  }
}

int __init_thread(pthread_internal_t* thread) {
  thread->cleanup_stack = nullptr;

  if (__predict_true((thread->attr.flags & PTHREAD_ATTR_FLAG_DETACHED) == 0)) {
    atomic_init(&thread->join_state, THREAD_NOT_JOINED);
  } else {
    atomic_init(&thread->join_state, THREAD_DETACHED);
  }

  // Set the scheduling policy/priority of the thread if necessary.
  bool need_set = true;
  int policy;
  sched_param param;
  if ((thread->attr.flags & PTHREAD_ATTR_FLAG_INHERIT) != 0) {
    // Unless the parent has SCHED_RESET_ON_FORK set, we've already inherited from the parent.
    policy = sched_getscheduler(0);
    need_set = ((policy & SCHED_RESET_ON_FORK) != 0);
    if (need_set) {
      if (policy == -1) {
        async_safe_format_log(ANDROID_LOG_WARN, "libc",
                              "pthread_create sched_getscheduler failed: %s", strerror(errno));
        return errno;
      }
      if (sched_getparam(0, &param) == -1) {
        async_safe_format_log(ANDROID_LOG_WARN, "libc",
                              "pthread_create sched_getparam failed: %s", strerror(errno));
        return errno;
      }
    }
  } else {
    policy = thread->attr.sched_policy;
    param.sched_priority = thread->attr.sched_priority;
  }
  // Backwards compatibility: before P, Android didn't have pthread_attr_setinheritsched,
  // and our behavior was neither of the POSIX behaviors.
  if ((thread->attr.flags & (PTHREAD_ATTR_FLAG_INHERIT|PTHREAD_ATTR_FLAG_EXPLICIT)) == 0) {
    need_set = (thread->attr.sched_policy != SCHED_NORMAL);
  }
  if (need_set) {
    if (sched_setscheduler(thread->tid, policy, &param) == -1) {
      async_safe_format_log(ANDROID_LOG_WARN, "libc",
                            "pthread_create sched_setscheduler(%d, {%d}) call failed: %s", policy,
                            param.sched_priority, strerror(errno));
#if defined(__LP64__)
      // For backwards compatibility reasons, we only report failures on 64-bit devices.
      return errno;
#endif
    }
  }

  return 0;
}
