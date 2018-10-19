/*
 * Copyright (C) 2013 The Android Open Source Project
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

#include <gtest/gtest.h>

#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

#include <tinyxml2.h>

#include "private/bionic_config.h"
#include "utils.h"

#if defined(__BIONIC__)
#define HAVE_REALLOCARRAY 1
#else
#define HAVE_REALLOCARRAY __GLIBC_PREREQ(2, 26)
#endif

TEST(malloc, malloc_std) {
  // Simple malloc test.
  void *ptr = malloc(100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  free(ptr);
}

TEST(malloc, malloc_overflow) {
  SKIP_WITH_HWASAN;
  errno = 0;
  ASSERT_EQ(nullptr, malloc(SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
}

TEST(malloc, calloc_std) {
  // Simple calloc test.
  size_t alloc_len = 100;
  char *ptr = (char *)calloc(1, alloc_len);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(alloc_len, malloc_usable_size(ptr));
  for (size_t i = 0; i < alloc_len; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, calloc_illegal) {
  SKIP_WITH_HWASAN;
  errno = 0;
  ASSERT_EQ(nullptr, calloc(-1, 100));
  ASSERT_EQ(ENOMEM, errno);
}

TEST(malloc, calloc_overflow) {
  SKIP_WITH_HWASAN;
  errno = 0;
  ASSERT_EQ(nullptr, calloc(1, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  errno = 0;
  ASSERT_EQ(nullptr, calloc(SIZE_MAX, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  errno = 0;
  ASSERT_EQ(nullptr, calloc(2, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  errno = 0;
  ASSERT_EQ(nullptr, calloc(SIZE_MAX, 2));
  ASSERT_EQ(ENOMEM, errno);
}

TEST(malloc, memalign_multiple) {
  SKIP_WITH_HWASAN; // hwasan requires power of 2 alignment.
  // Memalign test where the alignment is any value.
  for (size_t i = 0; i <= 12; i++) {
    for (size_t alignment = 1 << i; alignment < (1U << (i+1)); alignment++) {
      char *ptr = reinterpret_cast<char*>(memalign(alignment, 100));
      ASSERT_TRUE(ptr != nullptr) << "Failed at alignment " << alignment;
      ASSERT_LE(100U, malloc_usable_size(ptr)) << "Failed at alignment " << alignment;
      ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(ptr) % ((1U << i)))
          << "Failed at alignment " << alignment;
      free(ptr);
    }
  }
}

TEST(malloc, memalign_overflow) {
  SKIP_WITH_HWASAN;
  ASSERT_EQ(nullptr, memalign(4096, SIZE_MAX));
}

TEST(malloc, memalign_non_power2) {
  SKIP_WITH_HWASAN;
  void* ptr;
  for (size_t align = 0; align <= 256; align++) {
    ptr = memalign(align, 1024);
    ASSERT_TRUE(ptr != nullptr) << "Failed at align " << align;
    free(ptr);
  }
}

TEST(malloc, memalign_realloc) {
  // Memalign and then realloc the pointer a couple of times.
  for (size_t alignment = 1; alignment <= 4096; alignment <<= 1) {
    char *ptr = (char*)memalign(alignment, 100);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_LE(100U, malloc_usable_size(ptr));
    ASSERT_EQ(0U, (intptr_t)ptr % alignment);
    memset(ptr, 0x23, 100);

    ptr = (char*)realloc(ptr, 200);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_LE(200U, malloc_usable_size(ptr));
    ASSERT_TRUE(ptr != nullptr);
    for (size_t i = 0; i < 100; i++) {
      ASSERT_EQ(0x23, ptr[i]);
    }
    memset(ptr, 0x45, 200);

    ptr = (char*)realloc(ptr, 300);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_LE(300U, malloc_usable_size(ptr));
    for (size_t i = 0; i < 200; i++) {
      ASSERT_EQ(0x45, ptr[i]);
    }
    memset(ptr, 0x67, 300);

    ptr = (char*)realloc(ptr, 250);
    ASSERT_TRUE(ptr != nullptr);
    ASSERT_LE(250U, malloc_usable_size(ptr));
    for (size_t i = 0; i < 250; i++) {
      ASSERT_EQ(0x67, ptr[i]);
    }
    free(ptr);
  }
}

TEST(malloc, malloc_realloc_larger) {
  // Realloc to a larger size, malloc is used for the original allocation.
  char *ptr = (char *)malloc(100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  memset(ptr, 67, 100);

  ptr = (char *)realloc(ptr, 200);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(200U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(67, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, malloc_realloc_smaller) {
  // Realloc to a smaller size, malloc is used for the original allocation.
  char *ptr = (char *)malloc(200);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(200U, malloc_usable_size(ptr));
  memset(ptr, 67, 200);

  ptr = (char *)realloc(ptr, 100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(67, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, malloc_multiple_realloc) {
  // Multiple reallocs, malloc is used for the original allocation.
  char *ptr = (char *)malloc(200);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(200U, malloc_usable_size(ptr));
  memset(ptr, 0x23, 200);

  ptr = (char *)realloc(ptr, 100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0x23, ptr[i]);
  }

  ptr = (char*)realloc(ptr, 50);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(50U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 50; i++) {
    ASSERT_EQ(0x23, ptr[i]);
  }

  ptr = (char*)realloc(ptr, 150);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(150U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 50; i++) {
    ASSERT_EQ(0x23, ptr[i]);
  }
  memset(ptr, 0x23, 150);

  ptr = (char*)realloc(ptr, 425);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(425U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 150; i++) {
    ASSERT_EQ(0x23, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, calloc_realloc_larger) {
  // Realloc to a larger size, calloc is used for the original allocation.
  char *ptr = (char *)calloc(1, 100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));

  ptr = (char *)realloc(ptr, 200);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(200U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, calloc_realloc_smaller) {
  // Realloc to a smaller size, calloc is used for the original allocation.
  char *ptr = (char *)calloc(1, 200);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(200U, malloc_usable_size(ptr));

  ptr = (char *)realloc(ptr, 100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, calloc_multiple_realloc) {
  // Multiple reallocs, calloc is used for the original allocation.
  char *ptr = (char *)calloc(1, 200);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(200U, malloc_usable_size(ptr));

  ptr = (char *)realloc(ptr, 100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(100U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 100; i++) {
    ASSERT_EQ(0, ptr[i]);
  }

  ptr = (char*)realloc(ptr, 50);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(50U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 50; i++) {
    ASSERT_EQ(0, ptr[i]);
  }

  ptr = (char*)realloc(ptr, 150);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(150U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 50; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  memset(ptr, 0, 150);

  ptr = (char*)realloc(ptr, 425);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_LE(425U, malloc_usable_size(ptr));
  for (size_t i = 0; i < 150; i++) {
    ASSERT_EQ(0, ptr[i]);
  }
  free(ptr);
}

TEST(malloc, realloc_overflow) {
  SKIP_WITH_HWASAN;
  errno = 0;
  ASSERT_EQ(nullptr, realloc(nullptr, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  void* ptr = malloc(100);
  ASSERT_TRUE(ptr != nullptr);
  errno = 0;
  ASSERT_EQ(nullptr, realloc(ptr, SIZE_MAX));
  ASSERT_EQ(ENOMEM, errno);
  free(ptr);
}

#if defined(HAVE_DEPRECATED_MALLOC_FUNCS)
extern "C" void* pvalloc(size_t);
extern "C" void* valloc(size_t);

TEST(malloc, pvalloc_std) {
  size_t pagesize = sysconf(_SC_PAGESIZE);
  void* ptr = pvalloc(100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_TRUE((reinterpret_cast<uintptr_t>(ptr) & (pagesize-1)) == 0);
  ASSERT_LE(pagesize, malloc_usable_size(ptr));
  free(ptr);
}

TEST(malloc, pvalloc_overflow) {
  ASSERT_EQ(nullptr, pvalloc(SIZE_MAX));
}

TEST(malloc, valloc_std) {
  size_t pagesize = sysconf(_SC_PAGESIZE);
  void* ptr = pvalloc(100);
  ASSERT_TRUE(ptr != nullptr);
  ASSERT_TRUE((reinterpret_cast<uintptr_t>(ptr) & (pagesize-1)) == 0);
  free(ptr);
}

TEST(malloc, valloc_overflow) {
  ASSERT_EQ(nullptr, valloc(SIZE_MAX));
}
#endif

TEST(malloc, malloc_info) {
#ifdef __BIONIC__
  char* buf;
  size_t bufsize;
  FILE* memstream = open_memstream(&buf, &bufsize);
  ASSERT_NE(nullptr, memstream);
  ASSERT_EQ(0, malloc_info(0, memstream));
  ASSERT_EQ(0, fclose(memstream));

  tinyxml2::XMLDocument doc;
  ASSERT_EQ(tinyxml2::XML_SUCCESS, doc.Parse(buf));

  auto root = doc.FirstChildElement();
  ASSERT_NE(nullptr, root);
  ASSERT_STREQ("malloc", root->Name());
  ASSERT_STREQ("jemalloc-1", root->Attribute("version"));

  auto arena = root->FirstChildElement();
  for (; arena != nullptr; arena = arena->NextSiblingElement()) {
    int val;

    ASSERT_STREQ("heap", arena->Name());
    ASSERT_EQ(tinyxml2::XML_SUCCESS, arena->QueryIntAttribute("nr", &val));
    ASSERT_EQ(tinyxml2::XML_SUCCESS,
              arena->FirstChildElement("allocated-large")->QueryIntText(&val));
    ASSERT_EQ(tinyxml2::XML_SUCCESS,
              arena->FirstChildElement("allocated-huge")->QueryIntText(&val));
    ASSERT_EQ(tinyxml2::XML_SUCCESS,
              arena->FirstChildElement("allocated-bins")->QueryIntText(&val));
    ASSERT_EQ(tinyxml2::XML_SUCCESS,
              arena->FirstChildElement("bins-total")->QueryIntText(&val));

    auto bin = arena->FirstChildElement("bin");
    for (; bin != nullptr; bin = bin ->NextSiblingElement()) {
      if (strcmp(bin->Name(), "bin") == 0) {
        ASSERT_EQ(tinyxml2::XML_SUCCESS, bin->QueryIntAttribute("nr", &val));
        ASSERT_EQ(tinyxml2::XML_SUCCESS,
                  bin->FirstChildElement("allocated")->QueryIntText(&val));
        ASSERT_EQ(tinyxml2::XML_SUCCESS,
                  bin->FirstChildElement("nmalloc")->QueryIntText(&val));
        ASSERT_EQ(tinyxml2::XML_SUCCESS,
                  bin->FirstChildElement("ndalloc")->QueryIntText(&val));
      }
    }
  }
#endif
}

TEST(malloc, calloc_usable_size) {
  for (size_t size = 1; size <= 2048; size++) {
    void* pointer = malloc(size);
    ASSERT_TRUE(pointer != nullptr);
    memset(pointer, 0xeb, malloc_usable_size(pointer));
    free(pointer);

    // We should get a previous pointer that has been set to non-zero.
    // If calloc does not zero out all of the data, this will fail.
    uint8_t* zero_mem = reinterpret_cast<uint8_t*>(calloc(1, size));
    ASSERT_TRUE(pointer != nullptr);
    size_t usable_size = malloc_usable_size(zero_mem);
    for (size_t i = 0; i < usable_size; i++) {
      ASSERT_EQ(0, zero_mem[i]) << "Failed at allocation size " << size << " at byte " << i;
    }
    free(zero_mem);
  }
}

TEST(malloc, malloc_0) {
  void* p = malloc(0);
  ASSERT_TRUE(p != nullptr);
  free(p);
}

TEST(malloc, calloc_0_0) {
  void* p = calloc(0, 0);
  ASSERT_TRUE(p != nullptr);
  free(p);
}

TEST(malloc, calloc_0_1) {
  void* p = calloc(0, 1);
  ASSERT_TRUE(p != nullptr);
  free(p);
}

TEST(malloc, calloc_1_0) {
  void* p = calloc(1, 0);
  ASSERT_TRUE(p != nullptr);
  free(p);
}

TEST(malloc, realloc_nullptr_0) {
  // realloc(nullptr, size) is actually malloc(size).
  void* p = realloc(nullptr, 0);
  ASSERT_TRUE(p != nullptr);
  free(p);
}

TEST(malloc, realloc_0) {
  void* p = malloc(1024);
  ASSERT_TRUE(p != nullptr);
  // realloc(p, 0) is actually free(p).
  void* p2 = realloc(p, 0);
  ASSERT_TRUE(p2 == nullptr);
}

constexpr size_t MAX_LOOPS = 200;

// Make sure that memory returned by malloc is aligned to allow these data types.
TEST(malloc, verify_alignment) {
  uint32_t** values_32 = new uint32_t*[MAX_LOOPS];
  uint64_t** values_64 = new uint64_t*[MAX_LOOPS];
  long double** values_ldouble = new long double*[MAX_LOOPS];
  // Use filler to attempt to force the allocator to get potentially bad alignments.
  void** filler = new void*[MAX_LOOPS];

  for (size_t i = 0; i < MAX_LOOPS; i++) {
    // Check uint32_t pointers.
    filler[i] = malloc(1);
    ASSERT_TRUE(filler[i] != nullptr);

    values_32[i] = reinterpret_cast<uint32_t*>(malloc(sizeof(uint32_t)));
    ASSERT_TRUE(values_32[i] != nullptr);
    *values_32[i] = i;
    ASSERT_EQ(*values_32[i], i);
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(values_32[i]) & (sizeof(uint32_t) - 1));

    free(filler[i]);
  }

  for (size_t i = 0; i < MAX_LOOPS; i++) {
    // Check uint64_t pointers.
    filler[i] = malloc(1);
    ASSERT_TRUE(filler[i] != nullptr);

    values_64[i] = reinterpret_cast<uint64_t*>(malloc(sizeof(uint64_t)));
    ASSERT_TRUE(values_64[i] != nullptr);
    *values_64[i] = 0x1000 + i;
    ASSERT_EQ(*values_64[i], 0x1000 + i);
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(values_64[i]) & (sizeof(uint64_t) - 1));

    free(filler[i]);
  }

  for (size_t i = 0; i < MAX_LOOPS; i++) {
    // Check long double pointers.
    filler[i] = malloc(1);
    ASSERT_TRUE(filler[i] != nullptr);

    values_ldouble[i] = reinterpret_cast<long double*>(malloc(sizeof(long double)));
    ASSERT_TRUE(values_ldouble[i] != nullptr);
    *values_ldouble[i] = 5.5 + i;
    ASSERT_DOUBLE_EQ(*values_ldouble[i], 5.5 + i);
    // 32 bit glibc has a long double size of 12 bytes, so hardcode the
    // required alignment to 0x7.
#if !defined(__BIONIC__) && !defined(__LP64__)
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(values_ldouble[i]) & 0x7);
#else
    ASSERT_EQ(0U, reinterpret_cast<uintptr_t>(values_ldouble[i]) & (sizeof(long double) - 1));
#endif

    free(filler[i]);
  }

  for (size_t i = 0; i < MAX_LOOPS; i++) {
    free(values_32[i]);
    free(values_64[i]);
    free(values_ldouble[i]);
  }

  delete[] filler;
  delete[] values_32;
  delete[] values_64;
  delete[] values_ldouble;
}

TEST(malloc, mallopt_smoke) {
  errno = 0;
  ASSERT_EQ(0, mallopt(-1000, 1));
  // mallopt doesn't set errno.
  ASSERT_EQ(0, errno);
}

TEST(malloc, mallopt_decay) {
#if defined(__BIONIC__)
  errno = 0;
  ASSERT_EQ(1, mallopt(M_DECAY_TIME, 1));
  ASSERT_EQ(1, mallopt(M_DECAY_TIME, 0));
  ASSERT_EQ(1, mallopt(M_DECAY_TIME, 1));
  ASSERT_EQ(1, mallopt(M_DECAY_TIME, 0));
#else
  GTEST_LOG_(INFO) << "This tests a bionic implementation detail.\n";
#endif
}

TEST(malloc, mallopt_purge) {
#if defined(__BIONIC__)
  errno = 0;
  ASSERT_EQ(1, mallopt(M_PURGE, 0));
#else
  GTEST_LOG_(INFO) << "This tests a bionic implementation detail.\n";
#endif
}

TEST(malloc, reallocarray_overflow) {
#if HAVE_REALLOCARRAY
  // Values that cause overflow to a result small enough (8 on LP64) that malloc would "succeed".
  size_t a = static_cast<size_t>(INTPTR_MIN + 4);
  size_t b = 2;

  errno = 0;
  ASSERT_TRUE(reallocarray(nullptr, a, b) == nullptr);
  ASSERT_EQ(ENOMEM, errno);

  errno = 0;
  ASSERT_TRUE(reallocarray(nullptr, b, a) == nullptr);
  ASSERT_EQ(ENOMEM, errno);
#else
  GTEST_LOG_(INFO) << "This test requires a C library with reallocarray.\n";
#endif
}

TEST(malloc, reallocarray) {
#if HAVE_REALLOCARRAY
  void* p = reallocarray(nullptr, 2, 32);
  ASSERT_TRUE(p != nullptr);
  ASSERT_GE(malloc_usable_size(p), 64U);
#else
  GTEST_LOG_(INFO) << "This test requires a C library with reallocarray.\n";
#endif
}


#include <android-base/file.h>
#include <android-base/parseint.h>
#include <android-base/stringprintf.h>
#include <android-base/strings.h>
#include <linux/limits.h>
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <unordered_set>

#include <gtest/gtest.h>
#include <string>
#include <regex>
#include <utils/Log.h>

/* minimum entropy for malloc return addresses */
const size_t minEntropyBits = 8;

