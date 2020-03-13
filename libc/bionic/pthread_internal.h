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

#pragma once

#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>

#if __has_feature(hwaddress_sanitizer)
#include <sanitizer/hwasan_interface.h>
#else
#define __hwasan_thread_enter()
#define __hwasan_thread_exit()
#endif

#include <sys/cdefs.h>
#include <sys/mman.h>

#include "private/bionic_elf_tls.h"
#include "private/bionic_lock.h"
#include "private/bionic_tls.h"

// Has the thread been detached by a pthread_join or pthread_detach call?
#define PTHREAD_ATTR_FLAG_DETACHED 0x00000001

// Has the thread been joined by another thread?
#define PTHREAD_ATTR_FLAG_JOINED 0x00000002

// Used for pthread_attr_setinheritsched. We need two flags for this apparent
// boolean because our historical behavior matches neither of the POSIX choices.
#define PTHREAD_ATTR_FLAG_INHERIT 0x00000004
#define PTHREAD_ATTR_FLAG_EXPLICIT 0x00000008

#ifdef __aarch64__
#define BIONIC_USE_SCS 1
#endif

enum ThreadJoinState {
  THREAD_NOT_JOINED,
  THREAD_EXITED_NOT_JOINED,
  THREAD_JOINED,
  THREAD_DETACHED
};

class thread_local_dtor;

struct VmaNameBuffer {
  char name[32];
  void SetNameForCachedThread();
};

// Store stack cache non-invasively in a single-linked list of StackCacheEntry
// structures. Don't store the links invasively in the cached thread stacks (as you might
// imagine would be an optimization) so that we don't dirty a whole 4K stack page just to
// keep track of a stack cache entry.
struct StackCacheEntry {
  StackCacheEntry* next;
  // Cached stacks all have the same size and guard page layout, so we need to store only
  // the base address.  The field is atomic to synchronize with thread exit.
  _Atomic(void*) mmap_base;
  // The signal stack for this thread. You get a signal stack. And you get a signal stack.
  // Everybody gets a signal stack!
  void* alternate_signal_stack;
#ifdef BIONIC_USE_SCS
  // Return address hardening stack.
  void* shadow_call_stack_guard_region;
#endif
  // Holds the name of the memory region for the thread stack.  We put it here instead of
  // in the pthread_internal_t so we can give the stack a nice-looking name even after its
  // previous owner thread exits and we discard its stack pages.
  VmaNameBuffer vma_name_buffer;
};

struct StackCache {
  pthread_mutex_t lock;
  StackCacheEntry* first;
  size_t capacity;
  size_t size;
};

__LIBC_HIDDEN__ extern StackCache g_stack_cache;

class pthread_internal_t {
 public:
  class pthread_internal_t* next;
  class pthread_internal_t* prev;

  pid_t tid;

 private:
  pid_t cached_pid_;

 public:
  pid_t invalidate_cached_pid() {
    pid_t old_value;
    get_cached_pid(&old_value);
    set_cached_pid(0);
    return old_value;
  }

  void set_cached_pid(pid_t value) {
    cached_pid_ = value;
  }

  bool get_cached_pid(pid_t* cached_pid) {
    *cached_pid = cached_pid_;
    return (*cached_pid != 0);
  }

  pthread_attr_t attr;

  _Atomic(ThreadJoinState) join_state;
  bool stack_maybe_modified;
  StackCacheEntry* stack_cache_entry;

  __pthread_cleanup_t* cleanup_stack;

  void* (*start_routine)(void*);
  void* start_routine_arg;
  void* return_value;
  sigset64_t start_mask;

  void* alternate_signal_stack;

