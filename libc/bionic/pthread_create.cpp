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
#include <sys/random.h>
#include <unistd.h>

#include "pthread_internal.h"

#include <async_safe/log.h>

#include "platform/bionic/macros.h"
#include "private/ErrnoRestorer.h"
#include "private/ScopedPthreadMutexLocker.h"
#include "private/bionic_constants.h"
#include "private/bionic_defs.h"
#include "private/bionic_globals.h"
#include "private/bionic_ssp.h"
#include "private/bionic_systrace.h"
#include "private/bionic_tls.h"

#include <sys/system_properties.h>

// x86 uses segment descriptors rather than a direct pointer to TLS.
#if defined(__i386__)
#include <asm/ldt.h>
void __init_user_desc(struct user_desc*, bool, void*);
#endif

__attribute__((no_stack_protector))
void __init_tcb_stack_guard(bionic_tcb* tcb) {
  // GCC looks in the TLS for the stack guard on x86, so copy it there from our global.
  tcb->tls_slot(TLS_SLOT_STACK_GUARD) = reinterpret_cast<void*>(__stack_chk_guard);
}

void __init_bionic_tls_ptrs(bionic_tcb* tcb, bionic_tls* tls) {
  tcb->thread()->bionic_tls = tls;
  tcb->tls_slot(TLS_SLOT_BIONIC_TLS) = tls;
}

// Allocate a temporary bionic_tls that the dynamic linker's main thread can
// use while it's loading the initial set of ELF modules.
bionic_tls* __allocate_temp_bionic_tls() {
  size_t allocation_size = __BIONIC_ALIGN(sizeof(bionic_tls), PAGE_SIZE);
  void* allocation = mmap(nullptr, allocation_size,
                          PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS,
                          -1, 0);
  if (allocation == MAP_FAILED) {
    // Avoid strerror because it might need bionic_tls.
    async_safe_fatal("failed to allocate bionic_tls: error %d", errno);
  }
  return static_cast<bionic_tls*>(allocation);
}

void __free_temp_bionic_tls(bionic_tls* tls) {
  munmap(tls, __BIONIC_ALIGN(sizeof(bionic_tls), PAGE_SIZE));
}

static void* __allocate_alternate_signal_stack() {
  void* stack_base =
      mmap(nullptr, SIGNAL_STACK_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (stack_base == MAP_FAILED) {
    return nullptr;
  }
  // Create a guard to catch stack overflows in signal handlers.
  if (mprotect(stack_base, PTHREAD_GUARD_SIZE, PROT_NONE)) {
    munmap(stack_base, SIGNAL_STACK_SIZE);
    return nullptr;
  }
  // The name of the signal stack must live as long as the mapping.  Making it a static
  // string is the easiest approach.
  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, reinterpret_cast<char*>(stack_base) + PTHREAD_GUARD_SIZE,
        SIGNAL_STACK_SIZE - PTHREAD_GUARD_SIZE, "thread signal stack");
  return stack_base;
}

#ifdef BIONIC_USE_SCS
static void* __allocate_shadow_call_stack_guard_region() {
  // Allocate the stack and the guard region.
  char* scs_guard_region = reinterpret_cast<char*>(
      mmap(nullptr, SCS_GUARD_REGION_SIZE, PROT_NONE, MAP_PRIVATE | MAP_ANON, -1, 0));
  if (scs_guard_region == MAP_FAILED) {
    return nullptr;
  }

  // Make all but the last page writable.
  if (mprotect(scs_guard_region, SCS_GUARD_REGION_SIZE - PAGE_SIZE, PROT_READ | PROT_WRITE)) {
    munmap(scs_guard_region, SCS_GUARD_REGION_SIZE);
    return nullptr;
  }

  return scs_guard_region;
}
#endif

