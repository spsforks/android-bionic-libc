/*
 * Copyright (C) 2021 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <gtest/gtest.h>
#include <signal.h>
#include <stdio.h>
#include <sys/file.h>
#include <string>

#if defined(__BIONIC__)

#include "android-base/file.h"
#include "android-base/silent_death_test.h"
#include "android-base/test_utils.h"
#include "gwp_asan/options.h"
#include "platform/bionic/malloc.h"
#include "sys/system_properties.h"
#include "utils.h"

using gwp_asan_integration_DeathTest = SilentDeathTest;

// basename is a mess, use gnu basename explicitly to avoid the need for string
// mutation.
extern "C" const char* __gnu_basename(const char* path);

// GWP-ASan tests can run much slower, especially when combined with HWASan.
// Triple the deadline to avoid flakes (b/238585984).
extern "C" bool GetInitialArgs(const char*** args, size_t* num_args) {
  static const char* initial_args[] = {"--deadline_threshold_ms=270000"};
  *args = initial_args;
  *num_args = 1;
  return true;
}

// This file implements "torture testing" of the entire bionic-unit-tests under
// GWP-ASan, where we sample every single allocation. This is a good check that
// everything still works properly under GWP-ASan.
TEST(gwp_asan_integration, malloc_tests_under_torture) {
  // Do not override HWASan with GWP ASan.
  SKIP_WITH_HWASAN;

  RunGwpAsanTest("malloc.*:-malloc.mallinfo*");
}

// System properties are global for a device, so the tests that mutate the
// GWP-ASan system properties must be run mutually exclusive. Because
// bionic-unit-tests is run in an isolated gtest fashion (each test is run in
// its own process), we have to use flocks to synchronise between tests.
class GwpAsanExclusiveTestLock {
 private:
  int flock_fd_;

 public:
  GwpAsanExclusiveTestLock() {
    std::string path = testing::internal::GetArgvs()[0];
    flock_fd_ = open(path.c_str(), O_RDONLY);
    EXPECT_NE(flock_fd_, -1) << "failed to open self for a flock";
    EXPECT_NE(flock(flock_fd_, LOCK_EX), -1) << "failed to flock myself";
  }

  ~GwpAsanExclusiveTestLock() { close(flock_fd_); }
};

class SyspropRestorer {
 private:
  GwpAsanExclusiveTestLock exclusive_test_lock_;
  std::vector<std::string> all_sysprops_;

  static SyspropRestorer* exclusive_sysprop_restorer;
  static const char* kRunningGwpAsanTestsPropName;

  static void HandleSignalTermination(int sig, siginfo_t* /* info */, void* /* ucontext*/) {
    exclusive_sysprop_restorer->~SyspropRestorer();
    signal(sig, SIG_DFL);
    raise(sig);
  }

  static const std::vector<std::string>& AllGwpAsanSysprops() {
    static std::vector<std::string> all_sysprops;
    static bool is_initialized = false;
    if (is_initialized) return all_sysprops;

    std::string path = testing::internal::GetArgvs()[0];
    const char* basename = __gnu_basename(path.c_str());
    all_sysprops.push_back(std::string("libc.debug.gwp_asan.sample_rate.") + basename);
    all_sysprops.push_back(std::string("libc.debug.gwp_asan.process_sampling.") + basename);
    all_sysprops.push_back(std::string("libc.debug.gwp_asan.max_allocs.") + basename);
    all_sysprops.push_back("libc.debug.gwp_asan.sample_rate.system_default");
    all_sysprops.push_back("libc.debug.gwp_asan.sample_rate.app_default");
    all_sysprops.push_back("libc.debug.gwp_asan.process_sampling.system_default");
    all_sysprops.push_back("libc.debug.gwp_asan.process_sampling.app_default");
    all_sysprops.push_back("libc.debug.gwp_asan.max_allocs.system_default");
    all_sysprops.push_back("libc.debug.gwp_asan.max_allocs.app_default");

    // Be careful about iterator invalidation for `all_sysprops`!
    size_t base_props_size = all_sysprops.size();
    for (size_t i = 0; i < base_props_size; ++i) {
      all_sysprops.push_back("persist." + all_sysprops[i]);
    }
    is_initialized = true;
    return all_sysprops;
  }

  static void RestoreBackupSysprops() {
    for (const std::string& prop_name : AllGwpAsanSysprops()) {
      std::string backup_prop = prop_name + ".bkup";
      std::string live_value = SyspropRestorer::GetSysprop(prop_name);
      std::string backup_value = SyspropRestorer::GetSysprop(backup_prop);
      // Avoid creating empty-string properties if it's unnecessary, e.g. if
      // the backup and live values are both non-existent.
      if (live_value == backup_value) {
        continue;
      }
      __system_property_set(prop_name.c_str(), backup_value.c_str());
      __system_property_set(backup_prop.c_str(), "");
    }
  }

  // If you are halfway through a GWP-ASan test, and the test gets SIGKILLED,
  // the system properties used to enable GWP-ASan can be left laying around
  // on your system, which can affect every binary. Hopefully, the person
  // who's running bionic-unit-tests will re-run it, and so the test below
  // ensures that the device is brought back to a normal state.
  static void CheckAndRestoreDeviceIfPoisoned() {
    if (SyspropRestorer::GetSysprop(kRunningGwpAsanTestsPropName).empty()) return;
    fprintf(stderr,
            "Detected that GWP-ASan tests were previously improperly shut down. Performing "
            "cleanup and then resuming the tests as normal.\n");
    RestoreBackupSysprops();
    __system_property_set(kRunningGwpAsanTestsPropName, "");
  }

 public:
  SyspropRestorer() {
    CheckAndRestoreDeviceIfPoisoned();

    EXPECT_EQ(nullptr, exclusive_sysprop_restorer);
    exclusive_sysprop_restorer = this;

    struct sigaction act {};
    act.sa_sigaction = HandleSignalTermination;
    act.sa_flags = SA_SIGINFO;
    EXPECT_EQ(0, sigaction(SIGTERM, &act, nullptr)) << "failed to install signal handler";
    EXPECT_EQ(0, sigaction(SIGINT, &act, nullptr)) << "failed to install signal handler";

    __system_property_set(kRunningGwpAsanTestsPropName, "true");

    for (const std::string& prop : AllGwpAsanSysprops()) {
      std::string value = GetSysprop(prop);
      if (value.empty()) continue;

      __system_property_set(prop.c_str(), "");
      // In order to handle the case when ~SyspropRestorer() doesn't get run
      // (e.g. in case of SIGKILL), back up the sysprops to other sysprops,
      // rather than a process-local map.
      std::string backup_prop = prop + ".bkup";
      __system_property_set(backup_prop.c_str(), value.c_str());
    }
  }

  ~SyspropRestorer() {
    RestoreBackupSysprops();
    signal(SIGINT, SIG_DFL);
    exclusive_sysprop_restorer = nullptr;
    __system_property_set(kRunningGwpAsanTestsPropName, "");
  }

  static std::string GetSysprop(const std::string& name) {
    std::string value;
    const prop_info* pi = __system_property_find(name.c_str());
    if (pi == nullptr) return value;
    __system_property_read_callback(
        pi,
        [](void* cookie, const char* /* name */, const char* value, uint32_t /* serial */) {
          std::string* v = static_cast<std::string*>(cookie);
          *v = value;
        },
        &value);
    return value;
  }
};

