// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "delta_encoder.h"

#include <vector>

#include "debug.h"

#define RELOCATION_GROUPED_BY_INFO_FLAG 1
#define RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG 2
#define RELOCATION_GROUPED_BY_ADDEND_FLAG 4
#define RELOCATION_GROUP_HAS_ADDEND_FLAG 8

#define RELOCATION_GROUPED_BY_INFO(flags) (((flags) & RELOCATION_GROUPED_BY_INFO_FLAG) != 0)
#define RELOCATION_GROUPED_BY_OFFSET_DELTA(flags) (((flags) & RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG) != 0)
#define RELOCATION_GROUPED_BY_ADDEND(flags) (((flags) & RELOCATION_GROUPED_BY_ADDEND_FLAG) != 0)
#define RELOCATION_GROUP_HAS_ADDEND(flags) (((flags) & RELOCATION_GROUP_HAS_ADDEND_FLAG) != 0)

namespace relocation_packer {

// Encode relocations into a delta encoded (packed) representation.
template <typename R>
void RelocationDeltaCodec<R>::Encode(const std::vector<R>& relocations,
                                     std::vector<ElfAddr>* packed) {
  if (relocations.size() == 0)
    return;

  // Start with the relocation count, then append groups
  // TODO(dimitry): we might want to move it to DT_ANDROID_RELCOUNT section
  packed->push_back(static_cast<ElfAddr>(relocations.size()));

  // lets write starting offset (offset of the first reloc - first delta)
  ElfAddr start_offset = relocations.size() > 1 ?
      relocations[0].r_offset - (relocations[1].r_offset - relocations[0].r_offset) :
      relocations[0].r_offset;

  packed->push_back(start_offset);

  // this one is used to calculate delta
  ElfAddr previous_addend = 0;
  ElfAddr previous_offset = start_offset;

  for (size_t group_start = 0; group_start < relocations.size(); ) {
    ElfAddr group_flags = 0;
    ElfAddr group_offset_delta = 0;
    ElfAddr group_addend = 0;
    ElfAddr group_info = 0;

    ElfAddr group_size = 0;

    DetectGroup(relocations, group_start, previous_offset, &group_size, &group_flags,
        &group_offset_delta, &group_info, &group_addend);

    // write the group header
    packed->push_back(group_size);
    packed->push_back(group_flags);

    if (RELOCATION_GROUPED_BY_OFFSET_DELTA(group_flags)) {
      packed->push_back(group_offset_delta);
    }

    if (RELOCATION_GROUPED_BY_INFO(group_flags)) {
      packed->push_back(group_info);
    }

    if (RELOCATION_GROUP_HAS_ADDEND(group_flags) && RELOCATION_GROUPED_BY_ADDEND(group_flags)) {
      packed->push_back(group_addend - previous_addend);
      previous_addend = group_addend;
    }

    for (size_t i = 0; i < static_cast<size_t>(group_size); ++i) {
      CHECK((group_start + i) < relocations.size());
      const R* relocation = &relocations[group_start + i];

      if (!RELOCATION_GROUPED_BY_OFFSET_DELTA(group_flags)) {
        packed->push_back(relocation->r_offset - previous_offset);
      }
      previous_offset = relocation->r_offset;

      if (!RELOCATION_GROUPED_BY_INFO(group_flags)) {
        packed->push_back(relocation->r_info);
      }

      if (RELOCATION_GROUP_HAS_ADDEND(group_flags) && !RELOCATION_GROUPED_BY_ADDEND(group_flags)) {
        packed->push_back(relocation->r_addend - previous_addend);
        previous_addend = relocation->r_addend;
      }
    }

    group_start += group_size;
  }
}

// Decode relocations from a delta encoded (packed) representation.
template <typename R>
void RelocationDeltaCodec<R>::Decode(const std::vector<ElfAddr>& packed,
                                     std::vector<R>* relocations) {
  if (packed.size() < 5) {
    return;
  }

  size_t ndx = 0;
  ElfAddr current_count = 0;
  ElfAddr total_count = packed[ndx++];

  ElfAddr offset = packed[ndx++];
  ElfAddr info = 0;
  ElfAddr addend = 0;

  while(current_count < total_count) {
    // read group
    ElfAddr group_size = packed[ndx++];
    ElfAddr group_flags = packed[ndx++];
    ElfAddr group_offset_delta = 0;

    if (RELOCATION_GROUPED_BY_OFFSET_DELTA(group_flags)) {
      group_offset_delta = packed[ndx++];
    }

    if (RELOCATION_GROUPED_BY_INFO(group_flags)) {
      info = packed[ndx++];
    }

    if (RELOCATION_GROUP_HAS_ADDEND(group_flags) && RELOCATION_GROUPED_BY_ADDEND(group_flags)) {
      addend += packed[ndx++];
    }

    // now read not grouped info
    for (ElfAddr i = 0; i<group_size; ++i) {
      if (RELOCATION_GROUPED_BY_OFFSET_DELTA(group_flags)) {
        offset += group_offset_delta;
      } else {
        offset += packed[ndx++];
      }

      if (!RELOCATION_GROUPED_BY_INFO(group_flags)) {
        info = packed[ndx++];
      }

      if (RELOCATION_GROUP_HAS_ADDEND(group_flags) && !RELOCATION_GROUPED_BY_ADDEND(group_flags)) {
        addend += packed[ndx++];
      }

      R reloc;
      reloc.r_offset = offset;
      reloc.r_info = info;
      reloc.r_addend = addend;
      relocations->push_back(reloc);
    }
    current_count += group_size;
  }
}

template <typename R>
void RelocationDeltaCodec<R>::DetectGroupFields(const R& reloc_one, const R& reloc_two,
                                                ElfAddr current_offset_delta, ElfAddr* group_flags,
                                                ElfAddr* group_offset_delta, ElfAddr* group_info,
                                                ElfAddr* group_addend) {
  *group_flags = 0;

  if ((static_cast<ElfAddr>(reloc_two.r_offset) - static_cast<ElfAddr>(reloc_one.r_offset)) == current_offset_delta) {
    *group_flags |= RELOCATION_GROUPED_BY_OFFSET_DELTA_FLAG;
    if (group_offset_delta != nullptr) {
      *group_offset_delta = current_offset_delta;
    }
  }

  if (reloc_one.r_info == reloc_two.r_info) {
    *group_flags |= RELOCATION_GROUPED_BY_INFO_FLAG;
    if (group_info != nullptr) {
      *group_info = reloc_one.r_info;
    }
  }

  if (reloc_one.r_addend != 0 || reloc_two.r_addend != 0) {
    *group_flags |= RELOCATION_GROUP_HAS_ADDEND_FLAG;
    if (reloc_one.r_addend == reloc_two.r_addend) {
      *group_flags |= RELOCATION_GROUPED_BY_ADDEND_FLAG;
      if (group_addend != nullptr) {
        *group_addend = reloc_one.r_addend;
      }
    }
  }
}

static uint32_t group_weight(uint64_t flags) {
  uint32_t weight = 0;
  if (!RELOCATION_GROUP_HAS_ADDEND(flags)) {
    weight += 30;
  } else if (RELOCATION_GROUPED_BY_ADDEND(flags)) {
    weight += 10;
  }

  if (RELOCATION_GROUPED_BY_OFFSET_DELTA(flags)) {
    weight += 10;
  }

  if (RELOCATION_GROUPED_BY_INFO(flags)) {
    weight += 10;
  }

  return weight;
}

template <typename R>
void RelocationDeltaCodec<R>::DetectGroup(const std::vector<R>& relocations,
                                          size_t group_starts_with, ElfAddr previous_offset,
                                          ElfAddr* group_size, ElfAddr* group_flags,
                                          ElfAddr* group_offset_delta, ElfAddr* group_info,
                                          ElfAddr* group_addend) {
  CHECK(group_starts_with < relocations.size());

  const R& reloc_one = relocations[group_starts_with++];
  if (group_starts_with == relocations.size()) {
    *group_flags = reloc_one.r_addend == 0 ? 0 : RELOCATION_GROUP_HAS_ADDEND_FLAG;
    *group_size = 1;
    return;
  }

  const ElfAddr offset_delta = reloc_one.r_offset - previous_offset;

  // detect group_flags
  DetectGroupFields(reloc_one, relocations[group_starts_with], offset_delta, group_flags, group_offset_delta, group_info, group_addend);

  if (group_starts_with + 1 == relocations.size()) {
    *group_size = 2;
    return;
  }

  ElfAddr cnt = 1;
  for (size_t i = group_starts_with; i < relocations.size() - 1; ++i) {
    ElfAddr candidate_flags;
    // check if next group (reloc_current; reloc_next) has better grouped_by flags
    DetectGroupFields(relocations[i], relocations[i+1], offset_delta, &candidate_flags, nullptr, nullptr, nullptr);

    if (group_weight(*group_flags) < group_weight(candidate_flags)) {
      break;
    }
    cnt++;

    if (candidate_flags != *group_flags) {
      break;
    }

    if (i + 1 == relocations.size() - 1) { // last one
      cnt++;
    }
  }

  // if as a result of checking candidates we ended up with cnt == 1
  // reset flags to the default state
  if (cnt == 1) {
    *group_flags = reloc_one.r_addend == 0 ? 0 : RELOCATION_GROUP_HAS_ADDEND_FLAG;
  }

  *group_size = cnt;
}

template class RelocationDeltaCodec<Elf32_Rela>;
template class RelocationDeltaCodec<Elf64_Rela>;

}  // namespace relocation_packer
