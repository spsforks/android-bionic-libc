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

#include <sys/utsname.h>


TEST(sys_utsname, uname)
{
  struct utsname unm;

  { SCOPED_TRACE("uname()");
    ASSERT_FALSE( uname(&unm) == -1 );
  }
  { SCOPED_TRACE("uname().sysname");
    ASSERT_TRUE(unm.sysname);
    EXPECT_TRUE(unm.sysname[0]);
  }
  { SCOPED_TRACE("uname.nodename");
    ASSERT_TRUE(unm.nodename);
    EXPECT_TRUE(unm.nodename[0]);
  }
  { SCOPED_TRACE("uname.release");
    ASSERT_TRUE(unm.release);
    EXPECT_TRUE(unm.release[0]);
  }
  { SCOPED_TRACE("uname.version");
    ASSERT_TRUE(unm.version);
    EXPECT_TRUE(unm.version[0]);
  }
  { SCOPED_TRACE("uname.machine");
    ASSERT_TRUE(unm.machine);
    EXPECT_TRUE(unm.machine[0]);
  }
  { SCOPED_TRACE("uname.domainname");
    ASSERT_TRUE(unm.domainname);
    EXPECT_TRUE(unm.domainname[0]);
  }
}
