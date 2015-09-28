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

#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <string.h>
#include <sys/cdefs.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/user.h>

#include "private/bionic_prctl.h"
#include "private/libc_logging.h"

template <typename T>
class WPVectorMutator;

struct WPVectorHeader {
  size_t elementCount;  // in elements
  size_t mappingSize;   // in bytes
  ssize_t mutatorCount;
  ssize_t padding;
};

// This class implements a write protected vector backed by a single memory
// mapping. By default, the header information and contents are mprotected to
// be non-writable. Calling mutate() will conclass a mutator object through
// which modifications to the vector can be made. This class is not threadsafe.
template <typename T>
class WPVector {
  friend class WPVectorMutator<T>;
  // TODO: Implement for non-trivially copyable types?
  // std::is_trivially_copyable<T> isn't available.
  static_assert(__has_trivial_copy(T),
                "unimplemented for non-trivially copyable types");

  // Stored at the beginning of the mapping when created by mutator's reserve.
  const WPVectorHeader* header = nullptr;

 public:
  typedef T* iterator;
  typedef const T* const_iterator;

  WPVector() {
  }

  WPVector(const WPVector& copy) = delete;
  WPVector(WPVector&& move) = delete;

  WPVector& operator=(const WPVector& copy) = delete;
  WPVector& operator=(WPVector&& move) = delete;

  ~WPVector() {
    if (header) {
      munmap(const_cast<WPVectorHeader*>(header), header->mappingSize);
      header = nullptr;
    }
  }

  // Implementation needed for placement new, since we don't have <new>.
  static void* operator new(size_t, void* p) {
    return p;
  }

  // TODO: Allow storing the object in the same page as the data?
  static WPVector* allocate() {
    void* page = mmap(0, sizeof(WPVector), PROT_READ | PROT_WRITE,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (page == MAP_FAILED) {
      __libc_fatal("failed to allocate space for WPVector: %s", strerror(errno));
    }

    WPVector* result = reinterpret_cast<WPVector*>(page);
    new (result) WPVector();
    return result;
  }

  static void deallocate(WPVector* vec) {
    vec->~WPVector();
    munmap(vec, sizeof(WPVector));
  }

  void set_mapping_name(const char* name) {
    if (header) {
      prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, header, header->mappingSize, name);
    }
  }

  WPVectorMutator<T> mutate() {
    return WPVectorMutator<T>(*this);
  }

  const T& at(size_t i) const {
    if (!header) {
      __libc_fatal("attempted to get element %zu of an empty WPVector", i);
    }

    if (i >= size()) {
      __libc_fatal("attempted to get element %zu of a %zu element WPVector", i,
                   size());
    }

    return data()[i];
  }

  const T& operator[](size_t i) const {
    return at(i);
  }

  const T& front() const {
    return at(0);
  }

  const T& back() const {
    return at(size() - 1);
  }

  const T* data() const {
    if (!header) {
      return nullptr;
    }

    return reinterpret_cast<const T*>(header + 1);
  }

  const_iterator begin() const {
    return data();
  }

  const_iterator end() const {
    return data() + size();
  }

  const_iterator cbegin() const {
    return data();
  }

  const_iterator cend() const {
    return data() + size();
  }

  bool empty() const {
    return size() == 0;
  }

  size_t size() const {
    if (!header) {
      return 0;
    }
    return header->elementCount;
  }

  size_t max_size() const {
    return (PTRDIFF_MAX - sizeof(WPVectorHeader)) / sizeof(T);
  }

  // TODO: Implement capacity.
};

template <typename T>
class WPVectorMutator {
  WPVector<T>& vec;
  WPVectorHeader*& header;

 public:
  typedef typename WPVector<T>::iterator iterator;
  typedef typename WPVector<T>::const_iterator const_iterator;

  explicit WPVectorMutator(WPVector<T>& vector)
      : vec(vector), header(const_cast<WPVectorHeader*&>(vec.header)) {
    // Ensure that the header exists.
    if (!header) {
      reserve(1);
    } else if (header->mutatorCount == 0) {
      if (mprotect(header, header->mappingSize, PROT_READ | PROT_WRITE) != 0) {
        __libc_fatal("WPVectorMutator failed to make mapping writable: %s",
                     strerror(errno));
      }
    }

    ++header->mutatorCount;
  }

  WPVectorMutator(const WPVectorMutator& copy) = delete;
  WPVectorMutator(WPVectorMutator&& move) = default;
  WPVectorMutator& operator=(const WPVectorMutator& copy) = delete;
  WPVectorMutator& operator=(WPVectorMutator&& move) = default;

  ~WPVectorMutator() {
    if (header && --header->mutatorCount == 0) {
      if (mprotect(header, header->mappingSize, PROT_READ)) {
        __libc_fatal("WPVectorMutator failed to make mapping nonwritable: %s",
                     strerror(errno));
      }
    }
  }

 private:
  // Move the tail of the vector, adding or removing elements as necessary.
  // New elements will be in an uninitialized state if inserted.
  void move_tail(iterator src, ptrdiff_t offset) {
    if (src < begin() || src > end()) {
      __libc_fatal(
        "WPVectorMutator::move_tail received out of bounds iterators");
    }

    if (offset == 0) {
      return;
    }

    size_t src_index = src - begin();
    iterator dst;
    if (offset < 0) {
      if (src_index < static_cast<size_t>(-offset)) {
        __libc_fatal(
          "WPVectorMutator::move_tail attempted to erase past the beginning");
      }

      // Erase the elements between src and dst.
      dst = src + offset;
      for (auto it = dst; it < src; ++it) {
        it->~T();
        memset(it, 0, sizeof(*it));
      }
    } else {
      // Insert new elements between src and dst.
      size_t src_index = src - begin();
      reserve(size() + offset);

      // Recalculate src, since reserve might invalidate it.
      src = begin() + src_index;
      dst = src + offset;
    }

    memmove(dst, src, (end() - src) * sizeof(T));
    header->elementCount += offset;
  }

