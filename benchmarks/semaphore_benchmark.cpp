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

#include "benchmark.h"

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

static void BM_semaphore_sem_getvalue(int iters) {
  StopBenchmarkTiming();
  sem_t semaphore;
  sem_init(&semaphore, 1, 1);
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    int dummy;
    sem_getvalue(&semaphore, &dummy);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_semaphore_sem_getvalue);

static void BM_semaphore_sem_wait_sem_post(int iters) {
  StopBenchmarkTiming();
  sem_t semaphore;
  sem_init(&semaphore, 1, 1);
  StartBenchmarkTiming();

  for (int i = 0; i < iters; ++i) {
    sem_wait(&semaphore);
    sem_post(&semaphore);
  }

  StopBenchmarkTiming();
}
BENCHMARK(BM_semaphore_sem_wait_sem_post);

/*
 * This test tries its best to report the underlying futex wake syscall
 * overhead. It suffers from clock_gettime syscall overhead. Lock the CPU
 * speed for consistent results as we may not reach >50% cpu utilization.
 * Test still returns a result occasionally that is double.
 */
static int __sem_wait_run;

static void *__sem_wait(void *obj) {
    sem_t *semaphore = reinterpret_cast<sem_t *>(obj);

    while ((__sem_wait_run > 0) && !sem_wait(semaphore)) {
        ;
    }
    __sem_wait_run = -1;
    return NULL;
}

static void BM_semaphore_sem_post_wakeup(int iters) {
  StopBenchmarkTiming();

  sem_t semaphore;
  sem_init(&semaphore, 0, 0);

  pthread_attr_t attr;
  pthread_attr_init(&attr);
  __sem_wait_run = 1;
  struct sched_param param = { 0, };
  pthread_attr_setschedparam(&attr, &param);
  pthread_attr_setschedpolicy(&attr, SCHED_OTHER);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
  pthread_t pthread;
  pthread_create(&pthread, &attr, __sem_wait, &semaphore);
  pthread_attr_destroy(&attr);

  sched_setscheduler((pid_t)0, SCHED_IDLE, &param);
  for (int i = 0; i < iters; ++i) {
    int trys = 3, dummy = 0;
    do {
      if (__sem_wait_run < 0) {
        sched_setscheduler((pid_t)0, SCHED_OTHER, &param);
        fprintf(stderr, "__sem_wait died unexpectedly\n");
        return;
      }
      sched_yield();
      sem_getvalue(&semaphore, &dummy);
      if (dummy < 0) {  // POSIX.1-2001 possibility 1
        break;
      }
      if (dummy == 0) { // POSIX.1-2001 possibility 2
        --trys;
      }
    } while (trys);
    param.sched_priority = 1;
    sched_setscheduler((pid_t)0, SCHED_FIFO, &param);
    StartBenchmarkTiming();
    sem_post(&semaphore);
    StopBenchmarkTiming(); // Remember to subtract clock syscall overhead
    param.sched_priority = 0;
    sched_setscheduler((pid_t)0, SCHED_IDLE, &param);
  }
  sched_setscheduler((pid_t)0, SCHED_OTHER, &param);

  if (__sem_wait_run > 0) {
    __sem_wait_run = 0;
  }
  do {
    sem_post(&semaphore);
    sched_yield();
  } while (!__sem_wait_run);
}
BENCHMARK(BM_semaphore_sem_post_wakeup);
