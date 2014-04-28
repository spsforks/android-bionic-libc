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
#include "TemporaryFile.h"

#include <sys/vfs.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string>

template <typename StatFsT> void Check(StatFsT& sb) {
  EXPECT_EQ(4096, static_cast<int>(sb.f_bsize));
  EXPECT_EQ(0U, sb.f_bfree);
  EXPECT_EQ(0U, sb.f_ffree);
  EXPECT_EQ(0, sb.f_fsid.__val[0]);
  EXPECT_EQ(0, sb.f_fsid.__val[1]);
  EXPECT_EQ(255, static_cast<int>(sb.f_namelen));
}

TEST(sys_vfs, statfs) {
  struct statfs sb;
  ASSERT_EQ(0, statfs("/proc", &sb));
  Check(sb);
}

TEST(sys_vfs, statfs64) {
  struct statfs64 sb;
  ASSERT_EQ(0, statfs64("/proc", &sb));
  Check(sb);
}

TEST(sys_vfs, fstatfs) {
  struct statfs sb;
  int fd = open("/proc", O_RDONLY);
  ASSERT_EQ(0, fstatfs(fd, &sb));
  close(fd);
  Check(sb);
}
TEST(sys_vfs, fstatfs_2)
{
  TemporaryFile tf;
  struct statfs stfs;
  int fd;

  { SCOPED_TRACE("file prepare");

    FILE *tmpfile = fopen(tf.filename, "w");
    fprintf(tmpfile, "%s\n", "Quantum physics is not applicable for cats. That's terrible.");
    fclose(tmpfile);
    fd = open(tf.filename, O_RDWR);
  }
  { SCOPED_TRACE("fstatfs()");

    ASSERT_FALSE( fstatfs(fd, &stfs) < 0 );
    EXPECT_LT(0, stfs.f_type);
    EXPECT_LT(0, stfs.f_bsize);
    EXPECT_LT(0, stfs.f_blocks);
    EXPECT_LT(0, stfs.f_bfree);
    EXPECT_LT(0, stfs.f_bavail);
    EXPECT_LT(0, stfs.f_files);
    EXPECT_LT(0, stfs.f_ffree);
    EXPECT_LT(0, stfs.f_namelen);
    // Check(stfs) is wrong here
  }

  close(fd);
}
TEST(sys_vfs, fstatfs64) {
  struct statfs64 sb;
  int fd = open("/proc", O_RDONLY);
  ASSERT_EQ(0, fstatfs64(fd, &sb));
  close(fd);
  Check(sb);
}
