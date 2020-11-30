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

#include <alloca.h>
#include <assert.h>
#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "gwp_asan/guarded_pool_allocator.h"
#include "gwp_asan/options.h"
#include "gwp_asan_wrappers.h"
#include "malloc_common.h"
#include "platform/bionic/android_unsafe_frame_pointer_chase.h"
#include "platform/bionic/malloc.h"
#include "private/bionic_arc4random.h"
#include "private/bionic_globals.h"
#include "private/bionic_malloc_dispatch.h"
#include "sys/system_properties.h"
#include "sysprop_helpers.h"

#ifndef LIBC_STATIC
#include "bionic/malloc_common_dynamic.h"
#endif  // LIBC_STATIC

static gwp_asan::GuardedPoolAllocator GuardedAlloc;
static const MallocDispatch* prev_dispatch;

using Options = gwp_asan::options::Options;

extern "C" const char* __gnu_basename(const char* path);
extern "C" bool ParseGwpAsanOptions(Options* options, unsigned* process_sample_rate,
                                    char* options_str);

namespace {

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

const MallocDispatch gwp_asan_dispatch __attribute__((unused)) = {
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

bool isPowerOfTwo(uint64_t x) {
  assert(x != 0);
  return (x & (x - 1)) == 0;
}

bool ShouldGwpAsanSampleProcess(unsigned sample_rate) {
  if (!isPowerOfTwo(sample_rate)) {
    warning_log(
        "GWP-ASan process sampling rate of %u is not a power-of-two, and so modulo bias occurs.",
        sample_rate);
  }

  uint8_t random_number;
  __libc_safe_arc4random_buf(&random_number, sizeof(random_number));
  return random_number % sample_rate == 0;
}

bool GwpAsanInitialized = false;

using SampleRate_t = typeof(gwp_asan::options::Options::SampleRate);
using SimultaneousAllocations_t = typeof(gwp_asan::options::Options::MaxSimultaneousAllocations);
constexpr SampleRate_t kDefaultSampleRate = 2500;
constexpr SimultaneousAllocations_t kDefaultSimultaneousAllocations = 32;
// The probability (1 / kDefaultProcessLotteryRate) that a process will be
// ranodmly selected for sampling, for system apps and system processes.
// kDefaultProcessLotteryRate should always be a power of two to avoid modulo
// bias.
constexpr unsigned kDefaultProcessLotteryRate = 128;

// This function takes the provided options string, and tokenizes "1:2:3" into
// individual tokens[0] = "1", tokens[1] = "2", and tokens[2] = "3". If any of
// the tokens are missing (i.e. there are two ':'s next to each other), then the
// corresponding returned token is `nullptr`. This function returns true if
// parsing succeeded (even if some of the tokens are null), or false if anything
// was out of the ordinary (more than 3 tokens, non-numerical tokens, etc.).
//
// Note: strtok() isn't great for this, because "::::::5" would have the first
// token be "5" (where we want it to exit). android::base::Split() would also be
// much cleaner than doing this manually, but we can't pull in any cxx
// dependencies to libc. Hence, the ugly manual approach below.
bool TokenizeOptionsStringInplace(char* tokens[3], char* options_str) {
  assert(options_str != nullptr);
  tokens[0] = nullptr;
  tokens[1] = nullptr;
  tokens[2] = nullptr;

  int token_index = 0;
  char* token_start = options_str;
  char* it = options_str;
  while (*it != '\0') {
    char* c = it++;
    if (*c == ':') {
      // The last token should have been terminated by the null-terminator, not a colon.
      if (token_index == 2) {
        return false;
      }

      *c = '\0';

      if (c == token_start) {
        token_start++;
        token_index++;
        continue;
      }
      tokens[token_index++] = token_start;
      token_start = c + 1;
      continue;
    } else if (!isdigit(*c)) {
      return false;
    }
  }

  if (it != token_start) {
    tokens[token_index] = token_start;
  }
  return true;
}

static const char* kGwpAsanEnvVariable = "GWP_ASAN_OPTIONS";
// A system property to control the default GWP-ASan options for system apps and
// system native processes.
static const char* kGwpAsanDefaultSystemSysprop = "libc.debug.gwp_asan.system_default";
// A system property to control the default GWP-ASan options for non-system
// apps.
static const char* kGwpAsanDefaultAppSysprop = "libc.debug.gwp_asan.app_default";
static const char* kGwpAsanPrognameSyspropPrefix = "libc.debug.gwp_asan.";

void SetDefaultGwpAsanOptions(Options* options, unsigned* process_sample_rate,
                              const android_mallopt_gwp_asan_options_t& mallopt_options) {
  options->Enabled = true;
  options->InstallSignalHandlers = false;
  options->InstallForkHandlers = true;
  options->Backtrace = android_unsafe_frame_pointer_chase;
  options->SampleRate = kDefaultSampleRate;
  options->MaxSimultaneousAllocations = kDefaultSimultaneousAllocations;

  *process_sample_rate = 1;
  if (mallopt_options.use_lottery) {
    *process_sample_rate = kDefaultProcessLotteryRate;
  }
}

// Check whether someone has asked for specific GWP-ASan settings. The order of priority is:
//  1. Environment variables.
//  2. Process-specific system properties.
//  3. Global system properties.
// If none of these are specified, we fallback to the platform options. These settings are
// never considered for the zygote, the zygote pre-fork never has GWP-ASan enabled.
// Returns true if options were found, false otherwise.
bool GetGwpAsanOptions(Options* options, unsigned* process_sample_rate,
                       const android_mallopt_gwp_asan_options_t& mallopt_options) {
  SetDefaultGwpAsanOptions(options, process_sample_rate, mallopt_options);

  char settings_buf[128];
  const char* basename = __gnu_basename(mallopt_options.program_name);

  size_t program_specific_sysprop_size =
      strlen(kGwpAsanPrognameSyspropPrefix) + strlen(basename) + 1;
  char* program_specific_sysprop_name = static_cast<char*>(alloca(program_specific_sysprop_size));
  async_safe_format_buffer(program_specific_sysprop_name, program_specific_sysprop_size, "%s%s",
                           kGwpAsanPrognameSyspropPrefix, mallopt_options.program_name);

  const char* sysprop_names[2] = {nullptr, nullptr};
  // Tests use a blank program name to specify that system properties should not
  // be used. Tests still continue to use the environment variable though.
  if (*basename != '\0') {
    sysprop_names[0] = program_specific_sysprop_name;
    if (mallopt_options.use_lottery) {
      sysprop_names[1] = kGwpAsanDefaultAppSysprop;
    } else {
      sysprop_names[1] = kGwpAsanDefaultSystemSysprop;
    }
  }

  if (!get_config_from_env_or_sysprops(kGwpAsanEnvVariable, sysprop_names,
                                       /* sys_prop_names_size */ 2, settings_buf,
                                       sizeof(settings_buf))) {
    return false;
  }

  if (!ParseGwpAsanOptions(options, process_sample_rate, settings_buf)) {
    warning_log(
        "GWP-ASan options string \"%s\" for process \"%s\" was invalid, using default values.",
        settings_buf, mallopt_options.program_name);
    return false;
  }
  return true;
}

bool MaybeInitGwpAsan(libc_globals* globals,
                      const android_mallopt_gwp_asan_options_t& mallopt_options) {
  if (GwpAsanInitialized) {
    error_log("GWP-ASan was already initialized for this process.");
    return false;
  }

  Options options;
  unsigned process_sample_rate;
  GetGwpAsanOptions(&options, &process_sample_rate, mallopt_options);

  info_log("GWP-ASan: %s \"%s\": SampleRate %u, PSRate %u, MSA %u\n", mallopt_options.program_name,
           getenv("GWP_ASAN_OPTIONS"), options.SampleRate, process_sample_rate,
           options.MaxSimultaneousAllocations);

  if (!ShouldGwpAsanSampleProcess(process_sample_rate)) {
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

  GuardedAlloc.init(options);

  __libc_shared_globals()->gwp_asan_state = GuardedAlloc.getAllocatorState();
  __libc_shared_globals()->gwp_asan_metadata = GuardedAlloc.getMetadataRegion();

  return true;
}
};  // anonymous namespace

// The options string should match r"\d*:\d*:\d*". The first number is the
// allocation sampling rate ("0" meaning to disable GWP-ASan, or blank for
// default), the second is the process sampling rate, and the third is the
// number of simultaneous allocations supported. If the number of simultaneous
// allocations is not provided, it's calculated by the system to keep the same
// ratio of "sample_rate" to "maximum slots" as the default values. So:
//  - "", ":", "::": GWP-ASan uses the default options; allocation sample rate
//    of "2500", process sample rate of "128" (i.e. 0.78% of the time) for
//    system apps and system binaries, process sample rate of "1" (i.e. 100% of
//    the time) for non-system apps, and "32" maximum simultaneous allocations
//    ("slots").
//  - "0", "1000:0", "1000:25:0": GWP-ASan is disabled because the
//    allocation/process sample rate is "0", or the number of slots is "0".
//  - "1000:2:64": GWP-ASan is enabled with an allocation sampling rate of
//    "1000" (i.e. 1/1000 allocations get sampled), process sampling rate of "2"
//    (i.e. 50% of process startups enable GWP-ASan), and "64" maximum GWP-ASan
//    slots.
//  - "1000", "1000:", "1000::": GWP-ASan uses an allocation sampling rate of
//    "1000", a default process sampling rate ("1" for system apps and system
//    processes, "128" for non-system apps), and "80" maximum allocations. The
//    "80" is automatically calculated because the sampling rate is 2.5x more
//    often than the default "2500", and so we also multiply the number of slots
//    (default "32") by 2.5x to compensate.
//  - ":4", ":4:": GWP-ASan is enabled with default allocation sampling rate
//    ("2500"), a process sampling rate of "4" (i.e. 25% of launches), and
//    default number of slots ("32").
//  - "::64": GWP-ASan is enabled with default allocation sampling, default
//    process sampling, and with "64" slots.
bool ParseGwpAsanOptions(Options* options, unsigned* process_sample_rate, char* options_str) {
  char* tokens[3];
  if (!TokenizeOptionsStringInplace(tokens, options_str)) {
    return false;
  }

  SampleRate_t default_sample_rate = options->SampleRate;
  SimultaneousAllocations_t default_max_allocs = options->MaxSimultaneousAllocations;

  if (tokens[0] != nullptr) {
    options->SampleRate = strtoull(tokens[0], nullptr, 10);
  }

  if (tokens[1] != nullptr) {
    *process_sample_rate = strtoull(tokens[1], nullptr, 10);
  }

  if (tokens[2] != nullptr) {
    options->MaxSimultaneousAllocations = strtoull(tokens[2], nullptr, 10);
  } else {
    // Multiply the number of slots available, such that the ratio between
    // sampling rate and slots is kept the same as the default. For example, a
    // sampling rate of 1000 is 2.5x more frequent than default, and so requires
    // 80 slots (32 * 2.5).
    float frequency_multiplier = static_cast<float>(options->SampleRate) / default_sample_rate;
    options->MaxSimultaneousAllocations =
        /* default */ default_max_allocs / frequency_multiplier;
  }
  return true;
}

bool MaybeInitGwpAsanFromLibc(libc_globals* globals) {
  // Never initialize the Zygote here. A Zygote chosen for sampling would also
  // have all of its children sampled. Instead, the Zygote child will choose
  // whether it samples or not just after the Zygote forks. Note that the Zygote
  // changes its name after it's started, at this point it's still called
  // "app_process" or "app_process64".
  constexpr char* kAppProcessNamePrefix = "app_process";
  const char* progname = getprogname();
  if (strncmp(progname, kAppProcessNamePrefix, sizeof(*kAppProcessNamePrefix) - 1) == 0)
    return false;

  android_mallopt_gwp_asan_options_t mallopt_options;
  mallopt_options.program_name = progname;
  mallopt_options.use_lottery = false;

  return MaybeInitGwpAsan(globals, mallopt_options);
}

bool DispatchIsGwpAsan(const MallocDispatch* dispatch) {
  return dispatch == &gwp_asan_dispatch;
}

bool EnableGwpAsan(const android_mallopt_gwp_asan_options_t& options) {
  if (GwpAsanInitialized) {
    return true;
  }

  bool ret_value;
  __libc_globals.mutate(
      [&](libc_globals* globals) { ret_value = MaybeInitGwpAsan(globals, options); });
  return ret_value;
}
