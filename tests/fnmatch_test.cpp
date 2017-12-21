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
#include <locale.h>

#include <gtest/gtest.h>
#include <stdint.h>

#include <vector>

#include "buffer_tests.h"

#if defined(NOFORTIFY)
#define FNMATCH_TEST fnmatch_nofortify
#else
#define FNMATCH_TEST fnmatch
#endif

namespace {

char skipGlobCharacter(char c) {
  if (c >= 127) return ' ';
  if (c == '[') ++c;
  if ((c == '*') || (c == '?') || (c == '\\')) ++c;
  return c;
}

void DoFnmatchTest(uint8_t* buf1, uint8_t* buf2, size_t len) {
  if (len < 1) return;

  char c = skipGlobCharacter(' ' + (len % 96));
  memset(buf1, c, len - 1);
  buf1[len-1] = '\0';
  memset(buf2, c, len - 1);
  buf2[len-1] = '\0';
  ASSERT_EQ(0, fnmatch(reinterpret_cast<char*>(buf1),
                       reinterpret_cast<char*>(buf2), 0))
      << "\n\"" << c << "...\"[" << len << "] equal!!!\n";
}

void DoFnmatchFailTest(uint8_t* buf1, uint8_t* buf2, size_t len1, size_t len2) {
  // Do string length differences.
  char c = skipGlobCharacter(' ' + (len1 % 96));
  memset(buf1, c, len1 - 1);
  buf1[len1-1] = '\0';
  memset(buf2, c, len2 - 1);
  buf2[len2-1] = '\0';
  ASSERT_EQ(FNM_NOMATCH, fnmatch(reinterpret_cast<char*>(buf1),
                                 reinterpret_cast<char*>(buf2), 0))
      << "\n\"" << c << "...\"[" << len1
      << "] != \"" << c << "...\"[" << len2 << "]\n";

  // Do single character differences.
  //  Only do half the cases where len1 < len2 to save testing time.
  if (len1 > len2) return;
  //  Need at least a two character buffer to do this test.
  if (len1 <= 1) return;

  buf1[len1-1] = '\0';
  buf2[len1-1] = '\0';
  char diff_c = skipGlobCharacter(c + 1);

  buf1[len1-2] = diff_c;
  // failure report is not _accurate_ for len1 == 2, KISS sez leave it alone
  ASSERT_EQ(FNM_NOMATCH, fnmatch(reinterpret_cast<char*>(buf1),
                                 reinterpret_cast<char*>(buf2), 0))
      << "\n\"" << c << "..." << diff_c << "\" != \"" << c << "..." << c << "\"[" << len1 << "]\n";

  buf1[len1-2] = c;
  buf2[len1-2] = diff_c;
  ASSERT_EQ(FNM_NOMATCH, fnmatch(reinterpret_cast<char*>(buf1),
                                 reinterpret_cast<char*>(buf2), 0))
      << "\n\"" << c << "..." << c << "\" != \"" << c << "..." << diff_c << "\"[" << len1 << "]\n";
}

} // anonymous namespace

TEST(FNMATCH_TEST, overread) {
  RunCmpBufferOverreadTest(DoFnmatchTest, DoFnmatchFailTest);
}