class AslrMallocTest : public ::testing::Test {
 protected:
  void GetAddress(const char* test_name, uintptr_t* address) {
    int fds[2];
    ASSERT_TRUE(pipe(fds) != -1);

    auto pid = fork();
    ASSERT_TRUE(pid != -1);
    if (pid == 0) {
      ASSERT_TRUE(TEMP_FAILURE_RETRY(dup2(fds[1], STDOUT_FILENO)) != -1);

      close(fds[0]);
      close(fds[1]);

      // Exec a disabled test to print the name.
      std::string exec(android::base::GetExecutablePath());
      std::string filter = std::string("--gtest_filter=AslrMallocTest.DISABLED_print_alloc_size_") + test_name;
      char* const args[4] = { const_cast<char* const>(exec.c_str()),
                              const_cast<char* const>(filter.c_str()),
                              const_cast<char* const>("--gtest_also_run_disabled_tests"),
                              nullptr};
      execvp(exec.c_str(), args);
      exit(1);
    }
    close(fds[1]);

    std::string output;
    ASSERT_TRUE(android::base::ReadFdToString(fds[0], &output));
    close(fds[0]);

    int status;
    ASSERT_EQ(pid, wait(&status));
    ASSERT_EQ(0, WEXITSTATUS(status));

    std::smatch match;
    std::regex regex("Pointer=(0x[0-9a-f]+)");
    std::regex_search(output, match, regex);
    ASSERT_EQ(2U, match.size()) << output;

    ASSERT_TRUE(android::base::ParseUint(match[1].str().c_str(), address)) << match[1].str();
  }

