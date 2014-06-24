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

#ifndef SCOPED_MMAP_H
#define SCOPED_MMAP_H

#include "bionic_macros.h"
#include <limits.h>
#include <sys/mman.h>

// A smart pointer that closes the given fd on going out of scope.
// Use this when the fd is incidental to the purpose of your function,
// but needs to be cleaned up on exit.
class ScopedMmap {
 public:
  ScopedMmap(void* addr_hint, size_t length, int prot, int flags, int fd, off_t offset)
      : addr_(MAP_FAILED), length_(0), public_addr_(MAP_FAILED) {
    off_t offset_aligned = offset & PAGE_MASK;
    size_t length_aligned = length + (offset & ~PAGE_MASK);
    addr_ = mmap(addr_hint, length_aligned, prot, flags, fd, offset_aligned);
    if (addr_ == MAP_FAILED) {
      return;
    }

    length_ = length_aligned;
    public_addr_ = reinterpret_cast<uint8_t*>(addr_) + (offset & ~PAGE_MASK);
  }

  ~ScopedMmap() {
    if (addr_ != MAP_FAILED) {
      munmap(addr_, length_);
    }
  }

  void* get() const {
    return public_addr_;
  }

 private:
  // page-aligned address
  void* addr_;
  size_t length_;
  // requested address = (addr_ + (offset & ~PAGE_MASK))
  void* public_addr_;

  DISALLOW_COPY_AND_ASSIGN(ScopedMmap);
};

#endif  // SCOPED_MMAP_H