static void __init_alternate_signal_stack(pthread_internal_t* thread) {
  // We may have gotten a pre-assigned signal stack from the stack cache.  Create a new signal
  // stack only if we don't already have one. Allocate the stack here (in the brand-new thread)
  // instead of in pthread_create to shorten the path length in pthread_create.
  if (thread->alternate_signal_stack == nullptr) {
    thread->alternate_signal_stack = __allocate_alternate_signal_stack();
  }

  if (thread->alternate_signal_stack) {
    stack_t ss;
    ss.ss_sp = reinterpret_cast<uint8_t*>(thread->alternate_signal_stack) + PTHREAD_GUARD_SIZE;
    ss.ss_size = SIGNAL_STACK_SIZE - PTHREAD_GUARD_SIZE;
    ss.ss_flags = 0;
    sigaltstack(&ss, nullptr);
  }
}

static void __init_shadow_call_stack(pthread_internal_t* thread __unused) {
#ifdef BIONIC_USE_SCS
  // SCS guard region creation is mandatory, so we can assume that
  // thread->shadow_call_stack_guard_region is always set.
  char* scs_aligned_guard_region = reinterpret_cast<char*>(
      align_up(reinterpret_cast<uintptr_t>(thread->shadow_call_stack_guard_region), SCS_SIZE));

  // We need to ensure that [scs_offset,scs_offset+SCS_SIZE) is in the guard region and that there
  // is at least one unmapped page after the shadow call stack (to catch stack overflows). We can't
  // use arc4random_uniform in init because /dev/urandom might not have been created yet.
  size_t scs_offset =
      (getpid() == 1) ? 0 : (arc4random_uniform(SCS_GUARD_REGION_SIZE / SCS_SIZE - 1) * SCS_SIZE);

  // Make the stack readable and writable and store its address in register x18. This is
  // deliberately the only place where the address is stored.
  char *scs = scs_aligned_guard_region + scs_offset;
  __asm__ __volatile__("mov x18, %0" ::"r"(scs));
#endif
}

