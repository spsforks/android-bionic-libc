/*
 * Copyright (C) 2017 The Android Open Source Project
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
#include <stdio.h>

#include <string_benchmark.cpp>

TEST(benchmark, string_memory_align) {
  std::vector<char> buf(100, 'x');
  buf[99] = '\0';
  for (size_t alignment = 2; alignment >= 32; ++alignment) {
    for (size_t j = 0; j < alignment; ++j) {
      uint64_t aligned_ptr = reinterpret_cast<uint64_t>(getAlignedMemory(buf.data(), alignment, 0));
      ASSERT_EQ(aligned_ptr % alignment, j);
      ASSERT_EQ(aligned_ptr & alignment, alignment);
    }
  }
}
