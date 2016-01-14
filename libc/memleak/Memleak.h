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

#ifndef LIBMEMLEAK_MEMLEAK_H_
#define LIBMEMLEAK_MEMLEAK_H_

#include <sys/cdefs.h>

#include <vector>

struct Leak {
  uintptr_t begin;
  size_t size;
  static const size_t contents_length = 32;
  char contents[contents_length];
};

__BEGIN_DECLS
bool DetectLeaks(std::vector<Leak>& leaks, size_t limit = 100,
    size_t* num_leaks = NULL, size_t* leak_bytes = NULL);

bool DetectAndLogLeaks(bool log_contents = false, size_t limit = 100);
__END_DECLS

#endif // LIBMEMLEAK_MEMLEAK_H_
