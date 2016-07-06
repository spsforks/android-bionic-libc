#include "linker_cfi.h"

#include "linker_debug.h"
#include "private/bionic_page.h"

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
  assert((cfi_check & (kShadowAlign - 1)) == 0);

  // Don't fill anything below cfi_check. We can not represent those addresses
  // in the shadow, and must make sure at codegen to place all valid call
  // targets above cfi_check.
  begin = std::max(begin, cfi_check);
  uint16_t* shadow_begin = MemToShadow(begin);
  uint16_t* shadow_end = MemToShadow(end - 1) + 1;

  ShadowWrite sw(shadow_begin, shadow_end);
  uint16_t sv = ((begin - cfi_check) >> kShadowGranularity) + 1;
  for (uint16_t& s : sw) s = sv++;
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
  start = reinterpret_cast<uintptr_t>(mmap(nullptr, size, PROT_READ | PROT_WRITE,
                                           MAP_PRIVATE | MAP_ANONYMOUS | MAP_NORESERVE, -1, 0));
  CHECK(start);
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
  if (cfi_check)
    Add(si->base, si->base + si->size, cfi_check);
  else
    AddUnchecked(si->base, si->base + si->size);
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
}

void CFIShadow::AfterLoad(soinfo *soinfos[], size_t count, soinfo* solist) {
  if (!start) {
    MaybeInit(soinfos, count, solist);
    return;
  }

  // Add the new bunch of libraries to the CFI shadow.
  for (size_t i = 0; i < count; ++i)
    AddLibrary(soinfos[i]);
}

void CFIShadow::BeforeUnload(soinfo *si) {
  if (!start) return;
  AddInvalid(si->base, si->base + si->size);
}
