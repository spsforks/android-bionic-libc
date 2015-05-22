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

#include <android/appsdk.h>

TEST(appsdk, application_sdk_versions_levels_smoke) {
  // Check initial values
  ASSERT_EQ(0U, android_get_application_target_sdk_version());

  ASSERT_TRUE(android_set_application_sdk_versions(20U, 15U, 22U));
  ASSERT_EQ(20U, android_get_application_target_sdk_version());

  ASSERT_TRUE(android_set_application_sdk_versions(22U, 21U, 0U));
  ASSERT_EQ(22U, android_get_application_target_sdk_version());
}

TEST(appsdk, application_sdk_versions_invalid_input) {
  // Check initial values
  ASSERT_EQ(0U, android_get_application_target_sdk_version());

  ASSERT_TRUE(android_set_application_sdk_versions(20U, 15U, 22U));

  ASSERT_FALSE(android_set_application_sdk_versions(21U, 22U, 0U));
  ASSERT_FALSE(android_set_application_sdk_versions(23U, 21U, 1U));

  ASSERT_EQ(20U, android_get_application_target_sdk_version());
}

