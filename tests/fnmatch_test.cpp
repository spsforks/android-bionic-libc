/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <fnmatch.h>

#include <errno.h>
#include <gtest/gtest.h>
#include <malloc.h>
#include <math.h>
#include <stdint.h>

#include <algorithm>
#include <vector>

#include "buffer_tests.h"

#if defined(NOFORTIFY)
#define FNMATCH_TEST fnmatch_nofortify
#else
#define FNMATCH_TEST fnmatch
#endif

constexpr auto KB = 1024;
constexpr auto SMALL = 1 * KB;
constexpr auto MEDIUM = 4 * KB;

// TODO: where did this number come from?
#define ITER        500

// For every length we want to test, vary and change alignment
// of allocated memory, fill it with some values, calculate
// expected result and then run function and compare what we got.
// These tests contributed by Intel Corporation.
// TODO: make these tests more intention-revealing and less random.
template<class Character>
class StringTestState {
 public:
  explicit StringTestState(size_t MAX_LEN) : MAX_LEN(MAX_LEN), align1_index_(0), align2_index_(0) {
    int max_alignment = 64;

    // TODO: fix the tests to not sometimes use twice their specified "MAX_LEN".
    glob_ptr = reinterpret_cast<Character*>(memalign(sysconf(_SC_PAGESIZE), 2 * sizeof(Character) * MAX_LEN + max_alignment));
    glob_ptr1 = reinterpret_cast<Character*>(memalign(sysconf(_SC_PAGESIZE), 2 * sizeof(Character) * MAX_LEN + max_alignment));
    glob_ptr2 = reinterpret_cast<Character*>(memalign(sysconf(_SC_PAGESIZE), 2 * sizeof(Character) * MAX_LEN + max_alignment));

    InitLenArray();

    srandom(1234);
  }

  ~StringTestState() {
    free(glob_ptr);
    free(glob_ptr1);
    free(glob_ptr2);
  }

  void BeginIterations() {
    align1_index_ = 0;
    align2_index_ = 0;

    ResetPointers();
  }

  bool HasNextIteration() {
    return (align1_index_ != (alignments_size - 1) || align2_index_ != (alignments_size - 1));
  }

  void NextIteration() {
    if (align1_index_ == (alignments_size - 1) && align2_index_ == (alignments_size - 1)) {
      return;
    }

    if (align1_index_ == (alignments_size - 1)) {
      align1_index_ = 0;
      align2_index_++;
    } else {
      align1_index_++;
    }

    ResetPointers();
  }

  const size_t MAX_LEN;
  Character *ptr, *ptr1, *ptr2;
  size_t n;
  size_t len[ITER + 1];

 private:
  static size_t alignments[];
  static size_t alignments_size;
  Character *glob_ptr, *glob_ptr1, *glob_ptr2;
  size_t align1_index_, align2_index_;

  // Calculate input lengths and fill state.len with them.
  // Test small lengths with more density than big ones. Manually push
  // smallest (0) and biggest (MAX_LEN) lengths. Avoid repeats.
  // Return number of lengths to test.
  void InitLenArray() {
    n = 0;
    len[n++] = 0;
    for (size_t i = 1; i < ITER; ++i) {
      size_t l = static_cast<size_t>(exp(log(static_cast<double>(MAX_LEN)) * i / ITER));
      if (l != len[n - 1]) {
        len[n++] = l;
      }
    }
    len[n++] = MAX_LEN;
  }

  void ResetPointers() {
    if (align1_index_ == alignments_size || align2_index_ == alignments_size) {
      ptr = ptr1 = ptr2 = nullptr;
    } else {
      ptr = glob_ptr + alignments[align1_index_];
      ptr1 = glob_ptr1 + alignments[align1_index_];
      ptr2 = glob_ptr2 + alignments[align2_index_];
    }
  }
};

template<class Character>
size_t StringTestState<Character>::alignments[] = { 24, 32, 16, 48, 0, 1, 2, 3, 4, 5, 6, 7, 11 };

template<class Character>
size_t StringTestState<Character>::alignments_size = sizeof(alignments)/sizeof(size_t);

TEST(FNMATCH_TEST, compare) {
  StringTestState<char> state(SMALL);
  for (size_t i = 1; i < state.n; i++) {
    for (state.BeginIterations(); state.HasNextIteration(); state.NextIteration()) {
      memset(state.ptr1, 'v', state.MAX_LEN);
      memset(state.ptr2, 'n', state.MAX_LEN);
      state.ptr1[state.len[i] - 1] = '\0';
      state.ptr2[state.len[i] - 1] = '\0';

      size_t pos = 1 + (random() % (state.MAX_LEN - 1));
      int actual;
      int expected;
      if (pos >= state.len[i] - 1) {
        memcpy(state.ptr1, state.ptr2, state.len[i]);
        expected = 0;
        actual = fnmatch(state.ptr1, state.ptr2, 0);
      } else {
        memcpy(state.ptr1, state.ptr2, pos);
        if (state.ptr1[pos] > state.ptr2[pos]) {
          expected = FNM_NOMATCH;
        } else if (state.ptr1[pos] == state.ptr2[pos]) {
          state.ptr1[pos + 1] = '\0';
          state.ptr2[pos + 1] = '\0';
          expected = 0;
        } else {
          expected = FNM_NOMATCH;
        }
        actual = fnmatch(state.ptr1, state.ptr2, 0);
      }

      ASSERT_EQ(expected, actual);
    }
  }
}

