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

#include <dlfcn.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include <string>

#ifdef __LP64__
#define LP_SUFFIX "64"
#else
#define LP_SUFFIX "32"
#endif

TEST(atexit, dlclose) {
  std::string atexit_call_sequence;
  bool valid_this_in_static_dtor = false;
  void* handle = dlopen("libtest_atexit.so", RTLD_NOW);
  ASSERT_TRUE(handle != nullptr);

  void* sym = dlsym(handle, "register_atexit");
  ASSERT_TRUE(sym != nullptr);
  reinterpret_cast<void (*)(std::string*, bool*)>(sym)(&atexit_call_sequence, &valid_this_in_static_dtor);

  ASSERT_EQ(0, dlclose(handle));
  // this test verifies atexit call from atexit handler. as well as the order of calls
  ASSERT_EQ("Humpty Dumpty sat on a wall", atexit_call_sequence);
  ASSERT_TRUE(valid_this_in_static_dtor);
}

TEST(atexit, exit) {
  const std::string test_atexit_expected_output = "";
  const char* binary_file = "nativetest/test_atexit/test_atexit" LP_SUFFIX;

  char binary_path[PATH_MAX];
  const char* android_data = getenv("ANDROID_DATA");
  ASSERT_TRUE(android_data != NULL);

  snprintf(binary_path, sizeof(binary_path), "%s/%s", android_data, binary_file);
  char* argv[] = { binary_path, nullptr };

  int pipefd[2];

  ASSERT_TRUE(pipe(pipefd) == 0);

  pid_t cpid = fork();

  ASSERT_TRUE(cpid != -1);

  if (cpid == 0) { // child
    dup2(pipefd[1], 1); // replace stdout
    // close pipe
    close(pipefd[0]);
    close(pipefd[1]);

    ASSERT_TRUE(execvp(binary_path, argv) != -1);
  } else { // parent
    close(pipefd[1]);
    int in = pipefd[0];
    // read stdout
    char buf[1024];
    ssize_t size = 0, len;
    while ((len=read(in, buf+size, sizeof(buf) - size)) != 0) {
      if (len == -1 && errno == EAGAIN) {
          continue;
      }
      ASSERT_TRUE(len != -1);
      size += len;
    }

    ASSERT_TRUE(static_cast<size_t>(size) < sizeof(buf));
    int status;
    wait(&status);
    ASSERT_EQ(0, status);
    ASSERT_STREQ("Twas brillig, and the slithy toves did gyre and gimble in the wabe;", buf);
  }
}

