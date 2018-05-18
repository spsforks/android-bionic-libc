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
#include <sys/prctl.h>
#include <sys/param.h>
#include <unistd.h>

#include "pthread_internal.h"

#include <async_safe/log.h>

#include "elf_tls.h"
#include "private/bionic_defs.h"
#include "private/bionic_globals.h"
#include "private/bionic_macros.h"
#include "private/bionic_ssp.h"
#include "private/bionic_tls.h"
#include "private/ErrnoRestorer.h"

// x86 uses segment descriptors rather than a direct pointer to TLS.
#if defined(__i386__)
#include <asm/ldt.h>
void __init_user_desc(struct user_desc*, bool, void*);
#endif

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

void __init_tcb(bionic_tcb* tcb, pthread_internal_t* thread) {
  // Slot 0 must point to itself. The x86 Linux kernel reads the TLS from %fs:0.
  tcb->slots[TLS_SLOT_SELF] = tcb->slots;
  tcb->slots[TLS_SLOT_THREAD_ID] = thread;

  // __tls_get_addr assumes that the DTV pointer is never NULL. It must check
  // the DTV generation on every access, so initialize the DTV to one with a 0
  // generation.
  static const TlsDtv zero_dtv = {};
  tcb->slots[TLS_SLOT_DTV] = const_cast<TlsDtv*>(&zero_dtv);
}

void __init_tcb_stack_guard(bionic_tcb* tcb) {
  // XXX: __init_tcb_stack_guard is compiled with -fstack-protector. Move it somehow (or inline it)?
  tcb->slots[TLS_SLOT_STACK_GUARD] = reinterpret_cast<void*>(__stack_chk_guard);
}

