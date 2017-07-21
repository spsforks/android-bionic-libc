/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include "PlatformVersion.h"

#include <sys/system_properties.h>

#include <string>

int platform_version() {
  static int platform_version_ = 0;
  if (platform_version_ == 0) {
    char value[PROP_VALUE_MAX];
    if (__system_property_get("ro.build.version.sdk", value) == 0) {
      platform_version_ = -1;
    } else {
      platform_version_ = std::stoi(value);
    }
  }

  return platform_version_;
}
