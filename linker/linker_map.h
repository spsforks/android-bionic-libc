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
#ifndef LINKER_MAP_H
#define LINKER_MAP_H

#include <unistd.h>

#include "private/bionic_macros.h"

class MappedFileFragment {
 public:
  MappedFileFragment();
  ~MappedFileFragment();

  bool Map(int fd, off64_t offset, size_t size);

  void* get_ptr() const { return ptr_; }
  size_t get_size() const { return size_; }
 private:
  void* map_start_;
  size_t map_size_;
  void* ptr_;
  size_t size_;

  DISALLOW_COPY_AND_ASSIGN(MappedFileFragment);
};

#endif /* LINKER_MAP_H */
