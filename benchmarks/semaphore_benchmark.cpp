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

#include <pthread.h>
#include <semaphore.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>

#include <benchmark/benchmark.h>
#include "util.h"

static void BM_semaphore_sem_getvalue(benchmark::State& state) {
  sem_t semaphore;
  sem_init(&semaphore, 1, 1);

  while (state.KeepRunning()) {
    int unused;
    sem_getvalue(&semaphore, &unused);
  }
}
BIONIC_BENCHMARK(BM_semaphore_sem_getvalue);

static void BM_semaphore_sem_wait_sem_post(benchmark::State& state) {
  sem_t semaphore;
  sem_init(&semaphore, 1, 1);

  while (state.KeepRunning()) {
    sem_wait(&semaphore);
    sem_post(&semaphore);
  }
}
BIONIC_BENCHMARK(BM_semaphore_sem_wait_sem_post);

// This test reports the overhead of the underlying futex wake syscall on
// the producer. It does not report the overhead from issuing the wake to the
// point where the posted consumer thread wakes up. It suffers from
// clock_gettime syscall overhead. Lock the CPU speed for consistent results
// as we may not reach >50% cpu utilization.
//
// We will run a background thread that catches the sem_post wakeup and
// loops immediately returning back to sleep in sem_wait for the next one. This
// thread is run with policy SCHED_OTHER (normal policy), a middle policy.
//
// The primary thread will run at SCHED_IDLE (lowest priority policy) when
// monitoring the background thread to detect when it hits sem_wait sleep. It
// will do so with no clock running. Once we are ready, we will switch to
// SCHED_FIFO (highest priority policy) to time the act of running sem_post
// with the benchmark clock running. This ensures nothing else in the system
// can preempt our timed activity, including the background thread. We are
// also protected with the scheduling policy of letting a process hit a
// resource limit rather than get hit with a context switch.
//
// The background thread will start executing either on another CPU, or
// after we back down from SCHED_FIFO, but certainly not in the context of
// the timing of the sem_post.

static atomic_int BM_semaphore_sem_post_running;

static void* BM_semaphore_sem_post_start_thread(void* arg) {
  sem_t* semaphore = reinterpret_cast<sem_t*>(arg);
  while ((BM_semaphore_sem_post_running > 0) && !sem_wait(semaphore)) {
  }
  BM_semaphore_sem_post_running = -1;
  return nullptr;
}
