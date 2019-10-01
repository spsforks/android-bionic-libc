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

#pragma once

#include <dirent.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <inttypes.h>
#include <sys/mman.h>
#include <sys/system_properties.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <atomic>
#include <string>
#include <regex>

#include <android-base/file.h>
#include <android-base/macros.h>
#include <android-base/scopeguard.h>
#include <android-base/stringprintf.h>

#if defined(__LP64__)
#define PATH_TO_SYSTEM_LIB "/system/lib64/"
#else
#define PATH_TO_SYSTEM_LIB "/system/lib/"
#endif

#if defined(__GLIBC__)
#define BIN_DIR "/bin/"
#else
#define BIN_DIR "/system/bin/"
#endif

#if defined(__BIONIC__)
#define KNOWN_FAILURE_ON_BIONIC(x) xfail_ ## x
#else
#define KNOWN_FAILURE_ON_BIONIC(x) x
#endif

// bionic's dlsym doesn't work in static binaries, so we can't access icu,
// so any unicode test case will fail.
static inline bool have_dl() {
  return (dlopen("libc.so", 0) != nullptr);
}

extern "C" void __hwasan_init() __attribute__((weak));

static inline bool running_with_hwasan() {
  return &__hwasan_init != 0;
}

#define SKIP_WITH_HWASAN if (running_with_hwasan()) GTEST_SKIP()

static inline bool is_native_bridge_houdini() {
  char value[PROP_VALUE_MAX];
  return __system_property_get("ro.dalvik.vm.native.bridge", value) &&
        strcmp(value, "libhoudini.so") == 0;
}

static inline bool is_native_bridge_ndk_translation() {
  char value[PROP_VALUE_MAX];
  return __system_property_get("ro.dalvik.vm.native.bridge", value) &&
        strcmp(value, "libndk_translation.so") == 0;
}

static inline void* untag_address(void* addr) {
#if defined(__LP64__)
  if (running_with_hwasan()) {
    constexpr uintptr_t mask = (static_cast<uintptr_t>(1) << 56) - 1;
    addr = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(addr) & mask);
  }
#endif
  return addr;
}

#if defined(__linux__)

#include <sys/sysmacros.h>

struct map_record {
  uintptr_t addr_start;
  uintptr_t addr_end;

  int perms;

  size_t offset;

  dev_t device;
  ino_t inode;

  std::string pathname;
};

class Maps {
 public:
  static bool parse_maps(std::vector<map_record>* maps) {
    maps->clear();

    std::unique_ptr<FILE, decltype(&fclose)> fp(fopen("/proc/self/maps", "re"), fclose);
    if (!fp) return false;

    char line[BUFSIZ];
    while (fgets(line, sizeof(line), fp.get()) != nullptr) {
      map_record record;
      uint32_t dev_major, dev_minor;
      int path_offset;
      char prot[5]; // sizeof("rwxp")
      if (sscanf(line, "%" SCNxPTR "-%" SCNxPTR " %4s %" SCNxPTR " %x:%x %lu %n",
            &record.addr_start, &record.addr_end, prot, &record.offset,
            &dev_major, &dev_minor, &record.inode, &path_offset) == 7) {
        record.perms = 0;
        if (prot[0] == 'r') {
          record.perms |= PROT_READ;
        }
        if (prot[1] == 'w') {
          record.perms |= PROT_WRITE;
        }
        if (prot[2] == 'x') {
          record.perms |= PROT_EXEC;
        }

        // TODO: parse shared/private?

        record.device = makedev(dev_major, dev_minor);
        record.pathname = line + path_offset;
        if (!record.pathname.empty() && record.pathname.back() == '\n') {
          record.pathname.pop_back();
        }
        maps->push_back(record);
      }
    }

    return true;
  }
};

extern "C" pid_t gettid();

#endif

