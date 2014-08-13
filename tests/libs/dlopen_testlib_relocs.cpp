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

/* this is part of the test_relocs.c test, which is used to check that
 * the relocations generated in a shared object are properly handled
 * by the Bionic dynamic linker
 */

struct foo { int first, second; };
struct foo Foo = {1, 2};

int* FooPtr[] = { &Foo.first, &Foo.second };

extern "C" int relocs_func1() {
  return *FooPtr[0];
}

extern "C" int relocs_func2() {
  return *FooPtr[1];
}
