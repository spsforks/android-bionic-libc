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
    return 0;
  }

  // Check global memory barrier completes successfully.
  if ((supported & MEMBARRIER_CMD_GLOBAL) != 0) {
    ASSERT_EQ(0, syscall(__NR_membarrier, MEMBARRIER_CMD_GLOBAL, 0));
  }

  // Tuples of registration-command and barrier-command.
  int cmds[3][2] = {
    { MEMBARRIER_CMD_REGISTER_GLOBAL_EXPEDITED,
      MEMBARRIER_CMD_GLOBAL_EXPEDITED },
    { MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED,
      MEMBARRIER_CMD_PRIVATE_EXPEDITED },
    { MEMBARRIER_CMD_REGISTER_PRIVATE_EXPEDITED_SYNC_CORE,
      MEMBARRIER_CMD_PRIVATE_EXPEDITED_SYNC_CORE },
  };

  for (size_t i = 0; i < sizeof(cmds) / sizeof(cmds[0]); ++i) {
    int registration = cmds[i][0];
    int barrier = cmds[i][1];

    // Barrier command should only be supported in conjunction with registration.
    ASSERT_EQ((supported & registration) != 0, (supported & barrier) != 0);

    // Skip further tests if barrier not supported.
    if ((supported & barrier) == 0) {
      continue;
    }

    // Check barrier use without prior registration.
    if (cmd == MEMBARRIER_CMD_REGISTER_GLOBAL_EXPEDITED) {
      // Global barrier use is always okay.
      ASSERT_EQ(0, syscall(__NR_membarrier, barrier, 0));
    } else {
      // Private barrier should fail.
      ASSERT_EQ(-1, syscall(__NR_membarrier, barrier, 0));
      ASSERT_EQ(EPERM, errno);
      errno = 0;
    }

    // Check registration for barrier succeeds.
    ASSERT_EQ(0, syscall(__NR_membarrier, registration, 0));

    // Check barrier use after registration.
    ASSERT_EQ(0, syscall(__NR_membarrier, barrier, 0));
  }
#endif
}
