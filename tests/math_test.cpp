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

#define _DECLARE_C99_LDBL_MATH 1

#include <gtest/gtest.h>

#include <fenv.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>

float float_subnormal() {
  union {
    float f;
    uint32_t i;
  } u;
  u.i = 0x007fffff;
  return u.f;
}

double double_subnormal() {
  union {
    double d;
    uint64_t i;
  } u;
  u.i = 0x000fffffffffffffL;
  return u.d;
}

TEST(math, fpclassify) {
  ASSERT_EQ(FP_INFINITE, fpclassify(INFINITY));
  ASSERT_EQ(FP_INFINITE, fpclassify(HUGE_VALF));
  ASSERT_EQ(FP_INFINITE, fpclassify(HUGE_VAL));

  ASSERT_EQ(FP_NAN, fpclassify(nanf("")));
  ASSERT_EQ(FP_NAN, fpclassify(nan("")));

  ASSERT_EQ(FP_NORMAL, fpclassify(1.0f));
  ASSERT_EQ(FP_NORMAL, fpclassify(1.0));

  ASSERT_EQ(FP_SUBNORMAL, fpclassify(float_subnormal()));
  ASSERT_EQ(FP_SUBNORMAL, fpclassify(double_subnormal()));

  ASSERT_EQ(FP_ZERO, fpclassify(0.0f));
  ASSERT_EQ(FP_ZERO, fpclassify(0.0));
}

/* TODO: stlport breaks the isfinite macro
TEST(math, isfinite) {
  ASSERT_TRUE(isfinite(123.0f));
  ASSERT_TRUE(isfinite(123.0));
  ASSERT_FALSE(isfinite(HUGE_VALF));
  ASSERT_FALSE(isfinite(HUGE_VAL));
}
*/

TEST(math, isinf) {
  ASSERT_FALSE(isinf(123.0f));
  ASSERT_FALSE(isinf(123.0));
  ASSERT_TRUE(isinf(HUGE_VALF));
  ASSERT_TRUE(isinf(HUGE_VAL));
}

TEST(math, isnan) {
  ASSERT_FALSE(isnan(123.0f));
  ASSERT_FALSE(isnan(123.0));
  ASSERT_TRUE(isnan(nanf("")));
  ASSERT_TRUE(isnan(nan("")));
}

TEST(math, isnormal) {
  ASSERT_TRUE(isnormal(123.0f));
  ASSERT_TRUE(isnormal(123.0));
  ASSERT_FALSE(isnormal(float_subnormal()));
  ASSERT_FALSE(isnormal(double_subnormal()));
}

// TODO: isgreater, isgreaterequals, isless, islessequal, islessgreater, isunordered

/* TODO: stlport breaks the signbit macro
TEST(math, signbit) {
  ASSERT_EQ(0, signbit(0.0f));
  ASSERT_EQ(0, signbit(0.0));

  ASSERT_EQ(0, signbit(1.0f));
  ASSERT_EQ(0, signbit(1.0));

  ASSERT_NE(0, signbit(-1.0f));
  ASSERT_NE(0, signbit(-1.0));
}
*/

#if defined(__BIONIC__)
TEST(math, __fpclassifyd) {
  ASSERT_EQ(FP_INFINITE, __fpclassifyd(HUGE_VAL));
  ASSERT_EQ(FP_NAN, __fpclassifyd(nan("")));
  ASSERT_EQ(FP_NORMAL, __fpclassifyd(1.0));
  ASSERT_EQ(FP_SUBNORMAL, __fpclassifyd(double_subnormal()));
  ASSERT_EQ(FP_ZERO, __fpclassifyd(0.0));
}
#endif

#if defined(__BIONIC__)
TEST(math, __fpclassifyf) {
  ASSERT_EQ(FP_INFINITE, __fpclassifyf(HUGE_VALF));
  ASSERT_EQ(FP_NAN, __fpclassifyf(nanf("")));
  ASSERT_EQ(FP_NORMAL, __fpclassifyf(1.0f));
  ASSERT_EQ(FP_SUBNORMAL, __fpclassifyf(float_subnormal()));
  ASSERT_EQ(FP_ZERO, __fpclassifyf(0.0f));
}
#endif

#if defined(__BIONIC__)
TEST(math, __fpclassifyl) {
  if (sizeof(long double) != sizeof(double)) {
    ASSERT_EQ(FP_INFINITE, __fpclassifyl(HUGE_VALL));
    ASSERT_EQ(FP_NAN, __fpclassifyl(nanl("")));
    ASSERT_EQ(FP_NORMAL, __fpclassifyl(1.0));
    ASSERT_EQ(FP_SUBNORMAL, __fpclassifyl(double_subnormal()));
    ASSERT_EQ(FP_ZERO, __fpclassifyl(0.0));
  } else {
    // Just check we can link.
    ASSERT_EQ(FP_ZERO, __fpclassifyl(0.0));
  }
}
#endif

TEST(math, finitef) {
  ASSERT_TRUE(finitef(123.0f));
  ASSERT_FALSE(finitef(HUGE_VALF));
}

