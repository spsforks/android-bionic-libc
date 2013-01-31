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

#include <fenv.h>
#include <math.h>
#include <stdint.h>

/*static void TestRounding(float expectation1, float expectation2) {
  // volatile to prevent compiler optimizations.
  volatile float f = 1.968750f;
  volatile float m = 0x1.0p23f;
  volatile float x = f + m;
  ASSERT_FLOAT_EQ(expectation1, x);
  x -= m;
  ASSERT_EQ(expectation2, x);
}*/

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

/* TODO: stlport breaks isfinite
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

/* TODO: stlport breaks signbit
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

// TODO: __fpclassifyl

#if defined(__BIONIC__)
TEST(math, __isfinitef) {
  ASSERT_TRUE(__isfinitef(123.0f));
  ASSERT_FALSE(__isfinitef(HUGE_VALF));
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

TEST(math, finite) {
  ASSERT_TRUE(finite(123.0));
  ASSERT_FALSE(finite(HUGE_VAL));
}

// TODO: __isfinitel

TEST(math, __isinff) {
  ASSERT_FALSE(__isinff(123.0f));
  ASSERT_TRUE(__isinff(HUGE_VALF));
}

// TODO: __isinfl

TEST(math, __isnanf) {
  ASSERT_FALSE(__isnanf(123.0f));
  ASSERT_TRUE(__isnanf(nanf("")));
}

TEST(math, isnanf) {
  ASSERT_FALSE(isnanf(123.0f));
  ASSERT_TRUE(isnanf(nanf("")));
}

// TODO: __isnanl, nanl

#if defined(__BIONIC__)
TEST(math, __isnormalf) {
  ASSERT_TRUE(__isnormalf(123.0f));
  ASSERT_FALSE(__isnormalf(float_subnormal()));
}
#endif

#if defined(__BIONIC__)
TEST(math, __isnormal) {
  ASSERT_TRUE(__isnormal(123.0));
  ASSERT_FALSE(__isnormal(double_subnormal()));
}
#endif

// TODO: __isnormall

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

// TODO: __signbitl

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
  // TODO: logl
}

TEST(math, log2) {
  ASSERT_FLOAT_EQ(12.0, log2(4096.0));
  ASSERT_FLOAT_EQ(12.0f, log2f(4096.0f));
  // TODO: log2l
}

TEST(math, log10) {
  ASSERT_FLOAT_EQ(3.0, log10(1000.0));
  ASSERT_FLOAT_EQ(3.0f, log10f(1000.0f));
  // TODO: log10l
}

TEST(math, cbrt) {
  ASSERT_FLOAT_EQ(3.0, cbrt(27.0));
  ASSERT_FLOAT_EQ(3.0f, cbrtf(27.0f));
  // TODO: cbrtl
}

TEST(math, sqrt) {
  ASSERT_FLOAT_EQ(2.0, sqrt(4.0));
  ASSERT_FLOAT_EQ(2.0f, sqrtf(4.0f));
  // TODO: sqrtl
}

TEST(math, exp) {
  ASSERT_FLOAT_EQ(1.0, exp(0.0));
  ASSERT_FLOAT_EQ(M_E, exp(1.0));

  ASSERT_FLOAT_EQ(1.0f, expf(0.0f));
  ASSERT_FLOAT_EQ(static_cast<float>(M_E), expf(1.0f));

  // TODO: expl
}

TEST(math, exp2) {
  ASSERT_FLOAT_EQ(8.0, exp2(3.0));
  ASSERT_FLOAT_EQ(8.0f, exp2f(3.0f));
  // TODO: exp2l
}

TEST(math, expm1) {
  ASSERT_FLOAT_EQ(M_E - 1.0, expm1(1.0));
  ASSERT_FLOAT_EQ(static_cast<float>(M_E) - 1.0f, expm1f(1.0f));
  // TODO: expm1l
}

TEST(math, pow) {
  ASSERT_FLOAT_EQ(8.0, pow(2.0, 3.0));
  ASSERT_FLOAT_EQ(8.0f, powf(2.0f, 3.0f));
  // TODO: powl
}

TEST(math, ceil) {
  ASSERT_FLOAT_EQ(1.0, ceil(0.9));
  ASSERT_FLOAT_EQ(1.0f, ceilf(0.9f));
  // TODO: ceill
}

TEST(math, floor) {
  ASSERT_FLOAT_EQ(1.0, floor(1.1));
  ASSERT_FLOAT_EQ(1.0f, floorf(1.1f));
  // TODO: floorl
}

TEST(math, fabs) {
  ASSERT_FLOAT_EQ(1.0, fabs(-1.0));
  ASSERT_FLOAT_EQ(1.0f, fabsf(-1.0f));
  // TODO: fabsl
}

// TODO: frexp

TEST(math, ldexp) {
  ASSERT_FLOAT_EQ(16.0, ldexp(2.0, 3.0));
  ASSERT_FLOAT_EQ(16.0f, ldexpf(2.0f, 3.0f));
  // TODO: ldexpl
}

TEST(math, fmod) {
  ASSERT_FLOAT_EQ(2.0, fmod(12.0, 10.0));
  ASSERT_FLOAT_EQ(2.0f, fmodf(12.0f, 10.0f));
  // TODO: fmodl
}

TEST(math, fmax) {
  ASSERT_FLOAT_EQ(12.0, fmax(12.0, 10.0));
  ASSERT_FLOAT_EQ(12.0f, fmaxf(12.0f, 10.0f));
  // TODO: fmaxl
}

TEST(math, fmin) {
  ASSERT_FLOAT_EQ(10.0, fmin(12.0, 10.0));
  ASSERT_FLOAT_EQ(10.0f, fminf(12.0f, 10.0f));
  // TODO: fminl
}

TEST(math, fma) {
  ASSERT_FLOAT_EQ(10.0, fma(2.0, 3.0, 4.0));
  ASSERT_FLOAT_EQ(10.0f, fmaf(2.0f, 3.0f, 4.0f));
  // TODO: fmal
}

TEST(math, hypot) {
  ASSERT_FLOAT_EQ(5.0, hypot(3.0, 4.0));
  ASSERT_FLOAT_EQ(5.0f, hypotf(3.0f, 4.0f));
  // TODO: hypotl
}

TEST(math, erf) {
  ASSERT_FLOAT_EQ(0.84270078, erf(1.0));
  ASSERT_FLOAT_EQ(0.84270078f, erff(1.0f));
  // TODO: erfl
}

TEST(math, erfc) {
  ASSERT_FLOAT_EQ(0.15729921, erfc(1.0));
  ASSERT_FLOAT_EQ(0.15729921f, erfcf(1.0f));
  // TODO: erfcl
}

TEST(math, lrint) {
  fesetround(FE_UPWARD);
  ASSERT_EQ(1235, lrint(1234.01));
  ASSERT_EQ(1235, lrintf(1234.01f));
  // TODO: lrintl

  ASSERT_EQ(1235L, llrint(1234.01));
  ASSERT_EQ(1235L, llrintf(1234.01f));
  // TODO: llrintl
}

TEST(math, lround) {
  fesetround(FE_UPWARD); // lround ignores the rounding mode.
  ASSERT_EQ(1234, lround(1234.01));
  ASSERT_EQ(1234, lroundf(1234.01f));
  // TODO: lroundl

  ASSERT_EQ(1234L, llround(1234.01));
  ASSERT_EQ(1234L, llroundf(1234.01f));
  // TODO: llroundl
}

// TODO: int	ilogb(double);
// TODO: double	log1p(double);
// TODO: double	logb(double);
// TODO: double	nextafter(double, double);
// TODO: double	remainder(double, double);
// TODO: double	rint(double);

// TODO: double scalb(double, double)

// TODO: double	copysign(double, double);
// TODO: double	fdim(double, double);
// TODO: double	nearbyint(double);
// TODO: double	round(double);
// TODO: double	scalbln(double, long);
// TODO: double	scalbn(double, int);
// TODO: double	trunc(double);

// TODO: double	significand(double);
// TODO: float	significandf(float);

// TODO: int	ilogbf(float);
// TODO: float	log1pf(float);
// TODO: float	roundf(float);
// TODO: float	logbf(float);
// TODO: float	copysignf(float, float);
// TODO: float	nearbyintf(float);
// TODO: float	nextafterf(float, float);
// TODO: float	remainderf(float, float);
// TODO: float	rintf(float);
// TODO: float	scalblnf(float, long);
// TODO: float	scalbnf(float, int);
// TODO: float	truncf(float);
// TODO: float	fdimf(float, float);

// TODO: double	drem(double, double);
// TODO: float	dremf(float, float);

// TODO: double	j0(double);
// TODO: double	j1(double);
// TODO: double	jn(int, double);
// TODO: double	y0(double);
// TODO: double	y1(double);
// TODO: double	yn(int, double);
// TODO: float	j0f(float);
// TODO: float	j1f(float);
// TODO: float	jnf(int, float);
// TODO: float	scalbf(float, float);
// TODO: float	y0f(float);
// TODO: float	y1f(float);
// TODO: float	ynf(int, float);

// TODO: long double	copysignl(long double, long double);
// TODO: long double	fdiml(long double, long double);
// TODO: int		ilogbl(long double);
// TODO: long double	logbl(long double);
// TODO: long double	nearbyintl(long double);
// TODO: long double	nextafterl(long double, long double);
// TODO: double		nexttoward(double, long double);
// TODO: float		nexttowardf(float, long double);
// TODO: long double	nexttowardl(long double, long double);
// TODO: long double	remainderl(long double, long double);
// TODO: long double	rintl(long double);
// TODO: long double	roundl(long double);
// TODO: long double	scalblnl(long double, long);
// TODO: long double	scalbnl(long double, int);
// TODO: long double	truncl(long double);
// TODO: long double	lgammal(long double);
// TODO: long double	log1pl(long double);
// TODO: long double	tgammal(long double);

// TODO: double	lgamma(double);
// TODO: double gamma(double)
// TODO: double	tgamma(double);
// TODO: float	lgammaf(float);
// TODO: float	tgammaf(float);
// TODO: double	gamma_r(double, int *);
// TODO: double	lgamma_r(double, int *);
// TODO: float	gammaf(float);
// TODO: float	gammaf_r(float, int *);
// TODO: float	lgammaf_r(float, int *);

// TODO: double	frexp(double, int *);
// TODO: float	frexpf(float, int *);
// TODO: long double	frexpl(long double value, int *);
// TODO: double	modf(double, double *);
// TODO: float	modff(float, float *);
// TODO: long double	modfl(long double, long double *);
// TODO: double	remquo(double, double, int *);
// TODO: float	remquof(float, float, int *);
// TODO: long double	remquol(long double, long double, int *);
