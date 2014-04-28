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

#include <sys/prctl.h>
#include <string.h>

#include <gtest/gtest.h>
#include "CrashCatch.h"

/*
 * This test checks that
 *   prctl() works with PR_X_PDEATHSIG.
 */
TEST(sys_prctl, signals)
{
  CRASH_CATCH();

  int sig_r, sig_w;

  { SCOPED_TRACE("prctl() get_signal");
    ASSERT_FALSE( prctl(PR_GET_PDEATHSIG, (unsigned long) &sig_w, 0, 0, 0) < 0 );
  }
  { SCOPED_TRACE("prctl() set_signal");
    sig_r = sig_w;
    EXPECT_FALSE( prctl(PR_SET_PDEATHSIG, (unsigned long) sig_r, 0, 0, 0) < 0 );
  }
}

/*
 * This test checks that
 *   prctl() works with PR_X_DUMPABLE.
 */
TEST(sys_prctl, dump)
{
  CRASH_CATCH();

  int dumpable;

  { SCOPED_TRACE("prctl() get_dumpable");
    ASSERT_FALSE( (dumpable = prctl(PR_GET_DUMPABLE, 0, 0, 0, 0)) < 0 );
  }
  { SCOPED_TRACE("prctl() set_dumpable");
    EXPECT_FALSE( prctl(PR_SET_DUMPABLE, (unsigned long) dumpable, 0, 0, 0) < 0 );
  }
}

/*
 * This test checks that
 *   prctl() works with PR_X_KEEPCAPS.
 */
TEST(sys_prctl, keepcaps)
{
  CRASH_CATCH();

  int caps_r, caps_w;

  { SCOPED_TRACE("prctl() get_keepcaps");
    ASSERT_FALSE( (caps_w = prctl(PR_GET_KEEPCAPS, 0, 0, 0, 0)) < 0 );
  }
  { SCOPED_TRACE("prctl() change_keepcaps");
    caps_r = 1;
    ASSERT_FALSE( prctl(PR_SET_KEEPCAPS, (unsigned long) caps_r, 0, 0, 0) < 0 );
    EXPECT_EQ(caps_r, prctl(PR_GET_KEEPCAPS, 0, 0, 0, 0));
  }
  { SCOPED_TRACE("prctl() restore_keepcaps");
    ASSERT_FALSE( prctl(PR_SET_KEEPCAPS, (unsigned long) caps_w, 0, 0, 0) < 0 );
    EXPECT_EQ(caps_w, prctl(PR_GET_KEEPCAPS, 0, 0, 0, 0));
  }
}

/*
 * This test checks that
 *   prctl() works with PR_X_TIMING.
 */
TEST(sys_prctl, timing)
{
  CRASH_CATCH();

  int dumpable;

  { SCOPED_TRACE("prctl() get_timing");
    ASSERT_FALSE( (dumpable = prctl(PR_GET_TIMING, 0, 0, 0, 0)) < 0 );
  }
  { SCOPED_TRACE("prctl() set_timing");
    EXPECT_FALSE( prctl(PR_SET_TIMING, PR_TIMING_STATISTICAL, 0, 0, 0) < 0 );
  }
}

/*
 * This test checks that
 *   prctl() works with PR_X_NAME.
 */
TEST(sys_prctl, name)
{
  CRASH_CATCH();

  const char *name_new = "prctl_tests";
  char name_old[256] = "";
  char name_get[256] = "";

  { SCOPED_TRACE("prctl() get_name");
    ASSERT_FALSE( prctl(PR_GET_NAME, (unsigned long) &name_old, 0, 0, 0) < 0 );
  }
  { SCOPED_TRACE("prctl() change_name");
    ASSERT_FALSE( prctl(PR_SET_NAME, (unsigned long) name_new, 0, 0, 0) < 0 );
    ASSERT_FALSE( prctl(PR_GET_NAME, (unsigned long) &name_get, 0, 0, 0) < 0 );
    EXPECT_EQ(0, strncmp(name_new, name_get, 255));
  }
  { SCOPED_TRACE("prctl() restore_name");
    ASSERT_FALSE( prctl(PR_SET_NAME, (unsigned long) name_old, 0, 0, 0) < 0 );
    ASSERT_FALSE( prctl(PR_GET_NAME, (unsigned long) &name_get, 0, 0, 0) < 0 );
    EXPECT_EQ(0, strncmp(name_old, name_get, 255));
  }
}
