/*
 * Copyright (C) 2019 The Android Open Source Project
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

#include "linker_relocate.h"

#include <elf.h>
#include <link.h>

#include <type_traits>

#include "linker.h"
#include "linker_debug.h"
#include "linker_globals.h"
#include "linker_relocs.h"
#include "linker_reloc_iterators.h"
#include "linker_sleb128.h"
#include "linker_soinfo.h"
#include "private/bionic_globals.h"

#if defined(__arm__) || defined(__aarch64__)
#define USE_GNU_HASH_NEON 1
#else
#define USE_GNU_HASH_NEON 0
#endif

#if USE_GNU_HASH_NEON
#include "arch/arm_neon/linker_gnu_hash.h"
#endif

SymbolLookupLib::SymbolLookupLib(soinfo* si) {
  si_ = si;

  // For libs that only have SysV hashes, leave the gnu_bloom_filter_ field NULL to signal that
  // the fallback code path is needed.
  if (!si->is_gnu_hash()) {
    return;
  }

  gnu_maskwords_ = si->gnu_maskwords_;
  gnu_shift2_ = si->gnu_shift2_;
  gnu_bloom_filter_ = si->gnu_bloom_filter_;

  CHECK(gnu_bloom_filter_);

  strtab_ = si->strtab_;
  strtab_size_ = si->strtab_size_;
  symtab_ = si->symtab_;
  versym_ = si->get_versym_table();

  gnu_chain_ = si->gnu_chain_;
  gnu_nbucket_ = si->gnu_nbucket_;
  gnu_bucket_ = si->gnu_bucket_;
}

SymbolLookupList::SymbolLookupList(soinfo* si) : sole_lib_(si), begin_(&sole_lib_), end_(&sole_lib_ + 1) {
  CHECK(si != nullptr);
  fallback_lib_count_ += sole_lib_.needs_fallback();
}

SymbolLookupList::SymbolLookupList(const soinfo_list_t& global_group, const soinfo_list_t& local_group) {
  libs_.reserve(1 + global_group.size() + local_group.size());

  // Reserve a space in front for DT_SYMBOLIC lookup.
  libs_.push_back(SymbolLookupLib {});

  global_group.for_each([this](soinfo* si) {
    libs_.push_back(SymbolLookupLib(si));
    fallback_lib_count_ += libs_.back().needs_fallback();
  });

  local_group.for_each([this](soinfo* si) {
    libs_.push_back(SymbolLookupLib(si));
    fallback_lib_count_ += libs_.back().needs_fallback();
  });

  begin_ = &libs_[1];
  end_ = &libs_[0] + libs_.size();
}

/* "This element's presence in a shared object library alters the dynamic linker's
 * symbol resolution algorithm for references within the library. Instead of starting
 * a symbol search with the executable file, the dynamic linker starts from the shared
 * object itself. If the shared object fails to supply the referenced symbol, the
 * dynamic linker then searches the executable file and other shared objects as usual."
 *
 * http://www.sco.com/developers/gabi/2012-12-31/ch5.dynamic.html
 *
 * Note that this is unlikely since static linker avoids generating
 * relocations for -Bsymbolic linked dynamic executables.
 */
void SymbolLookupList::set_dt_symbolic_lib(soinfo* lib) {
  CHECK(!libs_.empty());
  fallback_lib_count_ -= libs_[0].needs_fallback();
  libs_[0] = lib ? SymbolLookupLib(lib) : SymbolLookupLib();
  begin_ = lib ? &libs_[0] : &libs_[1];
  fallback_lib_count_ += libs_[0].needs_fallback();
}

// TODO: linker_soinfo.cpp also has a version of this function, but with different arguments.
static inline bool check_symbol_version_2(const ElfW(Versym)* ver_table, uint32_t sym_idx,
                                          const ElfW(Versym) verneed) {
  if (ver_table == nullptr) return true;
  const uint32_t verdef = ver_table[sym_idx];
  if (verneed == kVersymNotNeeded) {
    return !(verdef & kVersymHiddenBit);
  } else {
    return verneed == (verdef & ~kVersymHiddenBit);
  }
}

