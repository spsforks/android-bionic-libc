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

#include <sys/cdefs.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtest/gtest.h>

// Provide implementations of stuff that's in bionic but not glibc.
#if !defined(__BIONIC__)

#if __has_builtin(__builtin_umul_overflow) || __GNUC__ >= 5
#if __LP64__
#define __size_add_overflow(a, b, result) __builtin_uaddl_overflow(a, b, result)
#define __size_mul_overflow(a, b, result) __builtin_umull_overflow(a, b, result)
#else
#define __size_add_overflow(a, b, result) __builtin_uadd_overflow(a, b, result)
#define __size_mul_overflow(a, b, result) __builtin_umul_overflow(a, b, result)
#endif  // __LP64__
#else
static int __size_add_overflow(size_t a, size_t b, size_t* result) {
  *result = a + b;
  return (b > 0) ? *result < a : *result > a;
}

static int __size_mul_overflow(size_t a, size_t b, size_t* result) {
  *result = a * b;
  static const size_t mul_no_overflow = 1UL << (sizeof(size_t) * 4);
  return (a >= mul_no_overflow || b >= mul_no_overflow) && a > 0 &&
         (size_t)-1 / a < b;
}
#endif  // __has_builtin(__builtin_umul_overflow) || __GNUC__ >= 5

static void __attribute__((unused)) __libc_fatal(const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vfprintf(stderr, fmt, args);
  va_end(args);
  abort();
}

// Required by private/libc_logging.h, which is included by private/WPVector.h.
#define __LIBC_HIDDEN__
#define __noreturn
#define __printflike(...)
#endif  // !defined(__BIONIC)

// This is awful.
#define class struct
#define private public
#include "private/WPVector.h"
#undef class
#undef private

static const size_t page_size = sysconf(_SC_PAGESIZE);

TEST(wpvector, wpvector_smoke) {
  const void* data;
  {
    WPVector<size_t> wpvec;
    auto vec = wpvec.mutate();
    ASSERT_EQ(0u, wpvec.size());
    ASSERT_EQ(vec.size(), wpvec.size());

    vec.push_back(0);
    ASSERT_EQ(1u, wpvec.size());
    ASSERT_EQ(vec.size(), wpvec.size());

    vec.push_back(1);
    ASSERT_EQ(2u, wpvec.size());
    ASSERT_EQ(vec.size(), wpvec.size());

    vec.push_back(2);
    ASSERT_EQ(3u, wpvec.size());
    ASSERT_EQ(vec.size(), wpvec.size());

    for (size_t i = 0; i < wpvec.size(); ++i) {
      ASSERT_EQ(i, wpvec[i]);
      ASSERT_EQ(wpvec[i], vec[i]);
    }

    vec.pop_back();
    ASSERT_EQ(2u, wpvec.size());
    ASSERT_EQ(vec.size(), wpvec.size());

    for (size_t i = 0; i < wpvec.size(); ++i) {
      ASSERT_EQ(i, wpvec[i]);
      ASSERT_EQ(wpvec[i], vec[i]);
    }

    data = reinterpret_cast<const char*>(wpvec.data()) - sizeof(WPVectorHeader);
  }

  ASSERT_EQ(-1, msync(const_cast<void*>(data), 1, MS_SYNC));
  ASSERT_EQ(ENOMEM, errno);
}

TEST(wpvector, wpvector_resize) {
  WPVector<char> wpvec;
  auto vec = wpvec.mutate();
  vec.push_back('a');
  vec.resize(page_size - sizeof(WPVectorHeader));
  ASSERT_EQ(page_size, wpvec.header->mMappingSize);
  ASSERT_EQ('a', vec[0]);
  for (size_t i = 1; i < vec.size(); ++i) {
    ASSERT_EQ('\0', vec[i]) << "i = " << i;
  }
}

TEST(wpvector, wpvector_readonly) {
  WPVector<char> wpvec;
  ASSERT_FALSE(wpvec.header);

  char* data = nullptr;

  {
    auto vec = wpvec.mutate();
    vec.resize(1);
    ASSERT_TRUE(wpvec.header);

    data = &vec[0];
    *data = 'a';

    {
      auto vec2 = wpvec.mutate();
      ASSERT_EQ(data, &vec[0]);
      *data = 'b';
    }

    ASSERT_EQ(data, &vec[0]);
    *data = 'c';
  }

  ASSERT_DEATH(*data = 'd', "");
}

TEST(wpvector, wpvector_large) {
  constexpr size_t struct_size = 16384 - sizeof(WPVectorHeader) + 1;
  ASSERT_GT(struct_size, page_size);
  ASSERT_NE(0u, struct_size % page_size);
  const size_t struct_pages = struct_size / page_size + 1;
  struct big {
    char data[struct_size];
  };

  WPVector<big> wpvec;
  auto vec = wpvec.mutate();
  vec.resize(1);
  ASSERT_EQ(vec.header->mMappingSize, page_size * (struct_pages + 1));
  vec.resize(2);
  ASSERT_EQ(vec.header->mMappingSize, page_size * (struct_pages * 2));
  vec.resize(3);
  ASSERT_EQ(vec.header->mMappingSize, page_size * (struct_pages * 3));
}

