/*
 * Copyright (C) 2023 The Android Open Source Project
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

#include <gtest/gtest.h>

#if defined(__BIONIC__)
#include "gtest_globals.h"
#include "platform/bionic/mte.h"
#include "utils.h"
#endif

TEST(MemtagGlobalsTest, test) {
#if defined(__BIONIC__) && defined(__aarch64__)
  std::string binary = GetTestlibRoot() + "/memtag_globals_binary.so";
  chmod(binary.c_str(), 0755);
  ExecTestHelper eth;
  eth.SetArgs({binary.c_str(), nullptr});
  eth.Run(
      [&]() {
        execve(binary.c_str(), eth.GetArgs(), eth.GetEnv());
        GTEST_FAIL() << "Failed to execve: " << strerror(errno) << "\n";
      },
      // For now, the binary won't exit, even if MTE is enabled, as the linker
      // doesn't have MTE globals support.
      // TODO(mitchp): Change this to be `if mte_supported()`, basically making
      // this an assert that global-overflow is caught by MTE.
      0,
      // TODO(mitchp): Adjust the test file, and make sure we don't hit any
      // assertion errors in the text by adjusting this regex here.
      "Assertions were passed");
#else
  GTEST_SKIP() << "bionic/arm64 only";
#endif
}