__attribute__((always_inline)) static inline const ElfW(Sym)*
finish_lookup(const SymbolLookupLib& lib, const char* name, size_t name_len, uint32_t hash31,
              const version_info* vi, uint32_t sym_idx) {
  // lookup versym for the version definition in this library
  // note the difference between "version is not requested" (vi == nullptr)
  // and "version not found". In the first case verneed is kVersymNotNeeded
  // which implies that the default version can be accepted; the second case results in
  // verneed = 1 (kVersymGlobal) and implies that we should ignore versioned symbols
  // for this library and consider only *global* ones.
  ElfW(Versym) verneed = kVersymNotNeeded;
  bool calculated_verneed = false;

  const ElfW(Sym)* const symtab = lib.symtab_;
  const char* const strtab = lib.strtab_;
  const uint32_t* const gnu_chain = lib.gnu_chain_;
  const ElfW(Versym)* const versym = lib.versym_;
  uint32_t chain_value = 0;
  const ElfW(Sym)* sym = nullptr;

  auto do_lookup = [&](auto calc_verneed) -> const ElfW(Sym)* {
    do {
      sym = symtab + sym_idx;
      chain_value = gnu_chain[sym_idx];
      if ((chain_value >> 1) == hash31) {
        ElfW(Versym) inner_verneed = calc_verneed();
        if (check_symbol_version_2(versym, sym_idx, inner_verneed) &&
            // TODO: This strtab bounds check isn't quite right (e.g. overflow...)
            sym->st_name + name_len + 1 <= lib.strtab_size_ &&
            memcmp(strtab + sym->st_name, name, name_len + 1) == 0 &&
            is_symbol_global_and_defined(lib.si_, sym)) {
          return sym;
        }
      }
      ++sym_idx;
    } while ((chain_value & 1) == 0);
    return nullptr;
  };

  if (vi != nullptr) {
    return do_lookup([&]() -> ElfW(Versym) {
      if (!calculated_verneed) {
        calculated_verneed = true;
        verneed = find_verdef_version_index(lib.si_, vi);
      }
      return verneed;
    });
  } else {
    return do_lookup([]() -> ElfW(Versym) { return kVersymNotNeeded; });
  }
}

template <bool EnableFallback>
__attribute__((noinline)) static const ElfW(Sym)*
soinfo_do_lookup_impl(const char* name, const version_info* vi,
                      soinfo** si_found_in, const SymbolLookupList& lookup_list) {
  const std::pair<uint32_t, uint32_t> hash_len = calculate_gnu_hash(name);
  const uint32_t hash = hash_len.first;
  const uint32_t name_len = hash_len.second;
  constexpr uint32_t kBloomMaskBits = sizeof(ElfW(Addr)) * 8;
  SymbolName fallback_symbol_name(name);

  const SymbolLookupLib* end = lookup_list.end();
  const SymbolLookupLib* it = lookup_list.begin();

  while (true) {
    const SymbolLookupLib* lib;
    uint32_t first_index;

    // Iterate over libraries until we find one whose Bloom filter matches the symbol we're
    // searching for.
    while (true) {
      if (it == end) return nullptr;
      lib = it++;

      if (EnableFallback && lib->needs_fallback()) {
        // Fallback -- search this DSO the slow way.
        if (const ElfW(Sym)* result = lib->si_->find_symbol_by_name(fallback_symbol_name, vi)) {
          *si_found_in = lib->si_;
          return result;
        }
        continue;
      }

      const uint32_t word_num = (hash / kBloomMaskBits) & lib->gnu_maskwords_;
      const ElfW(Addr) bloom_word = lib->gnu_bloom_filter_[word_num];
      const uint32_t h1 = hash % kBloomMaskBits;
      const uint32_t h2 = (hash >> lib->gnu_shift2_) % kBloomMaskBits;

      if ((1 & (bloom_word >> h1) & (bloom_word >> h2)) == 1) {
        first_index = lib->gnu_bucket_[hash % lib->gnu_nbucket_];
        if (first_index != 0) {
          break;
        }
      }
    }

    // Search the library's hash table chain.
    if (const ElfW(Sym)* sym = finish_lookup(*lib, name, name_len, hash >> 1, vi, first_index)) {
      *si_found_in = lib->si_;
      return sym;
    }
  }
}

