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

#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <unistd.h>

static void SafeClose(int fd) {
  int old_errno = errno;
  TEMP_FAILURE_RETRY(close(fd));
  errno = old_errno;
}

class ScopedFd {
public:
  explicit ScopedFd(const char* path) {
    fd_ = open(path, O_RDONLY);
  }
  explicit ScopedFd(int fd) : fd_(fd) {}
  ~ScopedFd() {
    if (fd_ >= 0) {
      SafeClose(fd_);
      fd_ = -1;
    }
  }
  int Get() { return fd_; }
  int Pass() {
    int result = fd_;
    fd_ = -1;
    return result;
  };
  void Reset(int fd) {
    if (fd != fd_) {
      if (fd_ >= 0)
        SafeClose(fd_);
      fd_ = fd;
    }
  }

private:
  int fd_;
  ScopedFd();
  void operator=(const ScopedFd&);
};

// fexecve takes 'char* const*' instead of 'const char* const*' for the
// 'argv' and 'envp' parameters. Despite the fact that the implementation
// (both in Bionic and GLibc) do not modify their content.
//
// It's also not possible to initialize a 'char* const' array as in:
//
//  char* const argv[] = {
//     "echo", "Hello", NULL
//  }
//
// Because the compiler will complain with:
//
//   error: deprecated conversion from string constant to 'char*'
//       [-Werror=write-strings]
//
// To keep out tests simple, use this macro to force-const-cast
// initialization tables to the right type.
//
#define CPARAM(x)  const_cast<char* const*>(x)

// /proc/self/exe is a symlink to the real program.
static const char kSelfPath[] = "/proc/self/exe";

TEST(fexecve, bad_fd) {
  const char* const argv[] = {
    "echo", "Hello", NULL
  };
  const char* const envp[] = {
    NULL
  };
  EXPECT_EQ(-1, fexecve(-1, CPARAM(argv), CPARAM(envp)));
  EXPECT_EQ(EINVAL, errno);
}

TEST(fexecve, bad_argv) {
  ScopedFd fd(kSelfPath);
  const char* const envp[] = {
    "PATH=/system/bin", NULL
  };
  EXPECT_EQ(-1, fexecve(fd.Get(), NULL, CPARAM(envp)));
  EXPECT_EQ(EINVAL, errno);
}

TEST(fexecve, bad_envp) {
  ScopedFd fd(kSelfPath);
  const char* const argv[] = {
    "echo", "Hello", NULL
  };
  EXPECT_EQ(-1, fexecve(fd.Get(), CPARAM(argv), NULL));
  EXPECT_EQ(EINVAL, errno);
}

TEST(fexecve, ls) {
#ifdef __ANDROID__
  const char kExePath[] = "/system/bin/ls";
#else
  const char kExePath[] = "/bin/ls";
#endif
  const char* const argv[] = {
    "ls", "/", NULL
  };
  const char* const envp[] = {
    "PATH=/system/bin:/bin:/usr/bin", NULL
  };
  errno = 0;
  ScopedFd fd(kExePath);
  EXPECT_NE(-1, fd.Get());
  EXPECT_EQ(0, errno);

  pid_t child = fork();
  if (child == 0) {
    // Prevent the tested program to display anything to out stdout/stderr.
    int null = open("/dev/null", O_WRONLY);
    EXPECT_NE(-1, null);

    dup2(null, 1);
    dup2(null, 2);

    EXPECT_EQ(0, fexecve(fd.Get(), CPARAM(argv), CPARAM(envp)));
    // Shouldn't be there
    exit(1);
  }

  int status;
  wait(&status);
  ASSERT_TRUE(WIFEXITED(status));
  ASSERT_EQ(0, WEXITSTATUS(status));
}
