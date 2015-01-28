// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "leb128.h"

#include <stdint.h>
#include <vector>

#include "elf_traits.h"

namespace relocation_packer {

// Empty constructor and destructor to silence chromium-style.
Leb128Encoder::Leb128Encoder() { }

Leb128Encoder::~Leb128Encoder() { }

// Add a single value to the encoding.  Values are encoded with variable
// length.  The least significant 7 bits of each byte hold 7 bits of data,
// and the most significant bit is set on each byte except the last.
void Leb128Encoder::Enqueue(int64_t value) {
  uint64_t uvalue = *reinterpret_cast<uint64_t*>(&value);
  do {
    const uint8_t byte = uvalue & 127;
    uvalue >>= 7;
    encoding_.push_back((uvalue ? 128 : 0) | byte);
  } while (uvalue);
}

// Add a vector of values to the encoding.
void Leb128Encoder::EnqueueAll(const std::vector<int64_t>& values) {
  for (size_t i = 0; i < values.size(); ++i)
    Enqueue(values[i]);
}

// Create a new decoder for the given encoded stream.
Leb128Decoder::Leb128Decoder(const std::vector<uint8_t>& encoding, size_t start_with) {
  encoding_ = encoding;
  cursor_ = start_with;
}

// Empty destructor to silence chromium-style.
Leb128Decoder::~Leb128Decoder() { }

// Decode and retrieve a single value from the encoding.  Read forwards until
// a byte without its most significant bit is found, then read the 7 bit
// fields of the bytes spanned to re-form the value.
int64_t Leb128Decoder::Dequeue() {
  uint64_t value = 0;

  size_t shift = 0;
  uint8_t byte;

  // Loop until we reach a byte with its high order bit clear.
  do {
    byte = encoding_[cursor_++];
    value |= static_cast<uint64_t>(byte & 127) << shift;
    shift += 7;
  } while (byte & 128);

  return *reinterpret_cast<int64_t*>(&value);
}

// Decode and retrieve all remaining values from the encoding.
void Leb128Decoder::DequeueAll(std::vector<int64_t>* values) {
  while (cursor_ < encoding_.size())
    values->push_back(Dequeue());
}

}  // namespace relocation_packer