#if defined(__BIONIC__)
TEST(math, __isfinite) {
  ASSERT_TRUE(__isfinite(123.0));
  ASSERT_FALSE(__isfinite(HUGE_VAL));
}
#endif

#if defined(__BIONIC__)
TEST(math, __isfinitef) {
  ASSERT_TRUE(__isfinitef(123.0f));
  ASSERT_FALSE(__isfinitef(HUGE_VALF));
}
#endif

#if defined(__BIONIC__)
TEST(math, __isfinitel) {
  ASSERT_TRUE(__isfinitel(123.0f));
  if (sizeof(long double) != sizeof(double)) {
    ASSERT_FALSE(__isfinitel(HUGE_VALL));
  }
}
#endif

TEST(math, finite) {
  ASSERT_TRUE(finite(123.0));
  ASSERT_FALSE(finite(HUGE_VAL));
}

TEST(math, __isinff) {
  ASSERT_FALSE(__isinff(123.0f));
  ASSERT_TRUE(__isinff(HUGE_VALF));
}

TEST(math, __isinfl) {
  ASSERT_FALSE(__isinfl(123.0));
  if (sizeof(long double) != sizeof(double)) {
    ASSERT_TRUE(__isinfl(HUGE_VALL));
  }
}

TEST(math, __isnanf) {
  ASSERT_FALSE(__isnanf(123.0f));
  ASSERT_TRUE(__isnanf(nanf("")));
}

TEST(math, __isnanl) {
  ASSERT_FALSE(__isnanl(123.0));
  if (sizeof(long double) != sizeof(double)) {
    ASSERT_TRUE(__isnanl(nanl("")));
  }
}

TEST(math, isnanf) {
  ASSERT_FALSE(isnanf(123.0f));
  ASSERT_TRUE(isnanf(nanf("")));
}

#if defined(__BIONIC__)
TEST(math, __isnormal) {
  ASSERT_TRUE(__isnormal(123.0));
  ASSERT_FALSE(__isnormal(double_subnormal()));
}
#endif

#if defined(__BIONIC__)
TEST(math, __isnormalf) {
  ASSERT_TRUE(__isnormalf(123.0f));
  ASSERT_FALSE(__isnormalf(float_subnormal()));
}
#endif

#if defined(__BIONIC__)
TEST(math, __isnormall) {
  ASSERT_TRUE(__isnormall(123.0));
  ASSERT_FALSE(__isnormall(double_subnormal()));
}
#endif

TEST(math, __signbit) {
  ASSERT_EQ(0, __signbit(0.0));
  ASSERT_EQ(0, __signbit(1.0));
  ASSERT_NE(0, __signbit(-1.0));
}

TEST(math, __signbitf) {
  ASSERT_EQ(0, __signbitf(0.0f));
  ASSERT_EQ(0, __signbitf(1.0f));
  ASSERT_NE(0, __signbitf(-1.0f));
}

TEST(math, __signbitl) {
  ASSERT_EQ(0, __signbitl(0.0));
  ASSERT_EQ(0, __signbitl(1.0));
  ASSERT_NE(0, __signbitl(-1.0));
}

TEST(math, inverse_trigonometry) {
  ASSERT_FLOAT_EQ(M_PI/2.0, acos(0.0));
  ASSERT_FLOAT_EQ(static_cast<float>(M_PI)/2.0f, acosf(0.0f));
  // TODO: acosl
  ASSERT_FLOAT_EQ(0.0, asin(0.0));
  ASSERT_FLOAT_EQ(0.0f, asinf(0.0f));
  // TODO: asinl
  ASSERT_FLOAT_EQ(0.0, atan(0.0));
  ASSERT_FLOAT_EQ(0.0f, atanf(0.0f));
  // TODO: atanl
  ASSERT_FLOAT_EQ(0.0, atan2(0.0, 0.0));
  ASSERT_FLOAT_EQ(0.0f, atan2f(0.0f, 0.0f));
  // TODO: atan2l
}

TEST(math, trigonometry) {
  ASSERT_FLOAT_EQ(1.0, cos(0.0));
  ASSERT_FLOAT_EQ(1.0f, cosf(0.0f));
  // TODO: cosl
  ASSERT_FLOAT_EQ(0.0, sin(0.0));
  ASSERT_FLOAT_EQ(0.0f, sinf(0.0f));
  // TODO: sinl
  ASSERT_FLOAT_EQ(0.0, tan(0.0));
  ASSERT_FLOAT_EQ(0.0f, tanf(0.0f));
  // TODO: tanl
}

TEST(math, inverse_hyperbolic_trigonometry) {
  ASSERT_FLOAT_EQ(0.0, acosh(1.0));
  ASSERT_FLOAT_EQ(0.0f, acoshf(1.0f));
  // TODO: acoshl
  ASSERT_FLOAT_EQ(0.0, asinh(0.0));
  ASSERT_FLOAT_EQ(0.0f, asinhf(0.0f));
  // TODO: asinhl
  ASSERT_FLOAT_EQ(0.0, atanh(0.0));
  ASSERT_FLOAT_EQ(0.0f, atanhf(0.0f));
  // TODO: atanhl
}

