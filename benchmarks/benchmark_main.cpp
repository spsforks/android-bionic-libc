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

#include <histogram-inl.h>

static int64_t gBytesProcessed;
static int64_t gBenchmarkIterationStartTimeNs;
static int64_t gBenchmarkTotalTimeNs;
static int64_t gBenchmarkStartTimeNs;

typedef std::vector< ::testing::Benchmark* > BenchmarkList;
static BenchmarkList* gBenchmarks;

typedef std::vector<std::pair< ::testing::Benchmark*, void (*)(::testing::Benchmark*) >> BenchmarkSetupList;
static BenchmarkSetupList *gBenchmarkSetupFns;

static std::vector<int64_t> gBenchmarkIterationTimesNs;
static int64_t gBenchmarkMaxIterationTimeNs;

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

int PrettyPrintInt(char* str, int len, unsigned int arg)
{
  if (arg >= (1<<30) && arg % (1<<30) == 0) {
    return snprintf(str, len, "%uGi", arg/(1<<30));
  } else if (arg >= (1<<20) && arg % (1<<20) == 0) {
    return snprintf(str, len, "%uMi", arg/(1<<20));
  } else if (arg >= (1<<10) && arg % (1<<10) == 0) {
    return snprintf(str, len, "%uKi", arg/(1<<10));
  } else if (arg >= 1000000000 && arg % 1000000000 == 0) {
    return snprintf(str, len, "%uG", arg/1000000000);
  } else if (arg >= 1000000 && arg % 1000000 == 0) {
    return snprintf(str, len, "%uM", arg/1000000);
  } else if (arg >= 1000 && arg % 1000 == 0) {
    return snprintf(str, len, "%uK", arg/1000);
  } else {
    return snprintf(str, len, "%u", arg);
  }
}

bool ShouldRun(Benchmark* b, int argc, char* argv[]) {
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
    int match = regexec(&re, b->Name(), 0, NULL, 0);
    regfree(&re);
    if (match != REG_NOMATCH) {
      return true;
    }
  }
  return false;
}

void BenchmarkRegister(Benchmark* b) {
  if (gBenchmarks == NULL) {
    gBenchmarks = new BenchmarkList;
  }
  gBenchmarks->push_back(b);
}

void BenchmarkRegisterSetupFn(Benchmark* b, void (*fn)(Benchmark*)) {
  if (gBenchmarkSetupFns == NULL) {
    gBenchmarkSetupFns = new BenchmarkSetupList;
  }
  gBenchmarkSetupFns->push_back(std::make_pair(b, fn));
}

void RunRepeatedly(Benchmark* b, int iterations) {
  gBytesProcessed = 0;
  gBenchmarkTotalTimeNs = 0;
  gBenchmarkStartTimeNs = NanoTime();
  b->RunFn(iterations);
  if (gBenchmarkStartTimeNs != 0) {
    gBenchmarkTotalTimeNs += NanoTime() - gBenchmarkStartTimeNs;
  }
}

