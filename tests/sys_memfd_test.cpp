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

#include <sys/cdefs.h>

#if defined(__BIONIC__)

#include <gtest/gtest.h>

#include <errno.h>
#include <fcntl.h>
#include <sys/memfd.h>

TEST(sys_memfd, memfd_create) {
  int fd = memfd_create("foo", MFD_CLOEXEC);
  if (fd == -1) {
    ASSERT_EQ(ENOSYS, errno);
    printf("memfd_create not supported\n");
    return;
  }

  char buf[PATH_MAX];
  snprintf(buf, sizeof(buf), "/proc/self/fd/%d", fd);
  ASSERT_NE(-1, readlink(buf, buf, sizeof(buf)));
  ASSERT_STREQ("memfd:foo", buf);
}
#endif
