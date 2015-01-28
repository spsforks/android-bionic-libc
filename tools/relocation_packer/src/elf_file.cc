// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "elf_file.h"

#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "debug.h"
#include "elf_traits.h"
#include "libelf.h"
#include "packer.h"

namespace relocation_packer {

// Stub identifier written to 'null out' packed data, "NULL".
static const uint32_t kStubIdentifier = 0x4c4c554eu;

// Out-of-band dynamic tags used to indicate the offset and size of the
// android packed relocations section.
#define DT_ANDROID_REL (DT_LOOS + 2)
#define DT_ANDROID_RELSZ  (DT_LOOS + 3)

#define DT_ANDROID_RELA (DT_LOOS + 4)
#define DT_ANDROID_RELASZ (DT_LOOS + 5)

#define SHT_ANDROID_REL (SHT_LOOS + 1)
#define SHT_ANDROID_RELA (SHT_LOOS + 2)

// Alignment to preserve, in bytes.  This must be at least as large as the
// largest d_align and sh_addralign values found in the loaded file.
// Out of caution for RELRO page alignment, we preserve to a complete target
// page.  See http://www.airs.com/blog/archives/189.
static const size_t kPreserveAlignment = 4096;

// Get section data.  Checks that the section has exactly one data entry,
// so that the section size and the data size are the same.  True in
// practice for all sections we resize when packing or unpacking.  Done
// by ensuring that a call to elf_getdata(section, data) returns NULL as
// the next data entry.
static Elf_Data* GetSectionData(Elf_Scn* section) {
  Elf_Data* data = elf_getdata(section, NULL);
  CHECK(data && elf_getdata(section, data) == NULL);
  return data;
}

// Rewrite section data.  Allocates new data and makes it the data element's
// buffer.  Relies on program exit to free allocated data.
static void RewriteSectionData(Elf_Data* data,
                        const void* section_data,
                        size_t size) {
  CHECK(size == data->d_size);
  uint8_t* area = new uint8_t[size];
  memcpy(area, section_data, size);
  data->d_buf = area;
}

// Verbose ELF header logging.
template <typename Ehdr>
static void VerboseLogElfHeader(const Ehdr* elf_header) {
  VLOG(1) << "e_phoff = " << elf_header->e_phoff;
  VLOG(1) << "e_shoff = " << elf_header->e_shoff;
  VLOG(1) << "e_ehsize = " << elf_header->e_ehsize;
  VLOG(1) << "e_phentsize = " << elf_header->e_phentsize;
  VLOG(1) << "e_phnum = " << elf_header->e_phnum;
  VLOG(1) << "e_shnum = " << elf_header->e_shnum;
  VLOG(1) << "e_shstrndx = " << elf_header->e_shstrndx;
}

// Verbose ELF program header logging.
template <typename Phdr>
static void VerboseLogProgramHeader(size_t program_header_index,
                             const Phdr* program_header) {
  std::string type;
  switch (program_header->p_type) {
    case PT_NULL: type = "NULL"; break;
    case PT_LOAD: type = "LOAD"; break;
    case PT_DYNAMIC: type = "DYNAMIC"; break;
    case PT_INTERP: type = "INTERP"; break;
    case PT_NOTE: type = "NOTE"; break;
    case PT_SHLIB: type = "SHLIB"; break;
    case PT_PHDR: type = "PHDR"; break;
    case PT_TLS: type = "TLS"; break;
    default: type = "(OTHER)"; break;
  }
  VLOG(1) << "phdr " << program_header_index << " : " << type;
  VLOG(1) << "  p_offset = " << program_header->p_offset;
  VLOG(1) << "  p_vaddr = " << program_header->p_vaddr;
  VLOG(1) << "  p_paddr = " << program_header->p_paddr;
  VLOG(1) << "  p_filesz = " << program_header->p_filesz;
  VLOG(1) << "  p_memsz = " << program_header->p_memsz;
}

// Verbose ELF section header logging.
template <typename Shdr>
static void VerboseLogSectionHeader(const std::string& section_name,
                             const Shdr* section_header) {
  VLOG(1) << "section " << section_name;
  VLOG(1) << "  sh_addr = " << section_header->sh_addr;
  VLOG(1) << "  sh_offset = " << section_header->sh_offset;
  VLOG(1) << "  sh_size = " << section_header->sh_size;
  VLOG(1) << "  sh_entsize = " << section_header->sh_entsize;
  VLOG(1) << "  sh_addralign = " << section_header->sh_addralign;
}

// Verbose ELF section data logging.
static void VerboseLogSectionData(const Elf_Data* data) {
  VLOG(1) << "  data";
  VLOG(1) << "    d_buf = " << data->d_buf;
  VLOG(1) << "    d_off = " << data->d_off;
  VLOG(1) << "    d_size = " << data->d_size;
  VLOG(1) << "    d_align = " << data->d_align;
}

// Load the complete ELF file into a memory image in libelf, and identify
// the .rel.dyn or .rela.dyn, .dynamic, and .android.rel.dyn or
// .android.rela.dyn sections.  No-op if the ELF file has already been loaded.
template <typename ELF>
bool ElfFile<ELF>::Load() {
  if (elf_)
    return true;

  Elf* elf = elf_begin(fd_, ELF_C_RDWR, NULL);
  CHECK(elf);

  if (elf_kind(elf) != ELF_K_ELF) {
    LOG(ERROR) << "File not in ELF format";
    return false;
  }

  auto elf_header = ELF::getehdr(elf);
  if (!elf_header) {
    LOG(ERROR) << "Failed to load ELF header: " << elf_errmsg(elf_errno());
    return false;
  }

  relocation_helper_.reset(RelocationHelper<ELF>::make_relocation_helper(elf_header->e_machine));

  if (relocation_helper_.get() == nullptr) {
    LOG(ERROR) << "ELF file architecture is not supported " << elf_header->e_machine;
    return false;
  }
  if (elf_header->e_type != ET_DYN) {
    LOG(ERROR) << "ELF file is not a shared object";
    return false;
  }

  // Require that our endianness matches that of the target, and that both
  // are little-endian.  Safe for all current build/target combinations.
  const int endian = elf_header->e_ident[EI_DATA];
  CHECK(endian == ELFDATA2LSB);
  CHECK(__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__);

  const int file_class = elf_header->e_ident[EI_CLASS];
  VLOG(1) << "endian = " << endian << ", file class = " << file_class;
  VerboseLogElfHeader(elf_header);

  auto elf_program_header = ELF::getphdr(elf);
  CHECK(elf_program_header != nullptr);

  const typename ELF::Phdr* dynamic_program_header = NULL;
  for (size_t i = 0; i < elf_header->e_phnum; ++i) {
    auto program_header = &elf_program_header[i];
    VerboseLogProgramHeader(i, program_header);

    if (program_header->p_type == PT_DYNAMIC) {
      CHECK(dynamic_program_header == NULL);
      dynamic_program_header = program_header;
    }
  }
  CHECK(dynamic_program_header != nullptr);

  size_t string_index;
  elf_getshdrstrndx(elf, &string_index);

  // Notes of the dynamic relocations, packed relocations, and .dynamic
  // sections.  Found while iterating sections, and later stored in class
  // attributes.
  Elf_Scn* found_relocations_section = nullptr;
  Elf_Scn* found_dynamic_section = nullptr;

  // Notes of relocation section types seen.  We require one or the other of
  // these; both is unsupported.
  bool has_rel_relocations = false;
  bool has_rela_relocations = false;

  // Flag set if we encounter any .debug* section.  We do not adjust any
  // offsets or addresses of any debug data, so if we find one of these then
  // the resulting output shared object should still run, but might not be
  // usable for debugging, disassembly, and so on.  Provides a warning if
  // this occurs.
  bool has_debug_section = false;

  Elf_Scn* section = NULL;
  while ((section = elf_nextscn(elf, section)) != nullptr) {
    auto section_header = ELF::getshdr(section);
    std::string name = elf_strptr(elf, string_index, section_header->sh_name);
    VerboseLogSectionHeader(name, section_header);

    // Note relocation section types.
    if (section_header->sh_type == SHT_REL || section_header->sh_type == SHT_ANDROID_REL) {
      has_rel_relocations = true;
    }
    if (section_header->sh_type == SHT_RELA || section_header->sh_type == SHT_ANDROID_RELA) {
      has_rela_relocations = true;
    }

    // Note special sections as we encounter them.
    if ((name == ".rel.dyn" || name == ".rela.dyn") &&
        section_header->sh_size > 0) {
      found_relocations_section = section;
    }

    if (section_header->sh_offset == dynamic_program_header->p_offset) {
      found_dynamic_section = section;
    }

    // If we find a section named .debug*, set the debug warning flag.
    if (std::string(name).find(".debug") == 0) {
      has_debug_section = true;
    }

    // Ensure we preserve alignment, repeated later for the data block(s).
    CHECK(section_header->sh_addralign <= kPreserveAlignment);

    Elf_Data* data = NULL;
    while ((data = elf_getdata(section, data)) != NULL) {
      CHECK(data->d_align <= kPreserveAlignment);
      VerboseLogSectionData(data);
    }
  }

  // Loading failed if we did not find the required special sections.
  if (!found_relocations_section) {
    LOG(ERROR) << "Missing or empty .rel.dyn or .rela.dyn section";
    return false;
  }
  if (!found_dynamic_section) {
    LOG(ERROR) << "Missing .dynamic section";
    return false;
  }

  // Loading failed if we could not identify the relocations type.
  if (!has_rel_relocations && !has_rela_relocations) {
    LOG(ERROR) << "No relocations sections found";
    return false;
  }
  if (has_rel_relocations && has_rela_relocations) {
    LOG(ERROR) << "Multiple relocations sections with different types found, "
               << "not currently supported";
    return false;
  }

  if (has_debug_section) {
    LOG(WARNING) << "Found .debug section(s), and ignored them";
  }

  elf_ = elf;
  relocations_section_ = found_relocations_section;
  dynamic_section_ = found_dynamic_section;
  relocations_type_ = has_rel_relocations ? REL : RELA;
  return true;
}

// Helper for ResizeSection().  Adjust the main ELF header for the hole.
template <typename ELF>
static void AdjustElfHeaderForHole(typename ELF::Ehdr* elf_header,
                                   typename ELF::Off hole_start,
                                   ssize_t hole_size) {
  if (elf_header->e_phoff > hole_start) {
    elf_header->e_phoff += hole_size;
    VLOG(1) << "e_phoff adjusted to " << elf_header->e_phoff;
  }
  if (elf_header->e_shoff > hole_start) {
    elf_header->e_shoff += hole_size;
    VLOG(1) << "e_shoff adjusted to " << elf_header->e_shoff;
  }
}

// Helper for ResizeSection().  Adjust all program headers for the hole.
template <typename ELF>
static void AdjustProgramHeadersForHole(typename ELF::Phdr* elf_program_header,
                                        size_t program_header_count,
                                        typename ELF::Off hole_start,
                                        ssize_t hole_size) {
  for (size_t i = 0; i < program_header_count; ++i) {
    auto program_header = &elf_program_header[i];

    if (program_header->p_offset > hole_start) {
      // The hole start is past this segment, so adjust offsets and addrs.
      program_header->p_offset += hole_size;
      VLOG(1) << "phdr " << i
              << " p_offset adjusted to "<< program_header->p_offset;

      // Only adjust vaddr and paddr if this program header has them.
      if (program_header->p_vaddr != 0) {
        program_header->p_vaddr += hole_size;
        VLOG(1) << "phdr " << i
                << " p_vaddr adjusted to " << program_header->p_vaddr;
      }
      if (program_header->p_paddr != 0) {
        program_header->p_paddr += hole_size;
        VLOG(1) << "phdr " << i
                << " p_paddr adjusted to " << program_header->p_paddr;
      }
    } else if (program_header->p_offset +
               program_header->p_filesz > hole_start) {
      // The hole start is within this segment, so adjust file and in-memory
      // sizes, but leave offsets and addrs unchanged.
      program_header->p_filesz += hole_size;
      VLOG(1) << "phdr " << i
              << " p_filesz adjusted to " << program_header->p_filesz;
      program_header->p_memsz += hole_size;
      VLOG(1) << "phdr " << i
              << " p_memsz adjusted to " << program_header->p_memsz;
    }
  }
}

// Helper for ResizeSection().  Adjust all section headers for the hole.
template <typename ELF>
static void AdjustSectionHeadersForHole(Elf* elf,
                                        typename ELF::Off hole_start,
                                        ssize_t hole_size) {
  size_t string_index;
  elf_getshdrstrndx(elf, &string_index);

  Elf_Scn* section = NULL;
  while ((section = elf_nextscn(elf, section)) != NULL) {
    auto section_header = ELF::getshdr(section);
    std::string name = elf_strptr(elf, string_index, section_header->sh_name);

    if (section_header->sh_offset > hole_start) {
      section_header->sh_offset += hole_size;
      VLOG(1) << "section " << name
              << " sh_offset adjusted to " << section_header->sh_offset;
      // Only adjust section addr if this section has one.
      if (section_header->sh_addr != 0) {
        section_header->sh_addr += hole_size;
        VLOG(1) << "section " << name
                << " sh_addr adjusted to " << section_header->sh_addr;
      }
    }
  }
}

// Helper for ResizeSection().  Adjust the .dynamic section for the hole.
template <typename ELF>
void ElfFile<ELF>::AdjustDynamicSectionForHole(Elf_Scn* dynamic_section,
                                               typename ELF::Off hole_start,
                                               ssize_t hole_size,
                                               relocations_type_t relocations_type) {
  CHECK(relocations_type != NONE);
  Elf_Data* data = GetSectionData(dynamic_section);

  auto dynamic_base = reinterpret_cast<typename ELF::Dyn*>(data->d_buf);
  std::vector<typename ELF::Dyn> dynamics(
      dynamic_base,
      dynamic_base + data->d_size / sizeof(dynamics[0]));

  for (size_t i = 0; i < dynamics.size(); ++i) {
    auto dynamic = &dynamics[i];
    const auto tag = dynamic->d_tag;
    // Any tags that hold offsets are adjustment candidates.
    const bool is_adjustable = (tag == DT_PLTGOT ||
                                tag == DT_HASH ||
                                tag == DT_STRTAB ||
                                tag == DT_SYMTAB ||
                                tag == DT_RELA ||
                                tag == DT_INIT ||
                                tag == DT_FINI ||
                                tag == DT_REL ||
                                tag == DT_JMPREL ||
                                tag == DT_INIT_ARRAY ||
                                tag == DT_FINI_ARRAY ||
                                tag == DT_ANDROID_REL||
                                tag == DT_ANDROID_RELA);
    if (is_adjustable && dynamic->d_un.d_ptr > hole_start) {
      dynamic->d_un.d_ptr += hole_size;
      VLOG(1) << "dynamic[" << i << "] " << dynamic->d_tag
              << " d_ptr adjusted to " << dynamic->d_un.d_ptr;
    }

    // DT_RELSZ or DT_RELASZ indicate the overall size of relocations.
    // Only one will be present.  Adjust by hole size.
    if (tag == DT_RELSZ || tag == DT_RELASZ || tag == DT_ANDROID_RELSZ || tag == DT_ANDROID_RELASZ) {
      dynamic->d_un.d_val += hole_size;
      VLOG(1) << "dynamic[" << i << "] " << dynamic->d_tag
              << " d_val adjusted to " << dynamic->d_un.d_val;
    }

    // Ignore DT_RELCOUNT and DT_RELACOUNT: (1) nobody uses them and
    // technically (2) the relative relocation count is not changed.

    // DT_RELENT and DT_RELAENT don't change, ignore them as well.
  }

  void* section_data = &dynamics[0];
  size_t bytes = dynamics.size() * sizeof(dynamics[0]);
  RewriteSectionData(data, section_data, bytes);
}

// Helper for ResizeSection().  Adjust the .dynsym section for the hole.
// We need to adjust the values for the symbols represented in it.
template <typename ELF>
static void AdjustDynSymSectionForHole(Elf_Scn* dynsym_section,
                                       typename ELF::Off hole_start,
                                       ssize_t hole_size) {
  Elf_Data* data = GetSectionData(dynsym_section);

  const auto dynsym_base = reinterpret_cast<typename ELF::Sym*>(data->d_buf);
  std::vector<typename ELF::Sym> dynsyms
      (dynsym_base,
       dynsym_base + data->d_size / sizeof(dynsyms[0]));

  for (size_t i = 0; i < dynsyms.size(); ++i) {
    auto dynsym = &dynsyms[i];
    auto type = ELF::elf_st_type(dynsym->st_info);
    const bool is_adjustable = (type == STT_OBJECT ||
                                type == STT_FUNC ||
                                type == STT_SECTION ||
                                type == STT_FILE ||
                                type == STT_COMMON ||
                                type == STT_TLS);
    if (is_adjustable && dynsym->st_value > hole_start) {
      dynsym->st_value += hole_size;
      VLOG(1) << "dynsym[" << i << "] type=" << type
              << " st_value adjusted to " << dynsym->st_value;
    }
  }

  void* section_data = &dynsyms[0];
  size_t bytes = dynsyms.size() * sizeof(dynsyms[0]);
  RewriteSectionData(data, section_data, bytes);
}

// Helper for ResizeSection().  Adjust the plt relocations section for the
// hole.  We need to adjust the offset of every relocation inside it that
// falls beyond the hole start.
template <typename ELF>
static void AdjustRelPltSectionForHoleRel(Elf_Scn* relplt_section,
                                          typename ELF::Off hole_start,
                                          ssize_t hole_size) {
  Elf_Data* data = GetSectionData(relplt_section);

  const typename ELF::Rel* relplt_base = reinterpret_cast<typename ELF::Rel*>(data->d_buf);
  std::vector<typename ELF::Rel> relplts(
      relplt_base,
      relplt_base + data->d_size / sizeof(relplts[0]));

  for (size_t i = 0; i < relplts.size(); ++i) {
    typename ELF::Rel* relplt = &relplts[i];
    if (relplt->r_offset > hole_start) {
      relplt->r_offset += hole_size;
      VLOG(1) << "relplt[" << i
              << "] r_offset adjusted to " << relplt->r_offset;
    }
  }

  void* section_data = &relplts[0];
  size_t bytes = relplts.size() * sizeof(relplts[0]);
  RewriteSectionData(data, section_data, bytes);
}

template <typename ELF>
static void AdjustRelPltSectionForHoleRela(Elf_Scn* relplt_section,
                                           typename ELF::Off hole_start,
                                           ssize_t hole_size) {
  Elf_Data* data = GetSectionData(relplt_section);

  const typename ELF::Rela* relplt_base = reinterpret_cast<typename ELF::Rela*>(data->d_buf);
  std::vector<typename ELF::Rela> relplts(
      relplt_base,
      relplt_base + data->d_size / sizeof(relplts[0]));

  for (size_t i = 0; i < relplts.size(); ++i) {
    typename ELF::Rela* relplt = &relplts[i];
    if (relplt->r_offset > hole_start) {
      relplt->r_offset += hole_size;
      VLOG(1) << "relaplt[" << i
              << "] r_offset adjusted to " << relplt->r_offset;
    }
  }

  void* section_data = &relplts[0];
  size_t bytes = relplts.size() * sizeof(relplts[0]);
  RewriteSectionData(data, section_data, bytes);
}

// Helper for ResizeSection().  Adjust the .symtab section for the hole.
// We want to adjust the value of every symbol in it that falls beyond
// the hole start.
template <typename ELF>
static void AdjustSymTabSectionForHole(Elf_Scn* symtab_section,
                                       typename ELF::Off hole_start,
                                       ssize_t hole_size) {
  Elf_Data* data = GetSectionData(symtab_section);

  const auto symtab_base = reinterpret_cast<typename ELF::Sym*>(data->d_buf);
  std::vector<typename ELF::Sym> symtab(
      symtab_base,
      symtab_base + data->d_size / sizeof(symtab[0]));

  for (size_t i = 0; i < symtab.size(); ++i) {
    auto sym = &symtab[i];
    if (sym->st_value > hole_start) {
      sym->st_value += hole_size;
      VLOG(1) << "symtab[" << i << "] value adjusted to " << sym->st_value;
    }
  }

  void* section_data = &symtab[0];
  size_t bytes = symtab.size() * sizeof(symtab[0]);
  RewriteSectionData(data, section_data, bytes);
}

// Resize a section.  If the new size is larger than the current size, open
// up a hole by increasing file offsets that come after the hole.  If smaller
// than the current size, remove the hole by decreasing those offsets.
template <typename ELF>
void ElfFile<ELF>::ResizeSection(Elf* elf, Elf_Scn* section, size_t new_size,
                                 typename ELF::Word new_sh_type, relocations_type_t relocations_type) {

  size_t string_index;
  elf_getshdrstrndx(elf, &string_index);
  auto section_header = ELF::getshdr(section);
  std::string name = elf_strptr(elf, string_index, section_header->sh_name);

  // libelf overrides sh_entsize for known sh_types, so it does not matter what we set
  // for SHT_REL/SHT_RELA.
  typename ELF::Xword new_entsize = (new_sh_type == SHT_ANDROID_REL || new_sh_type == SHT_ANDROID_RELA) ? 1 : 0;

  if (section_header->sh_size == new_size) {
    return;
  }

  // We have to massage d_un.d_val in the dynamic section where d_tag is DT_RELSZ or
  // DT_RELASZ and DT_RELCOUNT or DT_RELACOUNT.

  // Require that the section size and the data size are the same.  True
  // in practice for all sections we resize when packing or unpacking.
  Elf_Data* data = GetSectionData(section);
  CHECK(data->d_off == 0 && data->d_size == section_header->sh_size);

  // Require that the section is not zero-length (that is, has allocated
  // data that we can validly expand).
  CHECK(data->d_size && data->d_buf);

  auto hole_start = section_header->sh_offset;
  const ssize_t hole_size = new_size - data->d_size;

  VLOG_IF(1, (hole_size > 0)) << "expand section (" << name << ") size: " << data->d_size << " -> " << (data->d_size + hole_size);
  VLOG_IF(1, (hole_size < 0)) << "shrink section (" << name << ") size: " << data->d_size << " -> " << (data->d_size + hole_size);
  VLOG(1) << "Update section (" << name << ") entry size: " << section_header->sh_entsize << " -> " << new_entsize;

  // Resize the data and the section header.
  data->d_size += hole_size;
  section_header->sh_size += hole_size;
  section_header->sh_entsize = new_entsize;
  section_header->sh_type = new_sh_type;

  auto elf_header = ELF::getehdr(elf);
  auto elf_program_header = ELF::getphdr(elf);

  // Add the hole size to all offsets in the ELF file that are after the
  // start of the hole.  If the hole size is positive we are expanding the
  // section to create a new hole; if negative, we are closing up a hole.

  // Start with the main ELF header.
  AdjustElfHeaderForHole<ELF>(elf_header, hole_start, hole_size);

  // Adjust all program headers.
  AdjustProgramHeadersForHole<ELF>(elf_program_header,
                                   elf_header->e_phnum,
                                   hole_start,
                                   hole_size);

  // Adjust all section headers.
  AdjustSectionHeadersForHole<ELF>(elf, hole_start, hole_size);

  // We use the dynamic program header entry to locate the dynamic section.
  const typename ELF::Phdr* dynamic_program_header = nullptr;

  // Find the dynamic program header entry.
  for (size_t i = 0; i < elf_header->e_phnum; ++i) {
    auto program_header = &elf_program_header[i];

    if (program_header->p_type == PT_DYNAMIC) {
      dynamic_program_header = program_header;
    }
  }
  CHECK(dynamic_program_header);

  // Sections requiring special attention, and the packed android
  // relocations offset.
  Elf_Scn* dynamic_section = NULL;
  Elf_Scn* dynsym_section = NULL;
  Elf_Scn* plt_relocations_section = NULL;
  Elf_Scn* symtab_section = NULL;

  // Find these sections, and the packed android relocations offset.
  section = NULL;

  while ((section = elf_nextscn(elf, section)) != NULL) {
    auto section_header = ELF::getshdr(section);
    std::string name = elf_strptr(elf, string_index, section_header->sh_name);

    if (section_header->sh_offset == dynamic_program_header->p_offset) {
      dynamic_section = section;
    }
    if (name == ".dynsym") {
      dynsym_section = section;
    }
    if (name == ".rel.plt" || name == ".rela.plt") {
      plt_relocations_section = section;
    }
    if (name == ".symtab") {
      symtab_section = section;
    }
  }
  CHECK(dynamic_section != NULL);
  CHECK(dynsym_section != NULL);
  CHECK(plt_relocations_section != NULL);

  // Adjust the .dynamic section for the hole.  Because we have to edit the
  // current contents of .dynamic we disallow resizing it.
  CHECK(section != dynamic_section);
  AdjustDynamicSectionForHole(dynamic_section,
                              hole_start,
                              hole_size,
                              relocations_type);

  // Adjust the .dynsym section for the hole.
  AdjustDynSymSectionForHole<ELF>(dynsym_section, hole_start, hole_size);

  // Adjust the plt relocations section for the hole.
  if (relocations_type == RELA) {
    AdjustRelPltSectionForHoleRela<ELF>(plt_relocations_section,
                                        hole_start,
                                        hole_size);
  } else {
    AdjustRelPltSectionForHoleRel<ELF>(plt_relocations_section,
                                       hole_start,
                                       hole_size);
  }

  // If present, adjust the .symtab section for the hole.  If the shared
  // library was stripped then .symtab will be absent.
  if (symtab_section) {
    AdjustSymTabSectionForHole<ELF>(symtab_section, hole_start, hole_size);
  }
}

// Find the first slot in a dynamics array with the given tag.  The array
// always ends with a free (unused) element, and which we exclude from the
// search.  Returns dynamics->size() if not found.
template <typename ELF>
static size_t FindDynamicEntry(typename ELF::Sword tag,
                               std::vector<typename ELF::Dyn>* dynamics) {
  // Loop until the penultimate entry.  We exclude the end sentinel.
  for (size_t i = 0; i < dynamics->size() - 1; ++i) {
    if (dynamics->at(i).d_tag == tag) {
      return i;
    }
  }

  // The tag was not found.
  return dynamics->size();
}

// Replace dynamic entry.
template <typename ELF>
static void ReplaceDynamicEntry(typename ELF::Sword tag,
                                const typename ELF::Dyn& dyn,
                                std::vector<typename ELF::Dyn>* dynamics) {
  const size_t slot = FindDynamicEntry<ELF>(tag, dynamics);
  if (slot == dynamics->size()) {
    LOG(FATAL) << "Dynamic slot is not found for tag=" << tag;
  }

  // Replace this entry with the one supplied.
  dynamics->at(slot) = dyn;
  VLOG(1) << "dynamic[" << slot << "] overwritten with " << dyn.d_tag;
}

// Adjust a relocation.  For a relocation without addend, we find its target
// in the section and adjust that.  For a relocation with addend, the target
// is the relocation addend, and the section data at the target is zero.
template <typename ELF>
static void AdjustRelocationRel(ssize_t index,
                                typename ELF::Addr hole_start,
                                ssize_t hole_size,
                                typename ELF::Off* target) {
  // Adjust the target if after the hole start.
  if (*target > hole_start) {
    *target += hole_size;
    VLOG(1) << "relocation[" << index << "] target adjusted to " << *target;
  }
}

template <typename ELF>
static void AdjustRelocationRela(ssize_t index,
                                 typename ELF::Addr hole_start,
                                 ssize_t hole_size,
                                 typename ELF::Rela* relocation) {
  // The relocation's target is the addend.  Adjust if after the hole start.
  if (relocation->r_addend > static_cast<typename ELF::Sword>(hole_start)) {
    relocation->r_addend += hole_size;
    VLOG(1) << "relocation["
            << index << "] addend adjusted to " << relocation->r_addend;
  }
}

// For relative relocations without addends, adjust the file data to which
// they refer.  For relative relocations with addends, adjust the addends.
// This translates data into the area it will occupy after the hole in
// the dynamic relocations is added or removed.
template <typename ELF>
void ElfFile<ELF>::AdjustRelativeRelocationTargets(typename ELF::Off hole_start,
                                                   ssize_t hole_size,
                                                   std::vector<typename ELF::Rela>* relocations) {
  Elf_Scn* section = NULL;
  while ((section = elf_nextscn(elf_, section)) != NULL) {
    const auto section_header = ELF::getshdr(section);

    // Ignore sections that do not appear in a process memory image.
    if (section_header->sh_addr == 0) {
      continue;
    }

    Elf_Data* data = GetSectionData(section);

    // Ignore sections with no effective data.
    if (data->d_buf == NULL)
      continue;

    // Identify this section's start and end addresses.
    const auto section_start = section_header->sh_addr;
    const auto section_end = section_start + section_header->sh_size;

    // Create a copy of the section's data.
    std::unique_ptr<uint8_t[]> area_guard(new uint8_t[data->d_size]);
    uint8_t* area = area_guard.get();
    memcpy(area, data->d_buf, data->d_size);

    for (size_t i = 0; i < relocations->size(); ++i) {
      typename ELF::Rela* relocation = &relocations->at(i);
      // skip non-relative relocations
      if (!relocation_helper_->is_relative(*relocation)) {
        continue;
      }

      // See if this relocation points into the current section.
      if (relocation->r_offset >= section_start &&
          relocation->r_offset < section_end) {
        // The relocation's target is what it points to in area.
        // For relocations without addend, this is what we adjust; for
        // relocations with addend, we leave this (it will be zero)
        // and instead adjust the addend.
        if (relocations_type_ == RELA) {
          AdjustRelocationRela<ELF>(i, hole_start, hole_size, relocation);
        } else if (relocations_type_ == REL) {
          auto byte_offset = relocation->r_offset - section_start;
          auto target = reinterpret_cast<typename ELF::Off*>(area + byte_offset);
          AdjustRelocationRel<ELF>(i, hole_start, hole_size, target);
        } else {
          NOTREACHED();
        }
      }
    }

    // If we altered the data for this section, write it back.
    if (memcmp(area, data->d_buf, data->d_size)) {
      RewriteSectionData(data, area, data->d_size);
    }
  }
}

// Adjust relocations so that the offset that they indicate will be correct
// after the hole in the dynamic relocations is added or removed (in effect,
// relocate the relocations).
template <typename ELF>
static void AdjustRelocations(typename ELF::Off hole_start,
                              ssize_t hole_size,
                              std::vector<typename ELF::Rela>* relocations) {
  for (size_t i = 0; i < relocations->size(); ++i) {
    typename ELF::Rela* relocation = &relocations->at(i);
    if (relocation->r_offset > hole_start) {
      relocation->r_offset += hole_size;
      VLOG(1) << "relocation[" << i
              << "] offset adjusted to " << relocation->r_offset;
    }
  }
}

// Remove relative entries from dynamic relocations and write as packed
// data into android packed relocations.
template <typename ELF>
bool ElfFile<ELF>::PackRelocations() {
  // Load the ELF file into libelf.
  if (!Load()) {
    LOG(ERROR) << "Failed to load as ELF";
    return false;
  }

  // Retrieve the current dynamic relocations section data.
  Elf_Data* data = GetSectionData(relocations_section_);
  // we always pack rela, because packed format is pretty much the same
  std::vector<typename ELF::Rela> relocations;

  if (relocations_type_ == REL) {
    // Convert data to a vector of relocations.
    const typename ELF::Rel* relocations_base = reinterpret_cast<typename ELF::Rel*>(data->d_buf);
    ConvertRelArrayToRelaVector(relocations_base, data->d_size / sizeof(typename ELF::Rel), &relocations);
    LOG(INFO) << "Relocations   : REL";
  } else if (relocations_type_ == RELA) {
    // Convert data to a vector of relocations with addends.
    const typename ELF::Rela* relocations_base = reinterpret_cast<typename ELF::Rela*>(data->d_buf);
    relocations = std::vector<typename ELF::Rela>(
        relocations_base,
        relocations_base + data->d_size / sizeof(relocations[0]));

    LOG(INFO) << "Relocations   : RELA";
  } else {
    NOTREACHED();
  }

  return PackTypedRelocations(&relocations, data);
}

// Helper for PackRelocations().  Rel type is one of ELF::Rel or ELF::Rela.
template <typename ELF>
bool ElfFile<ELF>::PackTypedRelocations(std::vector<typename ELF::Rela>* relocations,
                                        Elf_Data* data) {
  typedef typename ELF::Rela Rela;

  // If no relocations then we have nothing packable.  Perhaps
  // the shared object has already been packed?
  if (relocations->empty()) {
    LOG(ERROR) << "No relocations found (already packed?)";
    return false;
  }

  const size_t rel_size = relocations_type_ == RELA ? sizeof(typename ELF::Rela) : sizeof(typename ELF::Rel);
  const size_t initial_bytes = relocations->size() * rel_size;

  LOG(INFO) << "Unpacked                   : " << initial_bytes << " bytes";
  std::vector<uint8_t> packed;
  RelocationPacker<ELF> packer;

  // Pack relocations: dry run to estimate memory savings.
  packer.PackRelocations(*relocations, &packed);
  const size_t packed_bytes_estimate = packed.size() * sizeof(packed[0]);
  LOG(INFO) << "Packed (before adjustments): " << packed_bytes_estimate << " bytes";

  // If we have insufficient relative relocations to form a run then
  // packing fails.
  if (packed.empty()) {
    LOG(INFO) << "Too few relocations to pack";
    return false;
  }

  // Pre-calculate the size of the hole we will close up when we rewrite
  // dynamic relocations.  We have to adjust relocation addresses to
  // account for this.
  typename ELF::Shdr* section_header = ELF::getshdr(relocations_section_);
  const typename ELF::Off hole_start = section_header->sh_offset;
  ssize_t hole_size = initial_bytes - packed_bytes_estimate;
  const ssize_t unaligned_hole_size = hole_size;

  // hole_size needs to be page_aligned.
  hole_size -= hole_size % kPreserveAlignment;

  LOG(INFO) << "Compaction                 : " << hole_size << " bytes";

  // Adjusting for alignment may have removed any packing benefit.
  if (hole_size == 0) {
    LOG(INFO) << "Too few relocations to pack after alignment";
    return false;
  }

  size_t data_padding_bytes = 0;

  if (!is_padding_relocations_) {
    // Apply relocations to all relative data to relocate it into the
    // area it will occupy once the hole in the dynamic relocations is removed.
    AdjustRelativeRelocationTargets(hole_start, -hole_size, relocations);
    // Adjust r_offset for relocations.
    AdjustRelocations<ELF>(hole_start, -hole_size, relocations);
    // pack relocations again
    packed.clear();
    packer.PackRelocations(*relocations, &packed);
    // size can't increase when we pack with reduced r_addend/r_offset
    // but just in case I am wrong - lets check it
    CHECK(packed.size() <= packed_bytes_estimate);
    // Make sure packing saved some space.
    if (packed.size() >= initial_bytes) {
      LOG(INFO) << "Packing relative relocations saves no space";
      return false;
    }

    data_padding_bytes = initial_bytes - hole_size - packed.size();
  } else {
    // do not adjust...
    data_padding_bytes = initial_bytes - packed_bytes_estimate;
  }

  // pad data
  std::vector<uint8_t> padding(data_padding_bytes, 0);
  packed.insert(packed.end(), padding.begin(), padding.end());

  const void* packed_data = &packed[0];
  const size_t packed_bytes = packed.size() * sizeof(packed[0]);


  // Run a loopback self-test as a check that packing is lossless.
  std::vector<Rela> unpacked;
  packer.UnpackRelocations(packed, &unpacked);
  CHECK(unpacked.size() == relocations->size());
  CHECK(!memcmp(&unpacked[0],
                &relocations->at(0),
                unpacked.size() * sizeof(unpacked[0])));

  // Rewrite the current dynamic relocations section with packed one then shrink it to size.
  const size_t bytes = packed.size() * sizeof(packed[0]);
  ResizeSection(elf_, relocations_section_, bytes, relocations_type_ == REL ? SHT_ANDROID_REL : SHT_ANDROID_RELA, relocations_type_);
  RewriteSectionData(data, packed_data, bytes);

  // TODO: (dimitry) fix string table and replace .rel.dyn/plt with .android.rel.dyn/plt

  // Rewrite the current packed android relocations section to hold the packed
  // relative relocations.

  // TODO: continue here

  // Rewrite .dynamic and rename relocation tags describing the packed android
  // relocations.
  data = GetSectionData(dynamic_section_);
  const typename ELF::Dyn* dynamic_base = reinterpret_cast<typename ELF::Dyn*>(data->d_buf);
  std::vector<typename ELF::Dyn> dynamics(
      dynamic_base,
      dynamic_base + data->d_size / sizeof(dynamics[0]));
  // Use two of the spare slots to describe the packed section.
  section_header = ELF::getshdr(relocations_section_);
  const typename ELF::Dyn offset_dyn
      = { relocations_type_ == REL ? DT_ANDROID_REL : DT_ANDROID_RELA, {section_header->sh_offset}};
  ReplaceDynamicEntry<ELF>(relocations_type_ == REL ? DT_REL : DT_RELA, offset_dyn, &dynamics);
  const typename ELF::Dyn size_dyn
      = { relocations_type_ == REL ? DT_ANDROID_RELSZ : DT_ANDROID_RELASZ, {section_header->sh_size}};
  ReplaceDynamicEntry<ELF>(relocations_type_ == REL ? DT_RELSZ : DT_RELASZ, size_dyn, &dynamics);
  const void* dynamics_data = &dynamics[0];
  const size_t dynamics_bytes = dynamics.size() * sizeof(dynamics[0]);
  RewriteSectionData(data, dynamics_data, dynamics_bytes);

  Flush();
  return true;
}

// Find packed relative relocations in the packed android relocations
// section, unpack them, and rewrite the dynamic relocations section to
// contain unpacked data.
template <typename ELF>
bool ElfFile<ELF>::UnpackRelocations() {
  // Load the ELF file into libelf.
  if (!Load()) {
    LOG(ERROR) << "Failed to load as ELF";
    return false;
  }

  typename ELF::Shdr* section_header = ELF::getshdr(relocations_section_);
  // Retrieve the current packed android relocations section data.
  Elf_Data* data = GetSectionData(relocations_section_);

  // Convert data to a vector of bytes.
  const uint8_t* packed_base = reinterpret_cast<uint8_t*>(data->d_buf);
  std::vector<uint8_t> packed(
      packed_base,
      packed_base + data->d_size / sizeof(packed[0]));

  if ((section_header->sh_type == SHT_ANDROID_RELA || section_header->sh_type == SHT_ANDROID_REL) &&
      packed.size() > 3 &&
      packed[0] == 'A' &&
      packed[1] == 'P' &&
      (packed[2] == 'U' || packed[2] == 'S') &&
      packed[3] == '2') {
    // Signature is APR1, unpack relocations using leb128
    LOG(INFO) << "Relocations   : " << (relocations_type_ == REL ? "REL" : "RELA");
  } else {
    LOG(ERROR) << "Packed relocations not found (not packed?)";
    return false;
  }

  return UnpackTypedRelocations(packed, data);
}

// Helper for UnpackRelocations().  Rel type is one of ELF::Rel or ELF::Rela.
template <typename ELF>
bool ElfFile<ELF>::UnpackTypedRelocations(const std::vector<uint8_t>& packed,
                                          Elf_Data* data) {
  // Unpack the data to re-materialize the relative relocations.
  const size_t packed_bytes = packed.size() * sizeof(packed[0]);
  LOG(INFO) << "Packed           : " << packed_bytes << " bytes";
  std::vector<typename ELF::Rela> unpacked_relocations;
  RelocationPacker<ELF> packer;
  packer.UnpackRelocations(packed, &unpacked_relocations);

  const size_t relocation_entry_size = relocations_type_ == REL ? sizeof(typename ELF::Rel) : sizeof(typename ELF::Rela);
  const size_t unpacked_bytes = unpacked_relocations.size() * relocation_entry_size;
  LOG(INFO) << "Unpacked         : " << unpacked_bytes << " bytes";

  // Retrieve the current dynamic relocations section data.
  data = GetSectionData(relocations_section_);

  size_t padding = 0;

  LOG(INFO) << "Relocations      : " << unpacked_relocations.size() << " entries";

  // If we found the same number of null relocation entries in the dynamic
  // relocations section as we hold as unpacked relative relocations, then
  // this is a padded file.

  const bool is_padded = packed_bytes == unpacked_bytes;

  // Unless padded, pre-apply relative relocations to account for the
  // hole, and pre-adjust all relocation offsets accordingly.
  typename ELF::Shdr* section_header = ELF::getshdr(relocations_section_);

  if (!is_padded) {
    // Pre-calculate the size of the hole we will open up when we rewrite
    // dynamic relocations.  We have to adjust relocation addresses to
    // account for this.
    const typename ELF::Off hole_start = section_header->sh_offset;
    ssize_t hole_size = unpacked_bytes - packed_bytes;

    // Adjust the hole size for the padding added to preserve alignment.
    LOG(INFO) << "Expansion     : " << hole_size << " bytes";

    // Apply relocations to all relative data to relocate it into the
    // area it will occupy once the hole in dynamic relocations is opened.
    AdjustRelativeRelocationTargets(hole_start, hole_size, &unpacked_relocations);
    // Relocate the relocations.
    AdjustRelocations<ELF>(hole_start, hole_size, &unpacked_relocations);
  }

  // Rewrite the current dynamic relocations section with unpacked version of
  // relocations.
  const void* section_data = nullptr;
  std::vector<typename ELF::Rel> unpacked_rel_reloactions;
  if (relocations_type_ == RELA) {
    section_data = &unpacked_relocations[0];
  } else {
    ConvertRelaVectorToRelVector(unpacked_relocations, &unpacked_rel_reloactions);
    section_data = &unpacked_rel_reloactions[0];
  }

  ResizeSection(elf_, relocations_section_, unpacked_bytes, relocations_type_ == REL ? SHT_REL : SHT_RELA, relocations_type_);
  RewriteSectionData(data, section_data, unpacked_bytes);

  // Rewrite .dynamic to remove two tags describing packed android relocations.
  data = GetSectionData(dynamic_section_);
  const typename ELF::Dyn* dynamic_base = reinterpret_cast<typename ELF::Dyn*>(data->d_buf);
  std::vector<typename ELF::Dyn> dynamics(
      dynamic_base,
      dynamic_base + data->d_size / sizeof(dynamics[0]));
  const typename ELF::Dyn offset_dyn
      = { relocations_type_ == REL ? DT_REL : DT_RELA, {section_header->sh_offset}};
  ReplaceDynamicEntry<ELF>(relocations_type_ == REL ? DT_ANDROID_REL : DT_ANDROID_RELA, offset_dyn, &dynamics);
  const typename ELF::Dyn size_dyn
      = { relocations_type_ == REL ? DT_RELSZ : DT_RELASZ, {section_header->sh_size}};
  ReplaceDynamicEntry<ELF>(relocations_type_ == REL ? DT_ANDROID_RELSZ : DT_ANDROID_RELASZ, size_dyn, &dynamics);
  const void* dynamics_data = &dynamics[0];
  const size_t dynamics_bytes = dynamics.size() * sizeof(dynamics[0]);
  RewriteSectionData(data, dynamics_data, dynamics_bytes);

  Flush();
  return true;
}

// Flush rewritten shared object file data.
template <typename ELF>
void ElfFile<ELF>::Flush() {
  // Flag all ELF data held in memory as needing to be written back to the
  // file, and tell libelf that we have controlled the file layout.
  elf_flagelf(elf_, ELF_C_SET, ELF_F_DIRTY);
  elf_flagelf(elf_, ELF_C_SET, ELF_F_LAYOUT);

  // Write ELF data back to disk.
  const off_t file_bytes = elf_update(elf_, ELF_C_WRITE);
  if (file_bytes == -1) {
    LOG(ERROR) << "elf_update failed: " << elf_errmsg(elf_errno());
  }
  CHECK(file_bytes > 0);
  VLOG(1) << "elf_update returned: " << file_bytes;

  // Clean up libelf, and truncate the output file to the number of bytes
  // written by elf_update().
  elf_end(elf_);
  elf_ = NULL;
  const int truncate = ftruncate(fd_, file_bytes);
  CHECK(truncate == 0);
}

template <typename ELF>
void ElfFile<ELF>::ConvertRelArrayToRelaVector(const typename ELF::Rel* rel_array, size_t rel_array_size,
                                               std::vector<typename ELF::Rela>* rela_vector) {
  for (size_t i = 0; i<rel_array_size; ++i) {
    typename ELF::Rela rela;
    rela.r_offset = rel_array[i].r_offset;
    rela.r_info = rel_array[i].r_info;
    rela.r_addend = 0;
    rela_vector->push_back(rela);
  }
}

template <typename ELF>
void ElfFile<ELF>::ConvertRelaVectorToRelVector(const std::vector<typename ELF::Rela>& rela_vector,
                                                std::vector<typename ELF::Rel>* rel_vector) {
  for (auto rela : rela_vector) {
    typename ELF::Rel rel;
    rel.r_offset = rela.r_offset;
    rel.r_info = rela.r_info;
    CHECK(rela.r_addend == 0);
    rel_vector->push_back(rel);
  }
}

template class ElfFile<ELF32_traits>;
template class ElfFile<ELF64_traits>;

}  // namespace relocation_packer
