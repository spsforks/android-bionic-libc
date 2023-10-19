/*
 * Copyright (C) 2023 The Android Open Source Project
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

#include <errno.h>
#include <sys/wait.h>
#include <unistd.h>

#include <chrono>
#include <sstream>
#include <string>

#include <gtest/gtest.h>

#include <android-base/file.h>

#include "utils.h"

#if defined(__BIONIC__)
#include <sys/mount.h>
#include <sys/system_properties.h>
#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <sys/_system_properties.h>

int64_t NanoTime() {
  auto t = std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now());
  return t.time_since_epoch().count();
}

class SystemPropertiesMountDir : public TemporaryDir {
 public:
  ~SystemPropertiesMountDir() {
    if (mounted_) {
      umount(path);
    }
  }

  int mount_system_properties() {
    mounted_ = true;
    return mount(path, "/dev/__properties__/", nullptr, MS_BIND | MS_REC, nullptr);
  }

 private:
  bool mounted_;
};

#endif

// Note that this test affects global state of the system
// this tests tries to mitigate this by using utime+pid
// prefix for the property name. It is still results in
// pollution of property service since properties cannot
// be removed.
//
// Note that there is also possibility to run into "out-of-memory"
// if this test if it is executed often enough without reboot.
TEST(properties, __system_property_reload_no_op) {
#if defined(__BIONIC__)
  std::stringstream ss;
  ss << "debug.test." << getpid() << "." << NanoTime() << "."
     << "property_empty";
  const std::string property_name = ss.str();
  char value[] = "test_value";
  ASSERT_EQ(0, __system_property_find(property_name.c_str()));
  __system_property_set(property_name.c_str(), value);
  ASSERT_EQ(0, __system_properties_zygote_reload());
  const prop_info* readptr = __system_property_find(property_name.c_str());
  std::string expected_name = property_name;
  __system_property_read_callback(
      readptr,
      [](void*, const char*, const char* value, unsigned /*serial*/) {
        ASSERT_STREQ("test_value", value);
      },
      &expected_name);
#else   // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif  // __BIONIC__
}

TEST(properties, __system_property_reload_invalid) {
#if defined(__BIONIC__)
  SystemPropertiesMountDir tempDir = SystemPropertiesMountDir();
  std::stringstream ss;
  ss << "debug.test." << getpid() << "." << NanoTime() << "."
     << "property_empty";
  const std::string property_name = ss.str();
  char value[] = "test_value";

  ASSERT_EQ(0, __system_property_find(property_name.c_str()));
  __system_property_set(property_name.c_str(), value);
  int ret = tempDir.mount_system_properties();
  if (ret != 0) {
    ASSERT_ERRNO(0);
  }

  ASSERT_EQ(-1, __system_properties_zygote_reload());
#else   // __BIONIC__
  GTEST_SKIP() << "bionic-only test";
#endif  // __BIONIC__
}
