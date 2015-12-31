/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include "gtest_globals.h"

#include <gtest/gtest.h>
#include "utils.h"

#include <string>

static std::string get_realpath_or_fail(const std::string& path) {
  std::string real_path;

  if (!get_realpath(path.c_str(), &real_path)) {
    printf("Failed to get realpath for \"%s\"", path.c_str());
    abort();
  }

  return real_path;
}

static std::string init_data_root() {
  const char* android_data = getenv("ANDROID_DATA");
  std::string path;

  if (android_data == nullptr) {
    // Calculate ANDROID_DATA assuming the binary is in "$ANDROID_DATA/somedir/binary-dir/binary"
    path = get_dirname(get_executable_path().c_str()) + "/../..";
  } else {
    path = android_data;
  }

  return get_realpath_or_fail(path);
}

static std::string init_testlib_root() {
  std::string path = get_data_root() + "/nativetest";
#if defined(__LP64__)
  path += "64";
#endif
  path += "/bionic-loader-test-libs";

  return get_realpath_or_fail(path);
}

const std::string& get_data_root() {
  static const std::string data_root = init_data_root();
  return data_root;
}

const std::string& get_testlib_root() {
  static const std::string testlib_root = init_testlib_root();
  return testlib_root;
}
