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

#include <unistd.h>

#include "jemalloc.h"

void* je_pvalloc(size_t bytes) {
  size_t pagesize = sysconf(_SC_PAGESIZE);
  return je_memalign(pagesize, (bytes + pagesize - 1) & ~(pagesize - 1));
}

#ifdef je_memalign
#undef je_memalign
#endif

void* je_memalign_wrapper(size_t boundary, size_t size) {
  // Align boundary to the next highest power of two.
  unsigned int power_of_2 = static_cast<unsigned int>(boundary);
  if (power_of_2 != 0) {
    power_of_2 = 1UL << (sizeof(unsigned int)*8 - 1 - __builtin_clz(power_of_2));
    if (power_of_2 != boundary) {
      boundary = power_of_2 << 1;
    }
  } else {
    boundary = 1;
  }
  return je_memalign(boundary, size);
}
