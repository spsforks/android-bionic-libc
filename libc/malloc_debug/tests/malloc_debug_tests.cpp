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

#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/param.h>

#include <gtest/gtest.h>

#include <private/bionic_macros.h>
#include <private/bionic_malloc_dispatch.h>

__BEGIN_DECLS

int property_set(const char*, const char*);
bool debug_initialize(const MallocDispatch*);
void debug_finalize();

void* debug_malloc(size_t);
void debug_free(void*);
void* debug_calloc(size_t, size_t);
void* debug_realloc(void*, size_t);
int debug_posix_memalign(void**, size_t, size_t);
void* debug_memalign(size_t, size_t);
size_t debug_malloc_usable_size(void*);

__END_DECLS

class MallocDebugTest : public ::testing::Test {
 protected:
  void SetUp() override {
  }

  void TearDown() override {
    if (initialized) {
      debug_finalize();
    }
  }

  void Init(const char* property_value) {
    property_set("libc.debug.malloc.options", property_value);
    ASSERT_TRUE(debug_initialize(&dispatch));
    initialized = true;
  }

  bool initialized = false;

  static MallocDispatch dispatch;
};

MallocDispatch MallocDebugTest::dispatch = {
  .calloc = calloc,
  .free = free,
  .mallinfo = mallinfo,
  .malloc = malloc,
  .malloc_usable_size = malloc_usable_size,
  .memalign = memalign,
  .posix_memalign = posix_memalign,
  .realloc = realloc,
};

void VerifyAllocCalls() {
  size_t alloc_size = 1024;

  // Verify debug_malloc.
  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(alloc_size));
  ASSERT_TRUE(pointer != nullptr);
  for (size_t i = 0; i < alloc_size; i++) {
    ASSERT_EQ(0xeb, pointer[i]);
  }
  debug_free(pointer);

  // Verify debug_calloc.
  pointer = reinterpret_cast<uint8_t*>(debug_calloc(1, alloc_size));
  ASSERT_TRUE(pointer != nullptr);
  for (size_t i = 0; i < alloc_size; i++) {
    ASSERT_EQ(0, pointer[i]);
  }
  debug_free(pointer);

  pointer = reinterpret_cast<uint8_t*>(debug_memalign(128, alloc_size));
  ASSERT_TRUE(pointer != nullptr);
  for (size_t i = 0; i < alloc_size; i++) {
    ASSERT_EQ(0xeb, pointer[i]);
  }
  debug_free(pointer);

  pointer = reinterpret_cast<uint8_t*>(debug_realloc(nullptr, alloc_size));
  ASSERT_TRUE(pointer != nullptr);
  for (size_t i = 0; i < alloc_size; i++) {
    ASSERT_EQ(0xeb, pointer[i]);
  }
  memset(pointer, 0xff, alloc_size);
  // Increase the size, verify the extra length is initialized to 0xeb,
  // but the rest is 0xff.
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, alloc_size * 2));
  ASSERT_TRUE(pointer != nullptr);
  for (size_t i = 0; i < alloc_size; i++) {
    ASSERT_EQ(0xff, pointer[i]);
  }
  for (size_t i = alloc_size; i < alloc_size * 2; i++) {
    ASSERT_EQ(0xeb, pointer[i]);
  }
  memset(pointer, 0xff, alloc_size * 2);
  // Shrink the size and verify nothing changes.
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, alloc_size));
  ASSERT_TRUE(pointer != nullptr);
  for (size_t i = 0; i < alloc_size; i++) {
    ASSERT_EQ(0xff, pointer[i]);
  }
  // This should free the pointer.
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, 0));
  ASSERT_TRUE(pointer == nullptr);
}

TEST_F(MallocDebugTest, fill) {
  Init("fill");
  VerifyAllocCalls();
}

TEST_F(MallocDebugTest, fill_on_alloc) {
  Init("fill_on_alloc");
  VerifyAllocCalls();
}

TEST_F(MallocDebugTest, expand_alloc) {
  Init("expand_alloc=1024");

  void* pointer = debug_malloc(10);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_LE(1034U, debug_malloc_usable_size(pointer));
  debug_free(pointer);

  pointer = debug_calloc(1, 20);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_LE(1044U, debug_malloc_usable_size(pointer));
  debug_free(pointer);

  pointer = debug_memalign(128, 15);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_LE(1039U, debug_malloc_usable_size(pointer));
  debug_free(pointer);

  pointer = debug_realloc(nullptr, 30);
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_LE(1054U, debug_malloc_usable_size(pointer));
  pointer = debug_realloc(pointer, 100);
  ASSERT_LE(1124U, debug_malloc_usable_size(pointer));
  debug_free(pointer);
}

