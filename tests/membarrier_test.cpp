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
  // Query returns bit mask of supported operations on success.
  int supported = syscall(__NR_membarrier, MEMBARRIER_CMD_QUERY, 0);
  ASSERT_TRUE((errno == ENOSYS && supported == -1) || (errno == 0 && supported != 0));
#endif
}
