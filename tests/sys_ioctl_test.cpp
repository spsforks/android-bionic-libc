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

#include <sys/ioctl.h>
#include <linux/input.h>
#include <linux/vt.h>
#include <fcntl.h>

#include <gtest/gtest.h>

/*
 * This test checks that
 *   ioctl() works for input device.
 */
TEST(sys_ioctl, ioctl_input_device)
{
  int fd;
  int res;
  struct input_id deviceInfo;

  ASSERT_GE( access("/dev/input/event0", F_OK), 0 );
  ASSERT_NE( (fd = open("/dev/input/event0", O_RDWR)), -1 );
  ASSERT_GE( (res = ioctl(fd, EVIOCGID, &deviceInfo)), 0 );
  close(fd);
}

/*
 * This test checks that
 *   ioctl() works for TTY device.
 */
TEST(sys_ioctl, ioctl_tty_device)
{
  int fd;
  int res;
  int vtnum;

  ASSERT_GE( access("/dev/tty0", F_OK), 0 );
  ASSERT_NE( (fd = open("/dev/tty0", O_RDWR | O_SYNC)), -1 );
  ASSERT_GE( (res = ioctl(fd, VT_OPENQRY, &vtnum)), 0 );
  close(fd);
}
