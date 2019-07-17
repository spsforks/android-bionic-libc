/*
 * Copyright (C) 2019 The Android Open Source Project
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

#ifdef __BIONIC_PTHREAD_COND_INLINE
#define COND_TEST(test_case, name) TEST(test_case, name##_header)
#define COND_TEST_F(fixture, name) TEST_F(fixture, name##_header)
#else
#define COND_TEST(test_case, name) TEST(test_case, name)
#define COND_TEST_F(fixture, name) TEST_F(fixture, name)
#endif

COND_TEST(pthread, pthread_condattr_init) {
  pthread_condattr_t attr;
  pthread_condattr_init(&attr);

  clockid_t clock;
  ASSERT_EQ(0, pthread_condattr_getclock(&attr, &clock));
  ASSERT_EQ(CLOCK_REALTIME, clock);

  int pshared;
  ASSERT_EQ(0, pthread_condattr_getpshared(&attr, &pshared));
  ASSERT_EQ(PTHREAD_PROCESS_PRIVATE, pshared);
}

COND_TEST(pthread, pthread_condattr_setclock) {
  pthread_condattr_t attr;
  pthread_condattr_init(&attr);

  ASSERT_EQ(0, pthread_condattr_setclock(&attr, CLOCK_REALTIME));
  clockid_t clock;
  ASSERT_EQ(0, pthread_condattr_getclock(&attr, &clock));
  ASSERT_EQ(CLOCK_REALTIME, clock);

  ASSERT_EQ(0, pthread_condattr_setclock(&attr, CLOCK_MONOTONIC));
  ASSERT_EQ(0, pthread_condattr_getclock(&attr, &clock));
  ASSERT_EQ(CLOCK_MONOTONIC, clock);

  ASSERT_EQ(EINVAL, pthread_condattr_setclock(&attr, CLOCK_PROCESS_CPUTIME_ID));
}

COND_TEST(pthread, pthread_cond_broadcast__preserves_condattr_flags) {
#if defined(__BIONIC__)
  pthread_condattr_t attr;
  pthread_condattr_init(&attr);

  ASSERT_EQ(0, pthread_condattr_setclock(&attr, CLOCK_MONOTONIC));
  ASSERT_EQ(0, pthread_condattr_setpshared(&attr, PTHREAD_PROCESS_SHARED));

  pthread_cond_t cond_var;
  ASSERT_EQ(0, pthread_cond_init(&cond_var, &attr));

  ASSERT_EQ(0, pthread_cond_signal(&cond_var));
  ASSERT_EQ(0, pthread_cond_broadcast(&cond_var));

  attr = static_cast<pthread_condattr_t>(*reinterpret_cast<uint32_t*>(cond_var.__private));
  clockid_t clock;
  ASSERT_EQ(0, pthread_condattr_getclock(&attr, &clock));
  ASSERT_EQ(CLOCK_MONOTONIC, clock);
  int pshared;
  ASSERT_EQ(0, pthread_condattr_getpshared(&attr, &pshared));
  ASSERT_EQ(PTHREAD_PROCESS_SHARED, pshared);
#else  // !defined(__BIONIC__)
  GTEST_SKIP() << "bionic-only test";
#endif  // !defined(__BIONIC__)
}

class pthread_CondWakeupTest : public ::testing::Test {
 protected:
  pthread_mutex_t mutex;
  pthread_cond_t cond;

  enum Progress {
    INITIALIZED,
    WAITING,
    SIGNALED,
    FINISHED,
  };
  std::atomic<Progress> progress;
  pthread_t thread;
  std::function<int (pthread_cond_t* cond, pthread_mutex_t* mutex)> wait_function;

 protected:
  void SetUp() override {
    ASSERT_EQ(0, pthread_mutex_init(&mutex, nullptr));
  }

  void InitCond(clockid_t clock=CLOCK_REALTIME) {
    pthread_condattr_t attr;
    ASSERT_EQ(0, pthread_condattr_init(&attr));
    ASSERT_EQ(0, pthread_condattr_setclock(&attr, clock));
    ASSERT_EQ(0, pthread_cond_init(&cond, &attr));
    ASSERT_EQ(0, pthread_condattr_destroy(&attr));
  }

  void StartWaitingThread(
      std::function<int(pthread_cond_t* cond, pthread_mutex_t* mutex)> wait_function) {
    progress = INITIALIZED;
    this->wait_function = wait_function;
    ASSERT_EQ(0, pthread_create(&thread, nullptr, reinterpret_cast<void* (*)(void*)>(WaitThreadFn),
                                this));
    while (progress != WAITING) {
      usleep(5000);
    }
    usleep(5000);
  }

  void RunTimedTest(
      clockid_t clock,
      std::function<int(pthread_cond_t* cond, pthread_mutex_t* mutex, const timespec* timeout)>
          wait_function) {
    timespec ts;
    ASSERT_EQ(0, clock_gettime(clock, &ts));
    ts.tv_sec += 1;

    StartWaitingThread([&wait_function, &ts](pthread_cond_t* cond, pthread_mutex_t* mutex) {
      return wait_function(cond, mutex, &ts);
    });

    progress = SIGNALED;
    ASSERT_EQ(0, pthread_cond_signal(&cond));
  }

  void RunTimedTest(clockid_t clock, std::function<int(pthread_cond_t* cond, pthread_mutex_t* mutex,
                                                       clockid_t clock, const timespec* timeout)>
                                         wait_function) {
    RunTimedTest(clock, [clock, &wait_function](pthread_cond_t* cond, pthread_mutex_t* mutex,
                                                const timespec* timeout) {
      return wait_function(cond, mutex, clock, timeout);
    });
  }

  void TearDown() override {
    ASSERT_EQ(0, pthread_join(thread, nullptr));
    ASSERT_EQ(FINISHED, progress);
    ASSERT_EQ(0, pthread_cond_destroy(&cond));
    ASSERT_EQ(0, pthread_mutex_destroy(&mutex));
  }

 private:
  static void WaitThreadFn(pthread_CondWakeupTest* test) {
    ASSERT_EQ(0, pthread_mutex_lock(&test->mutex));
    test->progress = WAITING;
    while (test->progress == WAITING) {
      ASSERT_EQ(0, test->wait_function(&test->cond, &test->mutex));
    }
    ASSERT_EQ(SIGNALED, test->progress);
    test->progress = FINISHED;
    ASSERT_EQ(0, pthread_mutex_unlock(&test->mutex));
  }
};

COND_TEST_F(pthread_CondWakeupTest, signal_wait) {
  InitCond();
  StartWaitingThread([](pthread_cond_t* cond, pthread_mutex_t* mutex) {
    return pthread_cond_wait(cond, mutex);
  });
  progress = SIGNALED;
  ASSERT_EQ(0, pthread_cond_signal(&cond));
}

COND_TEST_F(pthread_CondWakeupTest, broadcast_wait) {
  InitCond();
  StartWaitingThread([](pthread_cond_t* cond, pthread_mutex_t* mutex) {
    return pthread_cond_wait(cond, mutex);
  });
  progress = SIGNALED;
  ASSERT_EQ(0, pthread_cond_broadcast(&cond));
}

COND_TEST_F(pthread_CondWakeupTest, signal_timedwait_CLOCK_REALTIME) {
  InitCond(CLOCK_REALTIME);
  RunTimedTest(CLOCK_REALTIME, pthread_cond_timedwait);
}

COND_TEST_F(pthread_CondWakeupTest, signal_timedwait_CLOCK_MONOTONIC) {
  InitCond(CLOCK_MONOTONIC);
  RunTimedTest(CLOCK_MONOTONIC, pthread_cond_timedwait);
}

COND_TEST_F(pthread_CondWakeupTest, signal_timedwait_CLOCK_MONOTONIC_np) {
#if defined(__BIONIC__)
  InitCond(CLOCK_REALTIME);
  RunTimedTest(CLOCK_MONOTONIC, pthread_cond_timedwait_monotonic_np);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_timedwait_monotonic_np not available";
#endif  // __BIONIC__
}

COND_TEST_F(pthread_CondWakeupTest, signal_clockwait_monotonic_monotonic) {
#if defined(__BIONIC__)
  InitCond(CLOCK_MONOTONIC);
  RunTimedTest(CLOCK_MONOTONIC, pthread_cond_clockwait);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_clockwait not available";
#endif  // __BIONIC__
}

COND_TEST_F(pthread_CondWakeupTest, signal_clockwait_monotonic_realtime) {
#if defined(__BIONIC__)
  InitCond(CLOCK_MONOTONIC);
  RunTimedTest(CLOCK_REALTIME, pthread_cond_clockwait);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_clockwait not available";
#endif  // __BIONIC__
}

COND_TEST_F(pthread_CondWakeupTest, signal_clockwait_realtime_monotonic) {
#if defined(__BIONIC__)
  InitCond(CLOCK_REALTIME);
  RunTimedTest(CLOCK_MONOTONIC, pthread_cond_clockwait);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_clockwait not available";
#endif  // __BIONIC__
}

COND_TEST_F(pthread_CondWakeupTest, signal_clockwait_realtime_realtime) {
#if defined(__BIONIC__)
  InitCond(CLOCK_REALTIME);
  RunTimedTest(CLOCK_REALTIME, pthread_cond_clockwait);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_clockwait not available";
#endif  // __BIONIC__
}

static void pthread_cond_timedwait_timeout_helper(bool init_monotonic, clockid_t clock,
                                                  int (*wait_function)(pthread_cond_t* __cond,
                                                                       pthread_mutex_t* __mutex,
                                                                       const timespec* __timeout)) {
  pthread_mutex_t mutex;
  ASSERT_EQ(0, pthread_mutex_init(&mutex, nullptr));
  pthread_cond_t cond;

  if (init_monotonic) {
    pthread_condattr_t attr;
    pthread_condattr_init(&attr);

    ASSERT_EQ(0, pthread_condattr_setclock(&attr, CLOCK_MONOTONIC));
    clockid_t clock;
    ASSERT_EQ(0, pthread_condattr_getclock(&attr, &clock));
    ASSERT_EQ(CLOCK_MONOTONIC, clock);

    ASSERT_EQ(0, pthread_cond_init(&cond, &attr));
  } else {
    ASSERT_EQ(0, pthread_cond_init(&cond, nullptr));
  }
  ASSERT_EQ(0, pthread_mutex_lock(&mutex));

  timespec ts;
  ASSERT_EQ(0, clock_gettime(clock, &ts));
  ASSERT_EQ(ETIMEDOUT, wait_function(&cond, &mutex, &ts));
  ts.tv_nsec = -1;
  ASSERT_EQ(EINVAL, wait_function(&cond, &mutex, &ts));
  ts.tv_nsec = NS_PER_S;
  ASSERT_EQ(EINVAL, wait_function(&cond, &mutex, &ts));
  ts.tv_nsec = NS_PER_S - 1;
  ts.tv_sec = -1;
  ASSERT_EQ(ETIMEDOUT, wait_function(&cond, &mutex, &ts));
  ASSERT_EQ(0, pthread_mutex_unlock(&mutex));
}

COND_TEST(pthread, pthread_cond_timedwait_timeout) {
  pthread_cond_timedwait_timeout_helper(false, CLOCK_REALTIME, pthread_cond_timedwait);
}

COND_TEST(pthread, pthread_cond_timedwait_monotonic_np_timeout) {
#if defined(__BIONIC__)
  pthread_cond_timedwait_timeout_helper(false, CLOCK_MONOTONIC, pthread_cond_timedwait_monotonic_np);
  pthread_cond_timedwait_timeout_helper(true, CLOCK_MONOTONIC, pthread_cond_timedwait_monotonic_np);
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_timedwait_monotonic_np not available";
#endif  // __BIONIC__
}

COND_TEST(pthread, pthread_cond_clockwait_timeout) {
#if defined(__BIONIC__)
  pthread_cond_timedwait_timeout_helper(
      false, CLOCK_MONOTONIC,
      [](pthread_cond_t* __cond, pthread_mutex_t* __mutex, const timespec* __timeout) {
        return pthread_cond_clockwait(__cond, __mutex, CLOCK_MONOTONIC, __timeout);
      });
  pthread_cond_timedwait_timeout_helper(
      true, CLOCK_MONOTONIC,
      [](pthread_cond_t* __cond, pthread_mutex_t* __mutex, const timespec* __timeout) {
        return pthread_cond_clockwait(__cond, __mutex, CLOCK_MONOTONIC, __timeout);
      });
  pthread_cond_timedwait_timeout_helper(
      false, CLOCK_REALTIME,
      [](pthread_cond_t* __cond, pthread_mutex_t* __mutex, const timespec* __timeout) {
        return pthread_cond_clockwait(__cond, __mutex, CLOCK_REALTIME, __timeout);
      });
  pthread_cond_timedwait_timeout_helper(
      true, CLOCK_REALTIME,
      [](pthread_cond_t* __cond, pthread_mutex_t* __mutex, const timespec* __timeout) {
        return pthread_cond_clockwait(__cond, __mutex, CLOCK_REALTIME, __timeout);
      });
#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_clockwait not available";
#endif  // __BIONIC__
}

COND_TEST(pthread, pthread_cond_clockwait_invalid) {
#if defined(__BIONIC__)
  pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
  pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
  timespec ts;
  EXPECT_EQ(EINVAL, pthread_cond_clockwait(&cond, &mutex, CLOCK_PROCESS_CPUTIME_ID, &ts));

#else   // __BIONIC__
  GTEST_SKIP() << "pthread_cond_clockwait not available";
#endif  // __BIONIC__
}
