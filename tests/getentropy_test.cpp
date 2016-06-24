/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include <fcntl.h>
#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include <sys/cdefs.h>

#include <vector>

#include <android-base/unique_fd.h>
#include <gtest/gtest.h>

#if defined(__BIONIC__)

extern "C" int getentropy(void* buf, size_t len);

static bool is_good(char* buf, size_t len) {
  char x = 0;
  for (size_t i = 0; i < len; ++i) {
    x |= buf[i];
  }
  return x != 0;
}

TEST(getentropy, smoke) {
  char buf[256] = {};
  ASSERT_EQ(0, getentropy(buf, sizeof(buf)));
  ASSERT_TRUE(is_good(buf, sizeof(buf)));
}

TEST(getentropy, fd_exhaustion) {
  std::vector<android::base::unique_fd> fds;
  while (true) {
    int fd = open("/dev/null", O_RDONLY);
    if (fd < 0 && errno != EINTR) {
      ASSERT_EQ(EMFILE, errno);
      break;
    }
    fds.emplace_back(fd);
  }

  char buf[256] = {};
  ASSERT_EQ(0, getentropy(buf, sizeof(buf)));
  ASSERT_TRUE(is_good(buf, sizeof(buf)));
}

#endif // defined(__BIONIC__)