static void DoFnmatchTest(uint8_t* buf1, uint8_t* buf2, size_t len) {
  if (len >= 1) {
    int c = (32 + (len % 96));
    switch (c) {
      case '[':
        ++c;
      case '*':
      case '?':
      case '\\':
        ++c;
    }
    memset(buf1, c, len - 1);
    buf1[len-1] = '\0';
    memset(buf2, c, len - 1);
    buf2[len-1] = '\0';
    ASSERT_EQ(0, fnmatch(reinterpret_cast<char*>(buf1),
                         reinterpret_cast<char*>(buf2), 0))
        << "\nbuf1=\"" << reinterpret_cast<char*>(buf1)
        << "\"\nbuf2=\"" << reinterpret_cast<char*>(buf2) << "\"\n";
  }
}

static void DoFnmatchFailTest(uint8_t* buf1, uint8_t* buf2, size_t len1, size_t len2) {
  // Do string length differences.
  int c = (32 + (len1 % 96));
  switch (c) {
    case '[':
      ++c;
    case '*':
    case '?':
    case '\\':
      ++c;
  }
  memset(buf1, c, len1 - 1);
  buf1[len1-1] = '\0';
  memset(buf2, c, len2 - 1);
  buf2[len2-1] = '\0';
  ASSERT_EQ(FNM_NOMATCH, fnmatch(reinterpret_cast<char*>(buf1),
                                 reinterpret_cast<char*>(buf2), 0))
        << "\nbuf1=\"" << reinterpret_cast<char*>(buf1)
        << "\"\nbuf2=\"" << reinterpret_cast<char*>(buf2) << "\"\n";

  // Do single character differences.
  size_t len;
  if (len1 > len2) {
    len = len2;
  } else {
    len = len1;
  }
  // Need at least a two character buffer to do this test.
  if (len > 1) {
    buf1[len-1] = '\0';
    buf2[len-1] = '\0';
    int diff_c = (c + 1) % 96;
    switch (diff_c) {
      case '[':
        ++diff_c;
      case '*':
      case '?':
      case '\\':
        ++diff_c;
    }

    buf1[len-2] = diff_c;
    ASSERT_EQ(FNM_NOMATCH, fnmatch(reinterpret_cast<char*>(buf1),
                                   reinterpret_cast<char*>(buf2), 0))
        << "\nbuf1=\"" << reinterpret_cast<char*>(buf1)
        << "\"\nbuf2=\"" << reinterpret_cast<char*>(buf2) << "\"\n";

    buf1[len-2] = c;
    buf2[len-2] = diff_c;
    ASSERT_EQ(FNM_NOMATCH, fnmatch(reinterpret_cast<char*>(buf1),
                                   reinterpret_cast<char*>(buf2), 0))
        << "\nbuf1=\"" << reinterpret_cast<char*>(buf1)
        << "\"\nbuf2=\"" << reinterpret_cast<char*>(buf2) << "\"\n";
  }
}

// Use our own incrementer to cut down on the total number of calls.
static size_t LargeSetIncrement(size_t len) {
  if (len >= 4096) {
    return 4096;
  } else if (len >= 1024) {
    return 1024;
  } else if (len >= 256) {
    return 256;
  }
  return 1;
}

TEST(FNMATCH_TEST, align) {
  RunCmpBufferAlignTest(MEDIUM, DoFnmatchTest, DoFnmatchFailTest, LargeSetIncrement);
}

TEST(FNMATCH_TEST, overread) {
  RunCmpBufferOverreadTest(DoFnmatchTest, DoFnmatchFailTest);
}

TEST(FNMATCH_TEST, glob) {
  struct conditions {
    bool fail;
    const char* pattern;
    const char* string;
    int flags;
  };
  static const std::vector<conditions> tests = {
    { true,  "fn*tc",              "fnmatch",                0 },
    { false, "fn*tch",             "fnmatch",                0 },
    { true,  "fn*tc",              "fnmatch",                0 },
    { false, "fn*",                "fnmatch",                0 },
    { false, "fn????h",            "fnmatch",                0 },
    { true,  "f????h",             "fnmatch",                0 },
    { true,  "fn????H",            "fnmatch",                0 },
    { false, "fn????H",            "fnmatch",                FNM_CASEFOLD },
    { false, "fn????[Hh]",         "fnmatch",                0 },
    { false, "[^a-eg-z]n????[Hh]", "fnmatch",                0 },
    /* ToDo import shell test cases with FNM_PERIOD | FNM_FILE_NAME */
  };

  for(auto& t : tests) {
    EXPECT_EQ(t.fail ? FNM_NOMATCH : 0, fnmatch(t.pattern, t.string, t.flags))
        << "fnmatch(\"" << t.pattern << "\", \"" << t.string << "\", " << t.flags << ")";
  }
}
