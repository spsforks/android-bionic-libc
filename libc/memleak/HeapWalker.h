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

#ifndef LIBMEMLEAK_HEAP_WALKER_H_
#define LIBMEMLEAK_HEAP_WALKER_H_

#include "private/bionic_macros.h"

#include "Allocator.h"

// A range [begin, end)
struct Range {
  uintptr_t begin;
  uintptr_t end;
};

// Comparator for Ranges that returns equivalence for overlapping ranges
struct compare_range {
  bool operator()(const Range& a, const Range& b) const {
    return a.end <= b.begin;
  }
};

class HeapWalker {
 public:
  HeapWalker(Allocator<HeapWalker> allocator) : allocator_(allocator),
    ranges_(allocator), ranges_bytes_(0) {
    valid_range_.end = 0;
    valid_range_.begin = ~valid_range_.end;
  }
  ~HeapWalker() {}
  bool Add(uintptr_t begin, uintptr_t end);
  bool Live(uintptr_t begin, uintptr_t end);
  bool Leaked(allocator::vector<Range>&, size_t limit, size_t* num_leaks,
      size_t* leak_bytes);
  size_t Allocations();
  size_t AllocationBytes();

 private:
  DISALLOW_COPY_AND_ASSIGN(HeapWalker);
  Allocator<HeapWalker> allocator_;
  using RangeMap = allocator::map<bool, Range, compare_range>;
  RangeMap ranges_;
  size_t ranges_bytes_;
  Range valid_range_;
};

#endif