static const ElfW(Sym)* soinfo_do_lookup(const char* name, const version_info* vi,
                                         soinfo** si_found_in, const SymbolLookupList& lookup_list) {
  if (lookup_list.needs_fallback()) {
    return soinfo_do_lookup_impl<true>(name, vi, si_found_in, lookup_list);
  } else {
    return soinfo_do_lookup_impl<false>(name, vi, si_found_in, lookup_list);
  }
}

#if defined(USE_RELA)
static ElfW(Addr) get_addend(const ElfW(Rela)* rela, ElfW(Addr) reloc_addr __unused) {
  return rela->r_addend;
}
#else
static ElfW(Addr) get_addend(const ElfW(Rel)* rel, ElfW(Addr) reloc_addr) {
  // The i386 psABI specifies that R_386_GLOB_DAT doesn't have an addend. The ARM ELF ABI document
  // (IHI0044F) specifies that R_ARM_GLOB_DAT has an addend, but Bionic isn't adding it.
  if (ELFW(R_TYPE)(rel->r_info) == R_GENERIC_RELATIVE ||
      ELFW(R_TYPE)(rel->r_info) == R_GENERIC_IRELATIVE ||
      ELFW(R_TYPE)(rel->r_info) == R_GENERIC_ABSOLUTE ||
      ELFW(R_TYPE)(rel->r_info) == R_GENERIC_TLS_DTPREL ||
      ELFW(R_TYPE)(rel->r_info) == R_GENERIC_TLS_TPREL) {
    return *reinterpret_cast<ElfW(Addr)*>(reloc_addr);
  }
  return 0;
}
#endif

static bool is_tls_reloc(ElfW(Word) type) {
  switch (type) {
    case R_GENERIC_TLS_DTPMOD:
    case R_GENERIC_TLS_DTPREL:
    case R_GENERIC_TLS_TPREL:
    case R_GENERIC_TLSDESC:
      return true;
    default:
      return false;
  }
}

struct Relocator {
  Relocator(soinfo* si, const VersionTracker& version_tracker, const SymbolLookupList& lookup_list)
      : si(si),
        si_strtab(si->strtab_),
        si_symtab(si->symtab_),
        version_tracker(version_tracker),
        lookup_list(lookup_list),
        tls_tp_base(__libc_shared_globals()->static_tls_layout.offset_thread_pointer())
  {}

  soinfo* si;
  const char* si_strtab;
  ElfW(Sym)* si_symtab;

  const VersionTracker& version_tracker;
  const SymbolLookupList& lookup_list;

  // Cache key
  ElfW(Word) cache_sym_val = 0;
  // Cache value
  const ElfW(Sym)* cache_sym = nullptr;
  soinfo* cache_si = nullptr;

  std::vector<std::pair<TlsDescriptor*, size_t>> deferred_tlsdesc_relocs;
  size_t tls_tp_base;
};

template <bool DoRelAddend>
static inline void write_word(void* target, size_t value, const rel_t& reloc __unused) {
#if defined(USE_RELA)
  *static_cast<ElfW(Addr)*>(target) = value + reloc.r_addend;
#else
  if (DoRelAddend) {
    *static_cast<ElfW(Addr)*>(target) += value;
  } else {
    *static_cast<ElfW(Addr)*>(target) = value;
  }
#endif
}

__attribute__((always_inline))
static inline bool lookup_symbol(Relocator& relocator, uint32_t r_sym, soinfo** found_in, const ElfW(Sym)** sym) {
  if (r_sym == relocator.cache_sym_val) {
    *found_in = relocator.cache_si;
    *sym = relocator.cache_sym;
  } else {
    // TODO: Do we need the error checking from soinfo::get_string?
    const char* sym_name = relocator.si_strtab + relocator.si_symtab[r_sym].st_name;

    const version_info* vi = nullptr;
    if (!relocator.si->lookup_version_info(relocator.version_tracker, r_sym, sym_name, &vi)) {
      return false;
    }

    soinfo* local_found_in = nullptr;
    const ElfW(Sym)* local_sym = soinfo_do_lookup(sym_name, vi, &local_found_in, relocator.lookup_list);

    relocator.cache_sym_val = r_sym;
    relocator.cache_si = local_found_in;
    relocator.cache_sym = local_sym;
    *found_in = local_found_in;
    *sym = local_sym;
  }

  if (*sym == nullptr) {
    if (ELF_ST_BIND(relocator.si_symtab[r_sym].st_info) != STB_WEAK) {
      const char* sym_name = relocator.si_strtab + relocator.si_symtab[r_sym].st_name;
      DL_ERR("cannot locate symbol \"%s\" referenced by \"%s\"...", sym_name, relocator.si->get_realpath());
      return false;
    }
  }

  return true;
}

