// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Delta encode and decode REL/RELA section of elf file.
//
// The encoded data format is sequence of elements of ElfAddr type (unsigned long):
//
//   relocation_count - the total count of relocations
//   encoded_offset - the offset of the next relocation, with the lower two bits as flags.
//                  - 0x2 is set when an addend is present. 0x1 is set when num_same_info = 1.
//   (num_same_info) - the number of relocations with the same r_info. Omitted when == 1.
//   r_info - the r_info for the set of relocations.
//   (r_addend) - the r_addend for the first relocation in the set.
//   enoded_offset_delta - the delta for r_offset, with lower two bits representing num_same_delta.
//   (num_same_delta) - the number of relocations with the same offset delta. Omitted when < 4.
//   (r_addend...) - the r_addend for each relocation in num_same_delta.

#ifndef TOOLS_RELOCATION_PACKER_SRC_DELTA_ENCODER2_H_
#define TOOLS_RELOCATION_PACKER_SRC_DELTA_ENCODER2_H_

#include <vector>

#include "elf.h"
#include "elf_traits.h"

namespace relocation_packer {

template <typename ELF>
class RelocationOffsetResolver {
 public:
  RelocationOffsetResolver(Elf* elf) : elf_(elf) {}
  typename ELF::Sword Resolve(typename ELF::Sword r_offset);
 private:
  Elf* elf_;
  typename ELF::Shdr* prev_header_ = nullptr;
  Elf_Data* prev_data_ = nullptr;
};

// A RelocationDeltaCodec packs vectors of relative relocations with
// addends into more compact forms, and unpacks them to reproduce the
// pre-packed data.
template <typename ELF>
class RelocationDeltaCodec2 {
 public:
  typedef typename ELF::Addr ElfAddr;
  typedef typename ELF::Rela ElfRela;
  typedef RelocationOffsetResolver<ELF> OffsetResolver;

  // Encode relocations with addends into a more compact form.
  // |relocations| is a vector of relative relocation with addend structs.
  // |packed| is the vector of packed words into which relocations are packed.
  static void Encode(const std::vector<ElfRela>& relocations,
                     std::vector<ElfAddr>* packed,
                     OffsetResolver& offset_resolver);

  // Decode relative relocations with addends from their more compact form.
  // |packed| is the vector of packed relocations.
  // |relocations| is a vector of unpacked relative relocations.
  static void Decode(const std::vector<ElfAddr>& packed,
                     std::vector<ElfRela>* relocations,
                     OffsetResolver& offset_resolver);

 private:
  static ElfAddr CountSameInfo(const ElfRela* start, const ElfRela* end);
  static ElfAddr CountSameDelta(const ElfRela* start, const ElfRela* end);
  static bool AnyAddends(const ElfRela* start, const ElfRela* end, OffsetResolver& offset_resolver);
};

}  // namespace relocation_packer

#endif  // TOOLS_RELOCATION_PACKER_SRC_DELTA_ENCODER2_H_
