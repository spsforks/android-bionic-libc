/*
 * Copyright (C) 2017 The Android Open Source Project
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

// Our goal is to measure the cost of various C++ atomic operations.
// Android doesn't really control those. But since some of these operations can be quite
// expensive, this may be useful input for development of higher level code.
// Expected mappings from C++ atomics to hardware primitives can be found at
// http://www.cl.cam.ac.uk/~pes20/cpp/cpp0xmappings.html .

#include <benchmark/benchmark.h>

#include <time.h>
#include <unistd.h>

#include <signal.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <signal.h>

// Monte carlo stuff
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#define SEED 35791246

#include <chrono>


namespace ptrace_benchmark {
  // static int counter;

  enum MprotectStates {
    mprotect_change_only,      // set to RWX
    mprotect_restore_only,     // set back to X
    mprotect_both,             // first RWX, then X
  };
};

// Baseline of hopefully the cheapest possible system call.
void BM_ptrace_syscall_time(benchmark::State& state) {
  using namespace ptrace_benchmark;

  while (state.KeepRunning()) {
    time(NULL);
  }
}
BENCHMARK(BM_ptrace_syscall_time);

// Flush the instruction cache.
void BM_ptrace_syscall_cacheflush(benchmark::State& state, size_t range = 1) {
  using namespace ptrace_benchmark;

  int page_size = sysconf(_SC_PAGESIZE);
  size_t useless_memory_sizeof = page_size * (2 + range);

  // allocate some memory, 3 pages to guarantee that the middle page is exclusively ours.
  char* useless_memory = new char[useless_memory_sizeof];

  // touch the pages to make sure they are really there
  for (size_t i = 0; i < useless_memory_sizeof; ++i) {
    useless_memory[i] = static_cast<char>(i);
  }

  // find the "safe" point in the middle that's also aligned.
  uintptr_t page_aligned_address = reinterpret_cast<uintptr_t>(useless_memory) + page_size;
  page_aligned_address = page_aligned_address - (page_aligned_address % page_size);

  char* addr = reinterpret_cast<char*>(page_aligned_address);

  while (state.KeepRunning()) {
    __builtin___clear_cache(addr, addr + (range * page_size));
  }

  delete[] useless_memory;
}
BENCHMARK_CAPTURE(BM_ptrace_syscall_cacheflush, 1, 1);
BENCHMARK_CAPTURE(BM_ptrace_syscall_cacheflush, 10, 10);
BENCHMARK_CAPTURE(BM_ptrace_syscall_cacheflush, 100, 100);

// copied from linux kernel for armv7 and later
#define asm_isb(option) __asm__ __volatile__ ("isb " #option : : : "memory")

// Do an ISB.
void BM_ptrace_isb(benchmark::State& state) {
  using namespace ptrace_benchmark;

  while (state.KeepRunning()) {
#if defined(__arm__) || defined(__arch64__)
    asm_isb();
#endif
  }
}
BENCHMARK(BM_ptrace_isb);

// mprotect RO -> RW
void BM_ptrace_syscall_mprotect(benchmark::State& state, ptrace_benchmark::MprotectStates states, size_t range = 1) {
  using namespace ptrace_benchmark;

  int page_size = sysconf(_SC_PAGESIZE);
  size_t useless_memory_sizeof = page_size * (2 + range);

  // allocate some memory, 3 pages to guarantee that the middle page is exclusively ours.
  char* useless_memory = new char[useless_memory_sizeof];

  // touch the pages to make sure they are really there
  for (size_t i = 0; i < useless_memory_sizeof; ++i) {
    useless_memory[i] = static_cast<char>(i);
  }

  // find the "safe" point in the middle that's also aligned.
  uintptr_t page_aligned_address = reinterpret_cast<uintptr_t>(useless_memory) + page_size;
  page_aligned_address = page_aligned_address - (page_aligned_address % page_size);

  void* addr = reinterpret_cast<void*>(page_aligned_address);

  // set initial state to --x just like real code memory would be.
  mprotect(addr, static_cast<size_t>(page_size) * range, PROT_READ | PROT_WRITE | PROT_EXEC);

  while (state.KeepRunning()) {
    if (states == mprotect_both || states == mprotect_change_only) {
      // initial state is X, flip to RWX
      mprotect(addr, static_cast<size_t>(page_size) * range, PROT_READ | PROT_WRITE | PROT_EXEC);
    }

    // can now read memory and then change it.

    if (states == mprotect_both || states == mprotect_restore_only) {
      // flip back to X
      mprotect(addr, static_cast<size_t>(page_size) * range, PROT_EXEC);
    }
  }

  // restore back to default for data memory [or the delete will segfault]
  mprotect(addr, static_cast<size_t>(page_size) * range, PROT_READ | PROT_WRITE);

  delete[] useless_memory;
}

// fun fact: mprotecting seems to be a no-op if the state does not change
BENCHMARK_CAPTURE(BM_ptrace_syscall_mprotect, change_only, ptrace_benchmark::mprotect_change_only);
// fun fact: mprotecting seems to be a no-op if the state does not change
BENCHMARK_CAPTURE(BM_ptrace_syscall_mprotect, restore_only, ptrace_benchmark::mprotect_restore_only);
// the only valid benchmark appears to measure the cost of both.
// the presumably the real cost is about half of doing both calls.
BENCHMARK_CAPTURE(BM_ptrace_syscall_mprotect, mprotect_both, ptrace_benchmark::mprotect_both);

// how does the cost scale per-page?
// it looks like more pages is cheaper at once. we should coalesce mprotects if possible.
BENCHMARK_CAPTURE(BM_ptrace_syscall_mprotect, mprotect_both_10, ptrace_benchmark::mprotect_both, 10);
BENCHMARK_CAPTURE(BM_ptrace_syscall_mprotect, mprotect_both_100, ptrace_benchmark::mprotect_both, 100);
BENCHMARK_CAPTURE(BM_ptrace_syscall_mprotect, mprotect_both_1000, ptrace_benchmark::mprotect_both, 1000);

namespace ptrace_signal {
  volatile bool received_signal;

  void handle_signal(int /* signal */) {
    received_signal = true;
  }
};