__attribute__((noinline))
static bool process_relocation_slow_path(Relocator& relocator, const rel_t& reloc) {
  void* const rel_target = reinterpret_cast<void*>(reloc.r_offset + relocator.si->load_bias);
  const uint32_t r_type = ELFW(R_TYPE)(reloc.r_info);
  const uint32_t r_sym = ELFW(R_SYM)(reloc.r_info);
  const size_t load_bias = relocator.si->load_bias;

  soinfo* found_in = nullptr;
  const ElfW(Sym)* sym = nullptr;
  const char* sym_name = nullptr;
  size_t sym_addr = 0;

  ElfW(Addr) addend = get_addend(&reloc, reinterpret_cast<ElfW(Addr)>(rel_target));

  // TODO: Add the relocation types that are also in the fath paths, because we need them if we find
  // something unexpected (e.g. R_GENERIC_RELATIVE with a non-zero r_sym, a JUMP_SLOT in an ordinary
  // rel section, a non-JUMP_SLOT in the plt section).

  if (r_sym == 0) {
    // By convention in ld.bfd and lld, an omitted symbol on a TLS relocation
    // is a reference to the current module.
    if (is_tls_reloc(r_type)) {
      found_in = relocator.si;
    }
  } else if (ELF_ST_BIND(relocator.si_symtab[r_sym].st_info) == STB_LOCAL && is_tls_reloc(r_type)) {
    // In certain situations, the Gold linker accesses a TLS symbol using a
    // relocation to an STB_LOCAL symbol in .dynsym of either STT_SECTION or
    // STT_TLS type. Bionic doesn't support these relocations, so issue an
    // error. References:
    //  - https://groups.google.com/d/topic/generic-abi/dJ4_Y78aQ2M/discussion
    //  - https://sourceware.org/bugzilla/show_bug.cgi?id=17699
    sym = &relocator.si_symtab[r_sym];
    sym_name = relocator.si->get_string(sym->st_name);
    DL_ERR("unexpected TLS reference to local symbol \"%s\": "
           "sym type %d, rel type %u (idx %d of \"%s\")",
           sym_name, ELF_ST_TYPE(sym->st_info), r_type, 0/*TODO:idx*/, relocator.si->get_realpath());
    return false;
  } else {
    sym_name = relocator.si->get_string(relocator.si_symtab[r_sym].st_name);
    if (!lookup_symbol(relocator, r_sym, &found_in, &sym)) {
      return false;
    }
    if (sym == nullptr) {
      // A weak reference to an undefined symbol. We typically use a zero symbol address, but these
      // relocations are special.
      switch (r_type) {
#if defined(__x86_64__)
        case R_X86_64_PC32:
          sym_addr = reloc;
          break;
#elif defined(__i386__)
        case R_386_PC32:
          sym_addr = reloc;
          break;
#endif
      }
    } else {
      if (is_tls_reloc(r_type)) {
        if (ELF_ST_TYPE(sym->st_info) != STT_TLS) {
          DL_ERR("reference to non-TLS symbol \"%s\" from TLS relocation in \"%s\"",
                 sym_name, relocator.si->get_realpath());
          return false;
        }
        if (found_in->get_tls() == nullptr) {
          DL_ERR("TLS relocation refers to symbol \"%s\" in solib \"%s\" with no TLS segment",
                 sym_name, found_in->get_realpath());
          return false;
        }
        sym_addr = sym->st_value;
      } else {
        // TODO: Protect/unprotect pages for STT_GNU_IFUNC when there are text relocations.
        sym_addr = found_in->resolve_symbol_address(sym);
        // TODO: Protect/unprotect pages for STT_GNU_IFUNC when there are text relocations.
      }
    }
  }

  switch (r_type) {
    case R_GENERIC_IRELATIVE:
      // In the linker, ifuncs are called as soon as possible so that string functions work. We must
      // not call them again. (e.g. On arm32, resolving an ifunc changes the meaning of the addend
      // from a resolver function to the implementation.)
      if (!relocator.si->is_linker()) {
        // TODO: Protect/unprotect pages when there are text relocations.
        ElfW(Addr) ifunc_addr = call_ifunc_resolver(load_bias + addend);
        // TODO: Protect/unprotect pages when there are text relocations.
        *static_cast<size_t*>(rel_target) = ifunc_addr;
      }
      break;
    case R_GENERIC_COPY:
      // Copy relocations allow read-only data or code in a non-PIE executable to access a
      // variable from a DSO. The executable reserves extra space in its .bss section, and the
      // linker copies the variable into the extra space. The executable then exports its copy
      // to interpose the copy in the DSO.
      //
      // Bionic only supports PIE executables, so copy relocations aren't supported. The ARM and
      // AArch64 ABI documents only allow them for ET_EXEC (non-PIE) objects. See IHI0056B and
      // IHI0044F.
      DL_ERR("%s COPY relocations are not supported", relocator.si->get_realpath());
      return false;
    case R_GENERIC_TLS_TPREL:
      {
        ElfW(Addr) tpoff = 0;
        if (found_in == nullptr) {
          // Unresolved weak relocation. Leave tpoff at 0 to resolve
          // &weak_tls_symbol to __get_tls().
        } else {
          CHECK(found_in->get_tls() != nullptr); // We rejected a missing TLS segment above.
          const TlsModule& mod = get_tls_module(found_in->get_tls()->module_id);
          if (mod.static_offset != SIZE_MAX) {
            tpoff += mod.static_offset - relocator.tls_tp_base;
          } else {
            DL_ERR("TLS symbol \"%s\" in dlopened \"%s\" referenced from \"%s\" using IE access model",
                   sym_name, found_in->get_realpath(), relocator.si->get_realpath());
            return false;
          }
        }
        tpoff += sym_addr + addend;
        *static_cast<ElfW(Addr)*>(rel_target) = tpoff;
      }
      break;
    case R_GENERIC_TLS_DTPMOD:
      {
        size_t module_id = 0;
        if (found_in == nullptr) {
          // Unresolved weak relocation. Evaluate the module ID to 0.
        } else {
          CHECK(found_in->get_tls() != nullptr); // We rejected a missing TLS segment above.
          module_id = found_in->get_tls()->module_id;
        }
        *static_cast<ElfW(Addr)*>(rel_target) = module_id;
      }
      break;
    case R_GENERIC_TLS_DTPREL:
      *static_cast<ElfW(Addr)*>(rel_target) = sym_addr + addend;
      break;

#if defined(__aarch64__)
    // Bionic currently only implements TLSDESC for arm64. This implementation should work with
    // other architectures, as long as the resolver functions are implemented.
    case R_GENERIC_TLSDESC:
      {
        TlsDescriptor* desc = static_cast<TlsDescriptor*>(rel_target);
        if (found_in == nullptr) {
          // Unresolved weak relocation.
          desc->func = tlsdesc_resolver_unresolved_weak;
          desc->arg = addend;
        } else {
          CHECK(found_in->get_tls() != nullptr); // We rejected a missing TLS segment above.
          size_t module_id = found_in->get_tls()->module_id;
          const TlsModule& mod = get_tls_module(module_id);
          if (mod.static_offset != SIZE_MAX) {
            desc->func = tlsdesc_resolver_static;
            desc->arg = mod.static_offset - relocator.tls_tp_base + sym_addr + addend;
          } else {
            relocator.si->tlsdesc_args_.push_back({
              .generation = mod.first_generation,
              .index.module_id = module_id,
              .index.offset = sym_addr + addend,
            });
            // Defer the TLSDESC relocation until the address of the TlsDynamicResolverArg object
            // is finalized.
            relocator.deferred_tlsdesc_relocs.push_back({ desc, relocator.si->tlsdesc_args_.size() - 1 });
          }
        }
      }
      break;
#endif  // defined(__aarch64__)

#if defined(__x86_64__)
    case R_X86_64_32:
      *static_cast<ElfW(Addr)*>(rel_target) = sym_addr + addend;
      break;
    case R_X86_64_PC32:
      *static_cast<ElfW(Addr)*>(rel_target) = sym_addr + addend - reinterpret_cast<ElfW(Addr)>(rel_target);
      break;
#elif defined(__i386__)
    case R_386_PC32:
      *static_cast<ElfW(Addr)*>(rel_target) += sym_addr - reinterpret_cast<ElfW(Addr)>(rel_target);
      break;
#endif
    default:
      // TODO: output the relocation index again? (The `rel` field was broken for packed relocs...)
      DL_ERR("unknown reloc type %d", r_type/*, rel, idx*/);
      return false;
  }
  return true;
}