void __init_additional_stacks(pthread_internal_t* thread) {
  __init_alternate_signal_stack(thread);
  __init_shadow_call_stack(thread);
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

int pthread_set_stack_cache_max_count_np(size_t nr_threads) {
  if (kUseStackCache) {
    ScopedPthreadMutexLocker locker(&g_stack_cache.lock);
    if (__is_thread_cache_debug_enabled()) {
      async_safe_format_log(ANDROID_LOG_INFO, "libc", "setting stack cache size to %zu threads",
                            nr_threads);
    }
    g_stack_cache.capacity = nr_threads;
  }
  return 0;
}

static int __add_cached_stack_locked() {
  void* signal_stack = __allocate_alternate_signal_stack();
  if (signal_stack == nullptr) {
    return ENOMEM;
  }
  // We hold g_stack_cache.lock here.  __allocate_thread_mapping takes that lock, but only
  // when use_cached_stack is true.  We pass false here, saving us from deadlocking inside
  // __allocate_thread_mapping.
  ThreadMapping mapping = __allocate_thread_mapping(
      kCachedThreadStackMmapSize - kCachedThreadStackGuardSize - PTHREAD_GUARD_SIZE,
      kCachedThreadStackGuardSize,
      /*use_cached_stack=*/false);
  if (!mapping.mmap_base || !mapping.stack_cache_entry) {
    munmap(signal_stack, SIGNAL_STACK_SIZE);
    if (mapping.mmap_base) {
      munmap(mapping.mmap_base, mapping.mmap_size);
    }
#ifdef BIONIC_USE_SCS
    if (mapping.shadow_call_stack_guard_region) {
      munmap(mapping.shadow_call_stack_guard_region, SCS_GUARD_REGION_SIZE);
    }
#endif
    return ENOMEM;
  }
  StackCacheEntry* sce = mapping.stack_cache_entry;
  sce->next = g_stack_cache.first;
  atomic_init(&sce->mmap_base, mapping.mmap_base);
  sce->alternate_signal_stack = signal_stack;
#ifdef BIONIC_USE_SCS
  sce->shadow_call_stack_guard_region = mapping.shadow_call_stack_guard_region;
#endif
  sce->vma_name_buffer.SetNameForCachedThread();
  g_stack_cache.first = sce;
  g_stack_cache.size += 1;
  return 0;
}

int pthread_precache_thread_stacks_np(size_t nr_threads) {
  if (kUseStackCache) {
    ScopedPthreadMutexLocker locker(&g_stack_cache.lock);
    if (nr_threads > g_stack_cache.capacity) {
      nr_threads = g_stack_cache.capacity;
    }
    while (g_stack_cache.size < nr_threads) {
      if (int rc = __add_cached_stack_locked()) {
        return rc;
      }
    }
  }
  return 0;
}

void VmaNameBuffer::SetNameForCachedThread() {
  strcpy(name, "cached thread stack");
}

// Retrieve a StackCacheEntry from the global cache if we can find a valid one.  Using two
// out parameters lets us avoid a redundant atomic load of mmap_base on success.
static void __try_grab_cached_stack(StackCacheEntry** out_cache_entry, char** out_mmap_base) {
  ScopedPthreadMutexLocker locker(&g_stack_cache.lock);
  StackCacheEntry** prev = &g_stack_cache.first;
  StackCacheEntry* sce = *prev;
  void* mmap_base;
  // Skip stack cache entries that are transiently cleared during thread teardown.
  // The load-acquire of mmap_base synchronizes with thread teardown.  Once a stack cache
  // entry is on the stack cache list, its mmap_base transitions exactly once, from
  // nullptr to non-nullptr, so the check below is safe.
  while (sce &&
         (mmap_base = atomic_load_explicit(&sce->mmap_base, memory_order_acquire)) == nullptr) {
    if (__is_thread_cache_debug_enabled()) {
      async_safe_format_log(ANDROID_LOG_DEBUG, "libc", "skipping invalid SCE=%p", sce);
    }
    prev = &sce->next;
    sce = *prev;
  }
  if (sce) {
    if (__is_thread_cache_debug_enabled()) {
      async_safe_format_log(ANDROID_LOG_DEBUG, "libc", "found valid SCE=%p ne=%lu", sce,
                            static_cast<unsigned long>(g_stack_cache.size));
    }
    *prev = sce->next;
    *out_cache_entry = sce;
    *out_mmap_base = reinterpret_cast<char*>(mmap_base);
    g_stack_cache.size -= 1;
  } else {
    if (__is_thread_cache_debug_enabled()) {
      async_safe_format_log(ANDROID_LOG_DEBUG, "libc", "did not find SCE");
    }
  }
}

// Return whether the stack cache is enabled. Negative means the stack cache is disabled;
// positive means that it is enabled. The function never returns zero.
static int get_stack_cache_enable_uncached() {
  if (getpid() == 1) {
    return -1;  // Don't use the stack cache for init: eases debugging
  }
  char value[PROP_VALUE_MAX];
  value[0] = '\0';
  __system_property_get("debug.enable_stack_cache", value);
  // Enable the stack cache by default.
  bool enable = value[0] == '\0' || (value[0] == '1' && value[1] == '\0');
  async_safe_format_log(ANDROID_LOG_INFO, "libc", "stack cache enable state: %d",
                        static_cast<int>(enable));
  return enable ? 1 : -1;
}

// Return whether stack cache debugging is enabled. Negative means debugging is disabled;
// positive means that it is enabled. The function never returns zero.
static int get_stack_cache_debug_uncached() {
  if (getpid() == 1) {
    return -1;  // Don't use the stack cache for init: eases debugging
  }
  char value[PROP_VALUE_MAX];
  value[0] = '\0';
  __system_property_get("debug.stack_cache_debug", value);
  // Disable debugging by default.
  bool enable = (value[0] == '1' && value[1] == '\0');
  async_safe_format_log(ANDROID_LOG_INFO, "libc", "stack cache enable state: %d",
                        static_cast<int>(enable));
  return enable ? 1 : -1;
}

static int dynamic_is_stack_cache_enabled() {
  // Don't initialize state with get_stack_cache_enable_uncached(): we don't want to pay for
  // the automatically-emitted synchronization: get_stack_cache_enable_uncached() is
  // idempotent.  Initial value of state should be zero so it can be in BSS instead of data.
  static int state = 0;
  if (state == 0) {
    state = get_stack_cache_enable_uncached();
  }
  return state > 0;
}

bool __is_thread_cache_debug_enabled_1() {
  // See comment in get_stack_cache_debug_control().
  static int state = 0;
  if (state == 0) {
    state = get_stack_cache_debug_uncached();
  }
  return state > 0;
}

// Allocate a thread's primary mapping. This mapping includes static TLS and
// optionally a stack. Static TLS includes ELF TLS segments and the bionic_tls
// struct.
//
// The stack_guard_size must be a multiple of the PAGE_SIZE.
ThreadMapping __allocate_thread_mapping(size_t stack_size, size_t stack_guard_size,
                                        bool use_cached_stack) {
  const StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
  bool donate_stack_on_exit = true;

  if (!kUseStackCache) {
    use_cached_stack = false;
    donate_stack_on_exit = false;
  }

  if (stack_size == 0) {
    use_cached_stack = false;
    donate_stack_on_exit = false;
  }

  if (!dynamic_is_stack_cache_enabled()) {
    use_cached_stack = false;
    donate_stack_on_exit = false;
  }

  // Cached stacks must use the default guard region size and the caller must want an
  // actual stack (i.e., stack_size != 0).
  if (donate_stack_on_exit && stack_guard_size <= kCachedThreadStackGuardSize) {
    stack_guard_size = kCachedThreadStackGuardSize;
  } else {
    if (__is_thread_cache_debug_enabled() && donate_stack_on_exit) {
      async_safe_format_log(ANDROID_LOG_DEBUG, "libc",
                            "not using stack cache due to unusual guard size request of %zu",
                            stack_guard_size);
    }
    use_cached_stack = false;
    donate_stack_on_exit = false;
  }

  // Allocate in order: stack guard, stack, guard page.
  size_t mmap_size;
  if (__builtin_add_overflow(stack_size, stack_guard_size, &mmap_size)) return {};
  if (__builtin_add_overflow(mmap_size, layout.size(), &mmap_size)) return {};
  if (__builtin_add_overflow(mmap_size, PTHREAD_GUARD_SIZE, &mmap_size)) return {};

  // Align the result to a page size.
  const size_t unaligned_size = mmap_size;
  mmap_size = __BIONIC_ALIGN(mmap_size, PAGE_SIZE);
  if (mmap_size < unaligned_size) return {};

  // Compute the real cached thread mmap size here because it can vary
  // based on the amount of static TLS in a given process.
  const size_t cached_thread_mmap_size_full =
      __BIONIC_ALIGN(kCachedThreadStackMmapSize + layout.size(), PAGE_SIZE);

  if (mmap_size > cached_thread_mmap_size_full) {
    if (__is_thread_cache_debug_enabled()) {
      async_safe_format_log(ANDROID_LOG_DEBUG, "libc",
                            "not using stack cache due to oversize stack mmap_size=%zu max=%zu",
                            mmap_size, cached_thread_mmap_size_full);
    }
    use_cached_stack = false;
    donate_stack_on_exit = false;
  }

  if (donate_stack_on_exit && mmap_size < cached_thread_mmap_size_full) {
    // Snap small thread stack requests to the stack cache size.  Giving a thread a stack
    // _larger_ than requested is not a bug, and address space is abundant.
    stack_size += (cached_thread_mmap_size_full - mmap_size);
    mmap_size = cached_thread_mmap_size_full;
  }

  ThreadMapping result = {};
  result.mmap_size = mmap_size;
  if (use_cached_stack) {
    __try_grab_cached_stack(&result.stack_cache_entry, &result.mmap_base);
  }

  if (result.stack_cache_entry) {
    // result.mmap_base already copied from the stack cache entry by __try_grab_cached_stack.
    // If alternate_signal_stack isn't set for whatever reason, we'll allocate a signal stack
    // during thread startup.  Signal stack creation failure is non-fatal.
    result.alternate_signal_stack = result.stack_cache_entry->alternate_signal_stack;
#ifdef BIONIC_USE_SCS
    result.shadow_call_stack_guard_region =
        result.stack_cache_entry->shadow_call_stack_guard_region;
#endif
  } else {
    result.stack_clean = true;  // New mappings guaranteed to be zero.
    if (__is_thread_cache_debug_enabled()) {
      async_safe_format_log(ANDROID_LOG_DEBUG, "libc",
                            "pthread_create alloc stack_size=%zu "
                            "stack_guard_size=%zu mmap_size=%zu ps=%zu",
                            stack_size, stack_guard_size, mmap_size, sizeof(void*));
    }

    // Create a new private anonymous map. Make the entire mapping PROT_NONE, then carve out a
    // read+write area in the middle.
    const int flags = MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE;
    void* mmap_result = mmap(nullptr, mmap_size, PROT_NONE, flags, -1, 0);
    if (mmap_result == MAP_FAILED) {
      async_safe_format_log(ANDROID_LOG_ERROR, "libc",
                            "pthread_create failed: couldn't allocate %zu-bytes mapped space: %s",
                            mmap_size, strerror(errno));
      return {};
    }
    result.mmap_base = static_cast<char*>(mmap_result);

    const size_t writable_size = mmap_size - stack_guard_size - PTHREAD_GUARD_SIZE;
    if (mprotect(result.mmap_base + stack_guard_size, writable_size, PROT_READ | PROT_WRITE) != 0) {
      async_safe_format_log(
          ANDROID_LOG_ERROR, "libc",
          "pthread_create failed: couldn't mprotect R+W %zu-byte thread mapping region: %s",
          writable_size, strerror(errno));
      munmap(result.mmap_base, result.mmap_size);
      return {};
    }

    // We could try allocating a signal stack here, but since the target thread can do
    // that on its own, let's increase concurrency by delaying that allocation until
    // __pthread_start().

#ifdef BIONIC_USE_SCS
    // On systems that use a shadow call stack, shadow call stack creation must succeed: a
    // thread can't run without a SCS.  We must therefore allocate the shadow call stack
    // here, where we can still report failure from pthread_create.
    result.shadow_call_stack_guard_region = __allocate_shadow_call_stack_guard_region();
    if (result.shadow_call_stack_guard_region == nullptr) {
      async_safe_format_log(ANDROID_LOG_ERROR, "libc",
                            "pthread_create failed: couldn't allocate SCS guard region: %s",
                            strerror(errno));
      munmap(result.mmap_base, result.mmap_size);
      return {};
    }
#endif

    // If this stack can be cached, pre-allocate the cache link structure here.  This way,
    // we don't have to allocate on the exit path and the thread-exit code can reuse the
    // StackCacheEntry structure we grabbed to create the thread.  It's okay if the malloc
    // below fails: in that case, we just don't cache this thread's stack when it exits.
    if (donate_stack_on_exit) {
      result.stack_cache_entry =
          reinterpret_cast<StackCacheEntry*>(malloc(sizeof(StackCacheEntry)));
      if (__is_thread_cache_debug_enabled()) {
        async_safe_format_log(ANDROID_LOG_DEBUG, "libc", "allocated SCE=%p",
                              result.stack_cache_entry);
      }
    }
  }

  result.mmap_base_unguarded = result.mmap_base + stack_guard_size;
  result.mmap_size_unguarded = mmap_size - stack_guard_size - PTHREAD_GUARD_SIZE;
  result.static_tls = result.mmap_base + mmap_size - PTHREAD_GUARD_SIZE - layout.size();
  result.stack_base = result.mmap_base;
  result.stack_top = result.static_tls;

  return result;
}

static int __allocate_thread(pthread_attr_t* attr, bionic_tcb** tcbp, void** child_stack) {
  ThreadMapping mapping = {};
  char* stack_top;

  if (attr->stack_base == nullptr) {
    // The caller didn't provide a stack, so allocate one.

    // Make sure the guard size is a multiple of PAGE_SIZE.
    const size_t unaligned_guard_size = attr->guard_size;
    attr->guard_size = __BIONIC_ALIGN(attr->guard_size, PAGE_SIZE);
    if (attr->guard_size < unaligned_guard_size) return EAGAIN;

    mapping = __allocate_thread_mapping(attr->stack_size, attr->guard_size,
                                        /*use_cached_stack=*/true);
    if (mapping.mmap_base == nullptr) return EAGAIN;

    stack_top = mapping.stack_top;
    attr->stack_base = mapping.stack_base;
  } else {
    mapping = __allocate_thread_mapping(0, PTHREAD_GUARD_SIZE,
                                        /*use_cached_stack=*/false);
    if (mapping.mmap_base == nullptr) return EAGAIN;

    if (__is_thread_cache_debug_enabled()) {
      async_safe_format_log(ANDROID_LOG_DEBUG, "libc", "caller provided stack size=%zu",
                            attr->stack_size);
    }

    stack_top = static_cast<char*>(attr->stack_base) + attr->stack_size;
  }

  // Carve out space from the stack for the thread's pthread_internal_t. This
  // memory isn't counted in pthread_attr_getstacksize.

  // To safely access the pthread_internal_t and thread stack, we need to find a 16-byte aligned boundary.
  stack_top = align_down(stack_top - sizeof(pthread_internal_t), 16);

  pthread_internal_t* thread = reinterpret_cast<pthread_internal_t*>(stack_top);
  if (!mapping.stack_clean) {
    // If thread was not just now allocated by mmap(), it may not have been cleared to
    // zero.  So assume the worst and zero it.
    memset(thread, 0, sizeof(pthread_internal_t));
  }

  // If this thread has a stack cache entry, we want to put the VMA name there so that the
  // VMA name outlives the thread proper.  For a thread without a stack cache entry, we
  // just put the name on the thread stack.

  VmaNameBuffer* vma_name_buffer;
  if (mapping.stack_cache_entry) {
    vma_name_buffer = &mapping.stack_cache_entry->vma_name_buffer;
  } else {
    vma_name_buffer = reinterpret_cast<VmaNameBuffer*>(stack_top);
    stack_top = align_down(stack_top - sizeof(VmaNameBuffer), 16);
  }

  static_assert(arraysize(vma_name_buffer->name) >= arraysize("stack_and_tls:") + 11 + 1);
  async_safe_format_buffer(vma_name_buffer->name, arraysize(vma_name_buffer->name),
                           "stack_and_tls:%d", thread->tid);

  if (mapping.stack_clean) {
    // The stack is brand-new, so show the kernel where in memory it can find the name of this
    // memory region.  If we're reusing a cached thread, we don't have to call
    // PR_SET_VMA_ANON_NAME again: we instead just update the memory in place to reflect the
    // memory region's new role.
    prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, thread->mmap_base_unguarded,
          thread->mmap_size_unguarded, vma_name_buffer->name);
  }

  // Locate static TLS structures within the mapped region.
  const StaticTlsLayout& layout = __libc_shared_globals()->static_tls_layout;
  auto tcb = reinterpret_cast<bionic_tcb*>(mapping.static_tls + layout.offset_bionic_tcb());
  auto tls = reinterpret_cast<bionic_tls*>(mapping.static_tls + layout.offset_bionic_tls());

  // Initialize TLS memory.
  if (!mapping.stack_clean) {
    memset(mapping.static_tls, 0, layout.size());
  }

  __init_static_tls(mapping.static_tls);
  __init_tcb(tcb, thread);
  __init_tcb_dtv(tcb);
  __init_tcb_stack_guard(tcb);
  __init_bionic_tls_ptrs(tcb, tls);

  attr->stack_size = stack_top - static_cast<char*>(attr->stack_base);
  thread->attr = *attr;
  thread->mmap_base = mapping.mmap_base;
  thread->mmap_size = mapping.mmap_size;
  thread->mmap_base_unguarded = mapping.mmap_base_unguarded;
  thread->mmap_size_unguarded = mapping.mmap_size_unguarded;
  thread->stack_top = reinterpret_cast<uintptr_t>(stack_top);
  thread->stack_cache_entry = mapping.stack_cache_entry;
  thread->alternate_signal_stack = mapping.alternate_signal_stack;
#ifdef BIONIC_USE_SCS
  thread->shadow_call_stack_guard_region = mapping.shadow_call_stack_guard_region;
#endif

  *tcbp = tcb;
  *child_stack = stack_top;
  return 0;
}

