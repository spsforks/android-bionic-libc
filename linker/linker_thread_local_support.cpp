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

#include "linker.h"
#include "linker_debug.h"
#include "linker_main.h"

#include <unordered_map>

static std::unordered_map<void*, size_t> g_dso_handle_counters;

void increment_dso_handle_reference_counter(void* dso_handle) {
  auto it = g_dso_handle_counters.find(dso_handle);
  if (it != g_dso_handle_counters.end()) {
    CHECK(++it->second != 0);
  } else {
    soinfo* si = find_containing_library(dso_handle);
    if (si != nullptr) {
      ProtectedDataGuard guard;
      si->set_tls_nodelete();
    } else {
      // Not sure how permissive should we be here...
      PRINT("increment_dso_handle_reference_counter: Couldn't find soinfo by dso_handle=%p",
            dso_handle);
    }
    g_dso_handle_counters[dso_handle] = 1U;
  }
}

void decrement_dso_handle_reference_counter(void* dso_handle) {
  auto it = g_dso_handle_counters.find(dso_handle);
  CHECK(it != g_dso_handle_counters.end());
  CHECK(it->second != 0);

  if (--it->second == 0) {
    soinfo* si = find_containing_library(dso_handle);
    if (si != nullptr) {
      ProtectedDataGuard guard;
      si->unset_tls_nodelete();
    } else {
      PRINT("decrement_dso_handle_reference_counter: Couldn't find soinfo by dso_handle=%p",
            dso_handle);
    }
    g_dso_handle_counters.erase(it);
  }
}
