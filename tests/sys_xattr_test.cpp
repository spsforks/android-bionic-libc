/*
 * Copyright (C) 2015 The Android Open Source Project
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
#include <sys/xattr.h>

#include "TemporaryFile.h"

TEST(sys_xattr, setxattr) {
  TemporaryFile tf;
  char buf[10];
  ASSERT_EQ(0, setxattr(tf.filename, "user.foo", "bar", 4, 0));
  ASSERT_EQ(4, getxattr(tf.filename, "user.foo", buf, sizeof(buf)));
  ASSERT_STREQ("bar", buf);
  buf[0] = '\0';
  ASSERT_EQ(4, lgetxattr(tf.filename, "user.foo", buf, sizeof(buf)));
  ASSERT_STREQ("bar", buf);
}

TEST(sys_xattr, fsetxattr) {
  TemporaryFile tf;
  char buf[10];
  ASSERT_EQ(0, fsetxattr(tf.fd, "user.foo", "bar", 4, 0));
  ASSERT_EQ(4, fgetxattr(tf.fd, "user.foo", buf, sizeof(buf)));
  ASSERT_STREQ("bar", buf);
}

TEST(sys_xattr, fsetxattr_zerobuf) {
  TemporaryFile tf;
  char buf[10];
  ASSERT_EQ(0, fsetxattr(tf.fd, "user.foo", "", 0, 0));
  ASSERT_EQ(0, fgetxattr(tf.fd, "user.foo", buf, sizeof(buf)));
}

TEST(sys_xattr, fsetxattr_with_opath) {
  TemporaryFile tf;
  int fd = open(tf.filename, O_PATH);
  ASSERT_NE(-1, fd);

  int res = fsetxattr(fd, "user.foo", "bar", 4, 0);
#if defined(__BIONIC__)
  char buf[10];
  ASSERT_EQ(0, res);
  ASSERT_EQ(4, fgetxattr(fd, "user.foo", buf, sizeof(buf)));
  ASSERT_STREQ("bar", buf);
#else
  ASSERT_EQ(-1, res);
  ASSERT_EQ(EBADF, errno);
#endif
}
