/*
 * Copyright (C) 2022 The Android Open Source Project
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

#if !defined(__GLIBC__)

#include <fcntl.h>
// Old version of glibc lacks this newer kernel uapi header file
#include <linux/close_range.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#endif  // __GLIBC__

TEST(unistd, close_range) {
#if defined(__GLIBC__)
  GTEST_SKIP() << "glibc too old";
#else   // __GLIBC__
  // Create a test IPv6/UDP socket file descriptor
  int fd = socket(AF_INET6, SOCK_DGRAM, 0);
  ASSERT_GE(fd, 0);

  // Get the fd flags, this includes FD_CLOEXEC bit, it should be clear
  // due to lack of '| SOCK_CLOEXEC' up above.
  ASSERT_EQ(fcntl(fd, F_GETFD), 0);

  // Try to set CLOEXEC flag on the file descriptor
  // Note: this requires a 5.11+ kernel (but backported to 5.10-T)
  if (0 == close_range(fd, fd, CLOSE_RANGE_CLOEXEC)) {
    // on success we check the flag is now set
    ASSERT_EQ(fcntl(fd, F_GETFD), FD_CLOEXEC);
  }

  // Try to close the file descriptor
  // Note: this requires a 5.9+ kernel
  if (0 == close_range(fd, fd, 0)) {
    // we can't close it *again*
    ASSERT_EQ(close(fd), -1);
    ASSERT_EQ(errno, EBADF);
  } else {
    ASSERT_EQ(errno, ENOSYS);
    // since close_range() failed, we can close it normally
    ASSERT_EQ(close(fd), 0);
  }
#endif  // __GLIBC__
}