static inline void WaitUntilThreadSleep(std::atomic<pid_t>& tid) {
  while (tid == 0) {
    usleep(1000);
  }
  std::string filename = android::base::StringPrintf("/proc/%d/stat", tid.load());
  std::regex regex {R"(\s+S\s+)"};

  while (true) {
    std::string content;
    ASSERT_TRUE(android::base::ReadFileToString(filename, &content));
    if (std::regex_search(content, regex)) {
      break;
    }
    usleep(1000);
  }
}

static inline void AssertChildExited(int pid, int expected_exit_status,
                                     const std::string* error_msg = nullptr) {
  int status;
  std::string error;
  if (error_msg == nullptr) {
    error_msg = &error;
  }
  ASSERT_EQ(pid, TEMP_FAILURE_RETRY(waitpid(pid, &status, 0))) << *error_msg;
  if (expected_exit_status >= 0) {
    ASSERT_TRUE(WIFEXITED(status)) << *error_msg;
    ASSERT_EQ(expected_exit_status, WEXITSTATUS(status)) << *error_msg;
  } else {
    ASSERT_TRUE(WIFSIGNALED(status)) << *error_msg;
    ASSERT_EQ(-expected_exit_status, WTERMSIG(status)) << *error_msg;
  }
}

static inline void AssertCloseOnExec(int fd, bool close_on_exec) {
  int flags = fcntl(fd, F_GETFD);
  ASSERT_NE(flags, -1);
  ASSERT_EQ(close_on_exec ? FD_CLOEXEC : 0, flags & FD_CLOEXEC);
}

// The absolute path to the executable
const std::string& get_executable_path();

// Access to argc/argv/envp
int get_argc();
char** get_argv();
char** get_envp();

// ExecTestHelper is only used in bionic and glibc tests.
#ifndef __APPLE__
class ExecTestHelper {
 public:
  char** GetArgs() {
    return const_cast<char**>(args_.data());
  }
  const char* GetArg0() {
    return args_[0];
  }
  char** GetEnv() {
    return const_cast<char**>(env_.data());
  }
  const std::string& GetOutput() {
    return output_;
  }

  void SetArgs(const std::vector<const char*>& args) {
    args_ = args;
  }
  void SetEnv(const std::vector<const char*>& env) {
    env_ = env;
  }

  void Run(const std::function<void()>& child_fn, int expected_exit_status,
           const char* expected_output) {
    int fds[2];
    ASSERT_NE(pipe(fds), -1);

    pid_t pid = fork();
    ASSERT_NE(pid, -1);

    if (pid == 0) {
      // Child.
      close(fds[0]);
      dup2(fds[1], STDOUT_FILENO);
      dup2(fds[1], STDERR_FILENO);
      if (fds[1] != STDOUT_FILENO && fds[1] != STDERR_FILENO) close(fds[1]);
      child_fn();
      FAIL();
    }

    // Parent.
    close(fds[1]);
    output_.clear();
    char buf[BUFSIZ];
    ssize_t bytes_read;
    while ((bytes_read = TEMP_FAILURE_RETRY(read(fds[0], buf, sizeof(buf)))) > 0) {
      output_.append(buf, bytes_read);
    }
    close(fds[0]);

    std::string error_msg("Test output:\n" + output_);
    AssertChildExited(pid, expected_exit_status, &error_msg);
    if (expected_output != nullptr) {
      ASSERT_EQ(expected_output, output_);
    }
  }

 private:
  std::vector<const char*> args_;
  std::vector<const char*> env_;
  std::string output_;
};
#endif

class FdLeakChecker {
 public:
  FdLeakChecker() {
  }

  ~FdLeakChecker() {
    size_t end_count = CountOpenFds();
    EXPECT_EQ(start_count_, end_count);
  }

 private:
  static size_t CountOpenFds() {
    auto fd_dir = std::unique_ptr<DIR, decltype(&closedir)>{ opendir("/proc/self/fd"), closedir };
    size_t count = 0;
    dirent* de = nullptr;
    while ((de = readdir(fd_dir.get())) != nullptr) {
      if (de->d_type == DT_LNK) {
        ++count;
      }
    }
    return count;
  }

  size_t start_count_ = CountOpenFds();
};
