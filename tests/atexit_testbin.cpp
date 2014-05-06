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

#include <stdlib.h>
#include <stdio.h>

class TestMainStaticDtorClass;

static const TestMainStaticDtorClass* expected_this = NULL;

class TestMainStaticDtorClass {
public:
  TestMainStaticDtorClass() {
    expected_this = this;
  }

  ~TestMainStaticDtorClass() {
    if (this != expected_this) {
      fprintf(stderr, "\nerror: static d-tor called with incorrect his pointer: %p, expected: %p\n", this, expected_this);
    } else {
      fprintf(stderr, "6");
    }
  }

};


void func5() {
  fprintf(stderr, "5");
}

void func4() {
  fprintf(stderr, "4");
}

void func3() {
  fprintf(stderr, "3");
  atexit(func4);
}

void func2() {
  fprintf(stderr, "2");
}

void func1() {
  fprintf(stderr, "1");
}

int main() {
  // This should result in "123456" output to stderr
  static TestMainStaticDtorClass testMainVar;
  atexit(func5);
  atexit(func3);
  atexit(func2);
  atexit(func1);
  return 0;
}