enum class SectionType {
  // A jump table -- all relocations are probably JUMP_SLOT.
  JumpTable,
  // Ordinary rel section -- expect ABSOLUTE, GLOB_DAT, or RELATIVE.
  General,
};

template <SectionType SType, typename DoLoggingOrTextRels>
__attribute__((always_inline))
static inline bool process_relocation(Relocator& relocator, const rel_t& reloc) {
  constexpr bool do_logging_or_text_rel = DoLoggingOrTextRels::value;

  // TODO: Actually implement this...
  (void)do_logging_or_text_rel;

  void* const rel_target = reinterpret_cast<void*>(reloc.r_offset + relocator.si->load_bias);
  const uint32_t r_type = ELFW(R_TYPE)(reloc.r_info);
  const uint32_t r_sym = ELFW(R_SYM)(reloc.r_info);

  // TODO: Protect/unprotect pages for STT_GNU_IFUNC when there are text relocations,
  // when calling resolve_symbol_address. Maybe we need to fallback to a slow path somehow
  // when there are text relocations.

  // TODO: Make sure the specialized versions handle undefined weaks properly! (I think this
  // code handles them fine.)

  auto handle_reloc = [&](auto do_rel_addend) {
    constexpr bool do_rel_addend_val = decltype(do_rel_addend)::value;
    soinfo* found_in = nullptr;
    const ElfW(Sym)* sym = nullptr;
    if (r_sym != 0 && !lookup_symbol(relocator, r_sym, &found_in, &sym)) return false;
    size_t sym_addr = 0;
    if (sym != nullptr) sym_addr = found_in->resolve_symbol_address(sym);
    write_word<do_rel_addend_val>(rel_target, sym_addr, reloc);
    return true;
  };

  if constexpr (SType == SectionType::JumpTable) {
    if (__predict_true(r_type == R_GENERIC_JUMP_SLOT)) {
      return handle_reloc(std::false_type());
    }
  }

  if constexpr (SType == SectionType::General) {
    // Almost all dynamic relocations are of one of these types, and most will be
    // R_GENERIC_ABSOLUTE. The platform typically uses RELR instead, but R_GENERIC_RELATIVE is
    // common in non-platform binaries.
    if (r_type == R_GENERIC_ABSOLUTE) {
      return handle_reloc(std::true_type());
    } else if (r_type == R_GENERIC_GLOB_DAT) {
      return handle_reloc(std::false_type());
    } else if (r_type == R_GENERIC_RELATIVE && r_sym == 0) {
      write_word<true>(rel_target, relocator.si->load_bias, reloc);
      return true;
    }
  }

  return process_relocation_slow_path(relocator, reloc);
}

