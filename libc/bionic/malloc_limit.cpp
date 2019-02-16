/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <inttypes.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>

#include <private/bionic_malloc_dispatch.h>

#include "malloc_common.h"
#include "malloc_common_dynamic.h"
#include "malloc_heapprofd.h"
#include "malloc_limit.h"

__BEGIN_DECLS
static void* LimitCalloc(size_t n_elements, size_t elem_size);
static void LimitFree(void* mem);
static void* LimitMalloc(size_t bytes);
static void* LimitMemalign(size_t alignment, size_t bytes);
static int LimitPosixMemalign(void** memptr, size_t alignment, size_t size);
static void* LimitRealloc(void* old_mem, size_t bytes);
static void* LimitAlignedAlloc(size_t alignment, size_t size);
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
static void* LimitPvalloc(size_t bytes);
static void* LimitValloc(size_t bytes);
#endif

// Pass through functions.
static size_t LimitUsableSize(const void* mem);
static struct mallinfo LimitMallinfo();
static int LimitIterate(uintptr_t base, size_t size, void (*callback)(uintptr_t, size_t, void*), void* arg);
static void LimitMallocDisable();
static void LimitMallocEnable();
static int LimitMallopt(int param, int value);
__END_DECLS

static constexpr MallocDispatch __limit_dispatch
  __attribute__((unused)) = {
    LimitCalloc,
    LimitFree,
    LimitMallinfo,
    LimitMalloc,
    LimitUsableSize,
    LimitMemalign,
    LimitPosixMemalign,
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
    LimitPvalloc,
#endif
    LimitRealloc,
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
    LimitValloc,
#endif
    LimitIterate,
    LimitMallocDisable,
    LimitMallocEnable,
    LimitMallopt,
    LimitAlignedAlloc,
  };

static _Atomic uint64_t gAllocated;
static uint64_t gAllocLimit;

static inline bool CheckLimit(size_t bytes) {
  uint64_t total;
  if (__predict_false(__builtin_add_overflow(atomic_load(&gAllocated), bytes, &total) ||
      total > gAllocLimit)) {
    error_log("Allocation would exceed limit %" PRId64, gAllocLimit);
    return false;
  }
  return true;
}

static inline void* IncrementLimit(void* mem) {
  if (__predict_false(mem == nullptr)) {
    return nullptr;
  }
  atomic_fetch_add(&gAllocated, LimitUsableSize(mem));
  return mem;
}

void* LimitCalloc(size_t n_elements, size_t elem_size) {
  size_t total;
  if (__builtin_add_overflow(n_elements, elem_size, &total) || !CheckLimit(total)) {
    return nullptr;
  }
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return IncrementLimit(dispatch_table->calloc(n_elements, elem_size));
  }
  return IncrementLimit(Malloc(calloc)(n_elements, elem_size));
}

void LimitFree(void* mem) {
  atomic_fetch_sub(&gAllocated, LimitUsableSize(mem));
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->free(mem);
  }
  return Malloc(free)(mem);
}

void* LimitMalloc(size_t bytes) {
  if (!CheckLimit(bytes)) {
    return nullptr;
  }
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return IncrementLimit(dispatch_table->malloc(bytes));
  }
  return IncrementLimit(Malloc(malloc)(bytes));
}

static void* LimitMemalign(size_t alignment, size_t bytes) {
  if (!CheckLimit(bytes)) {
    return nullptr;
  }
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return IncrementLimit(dispatch_table->memalign(alignment, bytes));
  }
  return IncrementLimit(Malloc(memalign)(alignment, bytes));
}

static int LimitPosixMemalign(void** memptr, size_t alignment, size_t size) {
  if (!CheckLimit(size)) {
    return ENOMEM;
  }
  int retval;
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    retval = dispatch_table->posix_memalign(memptr, alignment, size);
  } else {
    retval = Malloc(posix_memalign)(memptr, alignment, size);
  }
  if (__predict_false(retval != 0)) {
    return retval;
  }
  IncrementLimit(*memptr);
  return 0;
}

static void* LimitAlignedAlloc(size_t alignment, size_t size) {
  if (!CheckLimit(size)) {
    return nullptr;
  }
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return IncrementLimit(dispatch_table->aligned_alloc(alignment, size));
  }
  return IncrementLimit(Malloc(aligned_alloc)(alignment, size));
}

