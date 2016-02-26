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

#include <gtest/gtest.h>

#include <fnmatch.h>

#include <memory>

struct {
        const char *pattern;
        const char *string;
        int flags;
        int expected;
} tests[] = {
        /* begin dietlibc tests */
        { "*.c", "foo.c", 0, 0 },
        { "*.c", ".c", 0, 0 },
        { "*.a", "foo.c", 0, FNM_NOMATCH },
        { "*.c", ".foo.c", 0, 0 },
        { "*.c", ".foo.c", FNM_PERIOD, FNM_NOMATCH },
        { "*.c", "foo.c", FNM_PERIOD, 0 },
        { "a\\*.c", "a*.c", FNM_NOESCAPE, FNM_NOMATCH },
        { "a\\*.c", "ax.c", 0, FNM_NOMATCH },
        { "a[xy].c", "ax.c", 0, 0 },
        { "a[!y].c", "ax.c", 0, 0 },
        { "a[a/z]*.c", "a/x.c", FNM_PATHNAME, FNM_NOMATCH },
        { "a/*.c", "a/x.c", FNM_PATHNAME, 0 },
        { "a*.c", "a/x.c", FNM_PATHNAME, FNM_NOMATCH },
        { "*/foo", "/foo", FNM_PATHNAME, 0 },
        { "-O[01]", "-O1", 0, 0 },
//        { "[[?*\\]", "\\", 0, 0 },
//        { "[]?*\\]", "]", 0, 0 },
        /* initial right-bracket tests */
        { "[!]a-]", "b", 0, 0 },
        { "[]-_]", "^", 0, 0 }, /* range: ']', '^', '_' */
        { "[!]-_]", "X", 0, 0 },
        { "??", "-", 0, FNM_NOMATCH },
        /* begin glibc tests */
        { "*LIB*", "lib", FNM_PERIOD, FNM_NOMATCH },
        { "a[/]b", "a/b", 0, 0 },
        { "a[/]b", "a/b", FNM_PATHNAME, FNM_NOMATCH },
        { "[a-z]/[a-z]", "a/b", 0, 0 },
        { "*", "a/b", FNM_PATHNAME, FNM_NOMATCH },
        { "*[/]b", "a/b", FNM_PATHNAME, FNM_NOMATCH },
        { "*[b]", "a/b", FNM_PATHNAME, FNM_NOMATCH },
        { "[*]/b", "a/b", 0, FNM_NOMATCH },
        { "[*]/b", "*/b", 0, 0 },
        { "[?]/b", "a/b", 0, FNM_NOMATCH },
        { "[?]/b", "?/b", 0, 0 },
        { "[[a]/b", "a/b", 0, 0 },
        { "[[a]/b", "[/b", 0, 0 },
        { "\\*/b", "a/b", 0, FNM_NOMATCH },
        { "\\*/b", "*/b", 0, 0 },
        { "\\?/b", "a/b", 0, FNM_NOMATCH },
        { "\\?/b", "?/b", 0, 0 },
        { "[/b", "[/b", 0, 0 },
        { "\\[/b", "[/b", 0, 0 },
        { "??""/b", "aa/b", 0, 0 },
        { "???b", "aa/b", 0, 0 },
        { "???b", "aa/b", FNM_PATHNAME, FNM_NOMATCH },
        { "?a/b", ".a/b", FNM_PATHNAME|FNM_PERIOD, FNM_NOMATCH },
        { "a/?b", "a/.b", FNM_PATHNAME|FNM_PERIOD, FNM_NOMATCH },
        { "*a/b", ".a/b", FNM_PATHNAME|FNM_PERIOD, FNM_NOMATCH },
        { "a/*b", "a/.b", FNM_PATHNAME|FNM_PERIOD, FNM_NOMATCH },
        { "[.]a/b", ".a/b", FNM_PATHNAME|FNM_PERIOD, FNM_NOMATCH },
        { "a/[.]b", "a/.b", FNM_PATHNAME|FNM_PERIOD, FNM_NOMATCH },
        { "*/?", "a/b", FNM_PATHNAME|FNM_PERIOD, 0 },
        { "?/*", "a/b", FNM_PATHNAME|FNM_PERIOD, 0 },
        { ".*/?", ".a/b", FNM_PATHNAME|FNM_PERIOD, 0 },
        { "*/.?", "a/.b", FNM_PATHNAME|FNM_PERIOD, 0 },
        { "*/*", "a/.b", FNM_PATHNAME|FNM_PERIOD, FNM_NOMATCH },
        { "*?*/*", "a/.b", FNM_PERIOD, 0 },
        { "*[.]/b", "a./b", FNM_PATHNAME|FNM_PERIOD, 0 },
        { "*[[:alpha:]]/*[[:alnum:]]", "a/b", FNM_PATHNAME, 0 },
        /* These three tests should result in error according to SUSv3.
         * See XCU 2.13.1, XBD 9.3.5, & fnmatch() */
//        { "*[![:digit:]]*/[![:d-d]", "a/b", FNM_PATHNAME, -FNM_NOMATCH },
//        { "*[![:digit:]]*/[[:d-d]", "a/[", FNM_PATHNAME, -FNM_NOMATCH },
//        { "*[![:digit:]]*/[![:d-d]", "a/[", FNM_PATHNAME, -FNM_NOMATCH },
        { "a?b", "a.b", FNM_PATHNAME|FNM_PERIOD, 0 },
        { "a*b", "a.b", FNM_PATHNAME|FNM_PERIOD, 0 },
        { "a[.]b", "a.b", FNM_PATHNAME|FNM_PERIOD, 0 },

        /* posix 2008 is unclear about these cases */
//        { "\\", "\\", 0, 0 },
        { "\\", "", 0, FNM_NOMATCH },

        /* musl bug fixed in da0fcdb8e913ca7cdf8931328f2b37e93309b2c5 */
        { "/", "\0", FNM_PATHNAME, FNM_NOMATCH },
        /* musl bug fixed in 6ec82a3b58ee1b873ff0dfad8fa9d41c3d25dcc0 */
        { "\\/", "/", FNM_PATHNAME, 0 },

#ifdef FNM_CASEFOLD
        { "a", "A", FNM_CASEFOLD, 0 },
        { "aaAA", "AaAa", FNM_CASEFOLD, 0 },
        { "[a]", "A", FNM_CASEFOLD, 0 },
        { "[!a]", "A", FNM_CASEFOLD, FNM_NOMATCH },
        { "[!A-C]", "b", FNM_CASEFOLD, FNM_NOMATCH },
        { "[!a-c]", "B", FNM_CASEFOLD, FNM_NOMATCH },
        { "[!a-c]", "d", FNM_CASEFOLD, 0 },
#endif
};


TEST(fnmatch, smoke) {
  for (size_t i = 0; i < sizeof(tests)/sizeof(*tests); ++i) {
    EXPECT_EQ(tests[i].expected, fnmatch(tests[i].pattern, tests[i].string, tests[i].flags)) << i << " " << tests[i].pattern << " " << tests[i].string;
  }
}

TEST(fnmatch, bug_22855370) {
  std::unique_ptr<char> str(strdup("[[:lower:]"));
  fnmatch(str.get(), str.get(), 0);
}