template <SectionType SType, typename DoLoggingOrTextRels>
__attribute__((noinline))
static bool plain_relocate_impl(Relocator& relocator, rel_t* rels, size_t rel_count) {
  for (size_t i = 0; i < rel_count; ++i) {
    if (!process_relocation<SType, DoLoggingOrTextRels>(relocator, rels[i])) {
      return false;
    }
  }
  return true;
}

template <SectionType SType, typename DoLoggingOrTextRels>
__attribute__((noinline))
static bool packed_relocate_impl(Relocator& relocator, sleb128_decoder decoder) {
  return for_all_packed_relocs(decoder, [&](const rel_t& reloc) {
    return process_relocation<SType, DoLoggingOrTextRels>(relocator, reloc);
  });
}

static bool needs_slow_relocate_loop(const Relocator& relocator __unused) {
  // TODO: Also enable this pathway when logging...
#if !defined(__LP64__)
  if (relocator.si->has_text_relocations) return true;
#endif
  return false;
}

template <SectionType SType, typename ...Args>
static bool plain_relocate(Relocator& relocator, Args ...args) {
  if (needs_slow_relocate_loop(relocator)) {
    return plain_relocate_impl<SType, std::true_type>(relocator, args...);
  } else {
    return plain_relocate_impl<SType, std::false_type>(relocator, args...);
  }
}

