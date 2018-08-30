/*
 * Copyright (C) 2018 The Android Open Source Project
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
// membarrier(2) is only supported for bionic builds (b/111199492).
#include <linux/membarrier.h>
#include <sys/syscall.h>
#endif

TEST(membarrier, membarrier) {
#if defined(__BIONIC__)
  errno = 0;

  // Query returns bit mask of supported operations on success.
  int supported = syscall(__NR_membarrier, MEMBARRIER_CMD_QUERY, 0);
  if (errno != 0) {
    ASSERT_TRUE(errno == ENOSYS && supported == -1);
    return;
  }

  // Check QUERY command support is reported by QUERY.
  ASSERT_TRUE((supported & (1 << MEMBARRIER_CMD_QUERY)) != 0);

  // Check memory barrier completes successfully.
  if ((supported & (1 << MEMBARRIER_CMD_SHARED)) != 0) {
    ASSERT_EQ(0, syscall(__NR_membarrier, MEMBARRIER_CMD_SHARED, 0));
  }

  // Check using for private expedited barrier without registration fails.
  if ((supported & (1 << MEMBARRIER_CMD_PRIVATE_EXPEDITED)) != 0) {
    ASSERT_EQ(-EPERM, syscall(__NR_membarrier, MEMBARRIER_CMD_PRIVATE_EXPEDITED, 0));
  }

  // Check registration for private expedited barrier commands succeeds.
  if ((supported & (1 << MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED)) != 0) {
    ASSERT_EQ(0, syscall(__NR_membarrier, MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED, 0));
  }

  // Check using for private expedited barrier after registration succeeds.
  if ((supported & (1 << MEMBARRIER_CMD_PRIVATE_EXPEDITED)) != 0) {
    ASSERT_EQ(0, syscall(__NR_membarrier, MEMBARRIER_CMD_PRIVATE_EXPEDITED, 0));
  }
#endif
}
