/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <errno.h>
#include <sched.h>
#include <sys/types.h>
#include <sys/wait.h>

static int child_fn(void* i_ptr) {
  *reinterpret_cast<int*>(i_ptr) = 42;
  return 123;
}

static void clone_test(int* exit_code) {
  void* child_stack[1024];

  int i = 0;
  pid_t tid = clone(child_fn, &child_stack[1024], CLONE_VM, &i);

  int status;
  ASSERT_EQ(tid, TEMP_FAILURE_RETRY(waitpid(tid, &status, __WCLONE)));

  ASSERT_EQ(42, i);

  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(123, WEXITSTATUS(status));
  if (exit_code != NULL) {
    // If any of the above asserts fire, this code will not be executed and
    // exit_code will have the same value as when the function was called.
    *exit_code = 13;
  }
}

TEST(sched, clone) {
#if __BIONIC__
  clone_test(NULL);
#else
  // There is a bug in glibc that causes pthread tests to fail when clone
  // is called in the same process. For now fork off a process to allow the
  // test to complete properly. Making this a DeathTest can work, but loses
  // fidelity about what failed.
  pid_t pid;
  if ((pid = fork()) == 0) {
    int exit_code = 0;
    clone_test(&exit_code);
    exit(exit_code);
  }
  int status;
  ASSERT_EQ(pid, TEMP_FAILURE_RETRY(waitpid(pid, &status, 0)));
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(13, WEXITSTATUS(status));
#endif
}
