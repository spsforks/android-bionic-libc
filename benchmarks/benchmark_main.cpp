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

#include <benchmark.h>

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <map>
#include <vector>

#include <inttypes.h>

static int64_t gBytesProcessed;
static int64_t gBenchmarkTotalTimeNs;
static int64_t gBenchmarkStartTimeNs;

typedef std::vector< ::testing::Benchmark* > BenchmarkList;
static BenchmarkList& Benchmarks() {
  static BenchmarkList benchmarks;
  return benchmarks;
}

static int Round(int n) {
  int base = 1;
  while (base*10 < n) {
    base *= 10;
  }
  if (n < 2*base) {
    return 2*base;
  }
  if (n < 5*base) {
    return 5*base;
  }
  return 10*base;
}

static int64_t NanoTime() {
  struct timespec t;
  t.tv_sec = t.tv_nsec = 0;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return static_cast<int64_t>(t.tv_sec) * 1000000000LL + t.tv_nsec;
}

namespace testing {

int PrettyPrintInt(char* str, int len, int64_t arg)
{
  if (arg >= (1<<30) && arg % (1<<30) == 0) {
    return snprintf(str, len, "%lldGi", arg/(1<<30));
  } else if (arg >= (1<<20) && arg % (1<<20) == 0) {
    return snprintf(str, len, "%lldMi", arg/(1<<20));
  } else if (arg >= (1<<10) && arg % (1<<10) == 0) {
    return snprintf(str, len, "%lldKi", arg/(1<<10));
  } else if (arg >= 1000000000) {
    return snprintf(str, len, "%lldG", arg/1000000000);
  } else if (arg >= 1000000) {
    return snprintf(str, len, "%lldM", arg/1000000);
  } else if (arg >= 1000) {
    return snprintf(str, len, "%lldK", arg/1000);
  } else {
    return snprintf(str, len, "%lld", arg);
  }
}

const char* Benchmark::Name() {
  return name_;
}

bool Benchmark::ShouldRun(int argc, char* argv[]) {
  if (argc == 1) {
    return true;  // With no arguments, we run all benchmarks.
  }
  // Otherwise, we interpret each argument as a regular expression and
  // see if any of our benchmarks match.
  for (int i = 1; i < argc; i++) {
    regex_t re;
    if (regcomp(&re, argv[i], 0) != 0) {
      fprintf(stderr, "couldn't compile \"%s\" as a regular expression!\n", argv[i]);
      exit(EXIT_FAILURE);
    }
    int match = regexec(&re, name_, 0, NULL, 0);
    regfree(&re);
    if (match != REG_NOMATCH) {
      return true;
    }
  }
  return false;
}

void BenchmarkRegister(Benchmark* b) {
  Benchmarks().push_back(b);
}

void Benchmark::RunIterations(int iterations) {
  gBytesProcessed = 0;
  gBenchmarkTotalTimeNs = 0;
  gBenchmarkStartTimeNs = NanoTime();
  RunFn(iterations);
  if (gBenchmarkStartTimeNs != 0) {
    gBenchmarkTotalTimeNs += NanoTime() - gBenchmarkStartTimeNs;
  }
}

void Benchmark::Run() {
  // run once in case it's expensive
  int iterations = 1;
  RunIterations(iterations);
  while (gBenchmarkTotalTimeNs < 1e9 && iterations < 1e9) {
    int last = iterations;
    if (gBenchmarkTotalTimeNs/iterations == 0) {
      iterations = 1e9;
    } else {
      iterations = 1e9 / (gBenchmarkTotalTimeNs/iterations);
    }
    iterations = std::max(last + 1, std::min(iterations + iterations/2, 100*last));
    iterations = Round(iterations);
    RunIterations(iterations);
  }

  char throughput[100];
  throughput[0] = '\0';
  if (gBenchmarkTotalTimeNs > 0 && gBytesProcessed > 0) {
    double mib_processed = static_cast<double>(gBytesProcessed)/1e6;
    double seconds = static_cast<double>(gBenchmarkTotalTimeNs)/1e9;
    snprintf(throughput, sizeof(throughput), " %8.2f MiB/s", mib_processed/seconds);
  }

  char full_name[100];
  snprintf(full_name, sizeof(full_name), "%s%s%s", name_,
           ArgName() ? "/" : "",
           ArgName() ? ArgName() : "");

  printf("%-25s %10d %10lld%s\n", full_name,
       iterations, gBenchmarkTotalTimeNs/iterations, throughput);
  fflush(stdout);
}

}  // namespace testing

void SetBenchmarkBytesProcessed(int64_t x) {
  gBytesProcessed = x;
}

void StopBenchmarkTiming() {
  if (gBenchmarkStartTimeNs != 0) {
    gBenchmarkTotalTimeNs += NanoTime() - gBenchmarkStartTimeNs;
  }
  gBenchmarkStartTimeNs = 0;
}

void StartBenchmarkTiming() {
  if (gBenchmarkStartTimeNs == 0) {
    gBenchmarkStartTimeNs = NanoTime();
  }
}

int main(int argc, char* argv[]) {
  if (Benchmarks().empty()) {
    fprintf(stderr, "No benchmarks registered!\n");
    exit(EXIT_FAILURE);
  }

  bool need_header = true;
  for (auto b : Benchmarks()) {
    if (b->ShouldRun(argc, argv)) {
      if (need_header) {
        printf("%-25s %10s %10s\n", "", "iterations", "ns/op");
        fflush(stdout);
        need_header = false;
      }
      b->Run();
    }
  }

  if (need_header) {
    fprintf(stderr, "No matching benchmarks!\n");
    fprintf(stderr, "Available benchmarks:\n");
    for (auto b : Benchmarks()) {
      fprintf(stderr, "  %s\n", b->Name());
    }
    exit(EXIT_FAILURE);
  }

  return 0;
}
