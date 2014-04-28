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

#include <sys/resource.h>
#include <errno.h>

static const
int resources[] = { RLIMIT_AS      , RLIMIT_CORE       , RLIMIT_CPU   ,
                    RLIMIT_DATA    , RLIMIT_FSIZE      , RLIMIT_LOCKS ,
                    RLIMIT_MEMLOCK , RLIMIT_MSGQUEUE   , RLIMIT_NICE  ,
                    RLIMIT_NOFILE  , RLIMIT_NPROC      , RLIMIT_RSS   ,
                    RLIMIT_RTPRIO  , RLIMIT_SIGPENDING , RLIMIT_STACK , };

#if defined(__GLIBC__)
/* The host glibc we're currently building with doesn't have prlimit64 yet. */
static int prlimit64(pid_t, int resource, const struct rlimit64* new_limit, struct rlimit64* old_limit) {
  if (new_limit != NULL) {
    return setrlimit64(resource, new_limit);
  } else {
    return getrlimit64(resource, old_limit);
  }
}
#endif

TEST(sys_resource, smoke) {
#if defined(__LP64__) || defined(__GLIBC__)
  ASSERT_EQ(sizeof(rlimit), sizeof(rlimit64));
  ASSERT_EQ(8U, sizeof(rlim_t));
#else
  ASSERT_NE(sizeof(rlimit), sizeof(rlimit64));
  ASSERT_EQ(4U, sizeof(rlim_t));
#endif

  // Read with getrlimit, getrlimit64, and prlimit64.
  // (prlimit is prlimit64 on LP64 and unimplemented on 32-bit.)
  rlimit l32;
  rlimit64 l64;
  rlimit64 pr_l64;
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, NULL, &pr_l64));
  ASSERT_EQ(l64.rlim_cur, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, pr_l64.rlim_cur);
  ASSERT_EQ(l64.rlim_max, pr_l64.rlim_max);
  if (l64.rlim_max == RLIM64_INFINITY) {
    ASSERT_EQ(RLIM_INFINITY, l32.rlim_max);
  } else {
    ASSERT_EQ(l64.rlim_max, l32.rlim_max);
  }

  // Write with setrlimit and read back with everything.
  l32.rlim_cur = 123;
  ASSERT_EQ(0, setrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, NULL, &pr_l64));
  ASSERT_EQ(123U, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, pr_l64.rlim_cur);

  // Write with setrlimit64 and read back with everything.
  l64.rlim_cur = 456;
  ASSERT_EQ(0, setrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, NULL, &pr_l64));
  ASSERT_EQ(456U, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, pr_l64.rlim_cur);

  // Write with prlimit64 and read back with everything.
  l64.rlim_cur = 789;
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, &l64, NULL));
  ASSERT_EQ(0, getrlimit(RLIMIT_CORE, &l32));
  ASSERT_EQ(0, getrlimit64(RLIMIT_CORE, &l64));
  ASSERT_EQ(0, prlimit64(0, RLIMIT_CORE, NULL, &pr_l64));
  ASSERT_EQ(789U, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, l32.rlim_cur);
  ASSERT_EQ(l64.rlim_cur, pr_l64.rlim_cur);
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