SyspropRestorer* SyspropRestorer::exclusive_sysprop_restorer = nullptr;
const char* SyspropRestorer::kRunningGwpAsanTestsPropName = "libc.debug.running_gwp_asan_tests";

// During torture testing, we set the sample rate to '1' and allow a huge amount
// of allocation slots for GWP-ASan. During testing, it was ascertained that
// ~40,000 allocations is about the right amount to ensure that all possible
// mallocs() are sampled, without exploding svelte devices, as this uses ~163MiB
// RAM (4KiB per live allocation).
static const char* kMaxAllocsForTesting = "40000";

TEST_F(gwp_asan_integration_DeathTest, DISABLED_assert_gwp_asan_enabled) {
  std::string maps;
  EXPECT_TRUE(android::base::ReadFileToString("/proc/self/maps", &maps));
  EXPECT_TRUE(maps.find("GWP-ASan") != std::string::npos) << maps;

  volatile int* x = new int;
  delete x;
  EXPECT_DEATH({ *x = 7; }, "");
}

// A weaker version of the above tests, only checking that GWP-ASan is enabled
// for any pointer, not *our* pointer. This allows us to test the system_default
// sysprops without potentially OOM-ing other random processes:
// b/273904016#comment5
TEST(gwp_asan_integration, DISABLED_assert_gwp_asan_enabled_weaker) {
  std::string maps;
  EXPECT_TRUE(android::base::ReadFileToString("/proc/self/maps", &maps));
  EXPECT_TRUE(maps.find("GWP-ASan") != std::string::npos) << maps;
}

TEST(gwp_asan_integration, DISABLED_assert_gwp_asan_disabled) {
  std::string maps;
  EXPECT_TRUE(android::base::ReadFileToString("/proc/self/maps", &maps));
  EXPECT_TRUE(maps.find("GWP-ASan") == std::string::npos);
}

