// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "leb128.h"

#include <limits.h>
#include <stdint.h>
#include <vector>

#include "elf_traits.h"

namespace {

template <typename T>
class uint_traits {};

template <>
class uint_traits<uint64_t> {
 public:
  typedef int64_t int_t;
};

template <>
class uint_traits<uint32_t> {
 public:
  typedef int32_t int_t;
};

}

namespace relocation_packer {

// Add a single value to the encoding.  Values are encoded with variable
// length.  The least significant 7 bits of each byte hold 7 bits of data,
// and the most significant bit is set on each byte except the last.  The
// value is sign extended up to a multiple of 7 bits (ensuring that the
// most significant bit is zero for a positive number and one for a
// negative number).
template <typename uint_t>
void Leb128Encoder<uint_t>::Enqueue(uint_t value) {
  do {
    uint8_t byte = value & 127;
    value >>= 7;

    if (value != 0)
      byte |= 128;
    encoding_.push_back(byte);
  } while (value);
}

// Add a vector of values to the encoding.
template <typename uint_t>
void Leb128Encoder<uint_t>::EnqueueAll(const std::vector<uint_t>& values) {
  for (size_t i = 0; i < values.size(); ++i) {
    Enqueue(values[i]);
  }
}

// Create a new decoder for the given encoded stream.
template <typename uint_t>
Leb128Decoder<uint_t>::Leb128Decoder(const std::vector<uint8_t>& encoding, size_t start_with) {
  encoding_ = encoding;
  cursor_ = start_with;
}

// Decode and retrieve a single value from the encoding.  Consume bytes
// until one without its most significant bit is found, and re-form the
// value from the 7 bit fields of the bytes consumed.
template <typename uint_t>
uint_t Leb128Decoder<uint_t>::Dequeue() {
  uint_t value = 0;
  size_t shift = 0;
  uint8_t byte;

  // Loop until we reach a byte with its high order bit clear.
  do {
    byte = encoding_[cursor_++];
    value |= (static_cast<uint_t>(byte & 127) << shift);
    shift += 7;
  } while (byte & 128);

  return static_cast<uint_t>(value);
}

// Decode and retrieve all remaining values from the encoding.
template <typename uint_t>
void Leb128Decoder<uint_t>::DequeueAll(std::vector<uint_t>* values) {
  while (cursor_ < encoding_.size()) {
    values->push_back(Dequeue());
  }
}

template class Leb128Encoder<uint32_t>;
template class Leb128Encoder<uint64_t>;
template class Leb128Decoder<uint32_t>;
template class Leb128Decoder<uint64_t>;

}  // namespace relocation_packer