TEST(math, hyperbolic_trigonometry) {
  ASSERT_FLOAT_EQ(1.0, cosh(0.0));
  ASSERT_FLOAT_EQ(1.0f, coshf(0.0f));
  // TODO: coshl
  ASSERT_FLOAT_EQ(0.0, sinh(0.0));
  ASSERT_FLOAT_EQ(0.0f, sinhf(0.0f));
  // TODO: sinhl
  ASSERT_FLOAT_EQ(0.0, tanh(0.0));
  ASSERT_FLOAT_EQ(0.0f, tanhf(0.0f));
  // TODO: tanhl
}

TEST(math, log) {
  ASSERT_FLOAT_EQ(1.0, log(M_E));
  ASSERT_FLOAT_EQ(1.0f, logf(static_cast<float>(M_E)));
  ASSERT_FLOAT_EQ(1.0, logl(M_E));
}

TEST(math, log2) {
  ASSERT_FLOAT_EQ(12.0, log2(4096.0));
  ASSERT_FLOAT_EQ(12.0f, log2f(4096.0f));
  ASSERT_FLOAT_EQ(12.0, log2l(4096.0));
}

TEST(math, log10) {
  ASSERT_FLOAT_EQ(3.0, log10(1000.0));
  ASSERT_FLOAT_EQ(3.0f, log10f(1000.0f));
  ASSERT_FLOAT_EQ(3.0, log10l(1000.0));
}

TEST(math, cbrt) {
  ASSERT_FLOAT_EQ(3.0, cbrt(27.0));
  ASSERT_FLOAT_EQ(3.0f, cbrtf(27.0f));
  ASSERT_FLOAT_EQ(3.0, cbrtl(27.0));
}

TEST(math, sqrt) {
  ASSERT_FLOAT_EQ(2.0, sqrt(4.0));
  ASSERT_FLOAT_EQ(2.0f, sqrtf(4.0f));
  ASSERT_FLOAT_EQ(2.0, sqrtl(4.0));
}

TEST(math, exp) {
  ASSERT_FLOAT_EQ(1.0, exp(0.0));
  ASSERT_FLOAT_EQ(M_E, exp(1.0));

  ASSERT_FLOAT_EQ(1.0f, expf(0.0f));
  ASSERT_FLOAT_EQ(static_cast<float>(M_E), expf(1.0f));

  ASSERT_FLOAT_EQ(1.0, expl(0.0));
  ASSERT_FLOAT_EQ(M_E, expl(1.0));
}

TEST(math, exp2) {
  ASSERT_FLOAT_EQ(8.0, exp2(3.0));
  ASSERT_FLOAT_EQ(8.0f, exp2f(3.0f));
  ASSERT_FLOAT_EQ(8.0, exp2l(3.0));
}

TEST(math, expm1) {
  ASSERT_FLOAT_EQ(M_E - 1.0, expm1(1.0));
  ASSERT_FLOAT_EQ(static_cast<float>(M_E) - 1.0f, expm1f(1.0f));
  ASSERT_FLOAT_EQ(M_E - 1.0, expm1l(1.0));
}

TEST(math, pow) {
  ASSERT_FLOAT_EQ(8.0, pow(2.0, 3.0));
  ASSERT_FLOAT_EQ(8.0f, powf(2.0f, 3.0f));
  ASSERT_FLOAT_EQ(8.0, powl(2.0, 3.0));
}

TEST(math, ceil) {
  ASSERT_FLOAT_EQ(1.0, ceil(0.9));
  ASSERT_FLOAT_EQ(1.0f, ceilf(0.9f));
  ASSERT_FLOAT_EQ(1.0, ceill(0.9));
}

TEST(math, floor) {
  ASSERT_FLOAT_EQ(1.0, floor(1.1));
  ASSERT_FLOAT_EQ(1.0f, floorf(1.1f));
  ASSERT_FLOAT_EQ(1.0, floorl(1.1));
}

TEST(math, fabs) {
  ASSERT_FLOAT_EQ(1.0, fabs(-1.0));
  ASSERT_FLOAT_EQ(1.0f, fabsf(-1.0f));
  ASSERT_FLOAT_EQ(1.0, fabsl(-1.0));
}

TEST(math, ldexp) {
  ASSERT_FLOAT_EQ(16.0, ldexp(2.0, 3.0));
  ASSERT_FLOAT_EQ(16.0f, ldexpf(2.0f, 3.0f));
  ASSERT_FLOAT_EQ(16.0, ldexpl(2.0, 3.0));
}

TEST(math, fmod) {
  ASSERT_FLOAT_EQ(2.0, fmod(12.0, 10.0));
  ASSERT_FLOAT_EQ(2.0f, fmodf(12.0f, 10.0f));
  if (sizeof(long double) != sizeof(double)) {
    ASSERT_FLOAT_EQ(2.0, fmodl(12.0, 10.0));
  }
}