  void TestRandomization(const char* test_name) {
    // Should be sufficient to see minEntropyBits when rounded up.
    size_t iterations = 2 * (1 << minEntropyBits);

    // Collect unique return addresses.
    std::unordered_set<uintptr_t> addresses;

    for (size_t i = 0; i < iterations; ++i) {
      uintptr_t address;
      ASSERT_NO_FATAL_FAILURE(GetAddress(test_name, &address));

      addresses.emplace(address);
    }

    size_t entropy = static_cast<size_t>(0.5 + log2(static_cast<double>(addresses.size())));
    ASSERT_LE(minEntropyBits, entropy) << "total_entries: " << addresses.size() << " iterations: " << iterations;
  }
};

TEST_F(AslrMallocTest, MallocRandomization_small) {
  TestRandomization("small");
}

TEST_F(AslrMallocTest, MallocRandomization_large) {
  TestRandomization("large");
}

TEST_F(AslrMallocTest, MallocRandomization_huge) {
  TestRandomization("huge");
}

TEST_F(AslrMallocTest, DISABLED_print_alloc_size_small) {
  void* ptr = malloc(8);
  ASSERT_TRUE(ptr != nullptr);
  printf("Pointer=%p", ptr);
  free(ptr);
}

TEST_F(AslrMallocTest, DISABLED_print_alloc_size_large) {
  void* ptr = malloc(32000);
  ASSERT_TRUE(ptr != nullptr);
  printf("Pointer=%p", ptr);
  free(ptr);
}

TEST_F(AslrMallocTest, DISABLED_print_alloc_size_huge) {
  void* ptr = malloc(100000);
  ASSERT_TRUE(ptr != nullptr);
  printf("Pointer=%p", ptr);
  free(ptr);
}
