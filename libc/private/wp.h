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

#ifndef _PRIVATE_WP_H
#define _PRIVATE_WP_H

#include <sys/cdefs.h>
#include <sys/mman.h>
#include <sys/user.h>

template <typename T>
class wp_mutator;

// Write protected wrapper class that aligns its contents to a page boundary,
// and sets the memory protection to be non-writable, except when being modified
// explicitly.
template <typename T>
class wp {
  friend class wp_mutator<T>;

  static_assert(sizeof(T) < PAGE_SIZE,
                "wp only supports contents up to PAGE_SIZE");

  union {
    T value;
    char padding[PAGE_SIZE];
  } contents __attribute__((aligned(PAGE_SIZE)));

 public:
  wp() {
    mprotect(&contents, PROT_READ, sizeof(contents));
  }

  const T* operator->() {
    return &contents.value;
  }

  const T& operator*() {
    return contents.value;
  }

  wp_mutator<T> mutate() {
    return wp_mutator<T>(&contents.value);
  }
};

template <typename T>
class wp_mutator {
  T* data;

 public:
  wp_mutator(T* data) : data(data) {
    mprotect(data, PROT_READ | PROT_WRITE, sizeof(T));
  }

  ~wp_mutator() {
    mprotect(data, PROT_READ, sizeof(T));
  }

  T* operator->() {
    return data;
  }

  T& operator*() {
    return *data;
  }
};

#endif  // _PRIVATE_WP_H
