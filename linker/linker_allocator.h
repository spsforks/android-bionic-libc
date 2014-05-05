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

#ifndef __LINKER_ALLOCATOR_H
#define __LINKER_ALLOCATOR_H

#include <stdlib.h>

struct LinkerAllocatorPage {
  LinkerAllocatorPage* next;
  unsigned char bytes[1];
};

class LinkerBlockAllocator {
 public:
  LinkerBlockAllocator();

  void init(size_t block_size);
  void* alloc();
  void free(void* block);
  void protect_read_write(void* block);
  void protect_read_only(void* block);
  void protect_all_read_write();
  void protect_all_read_only();

 private:
  void create_new_page();
  LinkerAllocatorPage* find_page(void* block);

  size_t block_size_;
  size_t page_size_;
  LinkerAllocatorPage* page_list_;
  void* free_block_list_;

  // do not copy
  LinkerBlockAllocator(const LinkerBlockAllocator& /* that */);
  LinkerBlockAllocator& operator=(const LinkerBlockAllocator& /* that */);
};

/*
 * Supply type instead of size.
 *
 * "Because reinterpret_cast is hard name to remeber."
 */
template<typename T>
class LinkerAllocator {
 public:
  void init() { block_allocator_.init(sizeof(T)); }
  T* alloc() { return reinterpret_cast<T*>(block_allocator_.alloc()); }
  void free(T* t) { block_allocator_.free(t); }
  void protect_read_write(T* t) { block_allocator_.protect_read_write(t); }
  void protect_read_only(T* t) { block_allocator_.protect_read_only(t); }
  void protect_all_read_write() { block_allocator_.protect_all_read_write(); }
  void protect_all_read_only() { block_allocator_.protect_all_read_only(); }
 private:
  LinkerBlockAllocator block_allocator_;
};

#endif // __LINKER_ALLOCATOR_H
