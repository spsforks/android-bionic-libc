// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Delta encode and decode REL/RELA section of elf file.
//
// The encoded data format is sequence of elements of int64_t type:
//
// [00] relocation_count - the total count of relocations
// [01] initial r_offset - this is initial r_offset for the
//                         relocation table.
// followed by group structures:
// [02] group
// ...
// [nn] group

// the generalized format of the group is (! - always present ? - depends on group_flags):
// --------------
// ! group_size
// ! group_flags
// ? group_r_offset_delta when RELOCATION_GROUPED_BY_OFFSET_DELTA flag is set
// ? group_r_info when RELOCATION_GROUPED_BY_INFO flag is set
// ? group_r_addend_group_delta when RELOCATION_GROUP_HAS_ADDEND and RELOCATION_GROUPED_BY_ADDEND flag is set
// The group description is followed by individual relocations.
// please note that there is a case when individual relocation
// section could be empty - that is if every field ends up groupped.
//
// The format for individual relocations section is:
// ? r_offset_delta - when RELOCATION_GROUPED_BY_OFFSET_DELTA is not set
// ? r_info - when RELOCATION_GROUPED_BY_INFO flag is not set
// ? r_addend_delta - RELOCATION_GROUP_HAS_ADDEND is set and RELOCATION_GROUPED_BY_ADDEND is not set
//
// The input ends when sum(group_size) == relocation_count

#ifndef TOOLS_RELOCATION_PACKER_SRC_DELTA_ENCODER_H_
#define TOOLS_RELOCATION_PACKER_SRC_DELTA_ENCODER_H_

#include <vector>

#include "elf.h"
#include "elf_traits.h"

namespace relocation_packer {

// A RelocationDeltaCodec packs vectors of relative relocations with
// addends into more compact forms, and unpacks them to reproduce the
// pre-packed data.
template <typename ELF>
class RelocationDeltaCodec {
 public:
  typedef typename ELF::Addr ElfAddr;
  typedef typename ELF::Rela ElfRela;

  // Encode relocations with addends into a more compact form.
  // |relocations| is a vector of relative relocation with addend structs.
  // |packed| is the vector of packed words into which relocations are packed.
  static void Encode(const std::vector<ElfRela>& relocations,
                     std::vector<ElfAddr>* packed);

  // Decode relative relocations with addends from their more compact form.
  // |packed| is the vector of packed relocations.
  // |relocations| is a vector of unpacked relative relocations.
  static void Decode(const std::vector<ElfAddr>& packed,
                     std::vector<ElfRela>* relocations);

 private:
  static void DetectGroup(const std::vector<ElfRela>& relocations,
                          size_t group_starts_with, ElfAddr previous_offset,
                          ElfAddr* group_size, ElfAddr* group_flags,
                          ElfAddr* group_offset_delta, ElfAddr* group_info,
                          ElfAddr* group_addend);

  static void DetectGroupFields(const ElfRela& reloc_one, const ElfRela& reloc_two,
                                ElfAddr current_offset_delta, ElfAddr* group_flags,
                                ElfAddr* group_offset_delta, ElfAddr* group_info,
                                ElfAddr* group_addend);
};

}  // namespace relocation_packer

#endif  // TOOLS_RELOCATION_PACKER_SRC_DELTA_ENCODER_H_