static void* LimitRealloc(void* old_mem, size_t bytes) {
  size_t old_usable_size = LimitUsableSize(old_mem);
  void* new_ptr;
  // Need to check the size only if the allocation will increase in size.
  if (bytes > old_usable_size && !CheckLimit(bytes - old_usable_size)) {
    // Free the old pointer.
    LimitFree(old_mem);
    return nullptr;
  }

  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    new_ptr = dispatch_table->realloc(old_mem, bytes);
  } else {
    new_ptr = Malloc(realloc)(old_mem, bytes);
  }

  if (__predict_false(new_ptr == nullptr)) {
    // This acts as if the pointer was freed.
    atomic_fetch_sub(&gAllocated, old_usable_size);
    return nullptr;
  }

  size_t new_usable_size = LimitUsableSize(new_ptr);
  // Assumes that most allocations increase in size, rather than shrink.
  if (__predict_false(old_usable_size > new_usable_size)) {
    atomic_fetch_sub(&gAllocated, old_usable_size - new_usable_size);
  } else {
    atomic_fetch_add(&gAllocated, new_usable_size - old_usable_size);
  }
  return new_ptr;
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
static void* LimitPvalloc(size_t bytes) {
  if (!CheckLimit(bytes)) {
    return nullptr;
  }
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return IncrementLimit(dispatch_table->pvalloc(bytes));
  }
  return IncrementLimit(Malloc(pvalloc)(bytes));
}

static void* LimitValloc(size_t bytes) {
  if (!CheckLimit(bytes)) {
    return nullptr;
  }
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return IncrementLimit(dispatch_table->valloc(bytes));
  }
  return IncrementLimit(Malloc(valloc)(bytes));
}
#endif

#if defined(LIBC_STATIC)
static bool EnableLimitDispatchTable() {
  // This is the only valid way to modify the dispatch tables for a
  // static executable so no locks are necessary.
  __libc_globals.mutate([](libc_globals* globals) {
    atomic_store(&globals->current_dispatch_table, &__limit_dispatch);
  });
  return true;
}
#else
static bool EnableLimitDispatchTable() {
  pthread_mutex_lock(&gGlobalsMutateLock);
  // This could conflict with the signal handler to enable heapprofd.
  // If that happens, allow this call to take up to about five ms to grab.
  // the lock. That should be plenty of time even in the worst case.
  bool enabled = false;
  for (size_t i = 0; i < 5; i++) {
    if (i != 0) {
      usleep(1000);
    }
    if (!atomic_exchange(&gGlobalsMutating, true)) {
      HeapprofdMaskSignalHandler();
      __libc_globals.mutate([](libc_globals* globals) {
        atomic_store(&globals->current_dispatch_table, &__limit_dispatch);
      });
      atomic_store(&gGlobalsMutating, false);
      HeapprofdUnmaskSignalHandler();
      enabled = true;
      break;
    }
  }
  pthread_mutex_unlock(&gGlobalsMutateLock);
  if (!enabled) {
    error_log("Cannot enable allocation limit dispatch table, heapprofd is blocking.");
  }
  return enabled;
}
#endif

bool LimitEnable(void* arg, size_t arg_size) {
  if (arg == nullptr || arg_size != sizeof(size_t)) {
    errno = EINVAL;
    return false;
  }

  static _Atomic bool limit_enabled;
  if (atomic_exchange(&limit_enabled, true)) {
    // The limit can only be enabled once.
    error_log("The allocation limit has already been set, it can only be set once.");
    return false;
  }

  gAllocLimit = *reinterpret_cast<size_t*>(arg);
#if __has_feature(hwaddress_sanitizer)
  size_t current_allocated = __sanitizer_get_current_allocated_bytes();
#else
  size_t current_allocated;
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    current_allocated = dispatch_table->mallinfo().uordblks;
  } else {
    current_allocated = Malloc(mallinfo)().uordblks;
  }
#endif
  atomic_store(&gAllocated, current_allocated);

  return EnableLimitDispatchTable();
}

static size_t LimitUsableSize(const void* mem) {
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->malloc_usable_size(mem);
  }
  return Malloc(malloc_usable_size)(mem);
}

static struct mallinfo LimitMallinfo() {
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->mallinfo();
  }
  return Malloc(mallinfo)();
}

static int LimitIterate(uintptr_t base, size_t size, void (*callback)(uintptr_t, size_t, void*), void* arg) {
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->iterate(base, size, callback, arg);
  }
  return Malloc(iterate)(base, size, callback, arg);
}

static void LimitMallocDisable() {
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    dispatch_table->malloc_disable();
  } else {
    Malloc(malloc_disable)();
  }
}

static void LimitMallocEnable() {
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    dispatch_table->malloc_enable();
  } else {
    Malloc(malloc_enable)();
  }
}

static int LimitMallopt(int param, int value) {
  auto dispatch_table = GetDefaultDispatchTable();
  if (__predict_false(dispatch_table != nullptr)) {
    return dispatch_table->mallopt(param, value);
  }
  return Malloc(mallopt)(param, value);
}
