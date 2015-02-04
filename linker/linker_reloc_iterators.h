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

#ifndef __LINKER_RELOC_ITERATORS_H
#define __LINKER_RELOC_ITERATORS_H

#include "linker.h"

class plain_reloc_iterator {
#if defined(USE_RELA)
  typedef ElfW(Rela) rel_t;
#else
  typedef ElfW(Rel) rel_t;
#endif
 public:
  plain_reloc_iterator(rel_t* rel_array, size_t count) : rel_array_(rel_array), count_(count), current_ndx_(0) {}

  bool has_next() {
    return current_ndx_ < count_;
  }

  rel_t* next() {
    return rel_array_ + (current_ndx_++);
  }
 private:
  rel_t* rel_array_;
  size_t count_;
  size_t current_ndx_;
};

#endif  // __LINKER_RELOC_ITERATORS_H
