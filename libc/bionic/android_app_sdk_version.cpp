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

#include <android/appsdk.h>
#include <stdint.h>

// These are provided by libdl.so; we want linker to know about api-levels.
extern "C" bool _dl_android_set_application_sdk_versions(uint32_t target, uint32_t min, uint32_t max);
extern "C" uint32_t _dl_andorid_get_application_target_sdk_version();

bool android_set_application_sdk_versions(uint32_t target, uint32_t min, uint32_t max) {
  return _dl_android_set_application_sdk_versions(target, min, max);
}

uint32_t android_get_application_target_sdk_version() {
  return _dl_andorid_get_application_target_sdk_version();
}

