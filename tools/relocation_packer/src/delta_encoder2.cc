// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "delta_encoder2.h"

#include <vector>

#include "debug.h"

template <typename ELF>
static bool AddressInSection(typename ELF::Addr address, typename ELF::Shdr* header) {
  return address >= header->sh_addr && address < header->sh_addr + header->sh_size;
}

namespace relocation_packer {

template <typename ELF>
typename ELF::Sword RelocationOffsetResolver<ELF>::Resolve(typename ELF::Sword r_offset) {
  typename ELF::Addr address = static_cast<typename ELF::Addr>(r_offset);
  if (!prev_header_ || !AddressInSection<ELF>(address, prev_header_)) {
    Elf_Scn* section = elf_getscn(elf_, 0);
    for (;;) {
      CHECK(section) << " Address out range: " << address;
      typename ELF::Shdr* header = ELF::getshdr(section);
      if (AddressInSection<ELF>(address, header)) {
        prev_header_ = header;
        prev_data_ = elf_getdata(section, nullptr);
        break;
      }
      section = elf_nextscn(elf_, section);
    }
  }
  typename ELF::Addr offset = address - prev_header_->sh_addr;
  CHECK(offset < prev_data_->d_size) << " Multi-data section?";
  char* ptr = reinterpret_cast<char*>(prev_data_->d_buf);
  return *reinterpret_cast<typename ELF::Sword*>(ptr + offset);
}

// Encode relocations into a delta encoded (packed) representation.
template <typename ELF>
void RelocationDeltaCodec2<ELF>::Encode(const std::vector<ElfRela>& relocations,
                                        std::vector<ElfAddr>* packed,
                                        OffsetResolver& offset_resolver) {
  if (relocations.size() == 0) return;

  packed->push_back(static_cast<ElfAddr>(relocations.size()));

  const ElfRela* cur_relocation = &relocations[0];
  const ElfRela* end_relocation = &relocations[relocations.size()];
  while (cur_relocation < end_relocation) {
    ElfAddr num_same_info = CountSameInfo(cur_relocation, end_relocation);
    bool has_addends = AnyAddends(cur_relocation, cur_relocation + num_same_info, offset_resolver);

    ElfAddr encoded_offset = cur_relocation->r_offset;
    CHECK((encoded_offset & 3) == 0);
    encoded_offset |= 1 * static_cast<ElfAddr>(num_same_info == 1);
    encoded_offset |= 2 * static_cast<ElfAddr>(has_addends);
    packed->push_back(encoded_offset);

    if (num_same_info > 1) {
      packed->push_back(num_same_info);
    }
    packed->push_back(cur_relocation->r_info);
    if (has_addends) {
      packed->push_back(cur_relocation->r_addend);
    }

    const ElfRela* same_info_end_relocation = cur_relocation + num_same_info;
    cur_relocation += 1;
    while (cur_relocation < same_info_end_relocation) {
      ElfAddr num_same_delta = CountSameDelta(cur_relocation, same_info_end_relocation);
      ElfAddr offest_delta = cur_relocation->r_offset - (cur_relocation - 1)->r_offset;
      CHECK((offest_delta & 3) == 0);
      if (num_same_delta < 4) {
        packed->push_back(offest_delta | num_same_delta);
      } else {
        packed->push_back(offest_delta);
        packed->push_back(num_same_delta);
      }
      if (has_addends) {
        for (ElfAddr i = 0; i < num_same_delta; ++i) {
          packed->push_back(cur_relocation->r_addend);
          cur_relocation += 1;
        }
      } else {
        cur_relocation += num_same_delta;
      }
    }
  }
}

// Decode relocations from a delta encoded (packed) representation.
template <typename ELF>
void RelocationDeltaCodec2<ELF>::Decode(const std::vector<ElfAddr>& packed,
                                        std::vector<ElfRela>* relocations,
                                        OffsetResolver& offset_resolver) {
  if (packed.size() < 4) {
    return;
  }
  size_t ndx = 0;
  ElfAddr total_count = packed[ndx++];

  while (relocations->size() < total_count) {
    ElfAddr encoded_offset = packed[ndx++];
    ElfAddr num_same_info = encoded_offset & 1;
    bool has_addends = (encoded_offset & 2) != 0;
    ElfAddr offset = encoded_offset & ~3;
    if (num_same_info == 0) {
      num_same_info = packed[ndx++];
    }

    ElfAddr info = packed[ndx++];
    typename ELF::Sword addend = 0;
    if (has_addends) {
      addend = packed[ndx++];
    } else {
      addend = offset_resolver.Resolve(offset);
    }
    relocations->push_back({ offset, info, addend });

    for (ElfAddr i = 1; i < num_same_info;) {
      ElfAddr offset_delta = packed[ndx++];
      ElfAddr num_same_delta = offset_delta & 3;
      offset_delta &= ~3;
      if (num_same_delta == 0) {
        num_same_delta = packed[ndx++];
      }

      for (ElfAddr j = 0; j < num_same_delta; ++j) {
        offset += offset_delta;
        if (has_addends) {
          addend = packed[ndx++];
        } else {
          addend = offset_resolver.Resolve(offset);
        }
        relocations->push_back({ offset, info, addend });
      }
      i += num_same_delta;
    }
  }
}

template <typename ELF>
bool RelocationDeltaCodec2<ELF>::AnyAddends(const ElfRela* start, const ElfRela* end,
                                            OffsetResolver& offset_resolver) {
  for (auto* cur = start; cur < end; ++cur) {
    if (cur->r_addend != 0 && offset_resolver.Resolve(cur->r_offset) != cur->r_addend) {
      return true;
    }
  }
  return false;
}

template <typename ELF>
typename RelocationDeltaCodec2<ELF>::ElfAddr RelocationDeltaCodec2<ELF>::CountSameInfo(
    const ElfRela* start, const ElfRela* end) {
  ElfAddr first_info = start->r_info;
  const ElfRela* cur = start + 1;
  for (; cur < end; ++cur) {
    if (cur->r_info != first_info) {
      break;
    }
  }
  return cur - start;
}

template <typename ELF>
typename RelocationDeltaCodec2<ELF>::ElfAddr RelocationDeltaCodec2<ELF>::CountSameDelta(
    const ElfRela* start, const ElfRela* end) {
  ElfAddr first_delta = start->r_offset - (start - 1)->r_offset;
  const ElfRela* cur = start + 1;
  for (; cur < end; ++cur) {
    ElfAddr offset_delta = cur->r_offset - (cur - 1)->r_offset;

    if (offset_delta != first_delta) {
      break;
    }
  }
  return cur - start;
}

template class RelocationDeltaCodec2<ELF32_traits>;
template class RelocationDeltaCodec2<ELF64_traits>;

}  // namespace relocation_packer
