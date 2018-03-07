/*
 * Copyright (C) 2018 The Android Open Source Project
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include <benchmark/benchmark.h>
#include "util.h"

void BM_malloc_sparse(benchmark::State& state) {
  void* p[10000];
  while (state.KeepRunning()) {
    for (size_t i = 0; i < sizeof(p) / sizeof(*p); i++) {
      p[i] = malloc(4000);
      if (p[i] == nullptr) {
        fprintf(stderr, "Failed to allocate memory.\n");
        break;
      }
      memset(p[i], 0, 4000);
    }
    for (size_t i = 0; i < sizeof(p) / sizeof(*p); i++) {
      if (i % 150) {
        free(p[i]);
        p[i] = nullptr;
      }
    }
  }

  // Free the rest of the pointers.
  for (size_t i = 0; i < sizeof(p) / sizeof(*p); i++) {
    if (p[i] != nullptr) {
      free(p[i]);
    }
  }
}
BIONIC_BENCHMARK(BM_malloc_sparse);

void BM_malloc_bubble(benchmark::State& state) {
  void* p[10000];
  while (state.KeepRunning()) {
    for (size_t i = 0; i < sizeof(p) / sizeof(*p); i++) {
      p[i] = malloc(4000);
      if (p[i] == nullptr) {
        fprintf(stderr, "Failed to allocate memory.\n");
        break;
      }
      memset(p[i], 0, 4000);
    }
    for (size_t i = 0; i < sizeof(p) / sizeof(*p) - 1; i++) {
      free(p[i]);
      p[i] = nullptr;
    }
  }

  // Free the rest of the pointers.
  for (size_t i = 0; i < sizeof(p) / sizeof(*p); i++) {
    if (p[i] != nullptr) {
      free(p[i]);
    }
  }
}
BIONIC_BENCHMARK(BM_malloc_bubble);

void BM_malloc_tiny1(benchmark::State& state) {
  void* p[10000];
  while (state.KeepRunning()) {
    for (size_t i = 0; i < sizeof(p) / sizeof(*p); i++) {
      p[i] = malloc((i % 4 + 1) * 16);
      if (p[i] == nullptr) {
        fprintf(stderr, "Failed to allocate memory.\n");
        break;
      }
    }
    for (size_t i = 0; i < sizeof(p) / sizeof(*p); i++) {
      free(p[i]);
    }
  }
}
BIONIC_BENCHMARK(BM_malloc_tiny1);

void BM_malloc_tiny2(benchmark::State& state) {
  void* p[10000];
  while (state.KeepRunning()) {
    for (size_t i = 0; i < sizeof(p) / sizeof(*p); i++) {
      p[i] = malloc((i % 4 + 1) * 16);
      if (p[i] == nullptr) {
        fprintf(stderr, "Failed to allocate memory.\n");
        break;
      }
    }
    for (size_t i = 1; i; i = (i + 57) % (sizeof(p) / sizeof(*p))) {
      free(p[i]);
      p[i] = nullptr;
    }
  }

  // Free the rest of the pointers.
  for (size_t i = 0; i < sizeof(p) / sizeof(*p); i++) {
    if (p[i] != nullptr) {
      free(p[i]);
    }
  }
}
BIONIC_BENCHMARK(BM_malloc_tiny2);

void BM_malloc_big1(benchmark::State& state) {
  void* p[2000];
  while (state.KeepRunning()) {
    for (size_t i = 0; i < sizeof(p) / sizeof(*p); i++) {
      p[i] = malloc((i % 4 + 1) * 16384);
      if (p[i] == nullptr) {
        fprintf(stderr, "Failed to allocate memory.\n");
        break;
      }
    }
    for (size_t i = 0; i < sizeof(p) / sizeof(*p); i++) {
      free(p[i]);
    }
  }
}
BIONIC_BENCHMARK(BM_malloc_big1);

void BM_malloc_big2(benchmark::State& state) {
  void* p[2000];
  while (state.KeepRunning()) {
    for (size_t i = 0; i < sizeof(p) / sizeof(*p); i++) {
      p[i] = malloc((i % 4 + 1) * 16384);
      if (p[i] == nullptr) {
        fprintf(stderr, "Failed to allocate memory.\n");
        break;
      }
    }
    for (size_t i = 1; i; i = (i + 57) % (sizeof(p) / sizeof(*p))) {
      free(p[i]);
      p[i] = nullptr;
    }
  }

  // Free the rest of the pointers.
  for (size_t i = 0; i < sizeof(p) / sizeof(*p); i++) {
    if (p[i] != nullptr) {
      free(p[i]);
    }
  }

}
BIONIC_BENCHMARK(BM_malloc_big2);

constexpr size_t kLoops = 100000;
constexpr size_t kShCount = 300;
constexpr size_t kMaxSize = 500;

struct foo {
  pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
  void* mem = nullptr;
};

static unsigned int rng(unsigned* r) {
  return *r = *r * 1103515245 + 12345;
}

static void* RunStress(void* arg) {
  struct foo* foo = reinterpret_cast<struct foo*>(arg);
  unsigned int r = static_cast<unsigned int>(pthread_self());

  for (size_t i = 0; i < kLoops; i++) {
    size_t j = rng(&r) % kShCount;
    size_t size = rng(&r) % kMaxSize;
    pthread_mutex_lock(&foo[j].lock);
    void* p = foo[j].mem;
    foo[j].mem = nullptr;
    pthread_mutex_unlock(&foo[j].lock);
    free(p);
    if (p == nullptr) {
      p = malloc(size);
      if (p == nullptr) {
        fprintf(stderr, "Failed to allocate memory.\n");
        break;
      }
      pthread_mutex_lock(&foo[j].lock);
      if (foo[j].mem == nullptr) {
        foo[j].mem = p;
        p = nullptr;
      }
      pthread_mutex_unlock(&foo[j].lock);
      free(p);
    }
  }
  return nullptr;
}

void BM_malloc_thread_stress(benchmark::State& state) {
  struct foo foo[kShCount];
  while (state.KeepRunning()) {
    pthread_t td1, td2;
    pthread_create(&td1, 0, RunStress, foo);
    pthread_create(&td2, 0, RunStress, foo);

    pthread_join(td1, nullptr);
    pthread_join(td2, nullptr);
  }
}
BIONIC_BENCHMARK(BM_malloc_thread_stress);

void BM_malloc_thread_local(benchmark::State& state) {
  struct foo foo1[kShCount];
  struct foo foo2[kShCount];
  while (state.KeepRunning()) {
    pthread_t td1, td2;
    pthread_create(&td1, 0, RunStress, foo1);
    pthread_create(&td2, 0, RunStress, foo2);

    pthread_join(td1, nullptr);
    pthread_join(td2, nullptr);
  }
}
BIONIC_BENCHMARK(BM_malloc_thread_local);
