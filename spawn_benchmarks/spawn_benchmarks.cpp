/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include <errno.h>
#include <spawn.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <android-base/file.h>
#include <benchmark/benchmark.h>

#if defined(__LP64__)
#define NATIVETEST "nativetest64"
#else
#define NATIVETEST "nativetest"
#endif

static std::string get_native_test_dir() {
  // Assuming the benchmarks are synced to a rooted device, do this path translation:
  //   /data/benchmarktest[64]/bionic-spawn-benchmarks -> /data/nativetest[64]
  // We can also locate the test programs if they're copied to /data/local/tmp/{benchmarktest64,nativetest64}.
  return android::base::GetExecutableDirectory() + "/../../" + NATIVETEST;
}

extern char** environ;

static void BM_spawn_test(benchmark::State& state, const char* const* argv) {
  for (auto _ : state) {
    pid_t child = 0;
    if (posix_spawn(&child, argv[0], nullptr, nullptr, const_cast<char**>(argv), environ) != 0) {
      fprintf(stderr, "fork failed: %s\n", strerror(errno));
      exit(1);
    }

    int wstatus = 0;
    const pid_t wait_result = TEMP_FAILURE_RETRY(waitpid(child, &wstatus, 0));
    if (wait_result != child) {
      fprintf(stderr, "error: waitpid on %d failed [%s]: %s\n", static_cast<int>(child), argv[0],
              strerror(errno));
      exit(1);
    }
    if (WIFEXITED(wstatus) && WEXITSTATUS(wstatus) == 127) {
      fprintf(stderr, "error: could not exec: %s: %s\n", argv[0], strerror(errno));
      exit(1);
    }
  }
}

#define SPAWN_BENCHMARK(name, ...)                                                    \
    BENCHMARK_CAPTURE(BM_spawn_test, name, (const char*[]) { __VA_ARGS__, nullptr })  \
        ->UseRealTime()                                                               \
        ->Unit(benchmark::kMicrosecond)                                               \

SPAWN_BENCHMARK(noop, (get_native_test_dir() + "/bench_noop/bench_noop").c_str());
SPAWN_BENCHMARK(noop_nostl, (get_native_test_dir() + "/bench_noop_nostl/bench_noop_nostl").c_str());
SPAWN_BENCHMARK(toybox_true, "/system/bin/true");
SPAWN_BENCHMARK(toybox_true_vendor, "/vendor/bin/true");
SPAWN_BENCHMARK(sh_true, "/system/bin/sh", "-c", "true");
SPAWN_BENCHMARK(sh_true_vendor, "/vendor/bin/sh", "-c", "true");

BENCHMARK_MAIN();
