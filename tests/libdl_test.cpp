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

#include <android/api-level.h>

extern "C" void android_set_application_target_sdk_version(int target);
extern "C" void android_set_application_disabled_changes(long int* disabled_changes, int len);

TEST(libdl, application_sdk_versions_smoke) {
  // Check initial values
  ASSERT_EQ(__ANDROID_API__, android_get_application_target_sdk_version());

  android_set_application_target_sdk_version(20);
  ASSERT_EQ(20, android_get_application_target_sdk_version());

  android_set_application_target_sdk_version(22);
  ASSERT_EQ(22, android_get_application_target_sdk_version());
}

TEST(libdl, application_disabled_changes) {
  // Check initial values
  ASSERT_EQ(1, android_is_change_enabled(1));
  ASSERT_EQ(1, android_is_change_enabled(2));
  ASSERT_EQ(1, android_is_change_enabled(3));

  long int disabled_changes[] = {2, 3};
  android_set_application_disabled_changes(disabled_changes, 2);
  ASSERT_EQ(1, android_is_change_enabled(1));
  ASSERT_EQ(0, android_is_change_enabled(2));
  ASSERT_EQ(0, android_is_change_enabled(3));
}
