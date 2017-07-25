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

#include "math_benchmark.h"

#include <fenv.h>
#include <math.h>
#include <map>

#include <benchmark/benchmark.h>

static const double values[] = { 1234.0, nan(""), HUGE_VAL, 0.0 };
static const char* names[] = { "1234.0", "nan", "HUGE_VAL", "0.0" };


static void SetLabel(benchmark::State& state) {
  state.SetLabel(names[state.range(0)]);
}

// Avoid optimization.
volatile double d;
volatile double v;

static void BM_math_sqrt(benchmark::State& state) {
  d = 0.0;
  v = 2.0;
  while (state.KeepRunning()) {
    d += sqrt(v);
  }
}

static void BM_math_log10(benchmark::State& state) {
  d = 0.0;
  v = 1234.0;
  while (state.KeepRunning()) {
    d += log10(v);
  }
}

static void BM_math_logb(benchmark::State& state) {
  d = 0.0;
  v = 1234.0;
  while (state.KeepRunning()) {
    d += logb(v);
  }
}

static void BM_math_isfinite_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isfinite(v);
  }
  SetLabel(state);
}

#if defined(__BIONIC__)
#define test_isfinite __isfinite
#else
#define test_isfinite __finite
#endif
static void BM_math_isfinite(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += test_isfinite(v);
  }
  SetLabel(state);
}

static void BM_math_isinf_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isinf(v);
  }
  SetLabel(state);
}

static void BM_math_isinf(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += (isinf)(v);
  }
  SetLabel(state);
}

static void BM_math_isnan_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isnan(v);
  }
  SetLabel(state);
}

static void BM_math_isnan(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += (isnan)(v);
  }
  SetLabel(state);
}

static void BM_math_isnormal_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += isnormal(v);
  }
  SetLabel(state);
}

#if defined(__BIONIC__)
static void BM_math_isnormal(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += (__isnormal)(v);
  }
  SetLabel(state);
}
#endif

static void BM_math_sin_fast(benchmark::State& state) {
  d = 1.0;
  while (state.KeepRunning()) {
    d += sin(d);
  }
}

static void BM_math_sin_feupdateenv(benchmark::State& state) {
  d = 1.0;
  while (state.KeepRunning()) {
    fenv_t __libc_save_rm;
    feholdexcept(&__libc_save_rm);
    fesetround(FE_TONEAREST);
    d += sin(d);
    feupdateenv(&__libc_save_rm);
  }
}

static void BM_math_sin_fesetenv(benchmark::State& state) {
  d = 1.0;
  while (state.KeepRunning()) {
    fenv_t __libc_save_rm;
    feholdexcept(&__libc_save_rm);
    fesetround(FE_TONEAREST);
    d += sin(d);
    fesetenv(&__libc_save_rm);
  }
}

static void BM_math_fpclassify(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += fpclassify(v);
  }
  SetLabel(state);
}

static void BM_math_signbit_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += signbit(v);
  }
  SetLabel(state);
}

static void BM_math_signbit(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += (__signbit)(v);
  }
  SetLabel(state);
}

static void BM_math_fabs_macro(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += fabs(v);
  }
  SetLabel(state);
}

static void BM_math_fabs(benchmark::State& state) {
  d = 0.0;
  v = values[state.range(0)];
  while (state.KeepRunning()) {
    d += (fabs)(v);
  }
  SetLabel(state);
}

void declare_math_benchmarks(std::map <std::string, benchmark_func_t>& str_to_func) {
  str_to_func.emplace(std::string("BM_math_sqrt"), BM_math_sqrt);
  str_to_func.emplace(std::string("BM_math_log10"), BM_math_log10);
  str_to_func.emplace(std::string("BM_math_logb"), BM_math_logb);
  str_to_func.emplace(std::string("BM_math_isfinite_macro"), BM_math_isfinite_macro);
  str_to_func.emplace(std::string("BM_math_isfinite"), BM_math_isfinite);
  str_to_func.emplace(std::string("BM_math_isinf_macro"), BM_math_isinf_macro);
  str_to_func.emplace(std::string("BM_math_isinf"), BM_math_isinf);
  str_to_func.emplace(std::string("BM_math_isnan_macro"), BM_math_isnan_macro);
  str_to_func.emplace(std::string("BM_math_isnan"), BM_math_isnan);
  str_to_func.emplace(std::string("BM_math_isnormal_macro"), BM_math_isnormal_macro);
#if defined(__BIONIC__)
  str_to_func.emplace(std::string("BM_math_isnormal"), BM_math_isnormal);
#endif
  str_to_func.emplace(std::string("BM_math_sin_fast"), BM_math_sin_fast);
  str_to_func.emplace(std::string("BM_math_sin_feupdateenv"), BM_math_sin_feupdateenv);
  str_to_func.emplace(std::string("BM_math_sin_fesetenv"), BM_math_sin_fesetenv);
  str_to_func.emplace(std::string("BM_math_fpclassify"), BM_math_fpclassify);
  str_to_func.emplace(std::string("BM_math_signbit_macro"), BM_math_signbit_macro);
  str_to_func.emplace(std::string("BM_math_signbit"), BM_math_signbit);
  str_to_func.emplace(std::string("BM_math_fabs_macro"), BM_math_fabs_macro);
  str_to_func.emplace(std::string("BM_math_fabs"), BM_math_fabs);
}
