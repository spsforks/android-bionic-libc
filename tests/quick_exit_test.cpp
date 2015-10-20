/*
 * Copyright (C) 2015 The Android Open Source Project
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

#include <stdio.h>
#include <stdlib.h>

TEST(quick_exit, smoke) {
  pid_t pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    quick_exit(99);
  }

  int status;
  ASSERT_EQ(pid, waitpid(pid, &status, 0));
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(99, WEXITSTATUS(status));
}

static int quick_exit_status = 0;

static void quick_exit_1(void) {
  ASSERT_EQ(quick_exit_status, 0);
  quick_exit_status = 1;
}

static void quick_exit_2(void) {
  ASSERT_EQ(quick_exit_status, 1);
}

static void not_run(void) {
  FAIL();
}

TEST(quick_exit, at_quick_exit_smoke) {
  pid_t pid = fork();
  ASSERT_NE(-1, pid) << strerror(errno);

  if (pid == 0) {
    ASSERT_EQ(at_quick_exit(quick_exit_2), 0);
    ASSERT_EQ(at_quick_exit(quick_exit_1), 0);
    atexit(not_run);
    quick_exit(99);
  }

  int status;
  ASSERT_EQ(pid, waitpid(pid, &status, 0));
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(99, WEXITSTATUS(status));
}

static void quick_exit_func5() {
  fprintf(stderr, "5");
}

static void quick_exit_func4() {
  fprintf(stderr, "4");
}

static void quick_exit_func3() {
  fprintf(stderr, "3");
  at_quick_exit(quick_exit_func4);
}

static void quick_exit_func2() {
  fprintf(stderr, "2");
}

static void quick_exit_func1() {
  fprintf(stderr, "1");
}

static void quick_exit_main() {
  // This should result in "12345" output to stderr
  at_quick_exit(quick_exit_func5);
  at_quick_exit(quick_exit_func3);
  at_quick_exit(quick_exit_func2);
  at_quick_exit(quick_exit_func1);
  quick_exit(0);
}

TEST(quick_exit, at_quick_exit) {
  ASSERT_EXIT(quick_exit_main(), testing::ExitedWithCode(0), "12345");
}