TEST(math, remainder) {
  ASSERT_FLOAT_EQ(2.0, remainder(12.0, 10.0));
  ASSERT_FLOAT_EQ(2.0f, remainderf(12.0f, 10.0f));
  ASSERT_FLOAT_EQ(2.0, remainderl(12.0, 10.0));
}

TEST(math, drem) {
  ASSERT_FLOAT_EQ(2.0, drem(12.0, 10.0));
  ASSERT_FLOAT_EQ(2.0f, dremf(12.0f, 10.0f));
}

TEST(math, fmax) {
  ASSERT_FLOAT_EQ(12.0, fmax(12.0, 10.0));
  ASSERT_FLOAT_EQ(12.0f, fmaxf(12.0f, 10.0f));
  ASSERT_FLOAT_EQ(12.0, fmaxl(12.0, 10.0));
}

TEST(math, fmin) {
  ASSERT_FLOAT_EQ(10.0, fmin(12.0, 10.0));
  ASSERT_FLOAT_EQ(10.0f, fminf(12.0f, 10.0f));
  ASSERT_FLOAT_EQ(10.0, fminl(12.0, 10.0));
}

TEST(math, fma) {
  ASSERT_FLOAT_EQ(10.0, fma(2.0, 3.0, 4.0));
  ASSERT_FLOAT_EQ(10.0f, fmaf(2.0f, 3.0f, 4.0f));
  ASSERT_FLOAT_EQ(10.0, fmal(2.0, 3.0, 4.0));
}

TEST(math, hypot) {
  ASSERT_FLOAT_EQ(5.0, hypot(3.0, 4.0));
  ASSERT_FLOAT_EQ(5.0f, hypotf(3.0f, 4.0f));
  ASSERT_FLOAT_EQ(5.0, hypotl(3.0, 4.0));
}

TEST(math, erf) {
  ASSERT_FLOAT_EQ(0.84270078, erf(1.0));
  ASSERT_FLOAT_EQ(0.84270078f, erff(1.0f));
  ASSERT_FLOAT_EQ(0.84270078, erfl(1.0));
}

TEST(math, erfc) {
  ASSERT_FLOAT_EQ(0.15729921, erfc(1.0));
  ASSERT_FLOAT_EQ(0.15729921f, erfcf(1.0f));
  ASSERT_FLOAT_EQ(0.15729921, erfcl(1.0));
}

TEST(math, lrint) {
  fesetround(FE_UPWARD); // lrint/lrintf/lrintl obey the rounding mode.
  ASSERT_EQ(1235, lrint(1234.01));
  ASSERT_EQ(1235, lrintf(1234.01f));
  ASSERT_EQ(1235, lrintl(1234.01));
  fesetround(FE_TOWARDZERO); // lrint/lrintf/lrintl obey the rounding mode.
  ASSERT_EQ(1234, lrint(1234.01));
  ASSERT_EQ(1234, lrintf(1234.01f));
  ASSERT_EQ(1234, lrintl(1234.01));

  fesetround(FE_UPWARD); // llrint/llrintf/llrintl obey the rounding mode.
  ASSERT_EQ(1235L, llrint(1234.01));
  ASSERT_EQ(1235L, llrintf(1234.01f));
  ASSERT_EQ(1235L, llrintl(1234.01));
  fesetround(FE_TOWARDZERO); // llrint/llrintf/llrintl obey the rounding mode.
  ASSERT_EQ(1234L, llrint(1234.01));
  ASSERT_EQ(1234L, llrintf(1234.01f));
  ASSERT_EQ(1234L, llrintl(1234.01));
}

TEST(math, rint) {
  fesetround(FE_UPWARD); // rint/rintf/rintl obey the rounding mode.
  feclearexcept(FE_ALL_EXCEPT); // rint/rintf/rintl do set the FE_INEXACT flag.
  ASSERT_EQ(1234.0, rint(1234.0));
  ASSERT_TRUE((fetestexcept(FE_ALL_EXCEPT) & FE_INEXACT) == 0);
  ASSERT_EQ(1235.0, rint(1234.01));
  ASSERT_TRUE((fetestexcept(FE_ALL_EXCEPT) & FE_INEXACT) != 0);

  feclearexcept(FE_ALL_EXCEPT); // rint/rintf/rintl do set the FE_INEXACT flag.
  ASSERT_EQ(1234.0f, rintf(1234.0f));
  ASSERT_TRUE((fetestexcept(FE_ALL_EXCEPT) & FE_INEXACT) == 0);
  ASSERT_EQ(1235.0f, rintf(1234.01f));
  ASSERT_TRUE((fetestexcept(FE_ALL_EXCEPT) & FE_INEXACT) != 0);

  feclearexcept(FE_ALL_EXCEPT); // rint/rintf/rintl do set the FE_INEXACT flag.
  ASSERT_EQ(1234.0, rintl(1234.0));
  ASSERT_TRUE((fetestexcept(FE_ALL_EXCEPT) & FE_INEXACT) == 0);
  ASSERT_EQ(1235.0, rintl(1234.01));
  ASSERT_TRUE((fetestexcept(FE_ALL_EXCEPT) & FE_INEXACT) != 0);

  fesetround(FE_TOWARDZERO); // rint/rintf obey the rounding mode.
  ASSERT_EQ(1234.0, rint(1234.01));
  ASSERT_EQ(1234.0f, rintf(1234.01f));
  ASSERT_EQ(1234.0, rintl(1234.01));
}

