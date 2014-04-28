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

#include <gtest/gtest.h>

#include <stdio.h>
#include <inttypes.h>
#include <stdint.h>


// This is non-standard functions, but is a part of NDK API, Bionic should contain those.
extern "C"
{
  intmax_t  strntoimax(const char *nptr, char **endptr, int base, size_t n);
  uintmax_t strntoumax(const char *nptr, char **endptr, int base, size_t n);
}

TEST(inttypes, misc)
{
  char buf[512];

  intptr_t i = 0;
  uintptr_t u = 0;

  snprintf(buf, sizeof(buf), "%08" PRIdPTR, i);
  snprintf(buf, sizeof(buf), "%08" PRIiPTR, i);
  snprintf(buf, sizeof(buf), "%08" PRIoPTR, i);
  snprintf(buf, sizeof(buf), "%08" PRIuPTR, u);
  snprintf(buf, sizeof(buf), "%08" PRIxPTR, u);
  snprintf(buf, sizeof(buf), "%08" PRIXPTR, u);

  sscanf(buf, "%08" SCNdPTR, &i);
  sscanf(buf, "%08" SCNiPTR, &i);
  sscanf(buf, "%08" SCNoPTR, &u);
  sscanf(buf, "%08" SCNuPTR, &u);
  sscanf(buf, "%08" SCNxPTR, &u);
}

TEST(inttypes, wcstoimax) {
  ASSERT_EQ(123, wcstoimax(L"123", NULL, 10));
}

TEST(inttypes, wcstoumax) {
  ASSERT_EQ(123U, wcstoumax(L"123", NULL, 10));
}

/*
 * This test checks that
 *   strtoimax() works,
 *   works correct with DEC and HEX numbers.
 */
TEST(inttypes, strtoimax)
{
  const char *str = "-18737357foobar12";
  char *rest_str;
  intmax_t num = 0;

  { SCOPED_TRACE("strtoimax() dec");
    // Decimal
    num = strtoimax(str, &rest_str, 10);
    EXPECT_EQ(-18737357, num);
    EXPECT_EQ(0, strcmp("foobar12", rest_str));
  }
  { SCOPED_TRACE("strtoimax() hex");
    // Hexadecimal
    num = strtoimax(str, &rest_str, 16);
    EXPECT_EQ(-0x18737357f, num);
    EXPECT_EQ(0, strcmp("oobar12", rest_str));
  }
}

/*
 * This test checks that
 *   strtoumax() works,
 *   works correct with negative, DEC and HEX numbers.
 */
TEST(inttypes, strtoumax)
{
  const char *str0 = "-18737357foobar12";
  const char *str = &(str0[1]);
  char *rest_str;
  uintmax_t num = 0;

  { SCOPED_TRACE("strtoumax() negative");
    // Negative value is also valid
    num = strtoumax(str0, &rest_str, 10);
    // Reverse code is used
    EXPECT_EQ(UINTMAX_MAX - 18737357 + 1, num);
    EXPECT_EQ(0, strcmp("foobar12", rest_str));
  }
  { SCOPED_TRACE("strtoumax() dec");
    // Decimal
    num = strtoumax(str, &rest_str, 10);
    EXPECT_EQ(18737357, num);
    EXPECT_EQ(0, strcmp("foobar12", rest_str));
  }
  { SCOPED_TRACE("strtoumax() hex");
    // Hexadecimal
    num = strtoumax(str, &rest_str, 16);
    EXPECT_EQ(0x18737357f, num);
    EXPECT_EQ(0, strcmp("oobar12", rest_str));
  }
}

/*
 * This test checks that
 *   strntoimax() works,
 *   works correct with DEC and HEX numbers,
 *   stops at reaching the length limit.
 */
TEST(inttypes, strntoimax)
{
  const char *str = "-18737357faa";
  char *rest_str;
  intmax_t num = 0;

  { SCOPED_TRACE("strntoimax() dec");
    // Decimal
    num = strntoimax(str, &rest_str, 10, 10);
    EXPECT_EQ(-18737357, num);
    EXPECT_EQ(0, strcmp("faa", rest_str));
  }
  { SCOPED_TRACE("strntoimax() hex");
    // Hexadecimal
    num = strntoimax(str, &rest_str, 16, 10);
    EXPECT_EQ(-0x18737357f, num);
    EXPECT_EQ(0, strcmp("aa", rest_str));
  }
}

/*
 * This test checks that
 *   strntoumax() works,
 *   works correct with negative, DEC and HEX numbers,
 *   stops at reaching the length limit.
 */
TEST(inttypes, strntoumax)
{
  const char *str0 = "-18737357fa2bar12";
  const char *str = &(str0[1]);
  char *rest_str;
  uintmax_t num = 0;

  { SCOPED_TRACE("strtoumax() negative");
    // Negative value is also valid
    num = strntoumax(str0, &rest_str, 10, 10);
    // Reverse code is used
    EXPECT_EQ(UINTMAX_MAX - 18737357 + 1, num);
    EXPECT_EQ(0, strcmp("fa2bar12", rest_str));
  }
  { SCOPED_TRACE("strtoumax() dec");
    // Decimal
    num = strntoumax(str, &rest_str, 10, 9);
    EXPECT_EQ(18737357, num);
    EXPECT_EQ(0, strcmp("fa2bar12", rest_str));
  }
  { SCOPED_TRACE("strtoumax() hex");
    // Hexadecimal
    num = strntoumax(str, &rest_str, 16, 9);
    EXPECT_EQ(0x18737357f, num);
    EXPECT_EQ(0, strcmp("a2bar12", rest_str));
  }
}

TEST(inttypes, imaxabs)
{
  intmax_t neg = -672423489;
  intmax_t pos =  672423489;

  { SCOPED_TRACE("imaxabs() neg-to-pos");
    EXPECT_EQ(pos, imaxabs(neg));
  }
  { SCOPED_TRACE("imaxabs() pos-to-pos");
    EXPECT_EQ(pos, imaxabs(pos));
  }
  { SCOPED_TRACE("imaxabs() INTMAX_MIN");
    // Placeholder. Value is not defined by standard.
  }
}

TEST(inttypes, imaxdiv)
{
  { SCOPED_TRACE("imaxdiv() pos-by-pos");
    imaxdiv_t res = imaxdiv(10001, 2);
    EXPECT_EQ(5000, res.quot);
    EXPECT_EQ(1, res.rem);
  }
  { SCOPED_TRACE("imaxdiv() pos-by-neg");
    imaxdiv_t res = imaxdiv(10001, -2);
    EXPECT_LT(0, res.rem);
  }
  { SCOPED_TRACE("imaxdiv() maxpos-by-pos");
    imaxdiv_t res = imaxdiv(INTMAX_MAX, 3);
#if INTMAX_MAX == 9223372036854775807LL
    EXPECT_EQ(3074457345618258602LL, res.quot);
    EXPECT_EQ(1, res.rem);
#elif INTMAX_MAX == 2147483647L
    EXPECT_EQ(715827882L, res.quot);
    EXPECT_EQ(1, res.rem);
#endif
  }
  { SCOPED_TRACE("imaxdiv() neg-by-pos");
    imaxdiv_t res = imaxdiv(-2011034, 17);
    EXPECT_EQ(-118296, res.quot);
    EXPECT_EQ(-2, res.rem);  // Negative remainder expected
  }
}
