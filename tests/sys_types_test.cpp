/*
 * Copyright (C) 2014 The Android Open Source Project
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

#include <sys/types.h>

TEST(sys_types, type_sizes) {
  // gids, pids, and uids should be 32-bit on all systems.
  ASSERT_EQ(4U, sizeof(gid_t));
  ASSERT_EQ(4U, sizeof(pid_t));
  ASSERT_EQ(4U, sizeof(uid_t));
  // id_t is the 'generic'.
  ASSERT_EQ(4U, sizeof(id_t));

  // Some types were too small on 32-bit Android by mistake,
  // but are correct on 64-bit Android.
#if defined(__LP64__)
  ASSERT_EQ(8U, sizeof(dev_t));
  ASSERT_EQ(8U, sizeof(off_t));
  ASSERT_EQ(8U, sizeof(time_t));
#else
  ASSERT_EQ(4U, sizeof(dev_t));
  ASSERT_EQ(4U, sizeof(off_t));
  ASSERT_EQ(4U, sizeof(time_t));
#endif
  // These were right even on 32-bit Android.
  ASSERT_EQ(8U, sizeof(loff_t));
  ASSERT_EQ(8U, sizeof(off64_t));
}

TEST(sys_types, makedev)
{
  dev_t dev;

  { SCOPED_TRACE("makedev() loop");
    dev = makedev(7, 0);
    EXPECT_EQ(7, major(dev));
    EXPECT_EQ(0, minor(dev));

    dev = makedev(7, 8);
    EXPECT_EQ(7, major(dev));
    EXPECT_EQ(8, minor(dev));

    dev = makedev(7, 42);
    EXPECT_EQ(7, major(dev));
    EXPECT_EQ(42, minor(dev));
  }

  { SCOPED_TRACE("makedev() ram");
    dev = makedev(1, 0);
    EXPECT_EQ(1, major(dev));
    EXPECT_EQ(0, minor(dev));

    dev = makedev(1, 5);
    EXPECT_EQ(1, major(dev));
    EXPECT_EQ(5, minor(dev));
  }

  { SCOPED_TRACE("makedev() tty");
    dev = makedev(5, 255);
    EXPECT_EQ(5, major(dev));
    EXPECT_EQ(255, minor(dev));

    dev = makedev(5, 4000);
    EXPECT_EQ(5, major(dev));
    EXPECT_EQ(4000, minor(dev));
  }
}
