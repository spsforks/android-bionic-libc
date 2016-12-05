/*
 * Copyright (C) 2016 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <monetary.h>

#include <gtest/gtest.h>

static void Check(const char* fmt, const char* ex1, const char* ex2, const char* ex3) {
  char buf[128];

  strfmon(buf, sizeof(buf), fmt, 123.45);
  EXPECT_STREQ(ex1, buf) << fmt;
  strfmon(buf, sizeof(buf), fmt, -123.45);
  EXPECT_STREQ(ex2, buf) << fmt;
  strfmon(buf, sizeof(buf), fmt, 3456.78);
  EXPECT_STREQ(ex3, buf) << fmt;
}

TEST(monetary, strfmon) {
  ASSERT_STREQ("C.UTF-8", setlocale(LC_ALL, "C.UTF-8"));

  char buf[128];

  // %%
  strfmon(buf, sizeof(buf), "%%");
  EXPECT_STREQ("%", buf);

  Check("%n",     "123.45",     "-123.45",     "3456.78");

  Check("%11n",   "     123.45", "    -123.45", "    3456.78");

  Check("%#5n",    "   123.45",   "-  123.45",   "  3456.78");
  Check("%=*#5n",  " **123.45",   "-**123.45",   " *3456.78");
  Check("%=0#5n",  " 00123.45",   "-00123.45",   " 03456.78");
  Check("%^#5n",   "   123.45",   "-  123.45",   "  3456.78");
  Check("%^#5.0n", "   123",      "-  123",      "  3457");
  Check("%^#5.4n", "   123.4500", "-  123.4500", "  3456.7800");
  Check("%(#5n",   "   123.45",   "(  123.45)",  "  3456.78");
  Check("%!(#5n",  "   123.45",   "(  123.45)",   "  3456.78");

  Check("%-14#5.4n", "   123.4500   ", "-  123.4500   ", "  3456.7800   ");
  Check("%14#5.4n",  "      123.4500", "   -  123.4500", "     3456.7800");
}
