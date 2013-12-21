/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <stdio.h>
#include <inttypes.h>

#if defined(__LP64__)
#define PTR_FORMAT "%016"
#else
#define PTR_FORMAT "%08"
#endif

TEST(inttypes, misc) {
  char buf[512];

  intptr_t i = 0;
  uintptr_t u = 0;

  snprintf(buf, sizeof(buf), PTR_FORMAT PRIdPTR, i);
  snprintf(buf, sizeof(buf), PTR_FORMAT PRIiPTR, i);
  snprintf(buf, sizeof(buf), PTR_FORMAT PRIoPTR, i);
  snprintf(buf, sizeof(buf), PTR_FORMAT PRIuPTR, u);
  snprintf(buf, sizeof(buf), PTR_FORMAT PRIxPTR, u);
  snprintf(buf, sizeof(buf), PTR_FORMAT PRIXPTR, u);

  sscanf(buf, PTR_FORMAT SCNdPTR, &i);
  sscanf(buf, PTR_FORMAT SCNiPTR, &i);
  sscanf(buf, PTR_FORMAT SCNoPTR, &u);
  sscanf(buf, PTR_FORMAT SCNuPTR, &u);
  sscanf(buf, PTR_FORMAT SCNxPTR, &u);
}
