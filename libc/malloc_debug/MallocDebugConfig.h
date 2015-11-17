/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef MALLOC_DEBUG_CONFIG_H
#define MALLOC_DEBUG_CONFIG_H

#include <stdint.h>

constexpr uint64_t FRONT_GUARD = 0x1;
constexpr uint64_t REAR_GUARD = 0x2;
constexpr uint64_t BACKTRACE = 0x4;
constexpr uint64_t FILL_ON_ALLOC = 0x8;
constexpr uint64_t FILL_ON_FREE = 0x10;
constexpr uint64_t EXPAND_ALLOC = 0x20;
constexpr uint64_t VERIFY_FREE = 0x40;
constexpr uint64_t TRACK_ALLOC = 0x80;
constexpr uint64_t RECORD_ALLOC = 0x100;

// If only these options are set, then no special header is needed.
constexpr uint64_t NO_HEADER_OPTIONS = FILL_ON_ALLOC | FILL_ON_FREE | EXPAND_ALLOC;

struct MallocDebugConfig {
  bool SetFromProperties();

  long front_guard_bytes;
  long rear_guard_bytes;

  bool enable_bt_on_signal = false;
  bool backtrace_enabled = false;
  long backtrace_frames;

  long fill_on_alloc_bytes;

  long fill_on_free_bytes;

  long expand_alloc_bytes;

  bool record_enabled = false;

  long verify_free_allocations;

  uint64_t options = 0;
  uint8_t fill_alloc_value;
  uint8_t fill_free_value;
  uint8_t front_guard_value;
  uint8_t rear_guard_value;

  static const uint64_t FRONT_GUARD = 0x1;
  static const uint64_t REAR_GUARD = 0x2;
  static const uint64_t BACKTRACE = 0x4;
  static const uint64_t FILL_ON_ALLOC = 0x8;
  static const uint64_t FILL_ON_FREE = 0x10;
  static const uint64_t EXPAND_ALLOC = 0x20;
  static const uint64_t VERIFY_FREE = 0x40;
  static const uint64_t TRACK_ALLOC = 0x80;
  static const uint64_t RECORD_ALLOC = 0x100;
  static const uint64_t NO_HEADER_OPTIONS = FILL_ON_ALLOC | FILL_ON_FREE | EXPAND_ALLOC;
};

#endif  // MALLOC_DEBUG_CONFIG_H
