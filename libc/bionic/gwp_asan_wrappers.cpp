/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <platform/bionic/malloc.h>
#include <private/bionic_malloc_dispatch.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "gwp_asan/guarded_pool_allocator.h"
#include "gwp_asan/options.h"
#include "gwp_asan/random.h"
#include "malloc_common.h"

static gwp_asan::GuardedPoolAllocator GuardedAlloc;
static const MallocDispatch* prev_dispatch;

using Options = gwp_asan::options::Options;

// TODO(mitchp): Turn on GWP-ASan here probabilistically.
bool ShouldGwpAsanSampleProcess() {
  return false;
}

// Initializes GWP-ASan. Called by android_mallopt().
bool InitGwpAsan(void* arg, size_t arg_size) {
  if (arg_size != sizeof(bool)) {
    return false;
  }

  // If the caller hasn't forced GWP-ASan on, check whether we should sample
  // this process.
  if ((*static_cast<bool*>(arg)) == false && !ShouldGwpAsanSampleProcess()) {
    return false;
  }

  Options Opts;
  Opts.Enabled = true;
  Opts.MaxSimultaneousAllocations = 32;
  Opts.SampleRate = 2500;
  Opts.InstallSignalHandlers = false;
  Opts.InstallForkHandlers = true;

  GuardedAlloc.init(Opts);
  info_log("GWP-ASan has been enabled.");
  return true;
}

// ============================================================================
// Implementation of gFunctions.
// ============================================================================

// This function handles initialisation as asked for by MallocInitImpl.
bool gwp_asan_initialize(const MallocDispatch* dispatch, bool* zygote_child, const char*) {
  // Always save the dispatch, in case we turn on GWP-ASan via mallopt() later.
  // GWP-ASan is generally turned on via. mallopt() (including below).
  prev_dispatch = dispatch;

  // Never initialize the Zygote here. A Zygote chosen for sampling would also
  // have all of its children sampled. Instead, the Zygote child will choose
  // whether it samples or not just after the Zygote forks.
  if (zygote_child == nullptr || *zygote_child == true ||
      strcmp(getprogname(), "app_process") == 0) {
    return false;
  }

  bool ForceInitGwpAsan = false;
  return android_mallopt(M_INITIALIZE_GWP_ASAN, &ForceInitGwpAsan, sizeof(ForceInitGwpAsan));
}

void gwp_asan_finalize() {
}

void gwp_asan_get_malloc_leak_info(uint8_t**, size_t*, size_t*, size_t*, size_t*) {
}

void gwp_asan_free_malloc_leak_info(uint8_t*) {
}

ssize_t gwp_asan_malloc_backtrace(void*, uintptr_t*, size_t) {
  // TODO(mitchp): GWP-ASan might be able to return the backtrace for the
  // provided address.
  return 0;
}

bool gwp_asan_write_malloc_leak_info(FILE*) {
  return false;
}

static void* GwpAsanGlobalFunctions[] = {
  (void*)gwp_asan_initialize,           (void*)gwp_asan_finalize,
  (void*)gwp_asan_get_malloc_leak_info, (void*)gwp_asan_free_malloc_leak_info,
  (void*)gwp_asan_malloc_backtrace,     (void*)gwp_asan_write_malloc_leak_info,
};

void** GetGwpAsanGlobalFunctions() {
  return GwpAsanGlobalFunctions;
}

// ============================================================================
// Implementation of GWP-ASan malloc wrappers.
// ============================================================================

void* gwp_asan_calloc(size_t n_elements, size_t elem_size) {
  if (__predict_false(GuardedAlloc.shouldSample())) {
    size_t bytes;
    if (!__builtin_mul_overflow(n_elements, elem_size, &bytes)) {
      if (void* result = GuardedAlloc.allocate(bytes)) {
        return result;
      }
    }
  }
  return prev_dispatch->calloc(n_elements, elem_size);
}

void gwp_asan_free(void* mem) {
  if (__predict_false(GuardedAlloc.pointerIsMine(mem))) {
    GuardedAlloc.deallocate(mem);
    return;
  }
  prev_dispatch->free(mem);
}

void* gwp_asan_malloc(size_t bytes) {
  if (__predict_false(GuardedAlloc.shouldSample())) {
    if (void* result = GuardedAlloc.allocate(bytes)) {
      return result;
    }
  }
  return prev_dispatch->malloc(bytes);
}

size_t gwp_asan_malloc_usable_size(const void* mem) {
  if (__predict_false(GuardedAlloc.pointerIsMine(mem))) {
    return GuardedAlloc.getSize(mem);
  }
  return prev_dispatch->malloc_usable_size(mem);
}

void* gwp_asan_realloc(void* old_mem, size_t bytes) {
  if (__predict_false(GuardedAlloc.pointerIsMine(old_mem))) {
    size_t old_size = GuardedAlloc.getSize(old_mem);
    void* new_ptr = gwp_asan_malloc(bytes);
    if (new_ptr) memcpy(new_ptr, old_mem, (bytes < old_size) ? bytes : old_size);
    GuardedAlloc.deallocate(old_mem);
    return new_ptr;
  }
  return prev_dispatch->realloc(old_mem, bytes);
}

int gwp_asan_malloc_iterate(uintptr_t base, size_t size,
                            void (*callback)(uintptr_t base, size_t size, void* arg), void* arg) {
  if (__predict_false(GuardedAlloc.pointerIsMine(reinterpret_cast<void*>(base)))) {
    // TODO(mitchp): GPA::iterate() returns void, but should return int.
    // TODO(mitchp): GPA::iterate() should take uintptr_t, not void*.
    GuardedAlloc.iterate(reinterpret_cast<void*>(base), size, callback, arg);
    return 0;
  }
  return prev_dispatch->malloc_iterate(base, size, callback, arg);
}

void gwp_asan_malloc_disable() {
  GuardedAlloc.disable();
  prev_dispatch->malloc_disable();
}

void gwp_asan_malloc_enable() {
  GuardedAlloc.enable();
  prev_dispatch->malloc_enable();
}

static MallocDispatch GwpAsanDispatch __attribute__((unused)) = {
  gwp_asan_calloc,
  gwp_asan_free,
  Malloc(mallinfo),
  gwp_asan_malloc,
  gwp_asan_malloc_usable_size,
  Malloc(memalign),
  Malloc(posix_memalign),
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  Malloc(pvalloc),
#endif
  gwp_asan_realloc,
#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
  Malloc(valloc),
#endif
  gwp_asan_malloc_iterate,
  gwp_asan_malloc_disable,
  gwp_asan_malloc_enable,
  Malloc(mallopt),
  Malloc(aligned_alloc),
  Malloc(malloc_info),
};

MallocDispatch* GetGwpAsanMallocDispatch() {
  return &GwpAsanDispatch;
}