TEST(math, nearbyint) {
  fesetround(FE_UPWARD); // nearbyint/nearbyintf/nearbyintl obey the rounding mode.
  feclearexcept(FE_ALL_EXCEPT); // nearbyint/nearbyintf/nearbyintl don't set the FE_INEXACT flag.
  ASSERT_EQ(1234.0, nearbyint(1234.0));
  ASSERT_TRUE((fetestexcept(FE_ALL_EXCEPT) & FE_INEXACT) == 0);
  ASSERT_EQ(1235.0, nearbyint(1234.01));
  ASSERT_TRUE((fetestexcept(FE_ALL_EXCEPT) & FE_INEXACT) == 0);

  feclearexcept(FE_ALL_EXCEPT);
  ASSERT_EQ(1234.0f, nearbyintf(1234.0f));
  ASSERT_TRUE((fetestexcept(FE_ALL_EXCEPT) & FE_INEXACT) == 0);
  ASSERT_EQ(1235.0f, nearbyintf(1234.01f));
  ASSERT_TRUE((fetestexcept(FE_ALL_EXCEPT) & FE_INEXACT) == 0);

  feclearexcept(FE_ALL_EXCEPT); // nearbyint/nearbyintf/nearbyintl don't set the FE_INEXACT flag.
  ASSERT_EQ(1234.0, nearbyintl(1234.0));
  ASSERT_TRUE((fetestexcept(FE_ALL_EXCEPT) & FE_INEXACT) == 0);
  ASSERT_EQ(1235.0, nearbyintl(1234.01));
  ASSERT_TRUE((fetestexcept(FE_ALL_EXCEPT) & FE_INEXACT) == 0);

  fesetround(FE_TOWARDZERO); // nearbyint/nearbyintf/nearbyintl obey the rounding mode.
  ASSERT_EQ(1234.0, nearbyint(1234.01));
  ASSERT_EQ(1234.0f, nearbyintf(1234.01f));
  ASSERT_EQ(1234.0, nearbyintl(1234.01));
}

TEST(math, lround) {
  fesetround(FE_UPWARD); // lround ignores the rounding mode.
  ASSERT_EQ(1234, lround(1234.01));
  ASSERT_EQ(1234, lroundf(1234.01f));
  ASSERT_EQ(1234, lroundl(1234.01));

  fesetround(FE_UPWARD); // llround ignores the rounding mode.
  ASSERT_EQ(1234L, llround(1234.01));
  ASSERT_EQ(1234L, llroundf(1234.01f));
  ASSERT_EQ(1234L, llroundl(1234.01));
}

TEST(math, ilogb) {
  ASSERT_EQ(FP_ILOGB0, ilogb(0.0));
  ASSERT_EQ(FP_ILOGBNAN, ilogb(nan("")));
  ASSERT_EQ(INT_MAX, ilogb(HUGE_VAL));
  ASSERT_EQ(0, ilogb(1.0));
  ASSERT_EQ(3, ilogb(10.0));

  ASSERT_EQ(FP_ILOGB0, ilogbf(0.0f));
  ASSERT_EQ(FP_ILOGBNAN, ilogbf(nanf("")));
  ASSERT_EQ(INT_MAX, ilogbf(HUGE_VALF));
  ASSERT_EQ(0, ilogbf(1.0f));
  ASSERT_EQ(3, ilogbf(10.0f));

  ASSERT_EQ(FP_ILOGB0, ilogbl(0.0));
  ASSERT_EQ(FP_ILOGBNAN, ilogbl(nanl("")));
  ASSERT_EQ(INT_MAX, ilogbl(HUGE_VALL));
  ASSERT_EQ(0, ilogbl(1.0));
  ASSERT_EQ(3, ilogbl(10.0));
}

TEST(math, logb) {
  ASSERT_EQ(-HUGE_VAL, logb(0.0));
  ASSERT_TRUE(isnan(logb(nan(""))));
  ASSERT_TRUE(isinf(logb(HUGE_VAL)));
  ASSERT_EQ(0.0, logb(1.0));
  ASSERT_EQ(3.0, logb(10.0));

  ASSERT_EQ(-HUGE_VALF, logbf(0.0f));
  ASSERT_TRUE(isnanf(logbf(nanf(""))));
  ASSERT_TRUE(__isinff(logbf(HUGE_VALF)));
  ASSERT_EQ(0.0f, logbf(1.0f));
  ASSERT_EQ(3.0f, logbf(10.0f));

  ASSERT_EQ(-HUGE_VAL, logbl(0.0));
  ASSERT_TRUE(isnan(logbl(nanl(""))));
  ASSERT_TRUE(isinf(logbl(HUGE_VALL)));
  ASSERT_EQ(0.0, logbl(1.0));
  ASSERT_EQ(3.0, logbl(10.0));
}

