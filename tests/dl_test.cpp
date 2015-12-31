/*
 * Copyright (C) 2012 The Android Open Source Project
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
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <string>

#include "gtest_globals.h"
#include "utils.h"


extern "C" int main_global_default_serial() {
  return 3370318;
}

extern "C" int main_global_protected_serial() {
  return 2716057;
}

// The following functions are defined in DT_NEEDED
// libdl_preempt_test.so library.

// This one calls main_global_default_serial
extern "C" int main_global_default_get_serial();

// This one calls main_global_protected_serial
extern "C" int main_global_protected_get_serial();

// This one calls lib_global_default_serial
extern "C" int lib_global_default_get_serial();

// This one calls lib_global_protected_serial
extern "C" int lib_global_protected_get_serial();

// This test verifies that the global default function
// main_global_default_serial() is preempted by
// the function defined above.
TEST(dl, main_preempts_global_default) {
  ASSERT_EQ(3370318, main_global_default_get_serial());
}

// This one makes sure that the global protected
// symbols do not get preempted
TEST(dl, main_does_not_preempt_global_protected) {
  ASSERT_EQ(3370318, main_global_protected_get_serial());
}

// check same things for lib
TEST(dl, lib_preempts_global_default) {
  ASSERT_EQ(3370318, lib_global_default_get_serial());
}

TEST(dl, lib_does_not_preempt_global_protected) {
  ASSERT_EQ(3370318, lib_global_protected_get_serial());
}

// Note: updating LD_LIBRARY_PATH at runtime has no effect, as the dynamic
// linker caches its value when the program is loaded. Therefore we need to
// fork and exec another program to try different paths.
static void test_ld_library_path_exec_program(const std::string& program,
                                              const std::string& lib_dir,
                                              const std::string& expected_error = "") {
  constexpr size_t kMaxErrorLength = 500;

  // Use a pipe to capture the dynamic linker output.
  int pipefd[2];
  ASSERT_EQ(pipe(pipefd), 0) << strerror(errno);

  pid_t child_pid = fork();
  ASSERT_NE(child_pid, -1) << strerror(errno);
  if (child_pid == 0) {
    // Child process
    close(pipefd[0]);  // Close read end.
    if (dup2(pipefd[1], STDERR_FILENO) != STDERR_FILENO ||
        dup2(pipefd[1], STDOUT_FILENO) != STDOUT_FILENO) {
      perror("dup2 failed");
      exit(1);
    }

    std::string ld_library_path = std::string("LD_LIBRARY_PATH=") + lib_dir;
    // Note that the const_cast's are safe because POSIX specifies that the
    // exec'd program cannot modify the contents of either array.
    char* args[] = { const_cast<char*>(program.c_str()), nullptr };
    char* env[] = { const_cast<char*>(ld_library_path.c_str()), nullptr };
    execve(program.c_str(), args, env);

    // execve() only returns if it fails.
    perror("execve failed");
    exit(1);
  } else {
    // Main process
    close(pipefd[1]);  // Close write end.

    int status;
    ASSERT_EQ(waitpid(child_pid, &status, 0), child_pid) << strerror(errno);

    static char output[kMaxErrorLength];
    ssize_t size_read = 0;
    ssize_t tmp;
    while ((tmp = read(pipefd[0], output + size_read, kMaxErrorLength - size_read)) > 0) {
      size_read += tmp;
    }
    EXPECT_EQ(tmp, 0);  // Ensure that no error occurred (negative return from read()).
    output[size_read] = '\0';
    close(pipefd[0]);

    std::string failure_info = std::string(output) + " (LD_LIBRARY_PATH = " + lib_dir + ")";
    if (expected_error.empty()) {
      ASSERT_TRUE(WIFEXITED(status)) << failure_info;
      ASSERT_EQ(WEXITSTATUS(status), 0) << failure_info;
      ASSERT_EQ(size_read, 0) << failure_info;
    } else {
      // An error should be signaled (either the process aborts, or returns a non-0 status).
      ASSERT_FALSE(WIFEXITED(status) && WEXITSTATUS(status) == 0) << failure_info;
      ASSERT_SUBSTR(expected_error, output) << "LD_LIBRARY_PATH = " << lib_dir;
    }
  }
}

TEST(dl, ld_library_path) {
  const std::string test_name = "bionic_internal_test_ld_library_path";
  const std::string lib_name = "libdl_test_ld_library_path";
#ifdef __LP64__
  const bool is_64 = true;
#else
  const bool is_64 = false;
#endif
  const std::string this_arch_bitness  = (is_64 ? "64" : "32");
  const std::string other_arch_bitness = (is_64 ? "32" : "64");

  const std::string& data_root = get_data_root();
  const std::string this_arch_nativetest_dir = data_root + "/nativetest" + (is_64 ? "64" : "");
  const std::string other_arch_nativetest_dir = data_root + "/nativetest" + (is_64 ? "" : "64");

  const std::string program_path = this_arch_nativetest_dir + "/" + test_name + "/" + test_name + this_arch_bitness;

  const std::string lib_dir_this_arch = this_arch_nativetest_dir + "/bionic-loader-test-libs/" + lib_name;
  const std::string lib_dir_other_arch = other_arch_nativetest_dir + "/bionic-loader-test-libs/" + lib_name;

  // first: library path, second: a substring in the expected error (success expected if empty).
  std::vector<std::pair<std::string, std::string>> lib_dir_list = {
    { "",
#ifdef __BIONIC__
      "not found"
#else
      "cannot open shared object file: No such file or directory"
#endif
    },
    { lib_dir_other_arch,
#ifdef __BIONIC__
      other_arch_bitness + "-bit instead of " + this_arch_bitness + "-bit"
#else
      "wrong ELF class: ELFCLASS" + other_arch_bitness
#endif
    },
    { lib_dir_this_arch, "" },
    { lib_dir_this_arch + ":" + lib_dir_other_arch, "" },
    { lib_dir_other_arch + ":" + lib_dir_this_arch, "" }
  };

  for (const auto& lib_dir_error : lib_dir_list) {
    test_ld_library_path_exec_program(program_path, lib_dir_error.first, lib_dir_error.second);
  }
}

// TODO: Add tests for LD_PRELOAD
