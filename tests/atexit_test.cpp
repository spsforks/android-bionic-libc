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

#include <gtest/gtest.h>

#include <dlfcn.h>
#include <libgen.h>
#include <limits.h>
#include <stdio.h>
#include <stdint.h>

#include <string>

TEST(atexit, dlclose) {
  std::string atexit_call_sequence;
  bool valid_this_in_static_dtor = false;
  void* handle = dlopen("libtest_atexit.so", RTLD_NOW);
  ASSERT_TRUE(handle != NULL);

  void* sym = dlsym(handle, "register_atexit");
  ASSERT_TRUE(sym != NULL);
  reinterpret_cast<void (*)(std::string*, bool*)>(sym)(&atexit_call_sequence, &valid_this_in_static_dtor);

  ASSERT_EQ(0, dlclose(handle));
  // this test verifies atexit call from atexit handler. as well as the order of calls
  ASSERT_EQ("Humpty Dumpty sat on a wall", atexit_call_sequence);
  ASSERT_TRUE(valid_this_in_static_dtor);
}

class TestMainStaticDtorClass {
 public:
  TestMainStaticDtorClass() {
    expected_this = this;
  }

  ~TestMainStaticDtorClass() {
    if (this != expected_this) {
      fprintf(stderr, "\nerror: static d-tor called with incorrect this pointer: %p, expected: %p\n", this, expected_this);
    } else {
      fprintf(stderr, "6");
    }
  }
 private:
  static const TestMainStaticDtorClass* expected_this;
};

const TestMainStaticDtorClass* TestMainStaticDtorClass::expected_this = NULL;

static void atexit_func5() {
  fprintf(stderr, "5");
}

static void atexit_func4() {
  fprintf(stderr, "4");
}

static void atexit_func3() {
  fprintf(stderr, "3");
  atexit(atexit_func4);
}

static void atexit_func2() {
  fprintf(stderr, "2");
}

static void atexit_func1() {
  fprintf(stderr, "1");
}

static void atexit_main() {
  // This should result in "123456" output to stderr
  static TestMainStaticDtorClass static_obj;
  atexit(atexit_func5);
  atexit(atexit_func3);
  atexit(atexit_func2);
  atexit(atexit_func1);
  exit(0);
}

TEST(atexit, exit) {
  ASSERT_EXIT(atexit_main(), testing::ExitedWithCode(0), "123456");
}

static int
check_library(const char*  libname)
{
    void*  lib = dlopen(libname, RTLD_NOW);

    int*   to_x;
    void  (*set_y)(int *);
    int    y = 0;

    if(lib == NULL){
       return 1;
    }

    to_x =(int *)dlsym(lib, "x");
    if (to_x == NULL){
       return 2;
    }

    if (*to_x != 1){
       return 3;
    }

    set_y =(void (*)(int*))dlsym(lib, "set_y");
    if (set_y == NULL){
       return 4;
    }

    y = 0;
    (*set_y)(&y);

    if (dlclose(lib) < 0){
        return 5;
    }

    if(y!=2){
       return 6;
    }
    return 0;
}

TEST(atexit, dlclose_destruction2)
{
   ASSERT_EQ(0,check_library("libdlclosetest2.so"));
}

static int
check_library1(const char*  libname)
{
    void*  lib = dlopen(libname, RTLD_NOW);

    int*   to_x;

    if(lib == NULL){
       return 1;
    }

    to_x =(int *)dlsym(lib, "x");
    if (to_x == NULL){
       return 2;
    }

    if (*to_x != 1){
       return 3;
    }

    if (dlclose(lib) < 0){
        return 4;
    }

    return 0;
}

TEST(atexit, dlclose_destruction1)
{
   ASSERT_EQ(0,check_library1("libdlclosetest1.so"));
}
