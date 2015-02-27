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

#if defined(__BIONIC__)
 // KLOG_* defines are not currently in the host GCC sysroot.
#include <gtest/gtest.h>

#include <sys/klog.h>

//We may have obsolete versions of these defines in the bionic sysroot
#ifndef KLOG_CONSOLE_OFF
#define KLOG_CONSOLE_OFF KLOG_DISABLE
#endif

#ifndef KLOG_CONSOLE_ON
#define KLOG_CONSOLE_ON KLOG_ENABLE
#endif

#ifndef KLOG_CONSOLE_LEVEL
#define KLOG_CONSOLE_LEVEL KLOG_SETLEVEL
#endif

#ifndef KLOG_SIZE_UNREAD
#define KLOG_SIZE_UNREAD KLOG_UNREADSIZE
#endif

#ifndef KLOG_SIZE_BUFFER
#define KLOG_SIZE_BUFFER KLOG_WRITE
#endif

TEST(sys_klog, klogctl)
{
  char testBuffer[128];

  { SCOPED_TRACE("klogctl() close_log");
    ASSERT_FALSE( klogctl(KLOG_CLOSE, NULL, 0) != 0 );
  }
  { SCOPED_TRACE("klogctl() open_log");
    ASSERT_FALSE( klogctl(KLOG_OPEN, NULL, 0) != 0 );
  }
  { SCOPED_TRACE("klogctl() read_log");
    ASSERT_FALSE( klogctl(KLOG_READ, testBuffer, sizeof(testBuffer)) < 0 );
    ASSERT_NE(0, testBuffer[0]);
  }
  { SCOPED_TRACE("klogctl() read_last_log");
    ASSERT_FALSE( klogctl(KLOG_READ_ALL, testBuffer, sizeof(testBuffer)) < 0 );
    ASSERT_NE(0, testBuffer[0]);
  }
  { SCOPED_TRACE("klogctl() read_last_log_and_clear");
    ASSERT_FALSE( klogctl(KLOG_READ_CLEAR, testBuffer, sizeof(testBuffer)) < 0 );
    ASSERT_NE(0, testBuffer[0]);
  }
  { SCOPED_TRACE("klogctl() clear_log");
    ASSERT_FALSE( klogctl(KLOG_CLEAR, NULL, 0) != 0 );
  }
  { SCOPED_TRACE("klogctl() disable_printk");
    ASSERT_FALSE( klogctl(KLOG_DISABLE, NULL, 0) != 0 );
  }
  { SCOPED_TRACE("klogctl() enable_printk");
    ASSERT_FALSE( klogctl(KLOG_ENABLE, NULL, 0) != 0 );
  }
  { SCOPED_TRACE("klogctl() set_level_of_log");
    ASSERT_FALSE( klogctl(KLOG_SETLEVEL, NULL, 1) != 0 );  // KERN_EMERG
    ASSERT_FALSE( klogctl(KLOG_SETLEVEL, NULL, 2) != 0 );
    ASSERT_FALSE( klogctl(KLOG_SETLEVEL, NULL, 3) != 0 );
    ASSERT_FALSE( klogctl(KLOG_SETLEVEL, NULL, 4) != 0 );
    ASSERT_FALSE( klogctl(KLOG_SETLEVEL, NULL, 5) != 0 );  // KERN_WARNING
    ASSERT_FALSE( klogctl(KLOG_SETLEVEL, NULL, 6) != 0 );
    ASSERT_FALSE( klogctl(KLOG_SETLEVEL, NULL, 7) != 0 );
    ASSERT_FALSE( klogctl(KLOG_SETLEVEL, NULL, 8) != 0 );  // KERN_DEBUG
    klogctl(KLOG_SETLEVEL, NULL, 5);
  }
  { SCOPED_TRACE("klogctl() size");
    ASSERT_LE(0, klogctl(KLOG_UNREADSIZE, NULL, 0));
    ASSERT_LE(0, klogctl(KLOG_WRITE, NULL, 0));
  }
}
#endif
