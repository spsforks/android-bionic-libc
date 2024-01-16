/*
 * Copyright (C) 2023 The Android Open Source Project
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

#include <stddef.h>
#include <sys/cdefs.h>

__BEGIN_DECLS

namespace gwp_asan {
struct AllocatorState;
struct AllocationMetadata;
};  // namespace gwp_asan

// GWP-ASan calbacks to support the recoverable mode. Separate from the
// debuggerd_callbacks_t because these values aren't available at debuggerd_init
// time, and have to be synthesized on request.
typedef struct {
  bool (*debuggerd_needs_gwp_asan_recovery)(void* fault_addr);
  void (*debuggerd_gwp_asan_pre_crash_report)(void* fault_addr);
  void (*debuggerd_gwp_asan_post_crash_report)(void* fault_addr);
} gwp_asan_callbacks_t;

struct __attribute__((packed)) debugger_process_info {
  void* abort_msg;
  void* fdsan_table;
  const gwp_asan::AllocatorState* gwp_asan_state;
  const gwp_asan::AllocationMetadata* gwp_asan_metadata;
  const char* scudo_stack_depot;
  const char* scudo_region_info;
  const char* scudo_ring_buffer;
  size_t scudo_ring_buffer_size;
  size_t scudo_stack_depot_size;
  bool recoverable_gwp_asan_crash;
};

// These callbacks are called in a signal handler, and thus must be async signal safe.
// If null, the callbacks will not be called.
typedef struct {
  debugger_process_info (*get_process_info)();
  gwp_asan_callbacks_t (*get_gwp_asan_callbacks)();
  void (*post_dump)();
} debuggerd_callbacks_t;

debuggerd_callbacks_t* debuggerd_get_callbacks();

__END_DECLS
