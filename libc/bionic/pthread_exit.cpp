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

#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

#include <async_safe/log.h>

#include "private/ScopedPthreadMutexLocker.h"
#include "private/ScopedSignalBlocker.h"
#include "private/bionic_constants.h"
#include "private/bionic_defs.h"
#include "pthread_internal.h"

// Argument order chosen to simplify the assembly code
extern "C" __noreturn void _exit_with_stack_teardown(void* stackBase, size_t stackSize,
                                                     int madvCommand, void* cacheMmapBase);
extern "C" __noreturn void __exit(int);
extern "C" int __set_tid_address(int*);
extern "C" void __cxa_thread_finalize();

/* CAVEAT: our implementation of pthread_cleanup_push/pop doesn't support C++ exceptions
 *         and thread cancelation
 */

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
void __pthread_cleanup_push(__pthread_cleanup_t* c, __pthread_cleanup_func_t routine, void* arg) {
  pthread_internal_t* thread = __get_thread();
  c->__cleanup_routine = routine;
  c->__cleanup_arg = arg;
  c->__cleanup_prev = thread->cleanup_stack;
  thread->cleanup_stack = c;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
void __pthread_cleanup_pop(__pthread_cleanup_t* c, int execute) {
  pthread_internal_t* thread = __get_thread();
  thread->cleanup_stack = c->__cleanup_prev;
  if (execute) {
    c->__cleanup_routine(c->__cleanup_arg);
  }
}

static StackCacheEntry* __extract_or_destroy_sce(pthread_internal_t* thread) {
  StackCacheEntry* sce = thread->stack_cache_entry;
  if (__is_thread_cache_debug_enabled()) {
    async_safe_format_log(ANDROID_LOG_DEBUG, "libc", "exiting thread sce=%p", sce);
  }

  if (sce) {
    bool free_sce = false;

    // If this thread has called mprotect on its own stack, we don't want to give the stack to
    // some other new thread.
    if (thread->stack_maybe_modified) {
      free_sce = true;
    }

    if (!free_sce) {
      ScopedPthreadMutexLocker locker(&g_stack_cache.lock);
      if (g_stack_cache.size < g_stack_cache.capacity) {
        // We set mmap_base either in __pthread_internal_free() (via pthread_join()) or in
        // _exit_with_stack_teardown() below.  The relaxed memory order is fine here
        // because we've taken the stack cache lock.  When we fill sce->mmap_base below,
        // we do it with release semantics to ensure we're really done with our stack
        // before allowing another thread to use it.
        sce->next = g_stack_cache.first;
        atomic_store_explicit(&sce->mmap_base, nullptr, memory_order_relaxed);
        g_stack_cache.first = sce;
        g_stack_cache.size += 1;
        if (__is_thread_cache_debug_enabled()) {
          async_safe_format_log(ANDROID_LOG_DEBUG, "libc", "added dummy SCE=%p", sce);
        }
      } else {
        free_sce = true;
      }
    }

    if (free_sce) {
      // The VMA name we set in __allocate_thread dangles here for a little bit because
      // we're freeing the memory to which the VMA name points, but that shouldn't be a
      // problem.  If it is, we can reset the VMA name here.
      if (__is_thread_cache_debug_enabled()) {
        async_safe_format_log(ANDROID_LOG_DEBUG, "libc", "freeing SCE=%p instead of donating", sce);
      }
      free(sce);  // Outside lock!
      sce = nullptr;
      thread->stack_cache_entry = nullptr;
    }
  }
  return sce;
}

__BIONIC_WEAK_FOR_NATIVE_BRIDGE
void pthread_exit(void* return_value) {
  pthread_internal_t* thread = __get_thread();

  // This thread is destroying itself, so adding its stack cache entry to the cache is
  // tricky.  We want to minimize the complexity of the platform-specific assembly code in
  // _exit_with_stack_teardown below, so do as much work as possible before we begin
  // tearing down this thread's internal state.  We might call free(), so we do the stack
  // donation before calling any thread destructors.  (The malloc hook can be set to
  // anything.)  We actually add our stack cache entry to the stack cache _before_ we tear
  // down the cache, but we set the entry's mmap_base pointer nullptr until we actually
  // finish with the stack in _exit_with_stack_teardown().  __grab_cached_stack() knows to
  // skip entries in this transient mmap_base===nullptr state.
  //
  // Synchronization with pthread_join() and pthread_detach() is tricky too. If we're a
  // detached thread, we need to donate our own stack, but if we're instead a joinable
  // thread, we need to defer stack donation to __pthread_internal_free() via
  // pthread_join().  The tricky bit is that we can *become* detached between the time we
  // enter pthread_exit() and the time we attempt to transition to
  // THREAD_EXITED_NOT_JOINED, but we need to do most of the thread list cache
  // manipulation before we call thread destructors because we need the heap to be in
  // working order.
  //
  // Let's just add a dummy (mmap_base==nullptr) cache entry in both the detached and
  // joinable cases and have pthread_join fill in mmap_base later.  We'll have a dummy
  // entry in the thread cache for every joinable-but-not-yet-joined thread, but we
  // shouldn't have too many of those.

  StackCacheEntry* sce = __extract_or_destroy_sce(thread);

  // Call dtors for thread_local objects first.
  __cxa_thread_finalize();

  thread->return_value = return_value;

  // Call the cleanup handlers.
  while (thread->cleanup_stack) {
    __pthread_cleanup_t* c = thread->cleanup_stack;
    thread->cleanup_stack = c->__cleanup_prev;
    c->__cleanup_routine(c->__cleanup_arg);
  }

  // Call the TLS destructors. It is important to do that before removing this
  // thread from the global list. This will ensure that if someone else deletes
  // a TLS key, the corresponding value will be set to NULL in this thread's TLS
  // space (see pthread_key_delete).
  pthread_key_clean_all();

  // Tell the kernel to stop using the alternate signal stack.
  stack_t ss;
  memset(&ss, 0, sizeof(ss));
  ss.ss_flags = SS_DISABLE;
  sigaltstack(&ss, nullptr);

  if (sce) {
    if (kCachedThreadMadvCommand != MADV_NORMAL) {
      madvise(thread->alternate_signal_stack, SIGNAL_STACK_SIZE, kCachedThreadMadvCommand);
    }
    sce->alternate_signal_stack = thread->alternate_signal_stack;
  } else if (thread->alternate_signal_stack) {
    munmap(thread->alternate_signal_stack, SIGNAL_STACK_SIZE);
  }

  ThreadJoinState old_state = THREAD_NOT_JOINED;
  while (old_state == THREAD_NOT_JOINED &&
         !atomic_compare_exchange_weak(&thread->join_state, &old_state, THREAD_EXITED_NOT_JOINED)) {
  }

  // We don't want to take a signal after unmapping the stack, the shadow call
  // stack, or dynamic TLS memory.
  ScopedSignalBlocker ssb;

#ifdef BIONIC_USE_SCS
  // Free the shadow call stack and guard pages.
  if (sce) {
    if (kCachedThreadMadvCommand != MADV_NORMAL) {
      madvise(thread->shadow_call_stack_guard_region, SCS_GUARD_REGION_SIZE,
              kCachedThreadMadvCommand);
    }
    sce->shadow_call_stack_guard_region = thread->shadow_call_stack_guard_region;
  } else {
    munmap(thread->shadow_call_stack_guard_region, SCS_GUARD_REGION_SIZE);
  }
#endif

  // Free the ELF TLS DTV and all dynamically-allocated ELF TLS memory.
  __free_dynamic_tls(__get_bionic_tcb());

  if (old_state == THREAD_DETACHED) {
    // The thread is detached, no one will use pthread_internal_t after pthread_exit.
    // So we can free mapped space, which includes pthread_internal_t and thread stack.
    // First make sure that the kernel does not try to clear the tid field
    // because we'll have freed the memory before the thread actually exits.
    __set_tid_address(nullptr);

    // pthread_internal_t is freed below with stack, not here.
    __pthread_internal_remove(thread);

    if (thread->mmap_size != 0) {
      // We need to free mapped space for detached threads when they exit.  That's not
      // something we can do in C.  If we're donating the stack, sce is non-nullptr, and
      // we set in ASM sce->mmap_base after we're sure that we absolutely won't use our
      // stack anymore.
      __hwasan_thread_exit();
      static_assert(sizeof(sce->mmap_base) == sizeof(void*));
      if (sce) {
        if (__is_thread_cache_debug_enabled()) {
          async_safe_format_log(ANDROID_LOG_DEBUG, "libc", "donating SCE=%p mmap_base=%p", sce,
                                thread->mmap_base);
        }
        sce->vma_name_buffer.SetNameForCachedThread();
      }
      _exit_with_stack_teardown(thread->mmap_base, thread->mmap_size, kCachedThreadMadvCommand,
                                sce ? &sce->mmap_base : nullptr);
    }
  }

  if (sce) {
    strcpy(sce->vma_name_buffer.name, "joinable thread stack");
  }

  // No need to free mapped space. Either there was no space mapped, or it is left for
  // the pthread_join caller to clean up.
  __hwasan_thread_exit();
  __exit(0);
}
