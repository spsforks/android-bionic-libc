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

#include <platform/bionic/android_unsafe_frame_pointer_chase.h>
#include <platform/bionic/malloc.h>
#include <private/bionic_arc4random.h>
#include <private/bionic_globals.h>
#include <private/bionic_malloc_dispatch.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "bionic/gwp_asan_wrappers.h"
#include "gwp_asan/guarded_pool_allocator.h"
#include "gwp_asan/optional/options_parser.h"
#include "gwp_asan/options.h"
#include "malloc_common.h"
#include "sys/system_properties.h"

#ifndef LIBC_STATIC
#include "bionic/malloc_common_dynamic.h"
#endif  // LIBC_STATIC

static gwp_asan::GuardedPoolAllocator GuardedAlloc;
static const MallocDispatch* prev_dispatch;

using Options = gwp_asan::options::Options;

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

static const MallocDispatch gwp_asan_dispatch __attribute__((unused)) = {
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

// The probability (1 / kProcessSampleRate) that a process will be ranodmly
// selected for sampling. kProcessSampleRate should always be a power of two to
// avoid modulo bias.
static constexpr uint8_t kProcessSampleRate = 4;

static bool ShouldGwpAsanSampleProcess() {
  uint8_t random_number;
  __libc_safe_arc4random_buf(&random_number, sizeof(random_number));
  return random_number % kProcessSampleRate == 0;
}

static constexpr char kGwpAsanEnvVariable[] = "GWP_ASAN_OPTIONS";
static constexpr char kGwpAsanGlobalSysprops[] = "debug.gwp_asan";
static constexpr char kGwpAsanProgramSyspropsPrefix[] = "debug.gwp_asan.";

static bool GetPropertyValueIfExists(const char* property_name, char* dest, size_t dest_size) {
  const prop_info* prop = __system_property_find(property_name);
  if (!prop) return false;

  struct PropCbCookie {
    char* dest;
    size_t size;
  };
  *dest = '\0';
  PropCbCookie cb_cookie = {dest, dest_size};

  __system_property_read_callback(
      prop,
      [](void* cookie, const char* /* name */, const char* value, uint32_t /* serial */) {
        auto* cb_cookie = reinterpret_cast<PropCbCookie*>(cookie);
        strncpy(cb_cookie->dest, value, cb_cookie->size);
      },
      &cb_cookie);
  if (*dest != '\0' && *dest != '0') return true;

  return false;
}

// Check whether someone has asked for specific GWP-ASan settings. The order of priority is:
//  1. Environment variables.
//  2. Process-specific system properties.
//  3. Global system properties.
// If none of these are specified, we fallback to the platform options. These settings are
// never considered for the zygote, the zygote pre-fork never has GWP-ASan enabled.
// Returns true if options were found, false otherwise.
static bool GetGwpAsanOptions(const char* progname, char* options, size_t size) {
  const char* env = getenv(kGwpAsanEnvVariable);
  if (env && *env != '\0' && *env != '0') {
    strncpy(options, env, size);
    return true;
  }

  size_t program_specific_sysprop_size =
      strlen(progname) * sizeof(char) + sizeof(kGwpAsanProgramSyspropsPrefix) / sizeof(char);
  char* program_specific_sysprop_name = static_cast<char*>(alloca(program_specific_sysprop_size));
  async_safe_format_buffer(program_specific_sysprop_name, program_specific_sysprop_size, "%s%s",
                           kGwpAsanProgramSyspropsPrefix, progname);

  if (GetPropertyValueIfExists(program_specific_sysprop_name, options, size) ||
      GetPropertyValueIfExists(kGwpAsanGlobalSysprops, options, size)) {
    return true;
  }

  return false;
}

bool MaybeInitGwpAsanFromLibc(libc_globals* globals) {
  // Never initialize the Zygote here. A Zygote chosen for sampling would also
  // have all of its children sampled. Instead, the Zygote child will choose
  // whether it samples or not just after the Zygote forks. Note that the Zygote
  // changes its name after it's started, at this point it's still called
  // "app_process" or "app_process64".
  const char* progname = getprogname();
  if (strncmp(progname, "app_process", 11) == 0) {
    // If there's an env flag or sysprop to enable GWP-ASan, warn that we're not enabling GWP-ASan
    // in the Zygote pre-fork.
    char options;
    if (GetGwpAsanOptions(progname, &options, /* strlen(options) */ 1)) {
      warning_log(
          "GWP-ASan was requested in the Zygote, but was ignored as the Zygote is ineligible.");
    }
    return false;
  }
  return MaybeInitGwpAsan(globals);
}

static bool GwpAsanInitialized = false;

namespace {
void PrintfWrapper(const char* Format, ...) {
  va_list List;
  va_start(List, Format);
  async_safe_fatal_va_list("GWP-ASan", Format, List);
  va_end(List);
}
};  // anonymous namespace

bool MaybeInitGwpAsan(libc_globals* globals, android_mallopt_gwp_asan_options_t* mallopt_options) {
  if (GwpAsanInitialized) {
    error_log("GWP-ASan was already initialized for this process.");
    return false;
  }

  const char* progname;
  if (mallopt_options && mallopt_options->program_name) {
    progname = mallopt_options->program_name;
  } else {
    progname = getprogname();
  }

  static constexpr size_t kOptionsSize = PROP_VALUE_MAX;
  char options_str[kOptionsSize];

  bool has_options = GetGwpAsanOptions(progname, options_str, kOptionsSize);
  if (!has_options && (mallopt_options == nullptr || mallopt_options->use_lottery == true) &&
      !ShouldGwpAsanSampleProcess()) {
    return false;
  }

  // GWP-ASan is compatible with heapprofd/malloc_debug/malloc_hooks iff
  // GWP-ASan was installed first. If one of these other libraries was already
  // installed, we don't enable GWP-ASan. These libraries are normally enabled
  // in libc_init after GWP-ASan, but if the new process is a zygote child and
  // trying to initialize GWP-ASan through mallopt(), one of these libraries may
  // be installed. It may be possible to change this in future by modifying the
  // internal dispatch pointers of these libraries at this point in time, but
  // given that they're all debug-only, we don't really mind for now.
  if (GetDefaultDispatchTable() != nullptr) {
    // Something else is installed.
    return false;
  }

  // GWP-ASan's initialization is always called in a single-threaded context, so
  // we can initialize lock-free.
  // Set GWP-ASan as the malloc dispatch table.
  globals->malloc_dispatch_table = gwp_asan_dispatch;
  atomic_store(&globals->default_dispatch_table, &gwp_asan_dispatch);

  // If malloc_limit isn't installed, we can skip the default_dispatch_table
  // lookup.
  if (GetDispatchTable() == nullptr) {
    atomic_store(&globals->current_dispatch_table, &gwp_asan_dispatch);
  }

  GwpAsanInitialized = true;

  prev_dispatch = NativeAllocatorDispatch();

  Options Opts;
  Opts.InstallSignalHandlers = false;
  Opts.InstallForkHandlers = true;
  Opts.Backtrace = android_unsafe_frame_pointer_chase;
  Opts.Enabled = true;
  Opts.MaxSimultaneousAllocations = 32;
  Opts.SampleRate = 2500;
  if (has_options) {
    gwp_asan::options::initOptions(options_str, PrintfWrapper);
    Opts = gwp_asan::options::getOptions();
  }

  GuardedAlloc.init(Opts);
  if (has_options) {
    info_log("GWP-ASan was initialized for \"%s\" with the following options: \"%s\".", progname,
             options_str);
  } else {
    info_log("GWP-ASan has been enabled for \"%s\" with default options.", progname);
  }

  __libc_shared_globals()->gwp_asan_state = GuardedAlloc.getAllocatorState();
  __libc_shared_globals()->gwp_asan_metadata = GuardedAlloc.getMetadataRegion();

  return true;
}

bool DispatchIsGwpAsan(const MallocDispatch* dispatch) {
  return dispatch == &gwp_asan_dispatch;
}
