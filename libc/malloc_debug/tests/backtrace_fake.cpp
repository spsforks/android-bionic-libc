/*
 * Copyright (C) 2015 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdint.h>

#include <vector>

#include <private/bionic_malloc_debug.h>

#include "backtrace.h"
#include "backtrace_fake.h"

constexpr size_t MAX_TRACES = 20;

static size_t g_fake_backtrace_idx = 0;
static std::vector<uintptr_t> g_fake_backtrace[MAX_TRACES];

void backtrace_fake_clear_all() {
  for (size_t i = 0; i < MAX_TRACES; i++) {
    g_fake_backtrace[i].clear();
  }
  g_fake_backtrace_idx = 0;
}

void backtrace_fake_add(size_t idx, uintptr_t fake_ip) {
  g_fake_backtrace[idx].push_back(fake_ip);
}

void backtrace_startup() {
}

void backtrace_shutdown() {
}

size_t backtrace_get(uintptr_t* frames, size_t frame_num) {
  if (frame_num == 0) {
    return 0;
  }

  size_t idx = 0;
  for (const auto& fake_ip : g_fake_backtrace[g_fake_backtrace_idx]) {
    frames[idx++] = fake_ip;
    if (idx == frame_num) {
      break;
    }
  }
  g_fake_backtrace_idx++;
  return idx;
}

void backtrace_log(uintptr_t* frames, size_t frame_count) {
  for (size_t i = 0; i < frame_count; i++) {
    error_log("  #%02zd pc %p", i, reinterpret_cast<void*>(frames[i]));
  }
}
