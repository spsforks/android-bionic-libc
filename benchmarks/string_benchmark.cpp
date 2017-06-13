/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <stdint.h>
#include <string.h>

#include <benchmark/benchmark.h>

constexpr auto KB = 1024;

#define AT_COMMON_SIZES \
    Arg(8)->Arg(64)->Arg(512)->Arg(1*KB)->Arg(8*KB)->Arg(16*KB)->Arg(32*KB)->Arg(64*KB)

static void BM_string_memcmp(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  char* src = new char[nbytes]; char* dst = new char[nbytes];
  memset(src, 'x', nbytes);
  memset(dst, 'x', nbytes);

  volatile int c __attribute__((unused)) = 0;
  while (state.KeepRunning()) {
    c += memcmp(dst, src, nbytes);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_string_memcmp)->AT_COMMON_SIZES;

static void BM_string_memcmp_aligned(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  char* src0 = new char[nbytes];
  char* src1 = new char[nbytes];
  char* src2 = new char[nbytes];
  char* src3 = new char[nbytes];
  char* src4 = new char[nbytes];
  char* dst0 = new char[nbytes];
  char* dst1 = new char[nbytes];
  char* dst2 = new char[nbytes];
  char* dst3 = new char[nbytes];
  char* dst4 = new char[nbytes];

  memset(src0, 'x', nbytes);
  memset(src1, 'x', nbytes);
  memset(src2, 'x', nbytes);
  memset(src3, 'x', nbytes);
  memset(src4, 'x', nbytes);

  memset(dst0, 'x', nbytes);
  memset(dst1, 'x', nbytes);
  memset(dst2, 'x', nbytes);
  memset(dst3, 'x', nbytes);
  memset(dst4, 'x', nbytes);

  dst1[nbytes/2]='z'; //mismatch at 1/2 size
  src2[nbytes-1]='u'; //mismatch in end
  dst3[5]='A'; //mismatch in 6th char
  src4[0]='y'; //mismatch in 1st char

  volatile int c __attribute__((unused)) = 0;
  while (state.KeepRunning()) {
    c += memcmp(dst0, src0, nbytes); //match
    c += memcmp(dst0, src0, nbytes); //match
    c += memcmp(dst0, src0, nbytes); //match
    c += memcmp(dst0, src0, nbytes); //match

    c += memcmp(dst1, src1, nbytes); //mismatch
    c += memcmp(dst2, src2, nbytes); //mismatch
    c += memcmp(dst3, src3, nbytes); //mismatch
    c += memcmp(dst4, src4, nbytes); //mismatch
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));

  delete[] src0;
  delete[] src1;
  delete[] src2;
  delete[] src3;
  delete[] src4;

  delete[] dst0;
  delete[] dst1;
  delete[] dst2;
  delete[] dst3;
  delete[] dst4;
}
BENCHMARK(BM_string_memcmp_aligned)->AT_COMMON_SIZES;

static void BM_string_memcmp_unaligned(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  char* src0 = new char[nbytes+128];
  char* src1 = new char[nbytes+128];
  char* src2 = new char[nbytes+128];
  char* src3 = new char[nbytes+128];
  char* src4 = new char[nbytes+128];
  char* dst0 = new char[nbytes+128];
  char* dst1 = new char[nbytes+128];
  char* dst2 = new char[nbytes+128];
  char* dst3 = new char[nbytes+128];
  char* dst4 = new char[nbytes+128];
  int offset=0, offset1=0, offset2=0;

  memset(src0, 'x', nbytes+128);
  memset(dst0, 'x', nbytes+128);

  memset(src1, 'x', nbytes+128);
  memset(src2, 'x', nbytes+128);
  memset(src3, 'x', nbytes+128);
  memset(src4, 'x', nbytes+128);

  memset(dst1, 'x', nbytes+128);
  memset(dst2, 'x', nbytes+128);
  memset(dst3, 'x', nbytes+128);
  memset(dst4, 'x', nbytes+128);

  dst1[nbytes/2]='z';   //mismatch at 1/2 size
  src2[nbytes-1]='u'; //mismatch in end
  dst3[5]='A'; //mismatch in 6th char
  memset(src4, 'y', 128); //mismatch in 1st char

  volatile int c __attribute__((unused)) = 0;
  while (state.KeepRunning()) {

    // Sweep offset through 128B to get all kind of unaligned with typical cache
    // line crossing.
    offset = (offset+1)%256;
    offset1 = (offset < 128) ? offset : 0;
    offset2 = (offset >= 128)? offset-128 : 0;

    c += memcmp(dst0+offset1,  src0+offset2,  nbytes); //match
    c += memcmp(dst0+offset1,  src0+offset2,  nbytes); //match
    c += memcmp(dst0+offset1,  src0+offset2,  nbytes); //match
    c += memcmp(dst0+offset1,  src0+offset2,  nbytes); //match

    c += memcmp(dst1+offset1, src1+offset2, nbytes); //mismatch
    c += memcmp(dst2+offset1, src2+offset2, nbytes); //mismatch
    c += memcmp(dst3+offset1, src3+offset2, nbytes); //mismatch
    c += memcmp(dst4+offset1, src4+offset2, nbytes); //mismatch
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));

  delete[] src0;
  delete[] src1;
  delete[] src2;
  delete[] src3;
  delete[] src4;

  delete[] dst0;
  delete[] dst1;
  delete[] dst2;
  delete[] dst3;
  delete[] dst4;
}
BENCHMARK(BM_string_memcmp_unaligned)->AT_COMMON_SIZES;

static void BM_string_memcpy(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  char* src = new char[nbytes]; char* dst = new char[nbytes];
  memset(src, 'x', nbytes);

  while (state.KeepRunning()) {
    memcpy(dst, src, nbytes);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
  delete[] src;
  delete[] dst;
}
BENCHMARK(BM_string_memcpy)->AT_COMMON_SIZES;

static void BM_string_memmove(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  char* buf = new char[nbytes + 64];
  memset(buf, 'x', nbytes + 64);

  while (state.KeepRunning()) {
    memmove(buf, buf + 1, nbytes); // Worst-case overlap.
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
  delete[] buf;
}
BENCHMARK(BM_string_memmove)->AT_COMMON_SIZES;

static void BM_string_memset(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  char* dst = new char[nbytes];

  while (state.KeepRunning()) {
    memset(dst, 0, nbytes);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
  delete[] dst;
}
BENCHMARK(BM_string_memset)->AT_COMMON_SIZES;

static void BM_string_strlen(benchmark::State& state) {
  const size_t nbytes = state.range(0);
  char* s = new char[nbytes];
  memset(s, 'x', nbytes);
  s[nbytes - 1] = 0;

  volatile int c __attribute__((unused)) = 0;
  while (state.KeepRunning()) {
    c += strlen(s);
  }

  state.SetBytesProcessed(uint64_t(state.iterations()) * uint64_t(nbytes));
  delete[] s;
}
BENCHMARK(BM_string_strlen)->AT_COMMON_SIZES;
