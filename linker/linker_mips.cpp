/*
 * Copyright (C) 2015 The Android Open Source Project
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

#if !defined(__LP64__) && __mips_isa_rev >= 5
#include <sys/prctl.h>
#if defined(PR_SET_FP_MODE)
#error "remove following defs when avail in Android's kernel headers"
#else
#define PR_SET_FP_MODE 45
#define PR_GET_FP_MODE 46
#define PR_FP_MODE_FR  (1 << 0)
#define PR_FP_MODE_FRE (1 << 1)
#endif
#endif

#include "linker.h"
#include "linker_debug.h"
#include "linker_phdr.h"
#include "linker_relocs.h"
#include "linker_reloc_iterators.h"
#include "linker_sleb128.h"

template bool soinfo::relocate<plain_reloc_iterator>(const VersionTracker& version_tracker,
                                                     plain_reloc_iterator&& rel_iterator,
                                                     const soinfo_list_t& global_group,
                                                     const soinfo_list_t& local_group);

template bool soinfo::relocate<packed_reloc_iterator<sleb128_decoder>>(
    const VersionTracker& version_tracker,
    packed_reloc_iterator<sleb128_decoder>&& rel_iterator,
    const soinfo_list_t& global_group,
    const soinfo_list_t& local_group);

template <typename ElfRelIteratorT>
bool soinfo::relocate(const VersionTracker& version_tracker,
                      ElfRelIteratorT&& rel_iterator,
                      const soinfo_list_t& global_group,
                      const soinfo_list_t& local_group) {
  for (size_t idx = 0; rel_iterator.has_next(); ++idx) {
    const auto rel = rel_iterator.next();

    if (rel == nullptr) {
      return false;
    }

    ElfW(Word) type = ELFW(R_TYPE)(rel->r_info);
    ElfW(Word) sym = ELFW(R_SYM)(rel->r_info);

    ElfW(Addr) reloc = static_cast<ElfW(Addr)>(rel->r_offset + load_bias);
    ElfW(Addr) sym_addr = 0;
    const char* sym_name = nullptr;

    DEBUG("Processing '%s' relocation at index %zd", get_soname(), idx);
    if (type == R_GENERIC_NONE) {
      continue;
    }

    const ElfW(Sym)* s = nullptr;
    soinfo* lsi = nullptr;

    if (sym != 0) {
      sym_name = get_string(symtab_[sym].st_name);
      const version_info* vi = nullptr;

      if (!lookup_version_info(version_tracker, sym, sym_name, &vi)) {
        return false;
      }

      if (!soinfo_do_lookup(this, sym_name, vi, &lsi, global_group, local_group, &s)) {
        return false;
      }

      if (s == nullptr) {
        // mips does not support relocation with weak-undefined symbols
        DL_ERR("cannot locate symbol \"%s\" referenced by \"%s\"...", sym_name, get_soname());
        return false;
      } else {
        // We got a definition.
        sym_addr = lsi->resolve_symbol_address(s);
      }
      count_relocation(kRelocSymbol);
    }

    switch (type) {
      case R_MIPS_REL32:
#if defined(__LP64__)
        // MIPS Elf64_Rel entries contain compound relocations
        // We only handle the R_MIPS_NONE|R_MIPS_64|R_MIPS_REL32 case
        if (ELF64_R_TYPE2(rel->r_info) != R_MIPS_64 ||
            ELF64_R_TYPE3(rel->r_info) != R_MIPS_NONE) {
          DL_ERR("Unexpected compound relocation type:%d type2:%d type3:%d @ %p (%zu)",
                 type, static_cast<unsigned>(ELF64_R_TYPE2(rel->r_info)),
                 static_cast<unsigned>(ELF64_R_TYPE3(rel->r_info)), rel, idx);
          return false;
        }
#endif
        count_relocation(s == nullptr ? kRelocAbsolute : kRelocRelative);
        MARK(rel->r_offset);
        TRACE_TYPE(RELO, "RELO REL32 %08zx <- %08zx %s", static_cast<size_t>(reloc),
                   static_cast<size_t>(sym_addr), sym_name ? sym_name : "*SECTIONHDR*");
        if (s != nullptr) {
          *reinterpret_cast<ElfW(Addr)*>(reloc) += sym_addr;
        } else {
          *reinterpret_cast<ElfW(Addr)*>(reloc) += load_bias;
        }
        break;
      default:
        DL_ERR("unknown reloc type %d @ %p (%zu)", type, rel, idx);
        return false;
    }
  }
  return true;
}

bool soinfo::mips_relocate_got(const VersionTracker& version_tracker,
                               const soinfo_list_t& global_group,
                               const soinfo_list_t& local_group) {
  ElfW(Addr)** got = plt_got_;
  if (got == nullptr) {
    return true;
  }

  // got[0] is the address of the lazy resolver function.
  // got[1] may be used for a GNU extension.
  // Set it to a recognizable address in case someone calls it (should be _rtld_bind_start).
  // FIXME: maybe this should be in a separate routine?
  if ((flags_ & FLAG_LINKER) == 0) {
    size_t g = 0;
    got[g++] = reinterpret_cast<ElfW(Addr)*>(0xdeadbeef);
    if (reinterpret_cast<intptr_t>(got[g]) < 0) {
      got[g++] = reinterpret_cast<ElfW(Addr)*>(0xdeadfeed);
    }
    // Relocate the local GOT entries.
    for (; g < mips_local_gotno_; g++) {
      got[g] = reinterpret_cast<ElfW(Addr)*>(reinterpret_cast<uintptr_t>(got[g]) + load_bias);
    }
  }

  // Now for the global GOT entries...
  got = plt_got_ + mips_local_gotno_;
  for (ElfW(Word) sym = mips_gotsym_; sym < mips_symtabno_; sym++, got++) {
    // This is an undefined reference... try to locate it.
    const ElfW(Sym)* local_sym = symtab_ + sym;
    const char* sym_name = get_string(local_sym->st_name);
    soinfo* lsi = nullptr;
    const ElfW(Sym)* s = nullptr;

    ElfW(Word) st_visibility = (local_sym->st_other & 0x3);

    if (st_visibility == STV_DEFAULT) {
      const version_info* vi = nullptr;

      if (!lookup_version_info(version_tracker, sym, sym_name, &vi)) {
        return false;
      }

      if (!soinfo_do_lookup(this, sym_name, vi, &lsi, global_group, local_group, &s)) {
        return false;
      }
    } else if (st_visibility == STV_PROTECTED) {
      if (local_sym->st_value == 0) {
        DL_ERR("%s: invalid symbol \"%s\" (PROTECTED/UNDEFINED) ", get_soname(), sym_name);
        return false;
      }
      s = local_sym;
      lsi = this;
    } else {
      DL_ERR("%s: invalid symbol \"%s\" visibility: 0x%x", get_soname(), sym_name, st_visibility);
      return false;
    }

    if (s == nullptr) {
      // We only allow an undefined symbol if this is a weak reference.
      if (ELF_ST_BIND(local_sym->st_info) != STB_WEAK) {
        DL_ERR("%s: cannot locate \"%s\"...", get_soname(), sym_name);
        return false;
      }
      *got = 0;
    } else {
      // FIXME: is this sufficient?
      // For reference see NetBSD link loader
      // http://cvsweb.netbsd.org/bsdweb.cgi/src/libexec/ld.elf_so/arch/mips/mips_reloc.c?rev=1.53&content-type=text/x-cvsweb-markup
      *got = reinterpret_cast<ElfW(Addr)*>(lsi->resolve_symbol_address(s));
    }
  }
  return true;
}

#if !defined(__LP64__)

#if defined(PT_MIPS_ABIFLAGS)
#error "remove following defs when Android's elf.h updated to glibc 2.21"
#else

#define PT_MIPS_ABIFLAGS  0x70000003	/* .MIPS.abiflags segment */