TEST(math, log1p) {
  ASSERT_EQ(-HUGE_VAL, log1p(-1.0));
  ASSERT_TRUE(isnan(log1p(nan(""))));
  ASSERT_TRUE(isinf(log1p(HUGE_VAL)));
  ASSERT_FLOAT_EQ(1.0, log1p(M_E - 1.0));

  ASSERT_EQ(-HUGE_VALF, log1pf(-1.0f));
  ASSERT_TRUE(isnanf(log1pf(nanf(""))));
  ASSERT_TRUE(__isinff(log1pf(HUGE_VALF)));
  ASSERT_FLOAT_EQ(1.0f, log1pf(static_cast<float>(M_E) - 1.0f));

#if !defined(__BIONIC__) // glibc has log1pl, but BSD (and thus bionic) doesn't.
  ASSERT_EQ(-HUGE_VALL, log1pl(-1.0));
  ASSERT_TRUE(isnan(log1pl(nanl(""))));
  ASSERT_TRUE(isinf(log1pl(HUGE_VALL)));
  ASSERT_FLOAT_EQ(1.0, log1pl(M_E - 1.0));
#endif
}

TEST(math, fdim) {
  ASSERT_FLOAT_EQ(0.0, fdim(1.0, 1.0));
  ASSERT_FLOAT_EQ(1.0, fdim(2.0, 1.0));
  ASSERT_FLOAT_EQ(0.0, fdim(1.0, 2.0));

  ASSERT_FLOAT_EQ(0.0f, fdimf(1.0f, 1.0f));
  ASSERT_FLOAT_EQ(1.0f, fdimf(2.0f, 1.0f));
  ASSERT_FLOAT_EQ(0.0f, fdimf(1.0f, 2.0f));

  ASSERT_FLOAT_EQ(0.0, fdiml(1.0, 1.0));
  ASSERT_FLOAT_EQ(1.0, fdiml(2.0, 1.0));
  ASSERT_FLOAT_EQ(0.0, fdiml(1.0, 2.0));
}

TEST(math, round) {
  fesetround(FE_TOWARDZERO); // round ignores the rounding mode and always rounds away from zero.

  ASSERT_FLOAT_EQ(1.0, round(0.5));
  ASSERT_FLOAT_EQ(-1.0, round(-0.5));
  ASSERT_FLOAT_EQ(0.0, round(0.0));
  ASSERT_FLOAT_EQ(-0.0, round(-0.0));
  ASSERT_TRUE(isnan(round(nan(""))));
  ASSERT_FLOAT_EQ(HUGE_VAL, round(HUGE_VAL));

  ASSERT_FLOAT_EQ(1.0f, roundf(0.5f));
  ASSERT_FLOAT_EQ(-1.0f, roundf(-0.5f));
  ASSERT_FLOAT_EQ(0.0f, roundf(0.0f));
  ASSERT_FLOAT_EQ(-0.0f, roundf(-0.0f));
  ASSERT_TRUE(isnanf(roundf(nanf(""))));
  ASSERT_FLOAT_EQ(HUGE_VALF, roundf(HUGE_VALF));

  ASSERT_FLOAT_EQ(1.0, roundl(0.5));
  ASSERT_FLOAT_EQ(-1.0, roundl(-0.5));
  ASSERT_FLOAT_EQ(0.0, roundl(0.0));
  ASSERT_FLOAT_EQ(-0.0, roundl(-0.0));
  ASSERT_TRUE(isnan(roundl(nanl(""))));
  ASSERT_FLOAT_EQ(HUGE_VALL, roundl(HUGE_VALL));
}

TEST(math, trunc) {
  fesetround(FE_UPWARD); // trunc ignores the rounding mode and always rounds toward zero.

  ASSERT_FLOAT_EQ(1.0, trunc(1.5));
  ASSERT_FLOAT_EQ(-1.0, trunc(-1.5));
  ASSERT_FLOAT_EQ(0.0, trunc(0.0));
  ASSERT_FLOAT_EQ(-0.0, trunc(-0.0));
  ASSERT_TRUE(isnan(trunc(nan(""))));
  ASSERT_FLOAT_EQ(HUGE_VAL, trunc(HUGE_VAL));

  ASSERT_FLOAT_EQ(1.0f, truncf(1.5f));
  ASSERT_FLOAT_EQ(-1.0f, truncf(-1.5f));
  ASSERT_FLOAT_EQ(0.0f, truncf(0.0f));
  ASSERT_FLOAT_EQ(-0.0f, truncf(-0.0f));
  ASSERT_TRUE(isnan(truncf(nanf(""))));
  ASSERT_FLOAT_EQ(HUGE_VALF, truncf(HUGE_VALF));

  ASSERT_FLOAT_EQ(1.0, truncl(1.5));
  ASSERT_FLOAT_EQ(-1.0, truncl(-1.5));
  ASSERT_FLOAT_EQ(0.0, truncl(0.0));
  ASSERT_FLOAT_EQ(-0.0, truncl(-0.0));
  ASSERT_TRUE(isnan(truncl(nan(""))));
  ASSERT_FLOAT_EQ(HUGE_VALL, truncl(HUGE_VALL));
}

