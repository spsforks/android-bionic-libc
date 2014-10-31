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

/* Basic test of setjmp() functionality */
#include <gtest/gtest.h>

#include <setjmp.h>
#include <stdio.h>
#include <stdlib.h>
#define INT_VALUE1 3
#define INT_VALUE2 5
#define FLOAT_VALUE1 (1.234)
#define FLOAT_VALUE2 (9.876)

TEST(setjmp, setjmp_int) {
  /* test that integer registers and static integer */
  jmp_buf jumper;
  static int si = INT_VALUE1;
#if defined(__BIONIC__)
  register int ri = INT_VALUE1;
#else
  int ri = INT_VALUE1; //same as declared with register same as declared with register
#endif
  int dummy_int;

  if (setjmp(jumper) == 0) {
    si = INT_VALUE2;
    ri = INT_VALUE2;
    longjmp(jumper, 1);
    ASSERT_TRUE(false);//should not be here
  } else {
    ASSERT_EQ(INT_VALUE2, si) << "setjmp() is broken for static variable !\n";
    dummy_int = ri;
    ASSERT_EQ(INT_VALUE1, dummy_int) << "setjmp() is broken for integer registers !\n";
    return;
  }
  ASSERT_TRUE(false);//should not be here
}

TEST(setjmp, setjmp_float) {
  /* test that float registers and static float */
  jmp_buf  jumper;
  static double sd = FLOAT_VALUE1;
#if defined(__BIONIC__)
  register double rd = FLOAT_VALUE1;
#else
  double rd = FLOAT_VALUE1; //same as declared with register when compiled with -O option
#endif
  double  dummy_double;

  if (setjmp(jumper) == 0) {
    sd = FLOAT_VALUE2;
    rd = FLOAT_VALUE2;
    longjmp(jumper, 1);
    ASSERT_TRUE(false);//should not be here
  } else {
    ASSERT_EQ(FLOAT_VALUE2, sd) << "setjmp() is broken for static float variables !\n";
    dummy_double = rd;
    ASSERT_EQ(FLOAT_VALUE1, dummy_double) << "setjmp() is broken for float registers !\n";
    return;
  }
  ASSERT_TRUE(false);//should not be here
}