TEST(gwp_asan_integration, sysprops_program_specific) {
  // Do not override HWASan with GWP ASan.
  SKIP_WITH_HWASAN;

  SyspropRestorer restorer;

  std::string path = testing::internal::GetArgvs()[0];
  const char* basename = __gnu_basename(path.c_str());
  __system_property_set((std::string("libc.debug.gwp_asan.sample_rate.") + basename).c_str(), "1");
  __system_property_set((std::string("libc.debug.gwp_asan.process_sampling.") + basename).c_str(),
                        "1");
  __system_property_set((std::string("libc.debug.gwp_asan.max_allocs.") + basename).c_str(),
                        kMaxAllocsForTesting);

  RunSubtestNoEnv("gwp_asan_integration_DeathTest.DISABLED_assert_gwp_asan_enabled");
}

TEST(gwp_asan_integration, sysprops_persist_program_specific) {
  // Do not override HWASan with GWP ASan.
  SKIP_WITH_HWASAN;

  SyspropRestorer restorer;

  std::string path = testing::internal::GetArgvs()[0];
  const char* basename = __gnu_basename(path.c_str());
  __system_property_set(
      (std::string("persist.libc.debug.gwp_asan.sample_rate.") + basename).c_str(), "1");
  __system_property_set(
      (std::string("persist.libc.debug.gwp_asan.process_sampling.") + basename).c_str(), "1");
  __system_property_set((std::string("persist.libc.debug.gwp_asan.max_allocs.") + basename).c_str(),
                        kMaxAllocsForTesting);

  RunSubtestNoEnv("gwp_asan_integration_DeathTest.DISABLED_assert_gwp_asan_enabled");
}

TEST(gwp_asan_integration, sysprops_non_persist_overrides_persist) {
  // Do not override HWASan with GWP ASan.
  SKIP_WITH_HWASAN;

  SyspropRestorer restorer;

  __system_property_set("libc.debug.gwp_asan.sample_rate.system_default", "1");
  __system_property_set("libc.debug.gwp_asan.process_sampling.system_default", "1");
  // Note, any processes launched elsewhere on the system right now will have
  // GWP-ASan enabled. Make sure that we only use a single slot, otherwise we
  // could end up causing said badly-timed processes to use up to 163MiB extra
  // penalty that 40,000 allocs would cause. See b/273904016#comment5 for more
  // context.
  __system_property_set("libc.debug.gwp_asan.max_allocs.system_default", "1");

  __system_property_set("persist.libc.debug.gwp_asan.sample_rate.system_default", "0");
  __system_property_set("persist.libc.debug.gwp_asan.process_sampling.system_default", "0");
  __system_property_set("persist.libc.debug.gwp_asan.max_allocs.system_default", "0");

  RunSubtestNoEnv("gwp_asan_integration.DISABLED_assert_gwp_asan_enabled_weaker");
}

TEST(gwp_asan_integration, sysprops_program_specific_overrides_default) {
  // Do not override HWASan with GWP ASan.
  SKIP_WITH_HWASAN;

  SyspropRestorer restorer;

  std::string path = testing::internal::GetArgvs()[0];
  const char* basename = __gnu_basename(path.c_str());
  __system_property_set(
      (std::string("persist.libc.debug.gwp_asan.sample_rate.") + basename).c_str(), "1");
  __system_property_set(
      (std::string("persist.libc.debug.gwp_asan.process_sampling.") + basename).c_str(), "1");
  __system_property_set((std::string("persist.libc.debug.gwp_asan.max_allocs.") + basename).c_str(),
                        kMaxAllocsForTesting);

  __system_property_set("libc.debug.gwp_asan.sample_rate.system_default", "0");
  __system_property_set("libc.debug.gwp_asan.process_sampling.system_default", "0");
  __system_property_set("libc.debug.gwp_asan.max_allocs.system_default", "0");

  RunSubtestNoEnv("gwp_asan_integration_DeathTest.DISABLED_assert_gwp_asan_enabled");
}

TEST(gwp_asan_integration, sysprops_can_disable) {
  // Do not override HWASan with GWP ASan.
  SKIP_WITH_HWASAN;

  SyspropRestorer restorer;

  __system_property_set("libc.debug.gwp_asan.sample_rate.system_default", "0");
  __system_property_set("libc.debug.gwp_asan.process_sampling.system_default", "0");
  __system_property_set("libc.debug.gwp_asan.max_allocs.system_default", "0");

  RunSubtestNoEnv("gwp_asan_integration.DISABLED_assert_gwp_asan_disabled");
}

TEST(gwp_asan_integration, env_overrides_sysprop) {
  // Do not override HWASan with GWP ASan.
  SKIP_WITH_HWASAN;

  SyspropRestorer restorer;

  __system_property_set("libc.debug.gwp_asan.sample_rate.system_default", "0");
  __system_property_set("libc.debug.gwp_asan.process_sampling.system_default", "0");
  __system_property_set("libc.debug.gwp_asan.max_allocs.system_default", "0");

  RunGwpAsanTest("gwp_asan_integration_DeathTest.DISABLED_assert_gwp_asan_enabled");
}

#endif  // defined(__BIONIC__)