 public:
  T& at(size_t i) {
    if (i >= size()) {
      __libc_fatal("attempted to get element %zu of a %zu element WPVector", i,
                   size());
    }

    return data()[i];
  }

  const T& at(size_t i) const {
    if (i >= size()) {
      __libc_fatal("attempted to get element %zu of a %zu element WPVector", i,
                   size());
    }

    return data()[i];
  }

  T& operator[](size_t i) {
    return at(i);
  }

  const T& operator[](size_t i) const {
    return at(i);
  }

  T& front() {
    return at(0);
  }

  const T& front() const {
    return at(0);
  }

  T& back() {
    return at(size() - 1);
  }

  const T& back() const {
    return at(size() - 1);
  }

  T* data() {
    return const_cast<T*>(vec.data());
  }

  const T* data() const {
    return vec.data();
  }

  iterator begin() {
    return data();
  }

  const_iterator begin() const {
    return data();
  }

  const_iterator cbegin() const {
    return data();
  }

  iterator end() {
    return begin() + size();
  }

  const_iterator end() const {
    return cbegin() + size();
  }

  const_iterator cend() const {
    return cbegin() + size();
  }

  bool empty() const {
    return vec.empty();
  }

  size_t size() const {
    return vec.size();
  }

  size_t max_size() const {
    return vec.max_size();
  }

  void reserve(size_t count) {
    size_t needed;
    if (__size_mul_overflow(count, sizeof(T), &needed)) {
      __libc_fatal("WPVectorMutator::reserve(%zu) mul overflowed", count);
    }

    if (needed > PTRDIFF_MAX) {
      __libc_fatal("WPVectorMutator::reserve(%zu) resizing to > PTRDIFF_MAX",
                   count);
    }

    if (__size_add_overflow(needed, sizeof(WPVectorHeader), &needed)) {
      __libc_fatal("WPVectorMutator::reserve(%zu) add overflowed", count);
    }

    // Round up to page size.
    if (needed & (PAGE_SIZE - 1)) {
      needed += PAGE_SIZE - (needed & (PAGE_SIZE - 1));
    }

    if (!header) {
      WPVectorHeader* new_header = static_cast<WPVectorHeader*>(
        mmap(nullptr, needed, PROT_READ | PROT_WRITE,
             MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));

      if (new_header == MAP_FAILED) {
        __libc_fatal("WPVector failed to allocate mapping of size %d",
                     PAGE_SIZE);
      }

      new_header->elementCount = 0;
      new_header->mappingSize = needed;
      new_header->mutatorCount = 0;
      new_header->padding = -1;
      header = new_header;
    } else if (needed > header->mappingSize) {
      void* result = mremap(header, header->mappingSize, needed, MREMAP_MAYMOVE);
      if (result == MAP_FAILED) {
        __libc_fatal(
          "WPVectorMutator failed to resize to capacity %zu"
          " [requested: %zu]",
          count, needed);
      }

      header = reinterpret_cast<WPVectorHeader*>(result);
      header->mappingSize = needed;
    }
  }

  // TODO: Implement capacity.

  void clear() {
    erase(begin(), end());
  }

  // TODO: Implement initializer_list version.
  iterator insert(const_iterator pos, size_t count, const T& value) {
    if (pos < cbegin()) {
      __libc_fatal(
        "WPVectorMutator::insert received iterator before the "
        "beginning");
    } else if (pos > cend()) {
      __libc_fatal(
        "WPVectorMutator::insert received iterator after one past "
        "the end");
    }

    size_t pos_index = pos - cbegin();
    move_tail(const_cast<iterator>(pos), count);
    // Recalculate pos, since reserve or move_tail might realloc
    pos = cbegin() + pos_index;
    for (auto it = const_cast<iterator>(pos); it < pos + count; ++it) {
      *it = value;
    }

    return begin() + pos_index;
  }

  iterator insert(const_iterator pos, const T& value) {
    return insert(pos, 1, value);
  }

  // Erases elements [first, last).
  iterator erase(const_iterator first, const_iterator last) {
    if (first == last) {
      return const_cast<iterator>(last);
    }

    if (first > last || first >= cend() || first < cbegin() || last > cend()) {
      __libc_fatal("WPVectorMutator::erase received invalid iterators");
    }

    move_tail(const_cast<iterator>(last), first - last);
    return const_cast<iterator>(first);
  }

  iterator erase(const_iterator pos) {
    return erase(pos, pos + 1);
  }

  // TODO: && version of push_back.
  void push_back(const T& value) {
    insert(cend(), value);
  }

  // TODO: Implement emplace_back.

  void pop_back() {
    erase(cend() - 1);
  }

  void resize(size_t new_size) {
    size_t old_size = size();
    move_tail(end(), new_size - old_size);

    iterator b = begin();
    if (new_size > old_size) {
      for (size_t i = old_size; i < new_size; ++i) {
        *(b + i) = T();
      }
    }
  }
};
