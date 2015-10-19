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

#include "linker_map.h"

#include <inttypes.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <unistd.h>

#include "private/libc_logging.h"

constexpr off64_t page_mask = ~static_cast<off64_t>(PAGE_SIZE-1);

static off64_t page_start(off64_t offset) {
  return offset & page_mask;
}

static off64_t page_end(off64_t offset) {
  return (offset + PAGE_SIZE -1) & page_mask;
}

static size_t page_offset(off64_t offset) {
  return static_cast<size_t>(offset & (PAGE_SIZE-1));
}

MappedFileFragment::MappedFileFragment() : map_start_(nullptr), map_size_(0),
                                           ptr_(nullptr), size_ (0)
{ }

MappedFileFragment::~MappedFileFragment() {
  if (map_start_ != nullptr) {
    munmap(map_start_, map_size_);
  }
}

bool MappedFileFragment::Map(int fd, off64_t offset, size_t size) {
  off64_t page_min = page_start(offset);
  off64_t page_max = page_end(offset + size);
  size_t map_size = static_cast<size_t>(page_max - page_min);

  if (page_max < page_min) {
    __libc_fatal("page_max < page_min: size=%zd, offset=%" PRId64, size, offset);
  }

  uint8_t* map_start = static_cast<uint8_t*>(
                          mmap64(nullptr, map_size, PROT_READ, MAP_PRIVATE, fd, page_min));

  if (map_start == MAP_FAILED) {
    return false;
  }

  map_start_ = map_start;
  map_size_ = map_size;

  ptr_ = map_start + page_offset(offset);
  size_ = size;

  return true;
}
