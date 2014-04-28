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

#include <sys/resource.h>
#include <errno.h>

static const
int resources[] = { RLIMIT_AS      , RLIMIT_CORE       , RLIMIT_CPU   ,
                    RLIMIT_DATA    , RLIMIT_FSIZE      , RLIMIT_LOCKS ,
                    RLIMIT_MEMLOCK , RLIMIT_MSGQUEUE   , RLIMIT_NICE  ,
                    RLIMIT_NOFILE  , RLIMIT_NPROC      , RLIMIT_RSS   ,
                    RLIMIT_RTPRIO  , RLIMIT_SIGPENDING , RLIMIT_STACK , };

#include <gtest/gtest.h>

TEST(sys_resource, rlimit_struct_size) {
#if defined(__LP64__) || defined(__GLIBC__)
  ASSERT_EQ(sizeof(rlimit), sizeof(rlimit64));
  ASSERT_EQ(8U, sizeof(rlim_t));
#else
  ASSERT_NE(sizeof(rlimit), sizeof(rlimit64));
  ASSERT_EQ(4U, sizeof(rlim_t));
#endif
}

class SysResourceTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32_));
    ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64_));
    ASSERT_EQ(0, prlimit(0, RLIMIT_CORE, nullptr, &pr_l32_));
    ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, nullptr, &pr_l64_));
  }

  void CheckResourceLimits();

 protected:
  rlimit l32_;
  rlimit64 l64_;
  rlimit pr_l32_;
  rlimit64 pr_l64_;
};

void SysResourceTest::CheckResourceLimits() {
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32_));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64_));
  ASSERT_EQ(0, prlimit(0, RLIMIT_CORE, nullptr, &pr_l32_));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, nullptr, &pr_l64_));

  ASSERT_EQ(l32_.rlim_cur, pr_l32_.rlim_cur);
  ASSERT_EQ(l64_.rlim_cur, pr_l64_.rlim_cur);

  if (l64_.rlim_cur == RLIM64_INFINITY) {
    ASSERT_EQ(RLIM_INFINITY, l32_.rlim_cur);
  } else {
    ASSERT_EQ(l64_.rlim_cur, l32_.rlim_cur);
  }

  ASSERT_EQ(l32_.rlim_max, pr_l32_.rlim_max);
  ASSERT_EQ(l64_.rlim_max, pr_l64_.rlim_max);

  if (l64_.rlim_max == RLIM64_INFINITY) {
    ASSERT_EQ(RLIM_INFINITY, l32_.rlim_max);
  } else {
    ASSERT_EQ(l64_.rlim_max, l32_.rlim_max);
  }
}

// Force rlim_max to be bigger than a constant so we can continue following test.
// Change resource limit setting with "ulimit -Hc" in the shell if this test fails.
TEST_F(SysResourceTest, RLIMIT_CORE_rlim_max_not_zero) {
  ASSERT_TRUE(l32_.rlim_max == RLIM_INFINITY || l32_.rlim_max >= 456U) <<
    "RLIMIT_CORE rlim_max = " << l32_.rlim_max;
}

TEST_F(SysResourceTest, get_resource_limit_equal) {
  CheckResourceLimits();
}

TEST_F(SysResourceTest, setrlimit) {
  l32_.rlim_cur = 123U;
  ASSERT_EQ(0, setrlimit(RLIMIT_CORE, &l32_));
  CheckResourceLimits();
  ASSERT_EQ(123U, l32_.rlim_cur);
}

TEST_F(SysResourceTest, setrlimit64) {
  l64_.rlim_cur = 456U;
  ASSERT_EQ(0, setrlimit64(RLIMIT_CORE, &l64_));
  CheckResourceLimits();
  ASSERT_EQ(456U, l64_.rlim_cur);
}

TEST_F(SysResourceTest, prlimit) {
  pr_l32_.rlim_cur = pr_l32_.rlim_max;
  ASSERT_EQ(0, prlimit(0, RLIMIT_CORE, &pr_l32_, nullptr));
  CheckResourceLimits();
  ASSERT_EQ(pr_l32_.rlim_max, pr_l32_.rlim_cur);
}

TEST_F(SysResourceTest, prlimit64) {
  pr_l64_.rlim_cur = pr_l64_.rlim_max;
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, &pr_l64_, nullptr));
  CheckResourceLimits();
  ASSERT_EQ(pr_l64_.rlim_max, pr_l64_.rlim_cur);
}