void BM_ptrace_signal_self(benchmark::State& state, bool kill_syscall, bool real_time) {
  using namespace ptrace_signal;

  struct sigaction sa;
  sa.sa_handler = &ptrace_signal::handle_signal;
  sa.sa_flags = SA_RESTART;

  int sig_to_send = SIGUSR1;
  if (real_time) {
    // Signals above this magical value are handled "especially" in the linux kernel
    // as a real time signal.
    //
    // (Note that with POSIX its unspecified if kill will deliver an RT signal,
    // but using sigqueue *is* specified to do so).
    sig_to_send = SIGRTMIN+1;
  }

  sigfillset(&sa.sa_mask);

  struct sigaction old_sa;
  if (sigaction(sig_to_send, &sa, &old_sa) == -1) {
    perror("Error: cannot handle signal"); // Should not happen
  }

  pid_t this_pid = getpid();

  while (state.KeepRunning()) {
    received_signal = false;

    if (kill_syscall) {
      kill(this_pid, sig_to_send);
    } else {
      sigqueue(this_pid, sig_to_send, /* value */ {});
    }

    // Signal delivery is asynchronous. Wait until we get it.
    while (!received_signal);
  }


  // Restore previous.
  sigaction(sig_to_send, &old_sa, NULL);
}

BENCHMARK_CAPTURE(BM_ptrace_signal_self, kill_default, true, false);
BENCHMARK_CAPTURE(BM_ptrace_signal_self, kill_real_time, true, true);
BENCHMARK_CAPTURE(BM_ptrace_signal_self, sigqueue_default, true, false);
BENCHMARK_CAPTURE(BM_ptrace_signal_self, sigqueue_real_time, true, true);

struct ForkedResults {
  std::atomic<uint64_t> duration_count;
  std::atomic<bool> ready;
  std::atomic<bool> done;
};

// Cost of fork+ptrace
void BM_ptrace_ptrace_child(benchmark::State& state) {
  while (state.KeepRunning()) {
    pid_t child;
    int status;

    child = fork();
    if (child == 0) {
      ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
      exit(0);
    } else {
      while(waitpid(child, &status, 0) && ! WIFEXITED(status)) {
        int sig = 0;
        ptrace(PTRACE_CONT, child, NULL, reinterpret_cast<void*>(sig));
      }
    }
  }
}

