/*
 * Copyright (C) 2016 The Android Open Source Project
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

#include "linker_soinfo.h"

#include <dlfcn.h>
#include <elf.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <async_safe/log.h>

#include "linker_debug.h"
#include "linker_globals.h"
#include "linker_logger.h"
#include "linker_utils.h"

// TODO(dimitry): These functions are currently located in linker.cpp - find a better place for it
bool find_verdef_version_index(const soinfo* si, const version_info* vi, ElfW(Versym)* versym);
ElfW(Addr) call_ifunc_resolver(ElfW(Addr) resolver_addr);
int get_application_target_sdk_version();

#if defined(__arm__)
#define USE_GNU_HASH_ASM 1
#else
#define USE_GNU_HASH_ASM 0
#endif

static std::pair<uint32_t, size_t> calculate_gnu_hash(const char* name);

#if USE_GNU_HASH_ASM
extern "C" uint64_t calculate_gnu_hash_asm(const char* name);
#endif

static const ElfW(Versym) kVersymHiddenBit = 0x8000;

LookupLib::LookupLib(soinfo* si) {
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

LookupList::LookupList(soinfo* si) : sole_lib_(si), begin_(&sole_lib_), end_(&sole_lib_ + 1) {
  CHECK(si != nullptr);
  fallback_lib_count_ += sole_lib_.needs_fallback();
}

LookupList::LookupList(const soinfo_list_t& global_group, const soinfo_list_t& local_group) {
  libs_.reserve(1 + global_group.size() + local_group.size());

  // Reserve a space in front for DT_SYMBOLIC lookup.
  libs_.push_back(LookupLib {});

  global_group.for_each([this](soinfo* si) {
    libs_.push_back(LookupLib(si));
    fallback_lib_count_ += libs_.back().needs_fallback();
  });

  local_group.for_each([this](soinfo* si) {
    libs_.push_back(LookupLib(si));
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
void LookupList::set_dt_symbolic_lib(soinfo* lib) {
  CHECK(!libs_.empty());
  fallback_lib_count_ -= libs_[0].needs_fallback();
  libs_[0] = lib ? LookupLib(lib) : LookupLib();
  begin_ = lib ? &libs_[0] : &libs_[1];
  fallback_lib_count_ += libs_[0].needs_fallback();
}

static inline bool check_symbol_version_2(const ElfW(Versym)* ver_table, uint32_t sym_idx,
                                          const ElfW(Versym) verneed) {
  if (ver_table == nullptr) return true;
  const uint32_t verdef = ver_table[sym_idx];
  // XXX: If verneed is kVersymNotNeeded, are we supposed to match with verdef of 0?
  // See https://www.akkadia.org/drepper/symbol-versioning...
  // XXX: This seems odd to me. Verify it...
  //  - we want sym@FOO, no versions in DSO ==> sym matches
  //  - we want sym@FOO, no FOO in DSO ==> sym@@BAR matches
  //  - we want sym@FOO, baz@FOO in DSO ==> sym@@BAR does not match?
  if (verneed == kVersymNotNeeded) {
    return !(verdef & kVersymHiddenBit);
  } else {
    return verneed == (verdef & ~kVersymHiddenBit);
  }
}

static inline bool is_symbol_global_and_defined(const ElfW(Sym)* s) {
  if (ELF_ST_BIND(s->st_info) == STB_GLOBAL ||
      ELF_ST_BIND(s->st_info) == STB_WEAK) {
    return s->st_shndx != SHN_UNDEF;
  }
  // else if (ELF_ST_BIND(s->st_info) != STB_LOCAL) {
  //   DL_WARN("Warning: unexpected ST_BIND value: %d for \"%s\" in \"%s\" (ignoring)",
  //           ELF_ST_BIND(s->st_info), si->get_string(s->st_name), si->get_realpath());
  // }
  return false;
}

__attribute__((always_inline))
static inline const ElfW(Sym)* finish_lookup(const LookupLib& lib, const char* name, size_t name_len,
                                             uint32_t hash31, const version_info* vi, uint32_t sym_idx) {
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
            // XXX: This strtab bounds check isn't quite right (e.g. overflow...)
            sym->st_name + name_len + 1 <= lib.strtab_size_ &&
            memcmp(strtab + sym->st_name, name, name_len + 1) == 0 &&
            is_symbol_global_and_defined(sym)) {
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
        if (!find_verdef_version_index(lib.si_, vi, &verneed)) {
          async_safe_fatal("can we detect bad verdef entries earlier instead?");
        }
        calculated_verneed = true;
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
                     soinfo** si_found_in, const LookupList& lookup_list) {
  const std::pair<uint32_t, size_t> hash_len = calculate_gnu_hash(name);
  const uint32_t hash = hash_len.first;
  const size_t name_len = hash_len.second;
  const uint32_t bloom_mask_bits = sizeof(ElfW(Addr)) * 8;
  SymbolName fallback_symbol_name(name);

  for (const LookupLib& lib : lookup_list) {
    if (EnableFallback && lib.needs_fallback()) {
      // Fallback -- search this DSO the slow way.
      const ElfW(Sym)* result = nullptr;
      if (!lib.si_->find_symbol_by_name(fallback_symbol_name, vi, &result)) {
        async_safe_fatal("can we detect bad verdef entries earlier instead?");
      }
      if (result != nullptr) {
        *si_found_in = lib.si_;
        return result;
      }
      continue;
    }

    const uint32_t word_num = (hash / bloom_mask_bits) & lib.gnu_maskwords_;
    const ElfW(Addr) bloom_word = lib.gnu_bloom_filter_[word_num];
    const uint32_t h1 = hash % bloom_mask_bits;
    const uint32_t h2 = (hash >> lib.gnu_shift2_) % bloom_mask_bits;

    if ((1 & (bloom_word >> h1) & (bloom_word >> h2)) == 1) {
      const uint32_t first_index = lib.gnu_bucket_[hash % lib.gnu_nbucket_];
      if (first_index != 0) {
        if (const ElfW(Sym)* sym = finish_lookup(lib, name, name_len, hash >> 1, vi, first_index)) {
          *si_found_in = lib.si_;
          return sym;
        }
      }
    }
  }
  return nullptr;
}

const ElfW(Sym)* soinfo_do_lookup(const char* name, const version_info* vi,
                                  soinfo** si_found_in, const LookupList& lookup_list) {
  if (lookup_list.needs_fallback()) {
    return soinfo_do_lookup_impl<true>(name, vi, si_found_in, lookup_list);
  } else {
    return soinfo_do_lookup_impl<false>(name, vi, si_found_in, lookup_list);
  }
}

soinfo::soinfo(android_namespace_t* ns, const char* realpath,
               const struct stat* file_stat, off64_t file_offset,
               int rtld_flags) {
  memset(this, 0, sizeof(*this));

  if (realpath != nullptr) {
    realpath_ = realpath;
  }

  flags_ = FLAG_NEW_SOINFO;
  version_ = SOINFO_VERSION;

  if (file_stat != nullptr) {
    this->st_dev_ = file_stat->st_dev;
    this->st_ino_ = file_stat->st_ino;
    this->file_offset_ = file_offset;
  }

  this->rtld_flags_ = rtld_flags;
  this->primary_namespace_ = ns;
}

soinfo::~soinfo() {
  g_soinfo_handles_map.erase(handle_);
}

void soinfo::set_dt_runpath(const char* path) {
  if (!has_min_version(3)) {
    return;
  }

  std::vector<std::string> runpaths;

  split_path(path, ":", &runpaths);

  std::string origin = dirname(get_realpath());
  // FIXME: add $PLATFORM.
  std::vector<std::pair<std::string, std::string>> params = {
    {"ORIGIN", origin},
#if defined(LIB_PATH)
    {"LIB", LIB_PATH},
#else
#error "LIB_PATH not defined"
#endif
  };
  for (auto&& s : runpaths) {
    format_string(&s, params);
  }

  resolve_paths(runpaths, &dt_runpath_);
}

const ElfW(Versym)* soinfo::get_versym(size_t n) const {
  auto table = get_versym_table();
  return table ? table + n : nullptr;
}

ElfW(Addr) soinfo::get_verneed_ptr() const {
  if (has_min_version(2)) {
    return verneed_ptr_;
  }

  return 0;
}

size_t soinfo::get_verneed_cnt() const {
  if (has_min_version(2)) {
    return verneed_cnt_;
  }

  return 0;
}

ElfW(Addr) soinfo::get_verdef_ptr() const {
  if (has_min_version(2)) {
    return verdef_ptr_;
  }

  return 0;
}

size_t soinfo::get_verdef_cnt() const {
  if (has_min_version(2)) {
    return verdef_cnt_;
  }

  return 0;
}

bool soinfo::find_symbol_by_name(SymbolName& symbol_name,
                                 const version_info* vi,
                                 const ElfW(Sym)** symbol) const {
  uint32_t symbol_index;
  bool success =
      is_gnu_hash() ?
      gnu_lookup(symbol_name, vi, &symbol_index) :
      elf_lookup(symbol_name, vi, &symbol_index);

  if (success) {
    *symbol = symbol_index == 0 ? nullptr : symtab_ + symbol_index;
  }

  return success;
}

static inline bool is_versym_hidden(const ElfW(Versym)* versym) {
  // the symbol is hidden if bit 15 of versym is set.
  return versym != nullptr && (*versym & kVersymHiddenBit) != 0;
}

static inline bool check_symbol_version(const ElfW(Versym) verneed,
                                        const ElfW(Versym)* verdef) {
  return verneed == kVersymNotNeeded ||
      verdef == nullptr ||
      verneed == (*verdef & ~kVersymHiddenBit);
}

bool soinfo::gnu_lookup(SymbolName& symbol_name,
                        const version_info* vi,
                        uint32_t* symbol_index) const {
  uint32_t hash = symbol_name.gnu_hash();
  uint32_t h2 = hash >> gnu_shift2_;

  uint32_t bloom_mask_bits = sizeof(ElfW(Addr))*8;
  uint32_t word_num = (hash / bloom_mask_bits) & gnu_maskwords_;
  ElfW(Addr) bloom_word = gnu_bloom_filter_[word_num];

  *symbol_index = 0;

  TRACE_TYPE(LOOKUP, "SEARCH %s in %s@%p (gnu)",
      symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

  // test against bloom filter
  if ((1 & (bloom_word >> (hash % bloom_mask_bits)) & (bloom_word >> (h2 % bloom_mask_bits))) == 0) {
    TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
        symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

    return true;
  }

  // bloom test says "probably yes"...
  uint32_t n = gnu_bucket_[hash % gnu_nbucket_];

  if (n == 0) {
    TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
        symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

    return true;
  }

  // lookup versym for the version definition in this library
  // note the difference between "version is not requested" (vi == nullptr)
  // and "version not found". In the first case verneed is kVersymNotNeeded
  // which implies that the default version can be accepted; the second case results in
  // verneed = 1 (kVersymGlobal) and implies that we should ignore versioned symbols
  // for this library and consider only *global* ones.
  ElfW(Versym) verneed = 0;
  if (!find_verdef_version_index(this, vi, &verneed)) {
    return false;
  }

  do {
    ElfW(Sym)* s = symtab_ + n;
    const ElfW(Versym)* verdef = get_versym(n);
    // skip hidden versions when verneed == kVersymNotNeeded (0)
    if (verneed == kVersymNotNeeded && is_versym_hidden(verdef)) {
        continue;
    }
    if (((gnu_chain_[n] ^ hash) >> 1) == 0 &&
        check_symbol_version(verneed, verdef) &&
        strcmp(get_string(s->st_name), symbol_name.get_name()) == 0 &&
        is_symbol_global_and_defined(s)) {
      TRACE_TYPE(LOOKUP, "FOUND %s in %s (%p) %zd",
          symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(s->st_value),
          static_cast<size_t>(s->st_size));
      *symbol_index = n;
      return true;
    }
  } while ((gnu_chain_[n++] & 1) == 0);

  TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p",
             symbol_name.get_name(), get_realpath(), reinterpret_cast<void*>(base));

  return true;
}

bool soinfo::elf_lookup(SymbolName& symbol_name,
                        const version_info* vi,
                        uint32_t* symbol_index) const {
  uint32_t hash = symbol_name.elf_hash();

  TRACE_TYPE(LOOKUP, "SEARCH %s in %s@%p h=%x(elf) %zd",
             symbol_name.get_name(), get_realpath(),
             reinterpret_cast<void*>(base), hash, hash % nbucket_);

  ElfW(Versym) verneed = 0;
  if (!find_verdef_version_index(this, vi, &verneed)) {
    return false;
  }

  for (uint32_t n = bucket_[hash % nbucket_]; n != 0; n = chain_[n]) {
    ElfW(Sym)* s = symtab_ + n;
    const ElfW(Versym)* verdef = get_versym(n);

    // skip hidden versions when verneed == 0
    if (verneed == kVersymNotNeeded && is_versym_hidden(verdef)) {
        continue;
    }

    if (check_symbol_version(verneed, verdef) &&
        strcmp(get_string(s->st_name), symbol_name.get_name()) == 0 &&
        is_symbol_global_and_defined(s)) {
      TRACE_TYPE(LOOKUP, "FOUND %s in %s (%p) %zd",
                 symbol_name.get_name(), get_realpath(),
                 reinterpret_cast<void*>(s->st_value),
                 static_cast<size_t>(s->st_size));
      *symbol_index = n;
      return true;
    }
  }

  TRACE_TYPE(LOOKUP, "NOT FOUND %s in %s@%p %x %zd",
             symbol_name.get_name(), get_realpath(),
             reinterpret_cast<void*>(base), hash, hash % nbucket_);

  *symbol_index = 0;
  return true;
}

ElfW(Sym)* soinfo::find_symbol_by_address(const void* addr) {
  return is_gnu_hash() ? gnu_addr_lookup(addr) : elf_addr_lookup(addr);
}

static bool symbol_matches_soaddr(const ElfW(Sym)* sym, ElfW(Addr) soaddr) {
  // Skip TLS symbols. A TLS symbol's value is relative to the start of the TLS segment rather than
  // to the start of the solib. The solib only reserves space for the initialized part of the TLS
  // segment. (i.e. .tdata is followed by .tbss, and .tbss overlaps other sections.)
  return sym->st_shndx != SHN_UNDEF &&
      ELF_ST_TYPE(sym->st_info) != STT_TLS &&
      soaddr >= sym->st_value &&
      soaddr < sym->st_value + sym->st_size;
}

ElfW(Sym)* soinfo::gnu_addr_lookup(const void* addr) {
  ElfW(Addr) soaddr = reinterpret_cast<ElfW(Addr)>(addr) - load_bias;

  for (size_t i = 0; i < gnu_nbucket_; ++i) {
    uint32_t n = gnu_bucket_[i];

    if (n == 0) {
      continue;
    }

    do {
      ElfW(Sym)* sym = symtab_ + n;
      if (symbol_matches_soaddr(sym, soaddr)) {
        return sym;
      }
    } while ((gnu_chain_[n++] & 1) == 0);
  }

  return nullptr;
}

ElfW(Sym)* soinfo::elf_addr_lookup(const void* addr) {
  ElfW(Addr) soaddr = reinterpret_cast<ElfW(Addr)>(addr) - load_bias;

  // Search the library's symbol table for any defined symbol which
  // contains this address.
  for (size_t i = 0; i < nchain_; ++i) {
    ElfW(Sym)* sym = symtab_ + i;
    if (symbol_matches_soaddr(sym, soaddr)) {
      return sym;
    }
  }

  return nullptr;
}

static void call_function(const char* function_name __unused,
                          linker_ctor_function_t function,
                          const char* realpath __unused) {
  if (function == nullptr || reinterpret_cast<uintptr_t>(function) == static_cast<uintptr_t>(-1)) {
    return;
  }

  TRACE("[ Calling c-tor %s @ %p for '%s' ]", function_name, function, realpath);
  function(g_argc, g_argv, g_envp);
  TRACE("[ Done calling c-tor %s @ %p for '%s' ]", function_name, function, realpath);
}

static void call_function(const char* function_name __unused,
                          linker_dtor_function_t function,
                          const char* realpath __unused) {
  if (function == nullptr || reinterpret_cast<uintptr_t>(function) == static_cast<uintptr_t>(-1)) {
    return;
  }

  TRACE("[ Calling d-tor %s @ %p for '%s' ]", function_name, function, realpath);
  function();
  TRACE("[ Done calling d-tor %s @ %p for '%s' ]", function_name, function, realpath);
}

template <typename F>
static void call_array(const char* array_name __unused,
                       F* functions,
                       size_t count,
                       bool reverse,
                       const char* realpath) {
  if (functions == nullptr) {
    return;
  }

  TRACE("[ Calling %s (size %zd) @ %p for '%s' ]", array_name, count, functions, realpath);

  int begin = reverse ? (count - 1) : 0;
  int end = reverse ? -1 : count;
  int step = reverse ? -1 : 1;

  for (int i = begin; i != end; i += step) {
    TRACE("[ %s[%d] == %p ]", array_name, i, functions[i]);
    call_function("function", functions[i], realpath);
  }

  TRACE("[ Done calling %s for '%s' ]", array_name, realpath);
}

void soinfo::call_pre_init_constructors() {
  if (g_is_ldd) return;

  // DT_PREINIT_ARRAY functions are called before any other constructors for executables,
  // but ignored in a shared library.
  call_array("DT_PREINIT_ARRAY", preinit_array_, preinit_array_count_, false, get_realpath());
}

void soinfo::call_constructors() {
  if (constructors_called || g_is_ldd) {
    return;
  }

  // We set constructors_called before actually calling the constructors, otherwise it doesn't
  // protect against recursive constructor calls. One simple example of constructor recursion
  // is the libc debug malloc, which is implemented in libc_malloc_debug_leak.so:
  // 1. The program depends on libc, so libc's constructor is called here.
  // 2. The libc constructor calls dlopen() to load libc_malloc_debug_leak.so.
  // 3. dlopen() calls the constructors on the newly created
  //    soinfo for libc_malloc_debug_leak.so.
  // 4. The debug .so depends on libc, so CallConstructors is
  //    called again with the libc soinfo. If it doesn't trigger the early-
  //    out above, the libc constructor will be called again (recursively!).
  constructors_called = true;

  if (!is_main_executable() && preinit_array_ != nullptr) {
    // The GNU dynamic linker silently ignores these, but we warn the developer.
    PRINT("\"%s\": ignoring DT_PREINIT_ARRAY in shared library!", get_realpath());
  }

  get_children().for_each([] (soinfo* si) {
    si->call_constructors();
  });

  if (!is_linker()) {
    bionic_trace_begin((std::string("calling constructors: ") + get_realpath()).c_str());
  }

  // DT_INIT should be called before DT_INIT_ARRAY if both are present.
  call_function("DT_INIT", init_func_, get_realpath());
  call_array("DT_INIT_ARRAY", init_array_, init_array_count_, false, get_realpath());

  if (!is_linker()) {
    bionic_trace_end();
  }
}

void soinfo::call_destructors() {
  if (!constructors_called) {
    return;
  }

  ScopedTrace trace((std::string("calling destructors: ") + get_realpath()).c_str());

  // DT_FINI_ARRAY must be parsed in reverse order.
  call_array("DT_FINI_ARRAY", fini_array_, fini_array_count_, true, get_realpath());

  // DT_FINI should be called after DT_FINI_ARRAY if both are present.
  call_function("DT_FINI", fini_func_, get_realpath());
}

void soinfo::add_child(soinfo* child) {
  if (has_min_version(0)) {
    child->parents_.push_back(this);
    this->children_.push_back(child);
  }
}

void soinfo::remove_all_links() {
  if (!has_min_version(0)) {
    return;
  }

  // 1. Untie connected soinfos from 'this'.
  children_.for_each([&] (soinfo* child) {
    child->parents_.remove_if([&] (const soinfo* parent) {
      return parent == this;
    });
  });

  parents_.for_each([&] (soinfo* parent) {
    parent->children_.remove_if([&] (const soinfo* child) {
      return child == this;
    });
  });

  // 2. Remove from the primary namespace
  primary_namespace_->remove_soinfo(this);
  primary_namespace_ = nullptr;

  // 3. Remove from secondary namespaces
  secondary_namespaces_.for_each([&](android_namespace_t* ns) {
    ns->remove_soinfo(this);
  });


  // 4. Once everything untied - clear local lists.
  parents_.clear();
  children_.clear();
  secondary_namespaces_.clear();
}

dev_t soinfo::get_st_dev() const {
  if (has_min_version(0)) {
    return st_dev_;
  }

  return 0;
};

ino_t soinfo::get_st_ino() const {
  if (has_min_version(0)) {
    return st_ino_;
  }

  return 0;
}

off64_t soinfo::get_file_offset() const {
  if (has_min_version(1)) {
    return file_offset_;
  }

  return 0;
}

uint32_t soinfo::get_rtld_flags() const {
  if (has_min_version(1)) {
    return rtld_flags_;
  }

  return 0;
}

uint32_t soinfo::get_dt_flags_1() const {
  if (has_min_version(1)) {
    return dt_flags_1_;
  }

  return 0;
}

void soinfo::set_dt_flags_1(uint32_t dt_flags_1) {
  if (has_min_version(1)) {
    if ((dt_flags_1 & DF_1_GLOBAL) != 0) {
      rtld_flags_ |= RTLD_GLOBAL;
    }

    if ((dt_flags_1 & DF_1_NODELETE) != 0) {
      rtld_flags_ |= RTLD_NODELETE;
    }

    dt_flags_1_ = dt_flags_1;
  }
}

void soinfo::set_nodelete() {
  rtld_flags_ |= RTLD_NODELETE;
}

void soinfo::set_realpath(const char* path) {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    realpath_ = path;
  }
#else
  realpath_ = path;
#endif
}

const char* soinfo::get_realpath() const {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    return realpath_.c_str();
  } else {
    return old_name_;
  }
#else
  return realpath_.c_str();
#endif
}

void soinfo::set_soname(const char* soname) {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    soname_ = soname;
  }
  strlcpy(old_name_, soname_, sizeof(old_name_));
#else
  soname_ = soname;
#endif
}

const char* soinfo::get_soname() const {
#if defined(__work_around_b_24465209__)
  if (has_min_version(2)) {
    return soname_;
  } else {
    return old_name_;
  }
#else
  return soname_;
#endif
}

// This is a return on get_children()/get_parents() if
// 'this->flags' does not have FLAG_NEW_SOINFO set.
static soinfo_list_t g_empty_list;

soinfo_list_t& soinfo::get_children() {
  if (has_min_version(0)) {
    return children_;
  }

  return g_empty_list;
}

const soinfo_list_t& soinfo::get_children() const {
  if (has_min_version(0)) {
    return children_;
  }

  return g_empty_list;
}

soinfo_list_t& soinfo::get_parents() {
  if (has_min_version(0)) {
    return parents_;
  }

  return g_empty_list;
}

static std::vector<std::string> g_empty_runpath;

const std::vector<std::string>& soinfo::get_dt_runpath() const {
  if (has_min_version(3)) {
    return dt_runpath_;
  }

  return g_empty_runpath;
}

android_namespace_t* soinfo::get_primary_namespace() {
  if (has_min_version(3)) {
    return primary_namespace_;
  }

  return &g_default_namespace;
}

void soinfo::add_secondary_namespace(android_namespace_t* secondary_ns) {
  CHECK(has_min_version(3));
  secondary_namespaces_.push_back(secondary_ns);
}

android_namespace_list_t& soinfo::get_secondary_namespaces() {
  CHECK(has_min_version(3));
  return secondary_namespaces_;
}

soinfo_tls* soinfo::get_tls() const {
  return has_min_version(5) ? tls_.get() : nullptr;
}

ElfW(Addr) soinfo::resolve_symbol_address(const ElfW(Sym)* s) const {
  if (ELF_ST_TYPE(s->st_info) == STT_GNU_IFUNC) {
    return call_ifunc_resolver(s->st_value + load_bias);
  }

  return static_cast<ElfW(Addr)>(s->st_value + load_bias);
}

const char* soinfo::get_string(ElfW(Word) index) const {
  if (has_min_version(1) && (index >= strtab_size_)) {
    async_safe_fatal("%s: strtab out of bounds error; STRSZ=%zd, name=%d",
        get_realpath(), strtab_size_, index);
  }

  return strtab_ + index;
}

bool soinfo::is_gnu_hash() const {
  return (flags_ & FLAG_GNU_HASH) != 0;
}

bool soinfo::can_unload() const {
  return !is_linked() ||
         (
             (get_rtld_flags() & (RTLD_NODELETE | RTLD_GLOBAL)) == 0
         );
}

bool soinfo::is_linked() const {
  return (flags_ & FLAG_LINKED) != 0;
}

bool soinfo::is_image_linked() const {
  return (flags_ & FLAG_IMAGE_LINKED) != 0;
}

bool soinfo::is_main_executable() const {
  return (flags_ & FLAG_EXE) != 0;
}

bool soinfo::is_linker() const {
  return (flags_ & FLAG_LINKER) != 0;
}

void soinfo::set_linked() {
  flags_ |= FLAG_LINKED;
}

void soinfo::set_image_linked() {
  flags_ |= FLAG_IMAGE_LINKED;
}

void soinfo::set_linker_flag() {
  flags_ |= FLAG_LINKER;
}

void soinfo::set_main_executable() {
  flags_ |= FLAG_EXE;
}

size_t soinfo::increment_ref_count() {
  return ++local_group_root_->ref_count_;
}

size_t soinfo::decrement_ref_count() {
  return --local_group_root_->ref_count_;
}

size_t soinfo::get_ref_count() const {
  return local_group_root_->ref_count_;
}

soinfo* soinfo::get_local_group_root() const {
  return local_group_root_;
}

void soinfo::set_mapped_by_caller(bool mapped_by_caller) {
  if (mapped_by_caller) {
    flags_ |= FLAG_MAPPED_BY_CALLER;
  } else {
    flags_ &= ~FLAG_MAPPED_BY_CALLER;
  }
}

bool soinfo::is_mapped_by_caller() const {
  return (flags_ & FLAG_MAPPED_BY_CALLER) != 0;
}

// This function returns api-level at the time of
// dlopen/load. Note that libraries opened by system
// will always have 'current' api level.
int soinfo::get_target_sdk_version() const {
  if (!has_min_version(2)) {
    return __ANDROID_API__;
  }

  return local_group_root_->target_sdk_version_;
}

uintptr_t soinfo::get_handle() const {
  CHECK(has_min_version(3));
  CHECK(handle_ != 0);
  return handle_;
}

void* soinfo::to_handle() {
  if (get_application_target_sdk_version() < __ANDROID_API_N__ || !has_min_version(3)) {
    return this;
  }

  return reinterpret_cast<void*>(get_handle());
}

void soinfo::generate_handle() {
  CHECK(has_min_version(3));
  CHECK(handle_ == 0); // Make sure this is the first call

  // Make sure the handle is unique and does not collide
  // with special values which are RTLD_DEFAULT and RTLD_NEXT.
  do {
    if (!is_first_stage_init()) {
      arc4random_buf(&handle_, sizeof(handle_));
    } else {
      // arc4random* is not available in init because /dev/urandom hasn't yet been
      // created. So, when running with init, use the monotonically increasing
      // numbers as handles
      handle_ += 2;
    }
    // the least significant bit for the handle is always 1
    // making it easy to test the type of handle passed to
    // dl* functions.
    handle_ = handle_ | 1;
  } while (handle_ == reinterpret_cast<uintptr_t>(RTLD_DEFAULT) ||
           handle_ == reinterpret_cast<uintptr_t>(RTLD_NEXT) ||
           g_soinfo_handles_map.find(handle_) != g_soinfo_handles_map.end());

  g_soinfo_handles_map[handle_] = this;
}

// TODO(dimitry): Move SymbolName methods to a separate file.

uint32_t calculate_elf_hash(const char* name) {
  const uint8_t* name_bytes = reinterpret_cast<const uint8_t*>(name);
  uint32_t h = 0, g;

  while (*name_bytes) {
    h = (h << 4) + *name_bytes++;
    g = h & 0xf0000000;
    h ^= g;
    h ^= g >> 24;
  }

  return h;
}

static std::pair<uint32_t, size_t> calculate_gnu_hash(const char* name) {
#if !USE_GNU_HASH_ASM
  uint32_t h = 5381;
  const uint8_t* name_bytes = reinterpret_cast<const uint8_t*>(name);
  while (*name_bytes != 0) {
    h += (h << 5) + *name_bytes++; // h*33 + c = h + h * 32 + c = h + h << 5 + c
  }
  return { h, reinterpret_cast<const char*>(name_bytes) - name };
#else
  uint64_t hash_len = calculate_gnu_hash_asm(name);
  return { static_cast<uint32_t>(hash_len), hash_len >> 32 };
#endif
}

uint32_t SymbolName::elf_hash() {
  if (!has_elf_hash_) {
    elf_hash_ = calculate_elf_hash(name_);
    has_elf_hash_ = true;
  }

  return elf_hash_;
}

uint32_t SymbolName::gnu_hash() {
  if (!has_gnu_hash_) {
    gnu_hash_ = calculate_gnu_hash(name_).first;
    has_gnu_hash_ = true;
  }

  return gnu_hash_;
}