void Run(Benchmark* b) {
  // run once in case it's expensive
  gBenchmarkIterationTimesNs.clear();
  gBenchmarkMaxIterationTimeNs = 0;
  int iterations = 1;
  int64_t realStartTime = NanoTime();
  RunRepeatedly(b, iterations);
  int64_t realTotalTime = NanoTime() - realStartTime;
  while (realTotalTime < 1e9 && iterations < 1e8) {
    int last = iterations;
    if (realTotalTime/iterations == 0) {
      iterations = 1e8;
    } else {
      iterations = 1e8 / (realTotalTime/iterations);
    }
    iterations = std::max(last + 1, std::min(iterations + iterations/2, 100*last));
    iterations = Round(iterations);
    gBenchmarkIterationTimesNs.reserve(iterations);
    gBenchmarkIterationTimesNs.clear();
    gBenchmarkMaxIterationTimeNs = 0;
    realStartTime = NanoTime();
    RunRepeatedly(b, iterations);
    realTotalTime = NanoTime() - realStartTime;
  }

  char throughput[100];
  throughput[0] = '\0';
  if (gBenchmarkTotalTimeNs > 0 && gBytesProcessed > 0) {
    double mib_processed = static_cast<double>(gBytesProcessed)/1e6;
    double seconds = static_cast<double>(gBenchmarkTotalTimeNs)/1e9;
    snprintf(throughput, sizeof(throughput), "%7.2f", mib_processed/seconds);
  }

  char histogram[100];
  histogram[0] = '\0';
  if (gBenchmarkIterationTimesNs.size() == (size_t)iterations) {
    int64_t avg = gBenchmarkTotalTimeNs / iterations;
    int64_t buckets = gBenchmarkMaxIterationTimeNs / avg * 100;
    if (buckets > 1e7) {
      buckets = 1e7;
    }

    ::histogram::Histogram<int64_t> h(b->Name(), 10, buckets);
    for (int64_t t : gBenchmarkIterationTimesNs) {
      h.AddValue(t);
    }

    ::histogram::Histogram<int64_t>::CumulativeData data;
    h.CreateHistogram(data);

    float ci = 0.95;
    int64_t ci_up = h.Percentile(1 - (1 - ci) / 2, data) - avg;
    int64_t ci_down = avg - h.Percentile((1 - ci) / 2, data);
    int64_t percent_up = ci_up * 100 / avg;
    int64_t percent_down = ci_down * 100 / avg;

    snprintf(histogram, sizeof(histogram), "+%lld/-%-lld%%",
             percent_up, percent_down);
  } else if (gBenchmarkIterationTimesNs.size() > 0) {
      snprintf(histogram, sizeof(histogram), " data points != iterations");
  }

  char full_name[100];
  full_name[0] = '\0';
  snprintf(full_name, sizeof(full_name), "%s%s%s", b->Name(),
           b->ArgName() ? "/" : "",
           b->ArgName() ? b->ArgName() : "");

  char iteration[100];
  iteration[0] = '\0';
  PrettyPrintInt(iteration, sizeof(iteration), iterations);

  printf("%-40s %4s %10lld %-10s %10s\n", full_name,
       iteration, gBenchmarkTotalTimeNs/iterations,
       histogram, throughput);
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

void IterationStart() {
  StartBenchmarkTiming();
  gBenchmarkIterationStartTimeNs = gBenchmarkTotalTimeNs;
}

void IterationEnd() {
  StopBenchmarkTiming();
  int64_t t = gBenchmarkTotalTimeNs - gBenchmarkIterationStartTimeNs;
  gBenchmarkIterationTimesNs.push_back(t);
  if (gBenchmarkMaxIterationTimeNs < t) {
    gBenchmarkMaxIterationTimeNs = t;
  }
}

int main(int argc, char* argv[]) {
  if (gBenchmarkSetupFns) {
    for (auto setup: *gBenchmarkSetupFns) {
      ::testing::Benchmark* b = setup.first;
      void (*fn)(::testing::Benchmark*) = setup.second;
      if (ShouldRun(b, argc, argv)) {
        fn(b);
      }
    }
  }

  if (!gBenchmarks || gBenchmarks->empty()) {
    fprintf(stderr, "No benchmarks registered!\n");
    exit(EXIT_FAILURE);
  }

  bool need_header = true;
  for (auto b : *gBenchmarks) {
    if (ShouldRun(b, argc, argv)) {
      if (need_header) {
        printf("%-34s %10s %10s %10s %10s\n", "", "iterations", "ns/op", "", "MiB/s");
        fflush(stdout);
        need_header = false;
      }
      Run(b);
    }
  }

  if (need_header) {
    fprintf(stderr, "No matching benchmarks!\n");
    fprintf(stderr, "Available benchmarks:\n");
    for (auto b : *gBenchmarks) {
      fprintf(stderr, "  %s\n", b->Name());
    }
    exit(EXIT_FAILURE);
  }

  return 0;
}