BENCHMARK(BM_ptrace_ptrace_child);

// pick a sufficiently high enough time so the fork cost is negligible.
volatile bool monte_carlo_iterations = 10000;

void monte_carlo() {
   int niter=0;
   double x,y;
   int i,count=0; /* # of points in the 1st quadrant of unit circle */
   double z;
   double pi;

   // printf("Enter the number of iterations used to estimate pi: ");
   // scanf("%d",&niter);
   niter = monte_carlo_iterations;

   /* initialize random numbers */
   srand(SEED);
   count=0;
   for ( i=0; i<niter; i++) {
      x = (double)rand()/RAND_MAX;
      y = (double)rand()/RAND_MAX;
      z = x*x+y*y;
      if (z<=1) count++;
      }
   pi=(double)count/niter*4;
   benchmark::ClobberMemory();
   // printf("# of trials= %d , estimate of pi is %g \n",niter,pi);
}

void BM_ptrace_monte_carlo_base(benchmark::State& state) {
  while (state.KeepRunning()) {
    monte_carlo();
  }
}

BENCHMARK(BM_ptrace_monte_carlo_base);



void BM_ptrace_monte_carlo_forked(benchmark::State& state, bool ptraced = false, bool do_operation = true) {
  while (state.KeepRunning()) {
    // auto start = std::chrono::high_resolution_clock::now();

    size_t map_length = 1024;
    void* shared_memory = mmap(nullptr, map_length, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0);
    if (shared_memory == MAP_FAILED) {
      // fprintf(stderr, "Failed to make mmap: %s", strerror(errno));
      perror("Failed to make mmap: ");
    }

    ForkedResults* results = new (shared_memory) ForkedResults();
    results->duration_count = 0;
    results->ready = false;
    results->done = false;

    pid_t child = fork();
    if (child == 0) {
      double duration;
      if (!do_operation) {
        // actually do the measurement
        results->ready = true;
        // auto start = std::chrono::system_clock::now();
        auto start = std::chrono::high_resolution_clock::now();

        // want to compare ptrace-attached vs unptraced performance
        if (ptraced) {
          ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
        }

        // auto end = std::chrono::system_clock::now();
        auto end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
      } else {

        // want to compare ptrace-attached vs unptraced performance
        if (ptraced) {
          ptrace(PTRACE_TRACEME, 0, nullptr, nullptr);
        }
      }

      if (do_operation) {
        /* warm up to put everything in the cache first */
        for (int i = 0; i < 1000; ++i) {
          monte_carlo();
        }

        // actually do the measurement
        results->ready = true;
        // auto start = std::chrono::system_clock::now();
        auto start = std::chrono::high_resolution_clock::now();

          monte_carlo();
        // auto end = std::chrono::system_clock::now();
        auto end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
      }

      // report it back via the shared memory
      uint64_t duration_as_long;
      static_assert(sizeof(double) == sizeof(uint64_t), "SIZE MISMATCH");
      memcpy(&duration_as_long, &duration, sizeof(duration_as_long));

      results->duration_count = duration_as_long;
      results->done = true;

      // exit , don't run rest of this code.
      exit(0);
    } else {

      if (ptraced) {
        int status;
        while(waitpid(child, &status, 0) && ! WIFEXITED(status)) {
          int sig = 0;
          ptrace(PTRACE_CONT, child, NULL, reinterpret_cast<void*>(sig));
        }
      }

      // wait until child is done.
      while (results->done == false);

      double duration;
      uint64_t duration_as_long = results->duration_count;
      memcpy(&duration, &duration_as_long, sizeof(duration_as_long));

      state.SetIterationTime(duration);

      // printf("Done at least once with %lf\n", duration);
    }

    // delete results;
    munmap(shared_memory, map_length);
  }
}

BENCHMARK_CAPTURE(BM_ptrace_monte_carlo_forked, ptrace_only, false, false)->UseManualTime()->MinTime(0.001);
BENCHMARK_CAPTURE(BM_ptrace_monte_carlo_forked, baseline, false, true)->UseManualTime()->MinTime(0.1);
BENCHMARK_CAPTURE(BM_ptrace_monte_carlo_forked, traced, true, true)->UseManualTime()->MinTime(0.1);