  // The start address of the shadow call stack's guard region (arm64 only).
  // This address is only used to deallocate the shadow call stack on thread
  // exit; the address of the stack itself is stored only in the x18 register.
  // Because the protection offered by SCS relies on the secrecy of the stack
  // address, storing the address here weakens the protection, but only
  // slightly, because it is relatively easy for an attacker to discover the
  // address of the guard region anyway (e.g. it can be discovered by reference
  // to other allocations), but not the stack itself, which is <0.1% of the size
  // of the guard region.
  //
  // There are at least two other options for discovering the start address of
  // the guard region on thread exit, but they are not as simple as storing in
  // TLS.
  // 1) Derive it from the value of the x18 register. This is only possible in
  //    processes that do not contain legacy code that might clobber x18,
  //    therefore each process must declare early during process startup whether
  //    it might load legacy code.
  // 2) Mark the guard region as such using prctl(PR_SET_VMA_ANON_NAME) and
  //    discover its address by reading /proc/self/maps. One issue with this is
  //    that reading /proc/self/maps can race with allocations, so we may need
  //    code to handle retries.
#ifdef BIONIC_USE_SCS
  void* shadow_call_stack_guard_region;
#endif

  // A pointer to the top of the stack. This lets android_unsafe_frame_pointer_chase determine the
  // top of the stack quickly, which would otherwise require special logic for the main thread.
  uintptr_t stack_top;

  Lock startup_handshake_lock;

  void* mmap_base;
  size_t mmap_size;

  // The location of the VMA to label as the thread's stack_and_tls.
  void* mmap_base_unguarded;
  size_t mmap_size_unguarded;

  thread_local_dtor* thread_local_dtors;

  /*
   * The dynamic linker implements dlerror(3), which makes it hard for us to implement this
   * per-thread buffer by simply using malloc(3) and free(3).
   */
  char* current_dlerror;
#define __BIONIC_DLERROR_BUFFER_SIZE 512
  char dlerror_buffer[__BIONIC_DLERROR_BUFFER_SIZE];

  bionic_tls* bionic_tls;

  int errno_value;
};

struct ThreadMapping final {
  char* mmap_base;
  size_t mmap_size;
  char* mmap_base_unguarded;
  size_t mmap_size_unguarded;

  char* static_tls;
  char* stack_base;
  char* stack_top;

  void* alternate_signal_stack;
#ifdef BIONIC_USE_SCS
  void* shadow_call_stack_guard_region;
#endif

  bool stack_clean;
  StackCacheEntry* stack_cache_entry;
};

__LIBC_HIDDEN__ void __init_tcb(bionic_tcb* tcb, pthread_internal_t* thread);
__LIBC_HIDDEN__ void __init_tcb_stack_guard(bionic_tcb* tcb);
__LIBC_HIDDEN__ void __init_tcb_dtv(bionic_tcb* tcb);
__LIBC_HIDDEN__ void __init_bionic_tls_ptrs(bionic_tcb* tcb, bionic_tls* tls);
__LIBC_HIDDEN__ bionic_tls* __allocate_temp_bionic_tls();
__LIBC_HIDDEN__ void __free_temp_bionic_tls(bionic_tls* tls);
__LIBC_HIDDEN__ void __init_additional_stacks(pthread_internal_t*);
__LIBC_HIDDEN__ int __init_thread(pthread_internal_t* thread);
__LIBC_HIDDEN__ ThreadMapping __allocate_thread_mapping(size_t stack_size, size_t stack_guard_size,
                                                        bool use_cached_stack);
__LIBC_HIDDEN__ void __set_main_thread_name();

__LIBC_HIDDEN__ pthread_t __pthread_internal_add(pthread_internal_t* thread);
__LIBC_HIDDEN__ pthread_internal_t* __pthread_internal_find(pthread_t pthread_id, const char* caller);
__LIBC_HIDDEN__ pid_t __pthread_internal_gettid(pthread_t pthread_id, const char* caller);
__LIBC_HIDDEN__ void __pthread_internal_remove(pthread_internal_t* thread);
__LIBC_HIDDEN__ void __pthread_internal_remove_and_free(pthread_internal_t* thread);

static inline __always_inline bionic_tcb* __get_bionic_tcb() {
  return reinterpret_cast<bionic_tcb*>(&__get_tls()[MIN_TLS_SLOT]);
}

// Make __get_thread() inlined for performance reason. See http://b/19825434.
static inline __always_inline pthread_internal_t* __get_thread() {
  return static_cast<pthread_internal_t*>(__get_tls()[TLS_SLOT_THREAD_ID]);
}

