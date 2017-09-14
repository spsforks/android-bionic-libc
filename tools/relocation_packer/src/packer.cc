// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "packer.h"

#include <vector>

#include "debug.h"
#include "delta_encoder.h"
#include "delta_encoder2.h"
#include "elf_traits.h"
#include "leb128.h"
#include "sleb128.h"

namespace relocation_packer {

// Pack relocations into a group encoded packed representation.
template <typename ELF>
void RelocationPacker<ELF>::PackRelocations(const std::vector<typename ELF::Rela>& relocations,
                                            std::vector<uint8_t>* packed, bool use_legacy_codec) {
  // Run-length encode.
  std::vector<typename ELF::Addr> packed_words;
  std::vector<uint8_t> leb128_packed;
  if (use_legacy_codec) {
    RelocationDeltaCodec<ELF>::Encode(relocations, &packed_words);
    Sleb128Encoder<typename ELF::Addr> sleb128_encoder;
    sleb128_encoder.EnqueueAll(packed_words);
    sleb128_encoder.GetEncoding(&leb128_packed);
  } else {
    RelocationOffsetResolver<ELF> offset_resolver(elf_);
    RelocationDeltaCodec2<ELF>::Encode(relocations, &packed_words, offset_resolver);
    Leb128Encoder<typename ELF::Addr> leb128_encoder;
    leb128_encoder.EnqueueAll(packed_words);
    leb128_encoder.GetEncoding(&leb128_packed);
  }

  // If insufficient data do nothing.
  if (packed_words.empty())
    return;

  packed->push_back('A');
  packed->push_back('P');
  packed->push_back('S');
  if (use_legacy_codec) {
    packed->push_back('2');
  } else {
    packed->push_back('3');
  }

  packed->insert(packed->end(), leb128_packed.begin(), leb128_packed.end());
}

// Unpack relative relocations from a run-length encoded packed
// representation.
template <typename ELF>
void RelocationPacker<ELF>::UnpackRelocations(
    const std::vector<uint8_t>& packed,
    std::vector<typename ELF::Rela>* relocations) {

  std::vector<typename ELF::Addr> packed_words;
  CHECK(packed.size() > 4 &&
        packed[0] == 'A' &&
        packed[1] == 'P' &&
        packed[2] == 'S' &&
        (packed[3] == '2' || packed[3] == '3'));
  bool is_legacy = packed[3] == '2';

  if (is_legacy) {
    Sleb128Decoder<typename ELF::Addr> decoder(packed, 4);
    decoder.DequeueAll(&packed_words);
    RelocationDeltaCodec<ELF>::Decode(packed_words, relocations);
  } else {
    Leb128Decoder<typename ELF::Addr> decoder(packed, 4);
    decoder.DequeueAll(&packed_words);
    RelocationOffsetResolver<ELF> offset_resolver(elf_);
    RelocationDeltaCodec2<ELF>::Decode(packed_words, relocations, offset_resolver);
  }
}

template class RelocationPacker<ELF32_traits>;
template class RelocationPacker<ELF64_traits>;

}  // namespace relocation_packer
