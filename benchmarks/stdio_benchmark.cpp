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

#include "stdio_benchmark.h"

#include <stdio.h>
#include <stdio_ext.h>
#include <stdlib.h>

#include <map>

#include <benchmark/benchmark.h>

template <typename Fn>
void ReadWriteTest(benchmark::State& state, Fn f, bool buffered) {
  size_t chunk_size = state.range(0);

  FILE* fp = fopen("/dev/zero", "rw");
  __fsetlocking(fp, FSETLOCKING_BYCALLER);
  char* buf = new char[chunk_size];

  if (!buffered) {
    setvbuf(fp, 0, _IONBF, 0);
  }

  while (state.KeepRunning()) {
    f(buf, chunk_size, 1, fp);
  }

  state.SetBytesProcessed(int64_t(state.iterations()) * int64_t(chunk_size));
  delete[] buf;
  fclose(fp);
}

void BM_stdio_fread(benchmark::State& state) {
  ReadWriteTest(state, fread, true);
}

void BM_stdio_fwrite(benchmark::State& state) {
  ReadWriteTest(state, fwrite, true);
}

void BM_stdio_fread_unbuffered(benchmark::State& state) {
  ReadWriteTest(state, fread, false);
}

void BM_stdio_fwrite_unbuffered(benchmark::State& state) {
  ReadWriteTest(state, fwrite, false);
}

static void FopenFgetsFclose(benchmark::State& state, bool no_locking) {
  char buf[1024];
  while (state.KeepRunning()) {
    FILE* fp = fopen("/proc/version", "re");
    if (no_locking) __fsetlocking(fp, FSETLOCKING_BYCALLER);
    if (fgets(buf, sizeof(buf), fp) == nullptr) abort();
    fclose(fp);
  }
}

static void BM_stdio_fopen_fgets_fclose_locking(benchmark::State& state) {
  FopenFgetsFclose(state, false);
}

void BM_stdio_fopen_fgets_fclose_no_locking(benchmark::State& state) {
  FopenFgetsFclose(state, true);
}

void declare_stdio_benchmarks(std::map <std::string, benchmark_func_t>& str_to_func) {
  str_to_func.emplace(std::string("BM_stdio_fread"), BM_stdio_fread);
  str_to_func.emplace(std::string("BM_stdio_fwrite"), BM_stdio_fwrite);
  str_to_func.emplace(std::string("BM_stdio_fread_unbuffered"), BM_stdio_fread_unbuffered);
  str_to_func.emplace(std::string("BM_stdio_fwrite_unbuffered"), BM_stdio_fwrite_unbuffered);
  str_to_func.emplace(std::string("BM_stdio_fopen_fgets_fclose_locking"),
                                  BM_stdio_fopen_fgets_fclose_locking);
  str_to_func.emplace(std::string("BM_stdio_fopen_fgets_fclose_no_locking"),
                                  BM_stdio_fopen_fgets_fclose_no_locking);
}