static inline __always_inline bionic_tls& __get_bionic_tls() {
  return *static_cast<bionic_tls*>(__get_tls()[TLS_SLOT_BIONIC_TLS]);
}

static inline __always_inline TlsDtv* __get_tcb_dtv(bionic_tcb* tcb) {
  uintptr_t dtv_slot = reinterpret_cast<uintptr_t>(tcb->tls_slot(TLS_SLOT_DTV));
  return reinterpret_cast<TlsDtv*>(dtv_slot - offsetof(TlsDtv, generation));
}

static inline void __set_tcb_dtv(bionic_tcb* tcb, TlsDtv* val) {
  tcb->tls_slot(TLS_SLOT_DTV) = &val->generation;
}

extern "C" __LIBC_HIDDEN__ int __set_tls(void* ptr);

__LIBC_HIDDEN__ void pthread_key_clean_all(void);

// Address space is precious on LP32, so use the minimum unit: one
// page.  On LP64, this size is also adequate for most guard regions.
#define PTHREAD_GUARD_SIZE PAGE_SIZE

// SIGSTKSZ (8KiB) is not big enough.
// An snprintf to a stack buffer of size PATH_MAX consumes ~7KiB of stack.
// On 64-bit, logging uses more than 8KiB by itself, ucontext is comically
// large on aarch64, and we have effectively infinite address space, so double
// the signal stack size.
#if defined(__LP64__)
#define SIGNAL_STACK_SIZE_WITHOUT_GUARD (32 * 1024)
#else
#define SIGNAL_STACK_SIZE_WITHOUT_GUARD (16 * 1024)
#endif

// Traditionally we gave threads a 1MiB stack. When we started
// allocating per-thread alternate signal stacks to ease debugging of
// stack overflows, we subtracted the same amount we were using there
// from the default thread stack size. This should keep memory usage
// roughly constant.
#define PTHREAD_STACK_SIZE_DEFAULT ((1 * 1024 * 1024) - SIGNAL_STACK_SIZE_WITHOUT_GUARD)

// Leave room for a guard page in the internally created signal stacks.
#define SIGNAL_STACK_SIZE (SIGNAL_STACK_SIZE_WITHOUT_GUARD + PTHREAD_GUARD_SIZE)

// Master switch to control use of the stack cache.
// Disable in 32-bit processes to conserve address space.
inline constexpr bool kUseStackCache = sizeof(void*) > 4;
// Default maximum number of thread stacks to keep.
inline constexpr size_t kDefaultMaxStackCacheEntries = 128;
// Set kCachedThreadStackGuardSize large enough in 64-bit processes for ART to avoid the need
// to call mprotect on thread creation. The additional address space use doesn't hurt anything
// for other threads, since address space is abundant in 64-bit processes.
inline constexpr size_t kCachedThreadStackGuardSize =
    (sizeof(void*) > 4 ? PAGE_SIZE * 4 : PTHREAD_GUARD_SIZE);
// Size of the mmap region for each cached stack exclusive of the static TLS region, which
// varies per-process in size.
inline constexpr size_t kCachedThreadStackMmapSize =
    PTHREAD_STACK_SIZE_DEFAULT + PTHREAD_GUARD_SIZE + kCachedThreadStackGuardSize;

// madvise(2) command to run on a thread stack that we're about to cache.  MADV_NORMAL here
// means to not issue any madvise at all.
static_assert(MADV_NORMAL == 0);
inline constexpr int kCachedThreadMadvCommand = MADV_DONTNEED;

__LIBC_HIDDEN__ bool __is_thread_cache_debug_enabled_1() __attribute_const__;

static inline __always_inline bool __is_thread_cache_debug_enabled() {
  return __predict_false(__is_thread_cache_debug_enabled_1());
}

// Needed by fork.
__LIBC_HIDDEN__ extern void __bionic_atfork_run_prepare();
__LIBC_HIDDEN__ extern void __bionic_atfork_run_child();
__LIBC_HIDDEN__ extern void __bionic_atfork_run_parent();
