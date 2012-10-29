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

#include <gtest/gtest.h>

#include <errno.h>
#include <pthread.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

TEST(pthread_barrier,init0) {
  pthread_barrier_t barrier;
  EXPECT_EQ(EINVAL, pthread_barrier_init(&barrier, NULL, 0));
}

TEST(pthread_barrier,init_destroy) {
  pthread_barrier_t barrier;
  EXPECT_EQ(0, pthread_barrier_init(&barrier, NULL, 10));
  EXPECT_EQ(0, pthread_barrier_destroy(&barrier));
}

TEST(pthread_barrier,init1) {
  pthread_barrier_t barrier;
  pthread_barrierattr_t attr;
  EXPECT_EQ(0, pthread_barrierattr_init(&attr));
  EXPECT_EQ(0, pthread_barrier_init(&barrier, &attr, 1));
  EXPECT_EQ(0, pthread_barrierattr_destroy(&attr));
  EXPECT_EQ(PTHREAD_BARRIER_SERIAL_THREAD, pthread_barrier_wait(&barrier));
  EXPECT_EQ(0, pthread_barrier_destroy(&barrier));
}

static const int kMaxThreads = 100;

struct ThreadState {
  pthread_barrier_t  barrier;
  pthread_mutex_t    lock;
  int                count;
  pthread_t          threads[kMaxThreads];
  pid_t              pids[kMaxThreads];

  void WritePid(pid_t pid) {
    pthread_mutex_lock(&lock);
    if (count < kMaxThreads) {
      pids[count++] = pid;
    }
    pthread_mutex_unlock(&lock);
  }
};

static void* thread_func(void* arg) {
  ThreadState* state = reinterpret_cast<ThreadState*>(arg);
#ifdef __BIONIC__
  state->WritePid(gettid());
#else
  state->WritePid(getpid());
#endif
  EXPECT_EQ(0, pthread_barrier_wait(&state->barrier));
  return NULL;
}

TEST(pthread_barrier,threads) {
  ThreadState  state;

  EXPECT_EQ(0, pthread_barrier_init(&state.barrier, NULL, kMaxThreads+1));
  EXPECT_EQ(0, pthread_mutex_init(&state.lock, NULL));
  state.count = 0;

  // Create kMaxThreads threads, each one will write its PID before
  // waiting for the barrier, which will be held until the call to
  // pthread_barrier_wait in this function.
  for (int i = 0; i < kMaxThreads; i++) {
    ASSERT_EQ(0, pthread_create(&state.threads[i], NULL, thread_func,
                                reinterpret_cast<void*>(&state)));
  }

  // Sleep until all the threads are waiting.
  for (;;) {
    int done = 0;
    pthread_mutex_lock(&state.lock);
    done = (state.count == kMaxThreads);
    pthread_mutex_unlock(&state.lock);
    if (done)
      break;
    usleep(1000);
  }

  EXPECT_EQ(PTHREAD_BARRIER_SERIAL_THREAD, pthread_barrier_wait(&state.barrier));
}

TEST(pthread_barrier,processes) {
  pthread_barrier_t* barrier;
  pthread_barrierattr_t attr;

  // Create a shared memory page that will be shared between this
  // process and its child, where we will put the barrier.
  void* shared_map = mmap(NULL, 4096, PROT_READ|PROT_WRITE,
                          MAP_SHARED|MAP_ANONYMOUS,-1,0);
  ASSERT_NE(MAP_FAILED, shared_map);

  // Initialize a process-shared barrier in the shared page.
  barrier = reinterpret_cast<pthread_barrier_t*>(shared_map);
  EXPECT_EQ(0, pthread_barrierattr_init(&attr));
  EXPECT_EQ(0, pthread_barrierattr_setpshared(&attr, PTHREAD_PROCESS_SHARED));
  EXPECT_EQ(0, pthread_barrier_init(barrier, &attr, 2));
  EXPECT_EQ(0, pthread_barrierattr_destroy(&attr));

  pid_t child = fork();
  if (child == 0) {
    EXPECT_EQ(0, pthread_barrier_wait(barrier));
    exit(0);
  }
  // Wait a bit for the child to block on the barrier.
  usleep(100000);
  EXPECT_EQ(PTHREAD_BARRIER_SERIAL_THREAD, pthread_barrier_wait(barrier));

  int status;
  wait(&status);
  EXPECT_TRUE(WIFEXITED(status));
  EXPECT_EQ(0, WEXITSTATUS(status));
}