TEST(sys_resource, priority) {
  int prio_def_process;
  int prio_def_pgrp;
  int prio_def_user;

  { SCOPED_TRACE("getpriority() defaults");

    errno = 0;
    prio_def_process = getpriority (PRIO_PROCESS, 0);
    ASSERT_FALSE(errno);

    prio_def_pgrp = getpriority (PRIO_PGRP, 0);
    ASSERT_FALSE(errno);

    prio_def_user = getpriority (PRIO_USER, 0);
    ASSERT_FALSE(errno);
  }

  { SCOPED_TRACE("setpriority() iterate");

    for (int prio = PRIO_MIN; prio < PRIO_MAX; ++prio)
      {
	errno = 0;
	// Process priority
	ASSERT_FALSE( setpriority (PRIO_PROCESS, 0, prio) != 0 );
	ASSERT_FALSE( getpriority (PRIO_PROCESS, 0) != prio );
	ASSERT_FALSE(errno);
	// Process group priority
	ASSERT_FALSE( setpriority (PRIO_PGRP, 0, prio) != 0 );
	ASSERT_FALSE( getpriority (PRIO_PGRP, 0) != prio );
	ASSERT_FALSE(errno);
	// User priority
	ASSERT_FALSE( setpriority (PRIO_USER, 0, prio) != 0 );
	ASSERT_FALSE( getpriority (PRIO_USER, 0) != prio );
	ASSERT_FALSE(errno);
      }
  }

  { SCOPED_TRACE("setpriority() restore");

    errno = 0;
    ASSERT_FALSE( setpriority (PRIO_PROCESS, 0, prio_def_process) != 0 );
    ASSERT_FALSE(errno);
    ASSERT_FALSE( setpriority (PRIO_PGRP, 0, prio_def_pgrp) != 0 );
    ASSERT_FALSE(errno);
    ASSERT_FALSE( setpriority (PRIO_USER, 0, prio_def_user) != 0 );
    ASSERT_FALSE(errno);
  }
}

TEST(sys_resource, rlimit)
{
  struct rlimit limit;
  int rlim_cur, rlim_max;
  int rlim_test = 10000;

  { SCOPED_TRACE("[get|set]rlimit()");

    for (unsigned int j = 0; j < (sizeof(resources) / sizeof(int)); ++j)
      {
	// Backup
	ASSERT_FALSE( getrlimit(resources[j], &limit) != 0 );
	rlim_cur = limit.rlim_cur;
	rlim_max = limit.rlim_max;

	// Amend
	limit.rlim_cur = rlim_test;
	limit.rlim_max = rlim_test;
	ASSERT_FALSE( setrlimit(resources[j], &limit) != 0 );

	// Check
	ASSERT_FALSE( getrlimit(resources[j], &limit) != 0 );
	EXPECT_EQ(rlim_test, limit.rlim_cur);
	EXPECT_EQ(rlim_test, limit.rlim_max);

	// Restore
	limit.rlim_cur = rlim_cur;
	limit.rlim_max = rlim_max;
	ASSERT_FALSE( setrlimit(resources[j], &limit) != 0 );

	// Check
	ASSERT_FALSE( getrlimit(resources[j], &limit) != 0 );
	EXPECT_EQ(rlim_cur, limit.rlim_cur);
	EXPECT_EQ(rlim_max, limit.rlim_max);
      }
  }
}

TEST(sys_resource, rusage)
{
  struct rusage usage;

  { SCOPED_TRACE("getrusage()");
    ASSERT_FALSE( getrusage(RUSAGE_SELF, &usage) != 0 );
  }
  { SCOPED_TRACE("getrusage() check_result");
    // Checking struct fields
    EXPECT_LE(0, usage.ru_utime.tv_sec);
    EXPECT_LE(0, usage.ru_utime.tv_usec);
    EXPECT_LE(0, usage.ru_stime.tv_sec);
    EXPECT_LE(0, usage.ru_stime.tv_usec);
    EXPECT_LE(0, usage.ru_maxrss);
    EXPECT_LE(0, usage.ru_ixrss);
    EXPECT_LE(0, usage.ru_idrss);
    EXPECT_LE(0, usage.ru_isrss);
    EXPECT_LE(0, usage.ru_minflt);
    EXPECT_LE(0, usage.ru_majflt);
    EXPECT_LE(0, usage.ru_nswap);
    EXPECT_LE(0, usage.ru_inblock);
    EXPECT_LE(0, usage.ru_oublock);
    EXPECT_LE(0, usage.ru_msgsnd);
    EXPECT_LE(0, usage.ru_msgrcv);
    EXPECT_LE(0, usage.ru_nsignals);
    EXPECT_LE(0, usage.ru_nvcsw);
    EXPECT_LE(0, usage.ru_nivcsw);
  }
}
