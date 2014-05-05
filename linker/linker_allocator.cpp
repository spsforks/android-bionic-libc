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
#include "linker_allocator.h"
#include <inttypes.h>
#include <sys/mman.h>
#include <unistd.h>

struct FreeBlockInfo {
  void* next_block;
  size_t num_free_blocks;
};

LinkerBlockAllocator::LinkerBlockAllocator()
  : block_size_(0),
    page_size_(0),
    page_list_(nullptr),
    free_block_list_(nullptr)
{}

void LinkerBlockAllocator::init(size_t block_size) {
  block_size_ = block_size < sizeof(FreeBlockInfo) ? sizeof(FreeBlockInfo) : block_size;
  page_size_ = sysconf(_SC_PAGE_SIZE);
}

void* LinkerBlockAllocator::alloc() {
  if (free_block_list_ == nullptr) {
    create_new_page();
  } else {
    protect_read_write(free_block_list_);
  }

  if (free_block_list_ == nullptr) {
    return nullptr;
  }


  FreeBlockInfo* block_info = reinterpret_cast<FreeBlockInfo*>(free_block_list_);
  if (block_info->num_free_blocks > 1) {
    FreeBlockInfo* next_block_info = reinterpret_cast<FreeBlockInfo*>(
      reinterpret_cast<char*>(free_block_list_) + block_size_);
    next_block_info->next_block = block_info->next_block;
    next_block_info->num_free_blocks = block_info->num_free_blocks - 1;
    free_block_list_ = next_block_info;
  } else {
    free_block_list_ = block_info->next_block;
  }

  block_info->next_block = nullptr;
  block_info->num_free_blocks = 0;

  return block_info;
}

void LinkerBlockAllocator::free(void* block) {
  if (block == nullptr) {
    return;
  }

  LinkerAllocatorPage* page = find_page(block);

  if (page == nullptr) {
    return;
  }

  ssize_t offset = reinterpret_cast<uint8_t*>(block) - page->bytes;

  if (offset < 0 || offset % block_size_ != 0) {
    return;
  }

  FreeBlockInfo* block_info = reinterpret_cast<FreeBlockInfo*>(block);

  protect_read_write(block);
  block_info->next_block = free_block_list_;
  block_info->num_free_blocks = 1;
  protect_read_only(block);

  free_block_list_ = block_info;
}

void LinkerBlockAllocator::protect_all_read_only() {
  for (LinkerAllocatorPage* page = page_list_; page != nullptr; page = page->next) {
    if (mprotect(page, page_size_, PROT_READ) != 0) {
      abort();
    }
  }
}

void LinkerBlockAllocator::protect_all_read_write() {
  for (LinkerAllocatorPage* page = page_list_; page != nullptr; page = page->next) {
    if (mprotect(page, page_size_, PROT_READ | PROT_WRITE) != 0) {
      abort();
    }
  }
}

void LinkerBlockAllocator::protect_read_write(void* block) {
  LinkerAllocatorPage* page = find_page(block);
  if (page == nullptr || mprotect(page, page_size_, PROT_READ | PROT_WRITE) != 0) {
    abort();
  }
}

void LinkerBlockAllocator::protect_read_only(void* block) {
  LinkerAllocatorPage* page = find_page(block);
  if (page == nullptr || mprotect(page, page_size_, PROT_READ ) != 0) {
    abort();
  }
}


void LinkerBlockAllocator::create_new_page() {
  LinkerAllocatorPage* page = reinterpret_cast<LinkerAllocatorPage*>(mmap(nullptr, page_size_,
      PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, 0, 0));
  if (page == MAP_FAILED) {
    return;
  }

  FreeBlockInfo* first_block = reinterpret_cast<FreeBlockInfo*>(page->bytes);
  first_block->next_block = free_block_list_;
  first_block->num_free_blocks = (page_size_ - sizeof(LinkerAllocatorPage*))/block_size_;

  free_block_list_ = first_block;

  page->next = page_list_;
  page_list_ = page;
}

LinkerAllocatorPage* LinkerBlockAllocator::find_page(void* block) {
  if (block == nullptr) {
    return nullptr;
  }

  LinkerAllocatorPage* page = page_list_;
  const uint8_t* page_ptr = reinterpret_cast<const uint8_t*>(page);
  while(page != nullptr) {
    if (block > page_ptr && block < (page_ptr + page_size_)) {
      return page;
    }

    page = page->next;
  }

  return nullptr;
}