TEST_F(MallocDebugTest, front_guard) {
  Init("front_guard=32");

  // Create a buffer for doing comparisons.
  std::vector<uint8_t> buffer(32);
  memset(buffer.data(), 0xaa, buffer.size());

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0);
  memset(pointer, 0xff, 100);
  debug_free(pointer);

  // Loop through a bunch alignments.
  for (size_t alignment = 1; alignment <= 256; alignment++) {
    pointer = reinterpret_cast<uint8_t*>(debug_memalign(alignment, 100));
    ASSERT_TRUE(pointer != nullptr);
    ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0);
    size_t alignment_mask = alignment - 1;
    if (!powerof2(alignment)) {
      alignment_mask = BIONIC_ROUND_UP_POWER_OF_2(alignment) - 1;
    }
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(pointer) & alignment_mask);
    memset(pointer, 0xff, 100);
    debug_free(pointer);
  }

  pointer = reinterpret_cast<uint8_t*>(debug_calloc(1, 100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0);
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, pointer[i]) << "debug_calloc non-zero byte at " << i;
  }
  debug_free(pointer);

  pointer = reinterpret_cast<uint8_t*>(debug_realloc(nullptr, 100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0);
  memset(pointer, 0xff, 100);
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, 200));
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[-buffer.size()], buffer.size()) == 0);
  memset(pointer, 0xff, 200);
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, 0));
  ASSERT_TRUE(pointer == nullptr);
}

TEST_F(MallocDebugTest, front_guard_corrupted) {
  Init("front_guard=32");

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  pointer[-32] = 0x00;
  debug_free(pointer);
}

TEST_F(MallocDebugTest, rear_guard) {
  Init("rear_guard=32");

  // Create a buffer for doing comparisons.
  std::vector<uint8_t> buffer(32);
  memset(buffer.data(), 0xbb, buffer.size());

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(100U, debug_malloc_usable_size(pointer));
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[100], buffer.size()) == 0);
  memset(pointer, 0xff, 100);
  debug_free(pointer);

  // Loop through a bunch alignments.
  for (size_t alignment = 1; alignment <= 256; alignment++) {
    pointer = reinterpret_cast<uint8_t*>(debug_memalign(alignment, 100));
    ASSERT_TRUE(pointer != nullptr);
    ASSERT_EQ(100U, debug_malloc_usable_size(pointer));
    ASSERT_TRUE(memcmp(buffer.data(), &pointer[100], buffer.size()) == 0);
    size_t alignment_mask = alignment - 1;
    if (!powerof2(alignment)) {
      alignment_mask = BIONIC_ROUND_UP_POWER_OF_2(alignment) - 1;
    }
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(pointer) & alignment_mask);
    memset(pointer, 0xff, 100);
    debug_free(pointer);
  }

  pointer = reinterpret_cast<uint8_t*>(debug_calloc(1, 100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_EQ(100U, debug_malloc_usable_size(pointer));
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[100], buffer.size()) == 0);
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, pointer[i]) << "debug_calloc non-zero byte at " << i;
  }
  debug_free(pointer);

  pointer = reinterpret_cast<uint8_t*>(debug_realloc(nullptr, 100));
  ASSERT_TRUE(pointer != nullptr);
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[100], buffer.size()) == 0);
  memset(pointer, 0xff, 100);
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, 200));
  ASSERT_TRUE(memcmp(buffer.data(), &pointer[200], buffer.size()) == 0);
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0xff, pointer[i]) << "debug_realloc not copied byte at " << i;
  }
  memset(pointer, 0xff, 200);
  pointer = reinterpret_cast<uint8_t*>(debug_realloc(pointer, 0));
  ASSERT_TRUE(pointer == nullptr);
}

TEST_F(MallocDebugTest, rear_guard_corrupted) {
  Init("rear_guard=32");

  uint8_t* pointer = reinterpret_cast<uint8_t*>(debug_malloc(100));
  ASSERT_TRUE(pointer != nullptr);
  //pointer[131] = 0x00;
  debug_free(pointer);
}

TEST_F(MallocDebugTest, tag_corrupted) {
}
