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

/*
 * Sometimes test can cause segmentation fault.
 * In risky tests just add CRASH_CATCH(); on top of scope.
 * This helps print out fail trace before the death of process.
 */

#ifndef CRASH_CATCH_H
#define CRASH_CATCH_H

#include <gtest/gtest.h>
#include <signal.h>

static struct sigaction old_sa[NSIG];

static void fail_sigaction(int signal __attribute__((unused)),
			   siginfo_t *info __attribute__((unused)),
			   void *reserved __attribute__((unused)))
{
  FAIL();
  exit(0);
}

static void try_to_catch_crashes()
{
  struct sigaction handler;
  memset(&handler, 0, sizeof(struct sigaction));
  handler.sa_sigaction = fail_sigaction;
  handler.sa_flags = SA_RESETHAND;

#define CATCH_SIG(X) sigaction(X, &handler, &old_sa[X])
  {
    CATCH_SIG(SIGSEGV);
  }
#undef CATCH_SIG
}

static void reset_death_signals()
{
#define UNCATCH_SIG(X) sigaction(X, &old_sa[X], &old_sa[X])
  {
    UNCATCH_SIG(SIGSEGV);
  }
#undef UNCATCH_SIG
}

inline static void quick_death()
{
  char *a = 0;
  a[0] = 0;
}

/*
 * Crash catcher on RAII pattern
 */

#define CRASH_CATCH() volatile RaiiCrashCatcher _crash_catcher

struct RaiiCrashCatcher
{
  RaiiCrashCatcher()
  {
    try_to_catch_crashes();
  }
  ~RaiiCrashCatcher()
  {
    reset_death_signals();
  }
};

#endif //CRASH_CATCH_H
