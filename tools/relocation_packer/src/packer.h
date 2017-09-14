// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// Pack relative relocations into a more compact form.

#ifndef TOOLS_RELOCATION_PACKER_SRC_PACKER_H_
#define TOOLS_RELOCATION_PACKER_SRC_PACKER_H_

#include <stdint.h>
#include <vector>

#include "elf.h"
#include "libelf.h"

namespace relocation_packer {

// A RelocationPacker packs vectors of relocations into more
// compact forms, and unpacks them to reproduce the pre-packed data.
template <typename ELF>
class RelocationPacker {
 public:
  RelocationPacker(Elf* elf) : elf_(elf) {}

  // Pack relocations into a more compact form.
  // |relocations| is a vector of relocation structs.
  // |packed| is the vector of packed bytes into which relocations are packed.
  void PackRelocations(const std::vector<typename ELF::Rela>& relocations,
                       std::vector<uint8_t>* packed, bool use_legacy_codec);

  // Unpack relocations from their more compact form.
  // |packed| is the vector of packed relocations.
  // |relocations| is a vector of unpacked relocation structs.
  void UnpackRelocations(const std::vector<uint8_t>& packed,
                         std::vector<typename ELF::Rela>* relocations);
 private:
  Elf* elf_;
};

}  // namespace relocation_packer

#endif  // TOOLS_RELOCATION_PACKER_SRC_PACKER_H_
