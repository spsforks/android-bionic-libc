// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "packer.h"

#include <vector>

#include "debug.h"
#include "delta_encoder.h"
#include "elf_traits.h"
#include "leb128.h"
#include "sleb128.h"

namespace relocation_packer {

// Pack relative relocations into a run-length encoded packed
// representation.
template <typename ELF>
void RelocationPacker<ELF>::PackRelocations(const std::vector<typename ELF::Rela>& relocations, std::vector<uint8_t>* packed) {
  // Run-length encode.
  std::vector<typename ELF::Sxword> packed_words;
  RelocationDeltaCodec<typename ELF::Rela> codec;
  codec.Encode(relocations, &packed_words);

  // If insufficient data do nothing.
  if (packed_words.empty())
    return;

  Sleb128Encoder sleb128_encoder;
  Leb128Encoder leb128_encoder;

  std::vector<uint8_t> leb128_packed;
  std::vector<uint8_t> sleb128_packed;

  leb128_encoder.EnqueueAll(packed_words);
  leb128_encoder.GetEncoding(&leb128_packed);

  sleb128_encoder.EnqueueAll(packed_words);
  sleb128_encoder.GetEncoding(&sleb128_packed);

  if (leb128_packed.size() <= sleb128_packed.size()) {
    packed->push_back('A');
    packed->push_back('P');
    packed->push_back('U');
    packed->push_back('2');
    packed->insert(packed->end(), leb128_packed.begin(), leb128_packed.end());
  } else {
    packed->push_back('A');
    packed->push_back('P');
    packed->push_back('S');
    packed->push_back('2');
    packed->insert(packed->end(), sleb128_packed.begin(), sleb128_packed.end());
  }
}

// Unpack relative relocations from a run-length encoded packed
// representation.
template <typename ELF>
void RelocationPacker<ELF>::UnpackRelocations(
    const std::vector<uint8_t>& packed,
    std::vector<typename ELF::Rela>* relocations) {

  std::vector<typename ELF::Sxword> packed_words;
  CHECK(packed.size() > 4 &&
        packed[0] == 'A' &&
        packed[1] == 'P' &&
        (packed[2] == 'U' || packed[2] == 'S') &&
        packed[3] == '2');

  if (packed[2] == 'U') {
    Leb128Decoder decoder(packed, 4);
    decoder.DequeueAll(&packed_words);
  } else {
    Sleb128Decoder decoder(packed, 4);
    decoder.DequeueAll(&packed_words);
  }

  RelocationDeltaCodec<typename ELF::Rela> codec;
  codec.Decode(packed_words, relocations);
}

template class RelocationPacker<ELF32_traits>;
template class RelocationPacker<ELF64_traits>;

}  // namespace relocation_packer