typedef struct {
  /* Version of flags structure.  */
  Elf32_Half version;
  /* The level of the ISA: 1-5, 32, 64.  */
  unsigned char isa_level;
  /* The revision of ISA: 0 for MIPS V and below, 1-n otherwise.  */
  unsigned char isa_rev;
  /* The size of general purpose registers.  */
  unsigned char gpr_size;
  /* The size of co-processor 1 registers.  */
  unsigned char cpr1_size;
  /* The size of co-processor 2 registers.  */
  unsigned char cpr2_size;
  /* The floating-point ABI.  */
  unsigned char fp_abi;
  /* Processor-specific extension.  */
  Elf32_Word isa_ext;
  /* Mask of ASEs used.  */
  Elf32_Word ases;
  /* Mask of general flags.  */
  Elf32_Word flags1;
  Elf32_Word flags2;
} Elf_MIPS_ABIFlags_v0;

/* Masks for the flags1 word of an ABI flags structure.  */
#define MIPS_AFL_FLAGS1_ODDSPREG  1  /* Uses odd single-precision registers.  */

/* Object attribute values.  */
enum
{
  /* Not tagged or not using any ABIs affected by the differences.  */
  Val_GNU_MIPS_ABI_FP_ANY = 0,
  /* Using hard-float -mdouble-float.  */
  Val_GNU_MIPS_ABI_FP_DOUBLE = 1,
  /* Using hard-float -msingle-float.  */
  Val_GNU_MIPS_ABI_FP_SINGLE = 2,
  /* Using soft-float.  */
  Val_GNU_MIPS_ABI_FP_SOFT = 3,
  /* Using -mips32r2 -mfp64.  */
  Val_GNU_MIPS_ABI_FP_OLD_64 = 4,
  /* Using -mfpxx.  */
  Val_GNU_MIPS_ABI_FP_XX = 5,
  /* Using -mips32r2 -mfp64.  */
  Val_GNU_MIPS_ABI_FP_64 = 6,
  /* Using -mips32r2 -mfp64 -mno-odd-spreg.  */
  Val_GNU_MIPS_ABI_FP_64A = 7,
  /* Maximum allocated FP ABI value.  */
  Val_GNU_MIPS_ABI_FP_MAX = 7
};
#endif

