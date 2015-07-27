/*
 * Copyright (C) 2015 The Android Open Source Project
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

__thread int local_var = 100;
int shared_var = 200;

static void reset_vars() {
  local_var = 1000;
  shared_var = 2000;
  // local_var should be reset by threads
}

// If input *(int*)p is 1, increase shared_var, otherwise local_var.
// Returns increased shared_var or local_var in *(int*)p;
static void* add_one(void *p) {
  int *data = (int*)p;
  if (*data == 1) {
    shared_var++;
    *data = shared_var;
  } else {
    local_var++;
    *data = local_var;
  }
  return nullptr;
}

static int run_one_thread(int n) {
  pthread_t t;
  int data = n;
  int error = pthread_create(&t, nullptr, add_one, &data);
  if (!error)
      error = pthread_join(t, nullptr);
  return error ? error : data;
}

TEST(thread_local_storage, shared) {
  reset_vars();
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2000);

  // Update shared_var, local_var remains 1000.
  ASSERT_EQ(run_one_thread(1), 2001);
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2001);

  ASSERT_EQ(run_one_thread(1), 2002);
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2002);

  ASSERT_EQ(run_one_thread(1), 2003);
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2003);
}

TEST(thread_local_storage, local) {
  reset_vars();
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2000);

  // When a child thread updates its own TLS variable,
  // this thread's local_var and shared_var are not changed.
  // TLS local_var is initialized to 100 in a thread.
  ASSERT_EQ(run_one_thread(2), 101);
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2000);

  ASSERT_EQ(run_one_thread(2), 101);
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2000);

  ASSERT_EQ(run_one_thread(2), 101);
  ASSERT_EQ(local_var, 1000);
  ASSERT_EQ(shared_var, 2000);
}

// Another test of TLS initialization, struct type.
struct Point {
  int x, y;
};

typedef Point Triangle[3];

__thread Triangle local_triangle = {{10,10}, {20,20}, {30,30}};
Triangle shared_triangle = {{1,1}, {2,2}, {3,3}};

static void reset_triangle() {
  static const Triangle t1 = {{3,3}, {4,4}, {5,5}};
  static const Triangle t2 = {{2,2}, {3,3}, {4,4}};
  memcpy(local_triangle, t1, sizeof(local_triangle));
  memcpy(shared_triangle, t2, sizeof(shared_triangle));
}

static void* move_triangle(void *p) {
  int *data = (int*)p;
  if (*data == 1) {
    shared_triangle[1].y++;
    *data = shared_triangle[1].y;
  } else {
    local_triangle[1].y++;
    *data = local_triangle[1].y;
  }
  return nullptr;
}

static int move_one_triangle(int n) {
  pthread_t t;
  int data = n;
  int error = pthread_create(&t, nullptr, move_triangle, &data);
  if (!error)
      error = pthread_join(t, nullptr);
  return error ? error : data;
}

TEST(thread_local_storage, shared_triangle) {
  reset_triangle();
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 3);

  // Update shared_triangle, local_triangle remains 1000.
  ASSERT_EQ(move_one_triangle(1), 4);
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 4);

  ASSERT_EQ(move_one_triangle(1), 5);
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 5);

  ASSERT_EQ(move_one_triangle(1), 6);
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 6);
}

TEST(thread_local_storage, local_triangle) {
  reset_triangle();
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 3);

  // Update local_triangle, parent thread's
  // shared_triangle and local_triangle are unchanged.
  ASSERT_EQ(move_one_triangle(2), 21);
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 3);

  ASSERT_EQ(move_one_triangle(2), 21);
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 3);

  ASSERT_EQ(move_one_triangle(2), 21);
  ASSERT_EQ(local_triangle[1].y, 4);
  ASSERT_EQ(shared_triangle[1].y, 3);
}
