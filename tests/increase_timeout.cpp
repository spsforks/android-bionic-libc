/*
 * Copyright (C) 2020 The Android Open Source Project
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

#include <stddef.h>

// Bump the test timeouts for MTE-on-QEMU. Required for some of the larger
// string tests.
extern "C" bool GetInitialArgs(const char*** args, size_t* num_args) {
  // 4x the default values.
  static const char* initial_args[] = {"--slow_threshold_ms=8000",
                                       "--deadline_threshold_ms=360000"};
  *args = initial_args;
  *num_args = 2;
  return true;
}