void __set_main_thread_name() {
  pthread_internal_t* thread = __get_thread();
  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, thread->mmap_base_unguarded, thread->mmap_size_unguarded,
        "stack_and_tls:main");
}

extern "C" int __rt_sigprocmask(int, const sigset64_t*, sigset64_t*, size_t);

__attribute__((no_sanitize("hwaddress")))
static int __pthread_start(void* arg) {
  pthread_internal_t* thread = reinterpret_cast<pthread_internal_t*>(arg);

  __hwasan_thread_enter();

  // Wait for our creating thread to release us. This lets it have time to
  // notify gdb about this thread before we start doing anything.
  // This also provides the memory barrier needed to ensure that all memory
  // accesses previously made by the creating thread are visible to us.
  thread->startup_handshake_lock.lock();

  __init_additional_stacks(thread);
  __rt_sigprocmask(SIG_SETMASK, &thread->start_mask, nullptr, sizeof(thread->start_mask));

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
  ScopedTrace trace("pthread_create");
  if (attr == nullptr) {
    pthread_attr_init(&thread_attr);
  } else {
    thread_attr = *attr;
    attr = nullptr; // Prevent misuse below.
  }

  bionic_tcb* tcb = nullptr;
  void* child_stack = nullptr;
  int result = __allocate_thread(&thread_attr, &tcb, &child_stack);
  if (result != 0) {
    return result;
  }

  pthread_internal_t* thread = tcb->thread();

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
  void* tls = &tcb->tls_slot(0);
#if defined(__i386__)
  // On x86 (but not x86-64), CLONE_SETTLS takes a pointer to a struct user_desc rather than
  // a pointer to the TLS itself.
  user_desc tls_descriptor;
  __init_user_desc(&tls_descriptor, false, tls);
  tls = &tls_descriptor;
#endif

  sigset64_t block_all_mask;
  sigfillset64(&block_all_mask);
  __rt_sigprocmask(SIG_SETMASK, &block_all_mask, &thread->start_mask, sizeof(thread->start_mask));
  int rc = clone(__pthread_start, child_stack, flags, thread, &(thread->tid), tls, &(thread->tid));
  __rt_sigprocmask(SIG_SETMASK, &thread->start_mask, nullptr, sizeof(thread->start_mask));
  if (rc == -1) {
    int clone_errno = errno;
    // We don't have to unlock the mutex at all because clone(2) failed so there's no child waiting to
    // be unblocked, but we're about to unmap the memory the mutex is stored in, so this serves as a
    // reminder that you can't rewrite this function to use a ScopedPthreadMutexLocker.
    thread->startup_handshake_lock.unlock();
    if (thread->mmap_size != 0) {
      munmap(thread->mmap_base, thread->mmap_size);
    }
    free(thread->stack_cache_entry);
    async_safe_format_log(ANDROID_LOG_ERROR, "libc", "pthread_create failed: clone failed: %s",
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
