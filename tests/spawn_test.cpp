/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <spawn.h>

#include <errno.h>
#include <gtest/gtest.h>

// Old versions of glibc didn't have POSIX_SPAWN_SETSID.
#if __GLIBC__
# if !defined(POSIX_SPAWN_SETSID)
#  define POSIX_SPAWN_SETSID 0
# endif
#endif

TEST(spawn, posix_spawnattr_init_posix_spawnattr_destroy) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));
  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setflags_EINVAL) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));
  ASSERT_EQ(EINVAL, posix_spawnattr_setflags(&sa, ~0));
  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setflags_posix_spawnattr_getflags) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  ASSERT_EQ(0, posix_spawnattr_setflags(&sa, POSIX_SPAWN_RESETIDS));
  short flags;
  ASSERT_EQ(0, posix_spawnattr_getflags(&sa, &flags));
  ASSERT_EQ(POSIX_SPAWN_RESETIDS, flags);

  constexpr short all_flags = POSIX_SPAWN_RESETIDS | POSIX_SPAWN_SETPGROUP | POSIX_SPAWN_SETSIGDEF |
                              POSIX_SPAWN_SETSIGMASK | POSIX_SPAWN_SETSCHEDPARAM |
                              POSIX_SPAWN_SETSCHEDULER | POSIX_SPAWN_USEVFORK | POSIX_SPAWN_SETSID;
  ASSERT_EQ(0, posix_spawnattr_setflags(&sa, all_flags));
  ASSERT_EQ(0, posix_spawnattr_getflags(&sa, &flags));
  ASSERT_EQ(all_flags, flags);

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setpgroup_posix_spawnattr_getpgroup) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  ASSERT_EQ(0, posix_spawnattr_setpgroup(&sa, 123));
  pid_t g;
  ASSERT_EQ(0, posix_spawnattr_getpgroup(&sa, &g));
  ASSERT_EQ(123, g);

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setsigmask_posix_spawnattr_getsigmask) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  sigset_t sigs;
  ASSERT_EQ(0, posix_spawnattr_getsigmask(&sa, &sigs));
  ASSERT_FALSE(sigismember(&sigs, SIGALRM));

  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  ASSERT_EQ(0, posix_spawnattr_setsigmask(&sa, &just_SIGALRM));

  ASSERT_EQ(0, posix_spawnattr_getsigmask(&sa, &sigs));
  ASSERT_TRUE(sigismember(&sigs, SIGALRM));

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setsigdefault_posix_spawnattr_getsigdefault) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  sigset_t sigs;
  ASSERT_EQ(0, posix_spawnattr_getsigdefault(&sa, &sigs));
  ASSERT_FALSE(sigismember(&sigs, SIGALRM));

  sigset_t just_SIGALRM;
  sigemptyset(&just_SIGALRM);
  sigaddset(&just_SIGALRM, SIGALRM);
  ASSERT_EQ(0, posix_spawnattr_setsigdefault(&sa, &just_SIGALRM));

  ASSERT_EQ(0, posix_spawnattr_getsigdefault(&sa, &sigs));
  ASSERT_TRUE(sigismember(&sigs, SIGALRM));

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setsschedparam_posix_spawnattr_getsschedparam) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  sched_param sp;
  ASSERT_EQ(0, posix_spawnattr_getschedparam(&sa, &sp));
  ASSERT_EQ(0, sp.sched_priority);

  sched_param sp123 = { .sched_priority = 123 };
  ASSERT_EQ(0, posix_spawnattr_setschedparam(&sa, &sp123));

  ASSERT_EQ(0, posix_spawnattr_getschedparam(&sa, &sp));
  ASSERT_EQ(123, sp.sched_priority);

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}

TEST(spawn, posix_spawnattr_setschedpolicy_posix_spawnattr_getschedpolicy) {
  posix_spawnattr_t sa;
  ASSERT_EQ(0, posix_spawnattr_init(&sa));

  int p;
  ASSERT_EQ(0, posix_spawnattr_getschedpolicy(&sa, &p));
  ASSERT_EQ(0, p);

  ASSERT_EQ(0, posix_spawnattr_setschedpolicy(&sa, 123));

  ASSERT_EQ(0, posix_spawnattr_getschedpolicy(&sa, &p));
  ASSERT_EQ(123, p);

  ASSERT_EQ(0, posix_spawnattr_destroy(&sa));
}