static char __attribute__((unused)) generate_value(size_t i) {
  return i % 256 + i / 256;
}

static bool is_mapped(const void* page) {
  if (msync(const_cast<void*>(page), 1, MS_SYNC) == -1) {
    if (errno != ENOMEM) {
      fprintf(stderr, "msync(%p) failed: %s\n", page, strerror(errno));
      abort();
    }
    return false;
  }
  return true;
}

static std::tuple<const char*, void*> prepare_mmap_move(WPVector<char>& wpvec,
                                                        size_t count) {
  // WPVector doesn't allocate until a mutator is created
  auto vec = wpvec.mutate();
  const char* orig_page = reinterpret_cast<const char*>(wpvec.header);
  char* next_page = const_cast<char*>(orig_page) + page_size;
  void* guard_page = nullptr;
  if (!is_mapped(next_page)) {
    // Map a page after the WPVector mapping to force it to move when resizing.
    guard_page = mmap(next_page, page_size, PROT_READ,
                      MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS, -1, 0);
    if (guard_page != next_page) {
      fprintf(stderr, "mmap failed: %s\n", strerror(errno));
      abort();
    }
  }

  for (size_t i = 0; i < count; ++i) {
    vec.push_back(generate_value(i));
  }

  if (vec.size() != count) {
    fprintf(stderr, "unexpected vector size: %zu [expected: %zu]", vec.size(),
            count);
  }

  return std::make_tuple(orig_page, guard_page);
}

#define PROLOGUE(count)                                                  \
  {                                                                      \
    void* old_page;                                                      \
    {                                                                    \
      WPVector<char> wpvec;                                              \
      const char* orig_page;                                             \
      void* guard_page;                                                  \
      std::tie(orig_page, guard_page) = prepare_mmap_move(wpvec, count); \
      auto vec = wpvec.mutate();                                         \
      const size_t orig_size = count;                                    \
      size_t expected_pages = 2;

#define EPILOGUE                                                         \
      EXPECT_EQ(page_size* expected_pages, wpvec.header->mMappingSize);  \
      EXPECT_NE(orig_page, reinterpret_cast<const char*>(wpvec.header)); \
      EXPECT_FALSE(is_mapped(orig_page));                                \
      EXPECT_TRUE(is_mapped(vec.header));                                \
      old_page = vec.header;                                             \
    }                                                                    \
    EXPECT_FALSE(is_mapped(old_page));                                   \
  }

TEST(wpvector, wpvector_realloc_a) {
  PROLOGUE(page_size - sizeof(WPVectorHeader))
    vec.push_back('\xFF');
    ASSERT_EQ(orig_size + 1, vec.size());

    ASSERT_EQ('\xFF', vec[vec.size() - 1]);
    for (size_t i = 0; i < vec.size(); ++i) {
      ASSERT_EQ(generate_value(i), vec[i]);
    }
  EPILOGUE
}

TEST(wpvector, wpvector_realloc_b) {
  PROLOGUE(page_size - sizeof(WPVectorHeader) - 1)
    vec.push_back('\xFF');
    ASSERT_EQ(orig_size + 1, vec.size());
    ASSERT_EQ(orig_page, reinterpret_cast<char*>(vec.header));
    ASSERT_EQ(page_size, vec.header->mMappingSize);

    vec.push_back('\xFF');
    ASSERT_EQ(orig_size + 2, vec.size());

    ASSERT_EQ('\xFF', vec[vec.size() - 1]);
    ASSERT_EQ('\xFF', vec[vec.size() - 2]);
    for (size_t i = 0; i < vec.size() - 2; ++i) {
      ASSERT_EQ(generate_value(i), vec[i]);
    }
  EPILOGUE
}

TEST(wpvector, wpvector_realloc_c) {
  PROLOGUE(page_size - sizeof(WPVectorHeader) - 1024)
    vec.insert(vec.end(), 1024, '\xFF');
    ASSERT_EQ(orig_size + 1024, vec.size());
    ASSERT_EQ(orig_page, reinterpret_cast<char*>(vec.header));
    ASSERT_EQ(page_size, vec.header->mMappingSize);

    vec.push_back('\xFF');

    for (size_t i = 0; i < orig_size; ++i) {
      ASSERT_EQ(generate_value(i), vec[i]);
    }
    for (size_t i = orig_size; i < vec.size(); ++i) {
      ASSERT_EQ('\xFF', vec[i]);
    }
  EPILOGUE
}

TEST(wpvector, wpvector_realloc_d) {
  PROLOGUE(page_size - sizeof(WPVectorHeader) - 1024)
    vec.insert(vec.begin() + 1024, 1025, '\xFF');
    ASSERT_EQ(orig_size + 1025, vec.size());
    ASSERT_NE(orig_page, reinterpret_cast<char*>(vec.header));

    for (size_t i = 0; i < 1024; ++i) {
      ASSERT_EQ(generate_value(i), vec[i]);
    }
    for (size_t i = 1024; i < 1024 + 1025; ++i) {
      ASSERT_EQ('\xFF', vec[i]);
    }
    for (size_t i = 1024 + 1025; i < vec.size(); ++i) {
      ASSERT_EQ(generate_value(i - 1025), vec[i]);
    }
  EPILOGUE
}

#undef PROLOGUE
#undef EPILOGUE
