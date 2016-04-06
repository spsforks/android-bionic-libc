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

#include <sys/quota.h>

#include <errno.h>
#include <gtest/gtest.h>

TEST(sys_quota, quotactl_dqblk) {
  dqblk current;
  errno = 0;
  int rc = quotactl(QCMD(Q_GETQUOTA, USRQUOTA), "/", getuid(), reinterpret_cast<char*>(&current));
  if (rc == -1) {
    ASSERT_EQ(ENOSYS, errno);
  }
}

TEST(sys_quota, quotactl_dqinfo) {
  dqinfo current;
  errno = 0;
  int rc = quotactl(QCMD(Q_GETINFO, USRQUOTA), "/", 0, reinterpret_cast<char*>(&current));
  if (rc == -1) {
    ASSERT_EQ(ENOSYS, errno);
  }
}
