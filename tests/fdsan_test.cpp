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

#include "BionicDeathTest.h"

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>

#if defined(__BIONIC__)
extern "C" int __libc_close_with_tag(int fd, void* tag);
extern "C" void __libc_exchange_close_tag(int fd, void* old_tag, void* new_tag);

TEST(fdsan, unowned_untagged_close) {
  int fd = open("/dev/null", O_RDONLY);
  ASSERT_EQ(0, close(fd));
}

TEST(fdsan, unowned_tagged_close) {
  int fd = open("/dev/null", O_RDONLY);
  ASSERT_EQ(0, __libc_close_with_tag(fd, nullptr));
}

TEST(fdsan, unowned_improperly_tagged_close) {
  int fd = open("/dev/null", O_RDONLY);
  EXPECT_DEATH(__libc_close_with_tag(fd, reinterpret_cast<void*>(0xdeadbeef)), "incorrect tag");
}

TEST(fdsan, unowned_incorrect_exchange) {
  int fd = open("/dev/null", O_RDONLY);
  EXPECT_DEATH(__libc_exchange_close_tag(fd, reinterpret_cast<void*>(0xbadc0de),
                                         reinterpret_cast<void*>(0xdeadbeef)),
               "failed to exchange");
}

TEST(fdsan, owned_untagged_close) {
  int fd = open("/dev/null", O_RDONLY);
  __libc_exchange_close_tag(fd, nullptr, reinterpret_cast<void*>(0xdeadbeef));
  EXPECT_DEATH(close(fd), "incorrect tag");
}

TEST(fdsan, owned_tagged_close) {
  int fd = open("/dev/null", O_RDONLY);
  __libc_exchange_close_tag(fd, nullptr, reinterpret_cast<void*>(0xdeadbeef));
  ASSERT_EQ(0, __libc_close_with_tag(fd, reinterpret_cast<void*>(0xdeadbeef)));
}

TEST(fdsan, owned_improperly_tagged_close) {
  int fd = open("/dev/null", O_RDONLY);
  __libc_exchange_close_tag(fd, nullptr, reinterpret_cast<void*>(0xdeadbeef));
  EXPECT_DEATH(__libc_close_with_tag(fd, reinterpret_cast<void*>(0xdeadc0de)), "incorrect tag");
}

TEST(fdsan, owned_incorrect_exchange) {
  int fd = open("/dev/null", O_RDONLY);
  __libc_exchange_close_tag(fd, nullptr, reinterpret_cast<void*>(0xdeadbeef));
  EXPECT_DEATH(__libc_exchange_close_tag(fd, reinterpret_cast<void*>(0xbadc0de),
                                         reinterpret_cast<void*>(0xdeadbeef)),
               "failed to exchange");
}
#endif
