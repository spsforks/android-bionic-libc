/*
 * Copyright (C) 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "linker_cfi.h"

#include "linker_debug.h"
#include "private/bionic_page.h"
#include "private/bionic_prctl.h"

#include <sys/mman.h>
#include <sys/types.h>

// Update shadow without making it writable by preparing the data on the side and mremap-ing it in
// place.
class ShadowWrite {
  char *shadow_start, *shadow_end;
  char *aligned_start, *aligned_end;
  char* tmp_start;

 public:
  ShadowWrite(uint16_t* s, uint16_t* e) {
    shadow_start = reinterpret_cast<char*>(s);
    shadow_end = reinterpret_cast<char*>(e);
    aligned_start = reinterpret_cast<char*>(PAGE_START(reinterpret_cast<uintptr_t>(shadow_start)));
    aligned_end = reinterpret_cast<char*>(PAGE_END(reinterpret_cast<uintptr_t>(shadow_end)));
    tmp_start =
        reinterpret_cast<char*>(mmap(nullptr, aligned_end - aligned_start, PROT_READ | PROT_WRITE,
                                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0));
    CHECK(tmp_start != MAP_FAILED);
    memcpy(tmp_start, aligned_start, shadow_start - aligned_start);
    memcpy(tmp_start + (shadow_end - aligned_start), shadow_end, aligned_end - shadow_end);
  }

  uint16_t* begin() {
    return reinterpret_cast<uint16_t*>(tmp_start + (shadow_start - aligned_start));
  }

  uint16_t* end() {
    return reinterpret_cast<uint16_t*>(tmp_start + (shadow_end - aligned_start));
  }

  ~ShadowWrite() {
    size_t size = aligned_end - aligned_start;
    mprotect(tmp_start, size, PROT_READ);
    void* res = mremap(tmp_start, size, size, MREMAP_MAYMOVE | MREMAP_FIXED,
                       reinterpret_cast<void*>(aligned_start));
    CHECK(res != MAP_FAILED);
  }
};

void CFIShadow::FixupVmaName() {
  prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, start, size, "cfi shadow");
}

void CFIShadow::AddConstant(uintptr_t begin, uintptr_t end, uint16_t v) {
  CHECK((v & 0xFF) == (v >> 8));
  uint16_t* shadow_begin = MemToShadow(begin);
  uint16_t* shadow_end = MemToShadow(end - 1) + 1;

  ShadowWrite sw(shadow_begin, shadow_end);
  memset(sw.begin(), v & 0xFF, (sw.end() - sw.begin()) * sizeof(*sw.begin()));
}

void CFIShadow::AddUnchecked(uintptr_t begin, uintptr_t end) {
  AddConstant(begin, end, kUncheckedShadow);
}

void CFIShadow::AddInvalid(uintptr_t begin, uintptr_t end) {
  AddConstant(begin, end, kInvalidShadow);
}

void CFIShadow::Add(uintptr_t begin, uintptr_t end, uintptr_t cfi_check) {
  assert((cfi_check & (kCfiCheckAlign - 1)) == 0);

  // DL_WARN("Add(%zx, %zx, %zx\n", begin, end, cfi_check);

  // Don't fill anything below cfi_check. We can not represent those addresses
  // in the shadow, and must make sure at codegen to place all valid call
  // targets above cfi_check.
  begin = std::max(begin, cfi_check) & ~(kShadowAlign - 1);
  uint16_t* shadow_begin = MemToShadow(begin);
  uint16_t* shadow_end = MemToShadow(end - 1) + 1;

  // Check that shadow range is currently unused.
  // FIXME: if this fails, replace all conflicts with either a resolution function (how?),
  // or, conservatively, with kUncheckedShadow.
  for (uint16_t *s = shadow_begin; s < shadow_end; ++s)
    CHECK(*s == kInvalidShadow);

  ShadowWrite sw(shadow_begin, shadow_end);
  uint16_t sv = ((begin + kShadowAlign - cfi_check) >> kCfiCheckGranularity) + 1;

  // DL_WARN("shadow begin %p end %p\n", shadow_begin, shadow_end);
  // DL_WARN("shadow value for begin (%zx) is %hu\n", begin, sv);

  // With each step of the loop below, __cfi_check address computation base is increased by
  // 2**ShadowGranularity.
  // To compensate for that, each next shadow value must be increased by 2**ShadowGranularity /
  // 2**CfiCheckGranularity.
  uint16_t sv_step = 1 << (kShadowGranularity - kCfiCheckGranularity);
  for (uint16_t& s : sw) {
    s = sv;
    sv += sv_step;
  }
}

uintptr_t GetMaxVirtualAddress() {
  char dummy;
  uintptr_t ptr = reinterpret_cast<uintptr_t>(&dummy);
  const uintptr_t kPtrSize = 8 * sizeof(uintptr_t);
  return (1UL << (kPtrSize - __builtin_clzl(ptr))) - 1;
}

void CFIShadow::MapShadow() {
  uintptr_t vma = GetMaxVirtualAddress();
  // Shadow is 2 -> 2**kShadowGranularity.
  size = (vma >> (kShadowGranularity - 1)) + 1;
  void * p =mmap(nullptr, size, PROT_READ | PROT_WRITE,
                 MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0);
  CHECK(p != MAP_FAILED);
  mprotect(p, size, PROT_READ);
  start = reinterpret_cast<uintptr_t>(p);
  // FIXME: mprotect start and size.
}

uintptr_t soinfo_find_cfi_check(soinfo *si) {
  SymbolName name("__cfi_check");
  const ElfW(Sym)* sym;
  if (si->find_symbol_by_name(name, nullptr, &sym) && sym)
    return si->resolve_symbol_address(sym);
  return 0;
}

void CFIShadow::AddLibrary(soinfo* si) {
  CHECK(start);
  if (si->base == 0 || si->size == 0) return;
  uintptr_t cfi_check = soinfo_find_cfi_check(si);
  if (cfi_check) {
#ifdef __arm__
    // Require Thumb encoding. FIXME: force thumb at compile time?
    CHECK((cfi_check & 1UL) == 1UL);
    cfi_check &= ~1UL;
#endif
    CHECK((cfi_check & (kCfiCheckAlign - 1)) == 0);
    Add(si->base, si->base + si->size, cfi_check);
  } else {
    AddUnchecked(si->base, si->base + si->size);
  }
}

void CFIShadow::MaybeInit(soinfo *soinfos[], size_t count, soinfo *solist) {
  CHECK(!start);

  // See if any of the new libraries use CFI.
  size_t i;
  for (i = 0; i < count; ++i)
    if (soinfo_find_cfi_check(soinfos[i]))
      break;

  // Nothing found.
  if (i == count) return;

  // Init shadow and add all currently loaded libraries (not just the new ones).
  MapShadow();
  for (soinfo* si = solist; si != nullptr; si = si->next)
    AddLibrary(si);
  FixupVmaName();
}

void CFIShadow::AfterLoad(soinfo *soinfos[], size_t count, soinfo* solist) {
  if (!start) {
    MaybeInit(soinfos, count, solist);
    return;
  }

  // Add the new bunch of libraries to the CFI shadow.
  for (size_t i = 0; i < count; ++i)
    AddLibrary(soinfos[i]);
  FixupVmaName();
}

void CFIShadow::BeforeUnload(soinfo *si) {
  if (!start) return;
  if (!si->base || !si->size) return;
  AddInvalid(si->base, si->base + si->size);
  FixupVmaName();
}

// Find __cfi_check in the caller and let it handle the problem. Since caller_pc is likely not a
// valid CFI target, we can not use CFI shadow for lookup. This does not need to be fast, do the
// regular symbol lookup.
void CFIShadow::CfiFail(uint64_t CallSiteTypeId, void* Ptr, void* DiagData, void* caller_pc) {
  soinfo* si = find_containing_library(caller_pc);
  // FIXME: __builtin_trap generates SIGTRAP on aarch64, but SIGILL on arm?
  if (!si) __builtin_trap();

  uintptr_t cfi_check = soinfo_find_cfi_check(si);
  if (!cfi_check) __builtin_trap();

  reinterpret_cast<CFICheckFn>(cfi_check)(CallSiteTypeId, Ptr, DiagData);
}