TEST(FNMATCH_TEST, glob) {
  struct conditions {
    bool success;
    const char* pattern;
    const char* string;
    int flags;
  };
  static const std::vector<conditions> tests = {
    { false, "fn*tc",              "fnmatch",                0 },
    { true,  "fn*tch",             "fnmatch",                0 },
    { false, "fn*tc",              "fnmatch",                0 },
    { true,  "fn*",                "fnmatch",                0 },
    { true,  "fn????h",            "fnmatch",                0 },
    { false, "f????h",             "fnmatch",                0 },
    { false, "fn????H",            "fnmatch",                0 },
    { true,  "fn????H",            "fnmatch",                FNM_CASEFOLD },
    { true,  "fn????[Hh]",         "fnmatch",                0 },
    { true,  "[^a-eg-z]n????[Hh]", "fnmatch",                0 },
    // various common posix shell test cases
# define FNM_SHELL (FNM_FILE_NAME | FNM_PERIOD)
    { false, "a[b]c",              "a",                      FNM_SHELL },
    { false, "a[b]c",              "b",                      FNM_SHELL },
    { false, "a[b]c",              "c",                      FNM_SHELL },
    { false, "a[b]c",              "d",                      FNM_SHELL },
    { true,  "a[b]c",              "abc",                    FNM_SHELL },
    { false, "a[b]c",              "abd",                    FNM_SHELL },
    { false, "a[b]c",              "bb",                     FNM_SHELL },
    { false, "a[b]c",              "cba",                    FNM_SHELL },
    { false, "a[b]c",              "fnmatch",                FNM_SHELL },
    { false, "a[b]c",              "Abc",                    FNM_SHELL },
    { true,  "a[b]c",              "Abc",                    FNM_SHELL |
                                                             FNM_CASEFOLD },
    { false, "a[\\b]c",            "a",                      FNM_SHELL },
    { false, "a[\\b]c",            "b",                      FNM_SHELL },
    { false, "a[\\b]c",            "c",                      FNM_SHELL },
    { false, "a[\\b]c",            "d",                      FNM_SHELL },
    { true,  "a[\\b]c",            "abc",                    FNM_SHELL },
    { false, "a[\\b]c",            "abd",                    FNM_SHELL },
    { false, "a[\\b]c",            "bb",                     FNM_SHELL },
    { false, "a[\\b]c",            "cba",                    FNM_SHELL },
    { false, "a[\\b]c",            "fnmatch",                FNM_SHELL },
    { false, "a[\\b]c",            "Abc",                    FNM_SHELL },
    { true,  "a[\\b]c",            "Abc",                    FNM_SHELL |
                                                             FNM_CASEFOLD },
    { false, "a?c",                "a",                      FNM_SHELL },
    { false, "a?c",                "b",                      FNM_SHELL },
    { false, "a?c",                "c",                      FNM_SHELL },
    { false, "a?c",                "d",                      FNM_SHELL },
    { true,  "a?c",                "abc",                    FNM_SHELL },
    { false, "a?c",                "abd",                    FNM_SHELL },
    { false, "a?c",                "bb",                     FNM_SHELL },
    { false, "a?c",                "cba",                    FNM_SHELL },
    { false, "a?c",                "fnmatch",                FNM_SHELL },
    { false, "a?c",                "Abc",                    FNM_SHELL },
    { true,  "a?c",                "Abc",                    FNM_SHELL |
                                                             FNM_CASEFOLD },
    { true,  "[-abc]",             "-",                      FNM_SHELL },
    { true,  "[abc-]",             "-",                      FNM_SHELL },
    { false, "[a-c]",              "-",                      FNM_SHELL },
    { true,  "*/b*/c.*",           "ax/b1/c.2",              FNM_SHELL },
    { false, "*/b*/?.*",           "ax/b1/..2",              FNM_SHELL },
    { true,  "*/b*/?.*",           "ax/b1/..2",              0 },
    { true,  "a*",                 "a",                      FNM_SHELL },
    { true,  "a*",                 "abc",                    FNM_SHELL },
    { true,  "a*c",                "abc",                    FNM_SHELL },
    { true,  "a**c",               "abc",                    FNM_SHELL },
    { true,  "a***c",              "abc",                    FNM_SHELL },
    { true,  "***??c",             "abc",                    FNM_SHELL },
    { false, "***???c",            "abc",                    FNM_SHELL },
    { false, "***????c",           "abc",                    FNM_SHELL },
    { true,  "?***?***c",          "abc",                    FNM_SHELL },
    { true,  "a*cd**?**??k",       "abcdecdhjk",             FNM_SHELL },
    { true,  "a**?**cd**?**??k",   "abcdecdhjk",             FNM_SHELL },
    { true,  "a**?**cd**?**??k**", "abcdecdhjk",             FNM_SHELL },
    { true,  "\\\\",               "\\",                     FNM_SHELL },
    { true,  "[\\\\]",             "\\",                     FNM_SHELL },
    { true,  "[",                  "[",                      FNM_SHELL },
    { false, "[",                  "]",                      FNM_SHELL },
    { true,  "[*",                 "[",                      FNM_SHELL },
    { true,  "[*",                 "[a",                     FNM_SHELL },
    { false, "[*",                 "]a",                     FNM_SHELL },
    { false, "[",                  "[a",                     FNM_SHELL },
    { false, "[",                  "]",                      FNM_SHELL },
    { true,  "[[]",                "[",                      FNM_SHELL },
    { false, "[[]",                "]",                      FNM_SHELL },
    { false, "[]]",                "[",                      FNM_SHELL },
    { true,  "[]]",                "]",                      FNM_SHELL },
    { true,  "[a-\\z]",            "c",                      FNM_SHELL },
    { false, "[a-\\z]",            "C",                      FNM_SHELL },
    { true,  "[:alpha:]",          "c",                      FNM_SHELL },
    { true,  "[:alpha:]",          "C",                      FNM_SHELL },
    { true,  "[:lower:]",          "c",                      FNM_SHELL },
    { false, "[:lower:]",          "C",                      FNM_SHELL },
    { true,  "[:upper:]",          "C",                      FNM_SHELL },
    { false, "[:alpha:]",          "1",                      FNM_SHELL },
    { true,  "[:digit:]",          "1",                      FNM_SHELL },
    { true,  "[:blank:]",          " ",                      FNM_SHELL },
    { true,  "[:blank:]",          "\t",                     FNM_SHELL },
    { false, "[:blank:]",          "\n",                     FNM_SHELL },
    { true,  "[:space:]",          "\n",                     FNM_SHELL },
    { false, "[:blank:]",          "a",                      FNM_SHELL },
    { false, "[]",                 "a",                      FNM_SHELL },
    { false, "??**?**?",           "abc",                    FNM_SHELL },
    { false, "??**?**c",           "abc",                    FNM_SHELL },
    { false, "?**c**?*",           "abc",                    FNM_SHELL },
    { false, "*c*?*",              "abc",                    FNM_SHELL },
    { false, "a*c*?*",             "abc",                    FNM_SHELL },
    { false, "a**???*",            "abc",                    FNM_SHELL },
    { false, "a*",                 "b",                      FNM_SHELL },
    { false, "a*",                 "bca",                    FNM_SHELL },
    { true,  "\\a*",               "a",                      FNM_SHELL },
    { true,  "\\a*",               "abc",                    FNM_SHELL },
    { false, "\\a*",               "b",                      FNM_SHELL },
    { false, "\\a*",               "bca",                    FNM_SHELL },
    { true,  "a/b[12]",            "a/b1",                   FNM_SHELL },
    { true,  "a/b[12]",            "a/b2",                   FNM_SHELL },
    { false, "a/b[12]",            "a/b3",                   FNM_SHELL },
    { false, "a/*2",               "a/b1",                   FNM_SHELL },
    { true,  "a/*2",               "a/b2",                   FNM_SHELL },
    { true,  "\\*",                "*",                      FNM_SHELL },
    { false, "\\*",                "a",                      FNM_SHELL },
    { true,  "a\\*",               "a*",                     FNM_SHELL },
    { false, "a\\*",               "ab",                     FNM_SHELL },
    { true,  "[a-c]b*",            "abc",                    FNM_SHELL },
    { true,  "[a-c]b*",            "bb",                     FNM_SHELL },
    { false, "[a-c]b*",            "bcd",                    FNM_SHELL },
    { false, "[^a-c]*",            "abc",                    FNM_SHELL },
    { true,  "[^a-c]*",            "d",                      FNM_SHELL },
    { false, "[^a-c]*",            ".a",                     FNM_SHELL },
    { true,  "[^a-c]*",            ".a",                     0 },
    { true,  "a\\*?/*",            "a*b/d",                  FNM_SHELL },
    { true,  "a/*/*/d",            "a/b/c/d",                FNM_SHELL },
    // There is no globstar support in fnmatch
    { false, "a/**/d",             "a/b/c/d",                FNM_SHELL },
    { true,  "a/**/d",             "a/b/c/d",                FNM_PERIOD },
    { false, "a/*/d",              "a/b/c/d",                FNM_SHELL },
    { true,  "a/*/d",              "a/b/c/d",                FNM_PERIOD },
    // FNM_EXTMATCH patterns, most expected to fail w/o the flag
    { true,  "?(fnmatch)",         "X(fnmatch)",             0 },
    { false, "?(fnmatch)",         "fnmatch",                0 },
    { false, "?(fn*tc|help)",      "fnmatch",                0 },
    { false, "?(fn*tc|fnmatch)",   "fnmatch",                0 },
    { false, "?(fn*tch|help)",     "fnmatch",                0 },
    { false, "?(fn*tch|help)",     "fnmatchfnmatch",         0 },
    { false, "*(fnmatch)",         "",                       0 },
    { false, "*(fn*tc|help)",      "fnmatch",                0 },
    { false, "*(fn*tc|fnmatch)",   "fnmatchfnmatch",         0 },
    { false, "+(fn*tc|help)",      "fnmatch",                0 },
    { false, "+(fn*tc|fnmatch)",   "fnmatch",                0 },
    { false, "+(fn*tc|fnmatch)",   "",                       0 },
    { false, "@(fn*tc|help)",      "fnmatch",                0 },
    { false, "@(fn*tc|fnmatch)",   "fnmatch",                0 },
    { false, "@(fn*tc|fnmatch)",   "",                       0 },
    { false, "@(fn*tc|fnmatch)",   "fnmatchfnmatch",         0 },
    { false, "!(help)fnmatch",     "fnmatch",                0 },
    { false, "!(fn*tc|fnmatc)h",   "fnmatch",                0 },
    { false, "BM_time_@(clock_getres|clock_gettime|gettimeofday|time)*",
                                   "BM_time_unknown",        0 },
    { false, "BM_time_@(clock_getres|clock_gettime|gettimeofday|time)*",
                                   "BM_time_time_syscall",   0 },
    { false, "BM_time_@(clock_@(getres|gettime)|gettimeofday|time)*",
                                   "BM_time_clock_getres",   0 },
    { false, "BM_time_@(clock_@(getres|gettime)|gettimeofday|time)*",
                                   "BM_time_clock_gettime",  0 },
    { false, "BM_time_@(clock_@(getres|gettime)|gettimeofday|time)*",
                                   "BM_time_clock_gettimeofday",
                                                             0 },
    { false, "BM_time_@(clock_@(getres|gettime)|gettimeofday|time)*",
                                   "BM_time_time_syscall",   0 },
    { false, "BM_time_@(clock_@(get*\\res|gettime)|gettimeofday|ti*\\me)*",
                                   "BM_time_clock_getres",   0 },
    { false, "BM_time_@(clock_@(get*\\res|gettime)|gettimeofday|ti*\\me)*",
                                   "BM_time_clock_getres",   FNM_NOESCAPE },
    { false, "BM_time_@(clock_@(get*\\res|gettime)|gettimeofday|ti*\\me)*",
                                   "BM_time_clock_gettime",  0 },
    { false, "BM_time_@(clock_@(get*\\res|gettime)|gettimeofday|ti*\\me)*",
                                   "BM_time_time_syscall",   0 },
    { false, "a+(b)c",             "abc",                    0 },
    { false, "a+(b+(c)d)f",        "abcdf",                  0 },
    { false, "a+(b\\+(c)d)f",      "abcdf",                  0 },
    { false, "a+(b\\+(c)d)f",      "ab+(c)df",               0 },
    { false, "a+(b\\+(c)d)f",      "ab+(cd)f",               0 },
    { false, "a+(b\\+(c\\)d)f",    "ab+(c)df",               0 },
    { false, "a+(b\\+(c)d)f",      "abcdf",                  FNM_NOESCAPE },
    { false, "a+(b\\+(c)d)f",      "ab\\cdf",                FNM_NOESCAPE },
    { false, "a+(b+\\(c)d)f",      "abcdf",                  0 },
    { false, "a+(b+\\(c)d)f",      "ab+(c)df",               0 },
    { false, "a+(b+\\(c\\)d)f",    "ab+(c)df",               0 },
    { false, "a+(b+\\(c\\)d)f",    "abcdf",                  FNM_NOESCAPE },
    { false, "a+(b+\\(c\\)d)f",    "ab+\\(c\\d)f",           FNM_NOESCAPE },
#ifdef FNM_EXTMATCH // And now the expected results with the flag
    { false, "?(fnmatch)",         "X(fnmatch)",             FNM_EXTMATCH },
    { true,  "?(fnmatch)",         "fnmatch",                FNM_EXTMATCH },
    { false, "?(fn*tc|help)",      "fnmatch",                FNM_EXTMATCH },
    { true,  "?(fn*tc|fnmatch)",   "fnmatch",                FNM_EXTMATCH },
    { true,  "?(fn*tch|help)",     "fnmatch",                FNM_EXTMATCH },
    { false, "?(fn*tch|help)",     "fnmatchfnmatch",         FNM_EXTMATCH },
    { true,  "*(fnmatch)",         "",                       FNM_EXTMATCH },
    { false, "*(fn*tc|help)",      "fnmatch",                FNM_EXTMATCH },
    { true,  "*(fn*tc|fnmatch)",   "fnmatchfnmatch",         FNM_EXTMATCH },
    { false, "+(fn*tc|help)",      "fnmatch",                FNM_EXTMATCH },
    { true,  "+(fn*tc|fnmatch)",   "fnmatch",                FNM_EXTMATCH },
    { false, "+(fn*tc|fnmatch)",   "",                       FNM_EXTMATCH },
    { false, "@(fn*tc|help)",      "fnmatch",                FNM_EXTMATCH },
    { true,  "@(fn*tc|fnmatch)",   "fnmatch",                FNM_EXTMATCH },
    { false, "@(fn*tc|fnmatch)",   "",                       FNM_EXTMATCH },
    { false, "@(fn*tc|fnmatch)",   "fnmatchfnmatch",         FNM_EXTMATCH },
    { true,  "!(help)fnmatch",     "fnmatch",                FNM_EXTMATCH },
    { false, "!(fn*tc|fnmatc)h",   "fnmatch",                FNM_EXTMATCH },
    { false, "BM_time_@(clock_getres|clock_gettime|gettimeofday|time)*",
                                   "BM_time_unknown",        FNM_EXTMATCH },
    { true,  "BM_time_@(clock_getres|clock_gettime|gettimeofday|time)*",
                                   "BM_time_time_syscall",   FNM_EXTMATCH },
    { true,  "BM_time_@(clock_@(getres|gettime)|gettimeofday|time)*",
                                   "BM_time_clock_getres",   FNM_EXTMATCH },
    { true,  "BM_time_@(clock_@(getres|gettime)|gettimeofday|time)*",
                                   "BM_time_clock_gettime",  FNM_EXTMATCH },
    { true,  "BM_time_@(clock_@(getres|gettime)|gettimeofday|time)*",
                                   "BM_time_clock_gettimeofday",
                                                             FNM_EXTMATCH },
    { true,  "BM_time_@(clock_@(getres|gettime)|gettimeofday|time)*",
                                   "BM_time_time_syscall",   FNM_EXTMATCH },
    { true,  "BM_time_@(clock_@(get*\\res|gettime)|gettimeofday|ti*\\me)*",
                                   "BM_time_clock_getres",   FNM_EXTMATCH },
    { false, "BM_time_@(clock_@(get*\\res|gettime)|gettimeofday|ti*\\me)*",
                                   "BM_time_clock_getres",   FNM_EXTMATCH |
                                                             FNM_NOESCAPE },
    { true,  "BM_time_@(clock_@(get*\\res|gettime)|gettimeofday|ti*\\me)*",
                                   "BM_time_clock_gettime",  FNM_EXTMATCH },
    { true,  "BM_time_@(clock_@(get*\\res|gettime)|gettimeofday|ti*\\me)*",
                                   "BM_time_time_syscall",   FNM_EXTMATCH },
    { true,  "a+(b)c",             "abc",                    FNM_EXTMATCH },
    { true,  "a+(b+(c)d)f",        "abcdf",                  FNM_EXTMATCH },
    { false, "a+(b\\+(c)d)f",      "abcdf",                  FNM_EXTMATCH },
    { false, "a+(b\\+(c)d)f",      "ab+(c)df",               FNM_EXTMATCH },
    { true,  "a+(b\\+(c)d)f",      "ab+(cd)f",               FNM_EXTMATCH },
    { true,  "a+(b\\+(c\\)d)f",    "ab+(c)df",               FNM_EXTMATCH },
    { false, "a+(b\\+(c)d)f",      "abcdf",                  FNM_EXTMATCH |
                                                             FNM_NOESCAPE },
    { true,  "a+(b\\+(c)d)f",      "ab\\cdf",                FNM_EXTMATCH |
                                                             FNM_NOESCAPE },
    { false, "a+(b+\\(c)d)f",      "abcdf",                  FNM_EXTMATCH },
    { false, "a+(b+\\(c)d)f",      "ab+(c)df",               FNM_EXTMATCH },
    { true,  "a+(b+\\(c\\)d)f",    "ab+(c)df",               FNM_EXTMATCH },
    { false, "a+(b+\\(c\\)d)f",    "abcdf",                  FNM_EXTMATCH |
                                                             FNM_NOESCAPE },
    { true,  "a+(b+\\(c\\)d)f",    "ab+\\(c\\d)f",           FNM_EXTMATCH |
                                                             FNM_NOESCAPE },
#endif
  };

  setlocale(LC_ALL, "C");
  for(auto& t : tests) {
    EXPECT_EQ(t.success ? 0 : FNM_NOMATCH, fnmatch(t.pattern, t.string, t.flags))
        << "fnmatch(\"" << t.pattern << "\", \"" << t.string << "\", " << t.flags << ")";
  }
}
