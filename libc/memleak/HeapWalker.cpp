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

#include <map>
#include <utility>

#include "private/bionic_macros.h"

#include "Allocator.h"
#include "HeapWalker.h"
#include "log.h"

bool HeapWalker::Add(uintptr_t begin, uintptr_t end) {
  auto inserted = ranges_.insert(std::pair<Range, bool>(Range{begin, end}, false));
  if (inserted.second) {
    valid_range_.begin = std::min(valid_range_.begin, begin);
    valid_range_.end = std::max(valid_range_.end, end);
    return true;
  } else {
    Range overlap = inserted.first->first;
    log_error("range %p-%p overlaps with existing range %p-%p",
        reinterpret_cast<void*>(begin),
        reinterpret_cast<void*>(end),
        reinterpret_cast<void*>(overlap.begin),
        reinterpret_cast<void*>(overlap.end));
    return false;
  }
}

bool HeapWalker::Live(uintptr_t begin, uintptr_t end) {
  allocator::vector<Range> to_do(1, Range{begin, end}, allocator_);
  while (!to_do.empty()) {
    Range range = to_do.back();
    to_do.pop_back();
    uintptr_t begin = (range.begin + (sizeof(uintptr_t) - 1)) & ~(sizeof(uintptr_t) - 1);
    /* TODO(ccross): we might need to consider a pointer to the end of a buffer
     * to be inside the buffer, which means the common case of a pointer to the
     * beginning of a buffer may keep two ranges live. */
    for (uintptr_t i = begin; i < range.end; i += sizeof(uintptr_t)) {
      uintptr_t val = *reinterpret_cast<uintptr_t*>(i);
      if (val >= valid_range_.begin && val < valid_range_.end) {
        RangeMap::iterator it = ranges_.find(Range{val, val + 1});
        if (it != ranges_.end()) {
          if (!it->second) {
            to_do.push_back(it->first);
            it->second = true;
          }
        }
      }
    }
  }

  return true;
}

size_t HeapWalker::Allocations() {
  return ranges_.size();
}

size_t HeapWalker::Leaked(allocator::vector<Range>& leaked, size_t limit) {
  leaked.clear();
  size_t num_leaks = 0;
  for (auto it = ranges_.begin(); it != ranges_.end(); it++) {
    if (!it->second) {
      num_leaks++;
      if (num_leaks <= limit) {
        leaked.push_back(it->first);
      }
    }
  }

  return num_leaks;
}
