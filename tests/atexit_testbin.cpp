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

static const TestMainStaticDtorClass* valid_this = NULL;

static class TestMainStaticDtorClass {
public:
  TestMainStaticDtorClass() {
    valid_this = this;
  }

  ~TestMainStaticDtorClass() {
    if (this != valid_this) {
      printf("\nerror: invalid this in static d-tor: %p, expected: %p\n", this, valid_this);
    }
  }

} testMainVar;

void func5() {
  printf("5");
}

void func4() {
  printf("4");
}

void func3() {
  printf("3");
  atexit(func4);
}

void func2() {
  printf("2");
}

void func1() {
  printf("1");
}

int main() {
  // This should result in "12345"
  atexit(func5);
  atexit(func3);
  atexit(func2);
  atexit(func1);
  return 0;
}