#  ifndef PT_MIPS_ABIFLAGS
#    define PT_MIPS_ABIFLAGS    0x70000003      /* .MIPS.abiflags segment */
#  endif

/* Return the address of the .MIPS.abiflags section in memory, if present.
 *
 * Input:
 *   phdr_table  -> program header table
 *   phdr_count  -> number of entries in tables
 *   load_bias   -> load bias
 * Output:
 *   mips_abiflags -> Address of structure in memory (null if missing)
 * Return:
 *   true when no malformed PT_MIPS_ABIFLAGS header was found
 */
static bool get_mips_abiflags(const ElfW(Phdr)* phdr_table,
                              size_t phdr_count,
                              ElfW(Addr) load_bias,
                              Elf_MIPS_ABIFlags_v0** mips_abiflags) {
  const ElfW(Phdr)* phdr = phdr_table;
  const ElfW(Phdr)* phdr_limit = phdr + phdr_count;

  for (phdr = phdr_table; phdr < phdr_limit; phdr++) {
    if (phdr->p_type != PT_MIPS_ABIFLAGS) {
      continue;
    }

    // perform size checks etc
    if (phdr->p_filesz < sizeof (Elf_MIPS_ABIFlags_v0)) {
      *mips_abiflags = nullptr;
      return false;
    }

    *mips_abiflags = reinterpret_cast<Elf_MIPS_ABIFlags_v0*>(load_bias + phdr->p_vaddr);
    return true;
  }
  *mips_abiflags = nullptr;
  return true;
}

#if __mips_isa_rev >= 5
static bool mips_fre_mode_on = false;  // have set FRE=1 mode for process
#endif