void __init_alternate_signal_stack(pthread_internal_t* thread) {
  // Create and set an alternate signal stack.
  void* stack_base = mmap(nullptr, SIGNAL_STACK_SIZE, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
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
    sigaltstack(&ss, nullptr);
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

// Initializes the initial TCB block with data from PT_TLS segments. It might
// only initialize part of the block (e.g. for statically-linked programs), so
// the memory should be zero-initialized prior to calling this function.
static void __init_tcb_initset(bionic_tcb* tcb) {
  TlsModules& modules = __libc_shared_globals->tls_modules;
  if (modules.initset_data != nullptr) {
    memcpy(reinterpret_cast<char*>(&tcb->slots) + modules.initset_data_offset,
           modules.initset_data, modules.initset_data_size);
  }
}

static void* __mmap_aligned(size_t align, size_t size) {
  align = MAX(align, PAGE_SIZE);
  const size_t overhead = align - PAGE_SIZE;
  size_t outer_size;
  if (__builtin_add_overflow(size, overhead, &outer_size)) {
    async_safe_format_log(ANDROID_LOG_WARN,
                          "libc",
                          "thread creation failed: stack size calculation overflowed");
    return nullptr;
  }
  const int prot = PROT_READ | PROT_WRITE;
  const int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE;
  char* const outer_beg = static_cast<char*>(mmap(nullptr, outer_size, prot, flags, -1, 0));
  if (outer_beg == MAP_FAILED) {
    async_safe_format_log(ANDROID_LOG_WARN,
                          "libc",
                          "thread creation failed: couldn't allocate %zu-bytes mapped space: %s",
                          outer_size, strerror(errno));
    return nullptr;
  }
  char* const outer_end = outer_beg + outer_size;
  char* const inner_beg = align_up(outer_beg, align);
  char* const inner_end = inner_beg + size;
  if (inner_end < outer_end) {
    munmap(inner_end, outer_end - inner_end);
  }
  if (outer_beg < inner_beg) {
    munmap(outer_beg, inner_beg - outer_beg);
  }
  return inner_beg;
}

struct StackItem {
  size_t size = 0;
  size_t align = 1;
  void* out_base = nullptr;
  bool is_guard = false;
};

struct AllocResult {
  void* base = nullptr;
  size_t size = 0;
};

static AllocResult __allocate_stack_items(const StackItem* items, size_t item_count) {
  // First pass: calculate the total size in pages.
  size_t max_align = PAGE_SIZE;
  const size_t off_top = -PAGE_SIZE;
  size_t off = off_top;
  for (size_t i = 0; i < item_count; ++i) {
    if (__builtin_sub_overflow(off, items[i].size, &off)) return {};
    off = align_down(off, items[i].align);
    if (off == 0) return {};
    max_align = MAX(max_align, items[i].align);
  }
  off = align_down(off, PAGE_SIZE);
  if (off == 0) return {};
  const size_t mmap_size = off_top - off;

  // Create a new private anonymous map.
  void* const space = __mmap_aligned(max_align, mmap_size);
  if (space == nullptr) return {};

  // Calculate item positions, protect guard page(s).
  char* pos = static_cast<char*>(space) + mmap_size;
  for (size_t i = 0; i < item_count; ++i) {
    pos -= items[i].size;
    pos = align_down(pos, items[i].align);
    if (items[i].out_base != nullptr) {
      memcpy(items[i].out_base, &pos, sizeof(void*));
    }
    if (items[i].size > 0 && items[i].is_guard) {
      if (mprotect(pos, items[i].size, PROT_NONE) == -1) {
        async_safe_format_log(ANDROID_LOG_WARN, "libc",
                              "thread creation failed: couldn't mprotect PROT_NONE %zu-byte stack guard region: %s",
                              items[i].size, strerror(errno));
        munmap(space, mmap_size);
        return {};
      }
      prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, pos, items[i].size, "thread stack guard");
    }
  }

  return { space, mmap_size };
}

struct ThreadMappedSpace {
  pthread_internal_t* thread;
  bionic_tcb* tcb;
  char* stack_top;
  char* stack_base;
  void* mmap_base;
  size_t mmap_size;
};

static ThreadMappedSpace __create_thread_mapped_space(bool alloc_thread, size_t stack_size,
                                                      size_t stack_guard_size) {
  ThreadMappedSpace ret = {};
  StackItem items[6] = {};

  if (alloc_thread) {
    items[0] = { sizeof(pthread_internal_t), alignof(pthread_internal_t), &ret.thread };
  }

  TlsModules& modules = __libc_shared_globals->tls_modules;

  const size_t tls_align = modules.initset_alignment;
  const size_t max_tls_align = MAX(alignof(bionic_tcb), tls_align);
  const size_t max_align = MAX(max_tls_align, PAGE_SIZE);

  items[1] = { MAX(modules.initset_size, BIONIC_TLS_RESERVATION_MIN), max_tls_align };
  items[2] = { kBionicTcbSlotsOffset, 1, &ret.tcb };

  if (stack_size) {
    items[3] = { 0, 16, &ret.stack_top };
    items[4] = { stack_size, 1 };
    items[5] = { stack_guard_size, max_align, &ret.stack_base, .is_guard = true };
  }

  AllocResult mapping = __allocate_stack_items(items, arraysize(items));
  if (mapping.base == nullptr) {
    return {};
  }

  __init_tcb_initset(ret.tcb);
  ret.mmap_base = mapping.base;
  ret.mmap_size = mapping.size;
  return ret;
}

// Using mmap, allocate a TCB containing initialized static TLS memory.
MainThreadTcb __allocate_main_thread_final_tcb() {
  ThreadMappedSpace space = __create_thread_mapped_space(false, 0, 0);
  if (space.mmap_base == nullptr) {
    async_safe_fatal("couldn't allocate main thread's TLS memory");
  }
  return {
    .mmap_base = space.mmap_base,
    .mmap_size = space.mmap_size,
    .tcb = space.tcb,
  };
}

static int __allocate_thread(pthread_attr_t* attr, pthread_internal_t** threadp, void** child_stack) {
  ThreadMappedSpace mapping;
  void* stack_top;

  if (attr->stack_base == nullptr) {
    // The caller didn't provide a stack, so allocate one.
    mapping = __create_thread_mapped_space(true, attr->stack_size, attr->guard_size);
    if (mapping.mmap_base == nullptr) return EAGAIN;
    attr->stack_base = mapping.stack_base;
    attr->stack_size = mapping.stack_top - mapping.stack_base;
    attr->guard_size = align_up(attr->guard_size, PAGE_SIZE);
    stack_top = mapping.stack_top;
  } else {
    mapping = __create_thread_mapped_space(true, 0, 0);
    if (mapping.mmap_base == nullptr) return EAGAIN;
    stack_top = align_down(static_cast<char*>(attr->stack_base) + attr->stack_size, 16);
  }

  pthread_internal_t* thread = mapping.thread;
  thread->mmap_base = mapping.mmap_base;
  thread->mmap_size = mapping.mmap_size;
  thread->attr = *attr;

  thread->bionic_tls = __allocate_bionic_tls();
  if (thread->bionic_tls == nullptr) {
    munmap(mapping.mmap_base, mapping.mmap_size);
    return EAGAIN;
  }

  thread->tcb = mapping.tcb;
  __init_tcb(thread->tcb, thread);
  __init_tcb_stack_guard(thread->tcb);

  *threadp = thread;
  *child_stack = stack_top;
  return 0;
}

__attribute__((no_sanitize("hwaddress")))
static int __pthread_start(void* arg) {
  pthread_internal_t* thread = reinterpret_cast<pthread_internal_t*>(arg);

  __hwasan_thread_enter();

  // Wait for our creating thread to release us. This lets it have time to
  // notify gdb about this thread before we start doing anything.
  // This also provides the memory barrier needed to ensure that all memory
  // accesses previously made by the creating thread are visible to us.
  thread->startup_handshake_lock.lock();

  __init_alternate_signal_stack(thread);

  void* result = thread->start_routine(thread->start_routine_arg);
  pthread_exit(result);

  return 0;
}

// A dummy start routine for pthread_create failures where we've created a thread but aren't
// going to run user code on it. We swap out the user's start routine for this and take advantage
// of the regular thread teardown to free up resources.
static void* __do_nothing(void*) {
  return nullptr;
}


__BIONIC_WEAK_FOR_NATIVE_BRIDGE
int pthread_create(pthread_t* thread_out, pthread_attr_t const* attr,
                   void* (*start_routine)(void*), void* arg) {
  ErrnoRestorer errno_restorer;

  pthread_attr_t thread_attr;
  if (attr == nullptr) {
    pthread_attr_init(&thread_attr);
  } else {
    thread_attr = *attr;
    attr = nullptr; // Prevent misuse below.
  }

  pthread_internal_t* thread = nullptr;
  void* child_stack = nullptr;
  int result = __allocate_thread(&thread_attr, &thread, &child_stack);
  if (result != 0) {
    return result;
  }

  // Create a lock for the thread to wait on once it starts so we can keep
  // it from doing anything until after we notify the debugger about it
  //
  // This also provides the memory barrier we need to ensure that all
  // memory accesses previously performed by this thread are visible to
  // the new thread.
  thread->startup_handshake_lock.init(false);
  thread->startup_handshake_lock.lock();

  thread->start_routine = start_routine;
  thread->start_routine_arg = arg;

  thread->set_cached_pid(getpid());

  int flags = CLONE_VM | CLONE_FS | CLONE_FILES | CLONE_SIGHAND | CLONE_THREAD | CLONE_SYSVSEM |
      CLONE_SETTLS | CLONE_PARENT_SETTID | CLONE_CHILD_CLEARTID;
  void* tls = reinterpret_cast<void*>(thread->tcb->slots);
#if defined(__i386__)
  // On x86 (but not x86-64), CLONE_SETTLS takes a pointer to a struct user_desc rather than
  // a pointer to the TLS itself.
  user_desc tls_descriptor;
  __init_user_desc(&tls_descriptor, false, tls);
  tls = &tls_descriptor;
#endif
  int rc = clone(__pthread_start, child_stack, flags, thread, &(thread->tid), tls, &(thread->tid));
  if (rc == -1) {
    int clone_errno = errno;
    // We don't have to unlock the mutex at all because clone(2) failed so there's no child waiting to
    // be unblocked, but we're about to unmap the memory the mutex is stored in, so this serves as a
    // reminder that you can't rewrite this function to use a ScopedPthreadMutexLocker.
    thread->startup_handshake_lock.unlock();
    if (thread->mmap_size != 0) {
      munmap(thread->mmap_base, thread->mmap_size);
    }
    async_safe_format_log(ANDROID_LOG_WARN, "libc", "pthread_create failed: clone failed: %s",
                          strerror(clone_errno));
    return clone_errno;
  }

  int init_errno = __init_thread(thread);
  if (init_errno != 0) {
    // Mark the thread detached and replace its start_routine with a no-op.
    // Letting the thread run is the easiest way to clean up its resources.
    atomic_store(&thread->join_state, THREAD_DETACHED);
    __pthread_internal_add(thread);
    thread->start_routine = __do_nothing;
    thread->startup_handshake_lock.unlock();
    return init_errno;
  }

  // Publish the pthread_t and unlock the mutex to let the new thread start running.
  *thread_out = __pthread_internal_add(thread);
  thread->startup_handshake_lock.unlock();

  return 0;
}
