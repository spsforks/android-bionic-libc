/*
 * Copyright (C) 2020 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "linker_note_gnu_property.h"

#include <elf.h>
#include <link.h>

#include "linker.h"
#include "linker_debug.h"
#include "linker_globals.h"
#include "linker_soinfo.h"

ElfNoteGNUPropertySection::ElfNoteGNUPropertySection(const soinfo* si)
    : ElfNoteGNUPropertySection(si->phdr, si->phnum, si->base, si->get_realpath()) {}

ElfNoteGNUPropertySection::ElfNoteGNUPropertySection(const ElfW(Phdr) * phdr, size_t phdr_count,
                                                     const ElfW(Addr) base, const char* name) {
  // Try to find PT_GNU_PROPERTY segment.
  auto note_gnu_property = FindSegment(phdr, phdr_count, base, name);
  // Perform some sanity checks.
  if (SanityCheck(note_gnu_property, name)) {
    // Parse section.
    Parse(note_gnu_property, name);
  }
}

const ElfW(NhdrGNUProperty) * ElfNoteGNUPropertySection::FindSegment(const ElfW(Phdr) * phdr,
                                                                     size_t phdr_count,
                                                                     const ElfW(Addr) base,
                                                                     const char* name) const {
  // According to Linux gABI extension this segment should contain
  // .note.gnu.property section only.
  INFO("\"%s\" phdr_count: %zu", name, phdr_count);
  for (size_t i = 0; i < phdr_count; ++i) {
    if (phdr[i].p_type != PT_GNU_PROPERTY) {
      continue;
    }

    INFO("\"%s\" PT_GNU_PROPERTY: found at segment index %zu", name, i);

    // Check segment size.
    if (phdr[i].p_memsz < sizeof(ElfW(NhdrGNUProperty))) {
      DL_ERR_AND_LOG(
          "\"%s\" PT_GNU_PROPERTY segment is too small: segment "
          "size %zu < minimum size %zu",
          name, static_cast<size_t>(phdr[i].p_memsz), sizeof(ElfW(NhdrGNUProperty)));
      return nullptr;
    }

    // PT_GNU_PROPERTY contains .note.gnu.property which has SHF_ALLOC
    // attribute, therefore it is loaded.
    return reinterpret_cast<ElfW(NhdrGNUProperty)*>(base + phdr[i].p_vaddr);
  }

  INFO("\"%s\" PT_GNU_PROPERTY: not found", name);
  return nullptr;
}

bool ElfNoteGNUPropertySection::SanityCheck(const ElfW(NhdrGNUProperty) * note_nhdr,
                                            const char* name) const {
  if (note_nhdr == nullptr) {
    return false;
  }

  // Check .note section type
  if (note_nhdr->nhdr.n_type != NT_GNU_PROPERTY_TYPE_0) {
    DL_ERR_AND_LOG("\"%s\" .gnu.note.property: unexpected section type %u", name,
                   note_nhdr->nhdr.n_type);
    return false;
  }

  // Some sanity checks
  if ((note_nhdr->nhdr.n_namesz != 4) ||
      (strncmp(reinterpret_cast<const char*>(&note_nhdr->n_name), "GNU", 4) != 0)) {
    DL_ERR_AND_LOG("\"%s\" .gnu.note.property: unexpected name size. Expected 4, got %u", name,
                   note_nhdr->nhdr.n_namesz);
    return false;
  }

  return true;
}

bool ElfNoteGNUPropertySection::Parse(const ElfW(NhdrGNUProperty) * note_nhdr, const char* name) {
  if (nullptr == note_nhdr) {
    // This is not an error condition.
    return true;
  }

  // The total length of the program property array is in _bytes_.
  ElfW(Word) offset = 0;
  while (offset < note_nhdr->nhdr.n_descsz) {
    INFO("\"%s\" .note.gnu.property: processing at offset 0x%x", name, offset);

    // At least the "header" part must fit.
    // The ABI doesn't say that pr_datasz can't be 0.
    if ((note_nhdr->nhdr.n_descsz - offset) < sizeof(ElfW(ProgProp))) {
      DL_ERR_AND_LOG(
          "\"%s\" .note.gnu.property: no more space left for a "
          "Program Property Note header",
          name);
      return false;
    }

    // Loop on program property array.
    const ElfW(ProgProp)* property =
        reinterpret_cast<const ElfW(ProgProp)*>(&note_nhdr->n_desc[offset]);
    ElfW(Word) property_size =
        align_up(sizeof(ElfW(ProgProp)) + property->pr_datasz, sizeof(ElfW(Addr)));
    if ((note_nhdr->nhdr.n_descsz - offset) < property_size) {
      DL_ERR_AND_LOG(
          "\"%s\" .note.gnu.property: property descriptor size is "
          "invalid: 0x%x bytes",
          name, property_size);
      return false;
    }

    // Cache found properties.
    switch (property->pr_type) {
#ifdef __aarch64__
      case GNU_PROPERTY_AARCH64_FEATURE_1_AND:
        properties_.bti_compatible =
            ((property->pr_data[0] & GNU_PROPERTY_AARCH64_FEATURE_1_BTI) != 0);
        INFO("\"%s\" .note.gnu.property: GNU_PROPERTY_AARCH64_FEATURE_1_AND = %u", name,
             properties_.bti_compatible);
        break;
#endif
      default:
        INFO("\"%s\" .note.gnu.property: found property pr_type %u pr_datasz 0x%x", name,
             property->pr_type, property->pr_datasz);
        break;
    }

    // Move offset, this should be safe to add beacuse of previous checks.
    offset += property_size;
  }

  return true;
}

#ifdef __aarch64__
bool ElfNoteGNUPropertySection::IsBTICompatible() const {
  return (g_platform_properties.bti_supported && properties_.bti_compatible);
}
#endif