bool soinfo::mips_check_and_adjust_fp_modes() {
  Elf_MIPS_ABIFlags_v0* abiflags;
  int mips_fpabi;

  // FP ABI-variant compatibility checks for MIPS o32 ABI
  if (!get_mips_abiflags(phdr, phnum, load_bias, &abiflags)) {
    DL_ERR("Corrupt PT_MIPS_ABIFLAGS header found \"%s\"", get_soname());
    return false;
  }
  if (abiflags == nullptr) {
    // Old compiles lack the new abiflags section.
    // These compilers used -mfp32 -mdouble-float -modd-spreg defaults,
    //   ie FP32 aka DOUBLE, using odd-numbered single-prec regs
    mips_fpabi = Val_GNU_MIPS_ABI_FP_DOUBLE;
  } else {
    mips_fpabi = abiflags->fp_abi;
    if ( (abiflags->flags1 & MIPS_AFL_FLAGS1_ODDSPREG)
         && (mips_fpabi == Val_GNU_MIPS_ABI_FP_XX ||
             mips_fpabi == Val_GNU_MIPS_ABI_FP_64A   ) ) {
      // Android supports fewer cases than Linux
      DL_ERR("Unsupported odd-single-prec FloatPt reg uses in \"%s\"", get_soname());
      return false;
    }
  }
  if (!(mips_fpabi == Val_GNU_MIPS_ABI_FP_DOUBLE ||
#if __mips_isa_rev >= 5
        mips_fpabi == Val_GNU_MIPS_ABI_FP_64A    ||
#endif
        mips_fpabi == Val_GNU_MIPS_ABI_FP_XX       )) {
    DL_ERR("Unsupported MIPS32 FloatPt ABI %d found in \"%s\"",
           mips_fpabi, get_soname());
    return false;
  }

#if __mips_isa_rev >= 5
  // Adjust process's FR Emulation mode, if needed
  //
  // On Mips R5 & R6, Android runs continuously in FR=1 64bit-fpreg mode.
  // NDK mips32 apps compiled with old compilers generate FP32 code
  //   which expects FR=0 32-bit fp registers.
  // NDK mips32 apps compiled with newer compilers generate modeless
  //   FPXX code which runs on both FR=0 and FR=1 modes.
  // Android itself is compiled in FP64A which requires FR=1 mode.
  // FP32, FPXX, and FP64A all interlink okay, without dynamic FR mode
  //   changes during calls.  For details, see
  //   http://dmz-portal.mips.com/wiki/MIPS_O32_ABI_-_FR0_and_FR1_Interlinking
  // Processes containing FR32 FR=0 code are run via kernel software assist,
  //   which maps all odd-numbered single-precision reg refs onto the
  //   upper half of the paired even-numbered double-precision reg.
  // FRE=1 triggers traps to the kernel's emulator on every single-precision
  //   fp op (for both odd and even-numbered registers).
  // Turning on FRE=1 traps is done at most once per process, simultanously
  //   for all threads of that process, when dlopen discovers FP32 code.
  // The kernel repacks threads' registers when FRE mode is turn on or off.
  //   These asynchronous adjustments are wrong if any thread was executing
  //   FPXX code using odd-numbered single-precision regs.
  // Current Android compilers default to the -mno-oddspreg option,
  //   and this requirement is checked by Android's dlopen.
  //   So FRE can always be safely turned on for FP32, anytime.
  // Deferred enhancement: Allow loading of odd-spreg FPXX modules.

  if (mips_fpabi == Val_GNU_MIPS_ABI_FP_DOUBLE && !mips_fre_mode_on) {
    // Turn on FRE mode, which emulates mode-sensitive FR=0 code on FR=1
    //   register files, by trapping to kernel on refs to single-precision regs
    if (prctl(PR_SET_FP_MODE, PR_FP_MODE_FR|PR_FP_MODE_FRE)) {
      DL_ERR("Kernel or cpu failed to set FRE mode required for running \"%s\"", get_soname());
      return false;
    }
    DL_WARN("Using FRE=1 mode to run \"%s\"", get_soname());
    mips_fre_mode_on = true;  // Avoid future redundant mode-switch calls
    // FRE mode is never turned back off.
    // Deferred enhancement:
    //   Reset FRE mode when dlclose() removes all FP32 modules
  }
#else
  // Android runs continuously in FR=0 32bit-fpreg mode.
#endif  // __mips_isa_rev
  return true;
}

#endif  // __LP64___