TEST(math, nextafter) {
  ASSERT_FLOAT_EQ(0.0, nextafter(0.0, 0.0));
  ASSERT_FLOAT_EQ(1.4012985e-45, nextafter(0.0, 1.0));
  ASSERT_FLOAT_EQ(0.0, nextafter(0.0, -1.0));

  ASSERT_FLOAT_EQ(0.0f, nextafterf(0.0f, 0.0f));
  ASSERT_FLOAT_EQ(1.4012985e-45f, nextafterf(0.0f, 1.0f));
  ASSERT_FLOAT_EQ(0.0f, nextafterf(0.0f, -1.0f));

#if !defined(__BIONIC__) // See Android.mk for the nextafterl problems.
  ASSERT_FLOAT_EQ(0.0, nextafterl(0.0, 0.0));
  ASSERT_FLOAT_EQ(1.4012985e-45, nextafterl(0.0, 1.0));
  ASSERT_FLOAT_EQ(0.0, nextafterl(0.0, -1.0));
#endif
}

// TODO: nexttoward
// TODO: nexttowardf
// TODO: nexttowardl

TEST(math, copysign) {
  ASSERT_FLOAT_EQ(0.0, copysign(0.0, 1.0));
  ASSERT_FLOAT_EQ(-0.0, copysign(0.0, -1.0));
  ASSERT_FLOAT_EQ(2.0, copysign(2.0, 1.0));
  ASSERT_FLOAT_EQ(-2.0, copysign(2.0, -1.0));

  ASSERT_FLOAT_EQ(0.0f, copysignf(0.0f, 1.0f));
  ASSERT_FLOAT_EQ(-0.0f, copysignf(0.0f, -1.0f));
  ASSERT_FLOAT_EQ(2.0f, copysignf(2.0f, 1.0f));
  ASSERT_FLOAT_EQ(-2.0f, copysignf(2.0f, -1.0f));

  ASSERT_FLOAT_EQ(0.0f, copysignl(0.0, 1.0));
  ASSERT_FLOAT_EQ(-0.0f, copysignl(0.0, -1.0));
  ASSERT_FLOAT_EQ(2.0f, copysignl(2.0, 1.0));
  ASSERT_FLOAT_EQ(-2.0f, copysignl(2.0, -1.0));
}

TEST(math, significand) {
  ASSERT_FLOAT_EQ(0.0, significand(0.0));
  ASSERT_FLOAT_EQ(1.2, significand(1.2));
  ASSERT_FLOAT_EQ(1.5375, significand(12.3));

  ASSERT_FLOAT_EQ(0.0f, significandf(0.0f));
  ASSERT_FLOAT_EQ(1.2f, significandf(1.2f));
  ASSERT_FLOAT_EQ(1.5375f, significandf(12.3f));

#if !defined(__BIONIC__) // glibc has significandl, but BSD (and thus bionic) doesn't.
  ASSERT_FLOAT_EQ(0.0, significandl(0.0));
  ASSERT_FLOAT_EQ(1.2, significandl(1.2));
  ASSERT_FLOAT_EQ(1.5375, significandl(12.3));
#endif
}

TEST(math, scalb) {
  ASSERT_FLOAT_EQ(12.0, scalb(3.0, 2.0));
  ASSERT_FLOAT_EQ(12.0f, scalbf(3.0f, 2.0f));

  ASSERT_FLOAT_EQ(12.0, scalbln(3.0, 2L));
  ASSERT_FLOAT_EQ(12.0f, scalblnf(3.0f, 2L));
  ASSERT_FLOAT_EQ(12.0, scalblnl(3.0, 2L));

  ASSERT_FLOAT_EQ(12.0, scalbn(3.0, 2));
  ASSERT_FLOAT_EQ(12.0f, scalbnf(3.0f, 2));
  ASSERT_FLOAT_EQ(12.0, scalbnl(3.0, 2));
}

TEST(math, gamma) {
  ASSERT_FLOAT_EQ(log(24.0), gamma(5.0));
  ASSERT_FLOAT_EQ(logf(24.0f), gammaf(5.0f));
}

extern "C" double gamma_r(double, int*);
extern "C" float gammaf_r(float, int*);

TEST(math, gamma_r) {
  int sign;
  ASSERT_FLOAT_EQ(log(24.0), gamma_r(5.0, &sign));
  ASSERT_EQ(1, sign);
  ASSERT_FLOAT_EQ(logf(24.0f), gammaf_r(5.0f, &sign));
  ASSERT_EQ(1, sign);
}

TEST(math, lgamma) {
  ASSERT_FLOAT_EQ(log(24.0), lgamma(5.0));
  ASSERT_FLOAT_EQ(logf(24.0f), lgammaf(5.0f));

  // TODO: this is in the BSD math.h, but not in their libm.
  // ASSERT_FLOAT_EQ(logl(24.0), lgammal(5.0));
}

