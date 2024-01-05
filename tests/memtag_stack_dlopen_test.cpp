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

#include <thread>

#include <dlfcn.h>
#include <stdlib.h>

#include <gtest/gtest.h>

#include <android-base/silent_death_test.h>
#include <android-base/test_utils.h>
#include "utils.h"

#if defined(__BIONIC__) && defined(__aarch64__)
__attribute__((target("mte"))) bool is_stack_mte_on() {
  alignas(16) int x = 0;
  void* p = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(&x) + (1UL << 57));
  void* p_cpy = p;
  __builtin_arm_stg(p);
  p = __builtin_arm_ldg(p);
  __builtin_arm_stg(&x);
  return p == p_cpy;
}
#endif

TEST(MemtagStackDlopenTest, DlopenRemapsStack) {
#if defined(__BIONIC__) && defined(__aarch64__)
  if (!running_with_mte()) GTEST_SKIP() << "Test requires MTE.";

  std::string path = android::base::GetExecutableDirectory() + "/libtest_simple_memtag_stack.so";
  ASSERT_EQ(0, access(path.c_str(), F_OK));  // Verify test setup.
  EXPECT_FALSE(is_stack_mte_on());
  void* handle = dlopen(path.c_str(), RTLD_NOW);
  ASSERT_TRUE(handle != nullptr);
  EXPECT_TRUE(is_stack_mte_on());
  bool is_thread_mte_on = false;
  std::thread th([&] { is_thread_mte_on = is_stack_mte_on(); });
  th.join();
  EXPECT_TRUE(is_thread_mte_on);
#else
  GTEST_SKIP() << "requires bionic arm64";
#endif
}
