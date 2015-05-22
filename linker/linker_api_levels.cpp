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

#include "linker.h"

static uint32_t g_target_api_level;
static uint32_t g_min_api_level;
static uint32_t g_max_api_level;

bool set_application_api_levels(uint32_t target, uint32_t min, uint32_t max) {
  if (target != 0 && (target < min || (max != 0 && target > max ))) {
    return false;
  }

  if (max != 0 && max < min) {
    return false;
  }

  g_target_api_level = target;
  g_min_api_level = min;
  g_max_api_level = max;

  return true;
}

uint32_t get_application_target_api_level() {
  return g_target_api_level;
}

uint32_t get_application_min_api_level() {
  return g_min_api_level;
}

uint32_t get_application_max_api_level() {
  return g_max_api_level;
}