extern "C" double lgamma_r(double, int*);
extern "C" float lgammaf_r(float, int*);

TEST(math, lgamma_r) {
  int sign;
  ASSERT_FLOAT_EQ(log(24.0), lgamma_r(5.0, &sign));
  ASSERT_EQ(1, sign);
  ASSERT_FLOAT_EQ(logf(24.0f), lgammaf_r(5.0f, &sign));
  ASSERT_EQ(1, sign);
}

TEST(math, tgamma) {
  ASSERT_FLOAT_EQ(24.0, tgamma(5.0));
  ASSERT_FLOAT_EQ(24.0f, tgammaf(5.0f));

  // TODO: this is in the BSD math.h, but not in their libm.
  // ASSERT_FLOAT_EQ(24.0, tgammal(5.0));
}

TEST(math, j0) {
  ASSERT_FLOAT_EQ(1.0, j0(0.0));
  ASSERT_FLOAT_EQ(1.0f, j0f(0.0f));

  ASSERT_FLOAT_EQ(0.76519769, j0(1.0));
  ASSERT_FLOAT_EQ(0.76519769f, j0f(1.0f));
}

TEST(math, j1) {
  ASSERT_FLOAT_EQ(0.0, j1(0.0));
  ASSERT_FLOAT_EQ(0.0f, j1f(0.0f));

  ASSERT_FLOAT_EQ(0.44005057, j1(1.0));
  ASSERT_FLOAT_EQ(0.44005057f, j1f(1.0f));
}

TEST(math, jn) {
  ASSERT_FLOAT_EQ(0.0, jn(4, 0.0));
  ASSERT_FLOAT_EQ(0.0f, jnf(4, 0.0f));

  ASSERT_FLOAT_EQ(0.0024766389, jn(4, 1.0));
  ASSERT_FLOAT_EQ(0.0024766389f, jnf(4, 1.0f));
}

TEST(math, y0) {
  ASSERT_FLOAT_EQ(-HUGE_VAL, y0(0.0));
  ASSERT_FLOAT_EQ(-HUGE_VALF, y0f(0.0f));

  ASSERT_FLOAT_EQ(0.088256963, y0(1.0));
  ASSERT_FLOAT_EQ(0.088256963f, y0f(1.0f));
}

TEST(math, y1) {
  ASSERT_FLOAT_EQ(-HUGE_VAL, y1(0.0));
  ASSERT_FLOAT_EQ(-HUGE_VALF, y1f(0.0f));

  ASSERT_FLOAT_EQ(-0.78121281, y1(1.0));
  ASSERT_FLOAT_EQ(-0.78121281f, y1f(1.0f));
}

TEST(math, yn) {
  ASSERT_FLOAT_EQ(-HUGE_VAL, yn(4, 0.0));
  ASSERT_FLOAT_EQ(-HUGE_VALF, ynf(4, 0.0f));

  ASSERT_FLOAT_EQ(-33.278423, yn(4, 1.0));
  ASSERT_FLOAT_EQ(-33.278423f, ynf(4, 1.0f));
}

TEST(math, frexp) {
  int exp;
  double dr = frexp(1024.0, &exp);
  ASSERT_FLOAT_EQ(1024.0, scalbn(dr, exp));

  float fr = frexpf(1024.0f, &exp);
  ASSERT_FLOAT_EQ(1024.0f, scalbnf(fr, exp));

  long double ldr = frexpl(1024.0, &exp);
  ASSERT_FLOAT_EQ(1024.0, scalbnl(ldr, exp));
}

TEST(math, modf) {
  double di;
  double df = modf(123.456, &di);
  ASSERT_FLOAT_EQ(123.0, di);
  ASSERT_FLOAT_EQ(0.456, df);

  float fi;
  float ff = modff(123.456f, &fi);
  ASSERT_FLOAT_EQ(123.0f, fi);
  ASSERT_FLOAT_EQ(0.45600128f, ff);

  long double ldi;
  long double ldf = modfl(123.456, &ldi);
  ASSERT_FLOAT_EQ(123.0, ldi);
  ASSERT_FLOAT_EQ(0.456, ldf);
}

TEST(math, remquo) {
  int q;
  double d = remquo(13.0, 4.0, &q);
  ASSERT_EQ(3, q);
  ASSERT_FLOAT_EQ(1.0, d);

  float f = remquof(13.0f, 4.0f, &q);
  ASSERT_EQ(3, q);
  ASSERT_FLOAT_EQ(1.0, f);

  long double ld = remquol(13.0, 4.0, &q);
  ASSERT_EQ(3, q);
  ASSERT_FLOAT_EQ(1.0, ld);
}
// https://code.google.com/p/android/issues/detail?id=6697
TEST(math, frexpf_public_bug_6697) {
  int exp;
  float fr = frexpf(14.1f, &exp);
  ASSERT_FLOAT_EQ(14.1f, scalbnf(fr, exp));
}
