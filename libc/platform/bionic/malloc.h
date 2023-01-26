/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>

// Structures for android_mallopt.

typedef struct {
  // Pointer to the buffer allocated by a call to M_GET_MALLOC_LEAK_INFO.
  uint8_t* buffer;
  // The size of the "info" buffer.
  size_t overall_size;
  // The size of a single entry.
  size_t info_size;
  // The sum of all allocations that have been tracked. Does not include
  // any heap overhead.
  size_t total_memory;
  // The maximum number of backtrace entries.
  size_t backtrace_size;
} android_mallopt_leak_info_t;

// Opcodes for android_mallopt.

enum {
  // Marks the calling process as a profileable zygote child, possibly
  // initializing profiling infrastructure.
  M_INIT_ZYGOTE_CHILD_PROFILING = 1,
#define M_INIT_ZYGOTE_CHILD_PROFILING M_INIT_ZYGOTE_CHILD_PROFILING
  M_RESET_HOOKS = 2,
#define M_RESET_HOOKS M_RESET_HOOKS
  // Set an upper bound on the total size in bytes of all allocations made
  // using the memory allocation APIs.
  //   arg = size_t*
  //   arg_size = sizeof(size_t)
  M_SET_ALLOCATION_LIMIT_BYTES = 3,
#define M_SET_ALLOCATION_LIMIT_BYTES M_SET_ALLOCATION_LIMIT_BYTES
  // Called after the zygote forks to indicate this is a child.
  M_SET_ZYGOTE_CHILD = 4,
#define M_SET_ZYGOTE_CHILD M_SET_ZYGOTE_CHILD

  // Options to dump backtraces of allocations. These options only
  // work when malloc debug has been enabled.

  // Writes the backtrace information of all current allocations to a file.
  // NOTE: arg_size has to be sizeof(FILE*) because FILE is an opaque type.
  //   arg = FILE*
  //   arg_size = sizeof(FILE*)
  M_WRITE_MALLOC_LEAK_INFO_TO_FILE = 5,
#define M_WRITE_MALLOC_LEAK_INFO_TO_FILE M_WRITE_MALLOC_LEAK_INFO_TO_FILE
  // Get information about the backtraces of all
  //   arg = android_mallopt_leak_info_t*
  //   arg_size = sizeof(android_mallopt_leak_info_t)
  M_GET_MALLOC_LEAK_INFO = 6,
#define M_GET_MALLOC_LEAK_INFO M_GET_MALLOC_LEAK_INFO
  // Free the memory allocated and returned by M_GET_MALLOC_LEAK_INFO.
  //   arg = android_mallopt_leak_info_t*
  //   arg_size = sizeof(android_mallopt_leak_info_t)
  M_FREE_MALLOC_LEAK_INFO = 7,
#define M_FREE_MALLOC_LEAK_INFO M_FREE_MALLOC_LEAK_INFO
  // Query whether the current process is considered to be profileable by the
  // Android platform. Result is assigned to the arg pointer's destination.
  //   arg = bool*
  //   arg_size = sizeof(bool)
  M_GET_PROCESS_PROFILEABLE = 9,
#define M_GET_PROCESS_PROFILEABLE M_GET_PROCESS_PROFILEABLE
  // Maybe enable GWP-ASan. Set *arg to force GWP-ASan to be turned on,
  // otherwise this mallopt() will internally decide whether to sample the
  // process. The program must be single threaded at the point when the
  // android_mallopt function is called.
  //   arg = android_mallopt_gwp_asan_options_t*
  //   arg_size = sizeof(android_mallopt_gwp_asan_options_t)
  M_INITIALIZE_GWP_ASAN = 10,
#define M_INITIALIZE_GWP_ASAN M_INITIALIZE_GWP_ASAN
  // Query whether memtag stack is enabled for this process.
  M_MEMTAG_STACK_IS_ON = 11,
#define M_MEMTAG_STACK_IS_ON M_MEMTAG_STACK_IS_ON
};

typedef struct {
  // The null-terminated name that the zygote is spawning. Because native
  // SpecializeCommon (where the GWP-ASan mallopt() is called from) happens
  // before argv[0] is set, we need the zygote to tell us the new app name.
  const char* program_name = nullptr;

  // An android_mallopt(M_INITIALIZE_GWP_ASAN) is always issued on process
  // startup and app startup, regardless of whether GWP-ASan is desired or not.
  // This allows the process/app's desire to be overwritten by the
  // "libc.debug.gwp_asan.*.app_default" or "libc.debug.gwp_asan.*.<name>"
  // system properties, as well as the "GWP_ASAN_*" environment variables.
  //
  // Worth noting, the "libc.debug.gwp_asan.*.app_default" sysprops *do not*
  // apply to system apps. They use the "libc.debug.gwp_asan.*.system_default"
  // sysprops.
  enum Action {
    // A (non-system) app used gwpAsan=true in its AndroidManifest.xml. This
    // enables GWP-ASan 100% of the time, in enforcing (i.e. crashing) mode.
    TURN_ON_FOR_APP,
    // System processes and system apps have GWP-ASan enabled by default (in
    // enforcing mode), but with process sampling, in enforcing (i.e. crashing)
    // mode.
    TURN_ON_WITH_SAMPLING,
    // Prior to Android U, a non-system app that didn't specify `gwpAsanMode` in
    // its AndroidManifest.xml didn't get GWP-ASan (i.e.
    // DONT_TURN_ON_UNLESS_OVERRIDDEN). Now, an unspecified `gwpAsanMode` in a
    // non-system app means to turn GWP-ASan on, with processing sampling, in
    // the non-crashing "recoverable" mode (i.e.
    // TURN_ON_FOR_APP_SAMPLED_NON_CRASHING). A non-system app can still fully
    // opt-out of GWP-ASan by specifying `gwpAsanMode="never"` in the manifest
    // (i.e. DONT_TURN_ON_UNLESS_OVERRIDDEN).
    DONT_TURN_ON_UNLESS_OVERRIDDEN,
    TURN_ON_FOR_APP_SAMPLED_NON_CRASHING,
    // Note: GWP-ASan cannot be disabled once it's been enabled.
  };

  Action desire = DONT_TURN_ON_UNLESS_OVERRIDDEN;
} android_mallopt_gwp_asan_options_t;

// Manipulates bionic-specific handling of memory allocation APIs such as
// malloc. Only for use by the Android platform and APEXes.
//
// On success, returns true. On failure, returns false and sets errno.
extern "C" bool android_mallopt(int opcode, void* arg, size_t arg_size);