template <SectionType SType, typename ...Args>
static bool packed_relocate(Relocator& relocator,Args ...args) {
  if (needs_slow_relocate_loop(relocator)) {
    return packed_relocate_impl<SType, std::true_type>(relocator, args...);
  } else {
    return packed_relocate_impl<SType, std::false_type>(relocator, args...);
  }
}

bool relocate_soinfo(soinfo* si, const SymbolLookupList& lookup_list) {

  VersionTracker version_tracker;

  if (!version_tracker.init(si)) {
    return false;
  }

  Relocator relocator(si, version_tracker, lookup_list);

  if (si->android_relocs_ != nullptr) {
    // check signature
    if (si->android_relocs_size_ > 3 &&
        si->android_relocs_[0] == 'A' &&
        si->android_relocs_[1] == 'P' &&
        si->android_relocs_[2] == 'S' &&
        si->android_relocs_[3] == '2') {
      DEBUG("[ android relocating %s ]", si->get_realpath());

      const uint8_t* packed_relocs = si->android_relocs_ + 4;
      const size_t packed_relocs_size = si->android_relocs_size_ - 4;

      if (!packed_relocate<SectionType::General>(relocator, sleb128_decoder(packed_relocs, packed_relocs_size))) {
        return false;
      }
    } else {
      DL_ERR("bad android relocation header.");
      return false;
    }
  }

  if (si->relr_ != nullptr) {
    DEBUG("[ relocating %s relr ]", si->get_realpath());
    if (!si->relocate_relr()) {
      return false;
    }
  }

#if defined(USE_RELA)
  if (si->rela_ != nullptr) {
    DEBUG("[ relocating %s rela ]", si->get_realpath());

    if (!plain_relocate<SectionType::General>(relocator, si->rela_, si->rela_count_)) {
      return false;
    }
  }
  if (si->plt_rela_ != nullptr) {
    DEBUG("[ relocating %s plt rela ]", si->get_realpath());
    if (!plain_relocate<SectionType::JumpTable>(relocator, si->plt_rela_, si->plt_rela_count_)) {
      return false;
    }
  }
#else
  if (si->rel_ != nullptr) {
    DEBUG("[ relocating %s rel ]", si->get_realpath());
    if (!plain_relocate<SectionType::General>(relocator, si->rel_, si->rel_count_)) {
      return false;
    }
  }
  if (si->plt_rel_ != nullptr) {
    DEBUG("[ relocating %s plt rel ]", si->get_realpath());
    if (!plain_relocate<SectionType::JumpTable>(relocator, si->plt_rel_, si->plt_rel_count_)) {
      return false;
    }
  }
#endif

#if defined(__mips__)
  if (!mips_relocate_got(version_tracker, global_group, local_group)) {
    return false;
  }
#endif

  // Once the tlsdesc_args_ vector's size is finalized, we can write the addresses of its elements
  // into the TLSDESC relocations.
#if defined(__aarch64__)
  // Bionic currently only implements TLSDESC for arm64.
  for (const std::pair<TlsDescriptor*, size_t>& pair : relocator.deferred_tlsdesc_relocs) {
    TlsDescriptor* desc = pair.first;
    desc->func = tlsdesc_resolver_dynamic;
    desc->arg = reinterpret_cast<size_t>(&relocator.si->tlsdesc_args_[pair.second]);
  }
#endif

  return true;
}

__attribute__((always_inline))
std::pair<uint32_t, uint32_t> calculate_gnu_hash(const char* name) {
#if USE_GNU_HASH_NEON
  uint64_t hash_len = calculate_gnu_hash_neon(name);
  return { static_cast<uint32_t>(hash_len), hash_len >> 32 };
#else
  // TODO: validate that size fits in 32-bits?
  uint32_t h = 5381;
  const uint8_t* name_bytes = reinterpret_cast<const uint8_t*>(name);
  #pragma unroll 8
  while (*name_bytes != 0) {
    h += (h << 5) + *name_bytes++; // h*33 + c = h + h * 32 + c = h + h << 5 + c
  }
  return { h, reinterpret_cast<const char*>(name_bytes) - name };
#endif
}
