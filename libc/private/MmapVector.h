/*
 * Copyright (C) 2018 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#pragma once

#include <errno.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <sys/prctl.h>
#include <sys/user.h>

#include <async_safe/log.h>

#include "bionic_macros.h"

template <typename T> class MmapVector;

// Inserting an element anywhere in a vector invalidates all of its iterators,
// so just wrap a pointer.
template <typename T>
class MmapVectorIterator {
public:
  T& operator*() { return *ptr_; }
  T* operator->() { return ptr_; }
  bool operator==(MmapVectorIterator other) { return ptr_ == other.ptr_; }
  bool operator!=(MmapVectorIterator other) { return ptr_ != other.ptr_; }
  ptrdiff_t operator-(MmapVectorIterator other) { return ptr_ - other.ptr_; }
  MmapVectorIterator operator+(ptrdiff_t n) { return MmapVectorIterator(ptr_ + n); }
  MmapVectorIterator operator-(ptrdiff_t n) { return MmapVectorIterator(ptr_ - n); }

private:
  friend class MmapVector<T>;
  explicit MmapVectorIterator(T* ptr) : ptr_(ptr) {}

  T* ptr_;
};

// An mmap-backed std::vector subset with few methods. Only supports trivial
// C++ classes (no constructors/destructors).
template <typename T>
class MmapVector {
public:
  // If T were allowed to have non-trivial constructors or destructors, this
  // class would need to call them. Use the __is_trivial built-in because we
  // don't have <type_traits>.
  static_assert(__is_trivial(T), "T must be a trivial type");

  using iterator = MmapVectorIterator<T>;

  // A constexpr ctor ensures that no dynamic initialization is needed.
  constexpr MmapVector() {}

  ~MmapVector() {
    if (data_) {
      munmap(data_, mmap_size(capacity_));
    }
  }

  T& operator[](size_t index) {
    return data_[index];
  }

  iterator begin() { return iterator(&data_[0]); }
  iterator end() { return iterator(&data_[size_]); }
  size_t size() { return size_; }
  void reserve(size_t val);
  void insert(iterator pos, const T& val);
  void erase(iterator pos);
  iterator lower_bound(const T& val);

private:
  static size_t mmap_size(size_t capacity) {
    return __BIONIC_ALIGN(capacity * sizeof(T), PAGE_SIZE);
  }

  T* data_ = nullptr;
  size_t size_ = 0;
  size_t capacity_ = 0;

  DISALLOW_COPY_AND_ASSIGN(MmapVector);
};

template <typename T>
void MmapVector<T>::reserve(size_t requested) {
  if (requested <= capacity_) {
    return;
  }
  size_t new_capacity = MAX(requested, capacity_ * 2);
  size_t new_bytes = mmap_size(new_capacity);
  new_capacity = new_bytes / sizeof(T);
  if (new_capacity < requested) {
    async_safe_fatal("size overflow");
  }

  void* ptr = mmap(nullptr, new_bytes, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
  if (ptr == MAP_FAILED) {
    async_safe_fatal("mmap failed: %s", strerror(errno));
  }

  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, ptr, new_bytes, "bionic_mmap_vector");

  if (data_ != nullptr) {
    memcpy(ptr, data_, size_ * sizeof(T));
    munmap(data_, mmap_size(capacity_));
  }

  data_ = static_cast<T*>(ptr);
  capacity_ = new_capacity;
}

template <typename T>
void MmapVector<T>::insert(iterator pos, const T& val) {
  size_t idx = pos - begin();
  reserve(size() + 1);
  if (idx < size_) {
    memmove(&data_[idx + 1], &data_[idx], (size_ - idx) * sizeof(T));
  }
  data_[idx] = val;
  size_++;
}

template <typename T>
void MmapVector<T>::erase(iterator pos) {
  size_t idx = pos - begin();
  if (idx + 1 < size_) {
    memmove(&data_[idx], &data_[idx + 1], (size_ - idx - 1) * sizeof(T));
  }
  memset(&data_[size_], 0, sizeof(T));
  size_--;
}

// Return the first index of the vector that is not less than the given value
// or size() if no such index exists.
template <typename T>
MmapVectorIterator<T> MmapVector<T>::lower_bound(const T& val) {
  size_t i0 = 0;
  size_t i1 = size_;
  while (i0 < i1) {
    // On each loop iteration, i1 satisfies the function's requirement, but it
    // might not be the first good index. We need to test the range [i0, i1).
    const size_t mid = i0 + (i1 - i0) / 2;
    if (data_[mid] < val) {
      i0 = mid + 1;
    } else {
      i1 = mid;
    }
  }
  return begin() + i0;
}
