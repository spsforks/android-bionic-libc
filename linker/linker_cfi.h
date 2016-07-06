#ifndef _LINKER_CFI_H_
#define _LINKER_CFI_H_

#include "linker.h"
#include "linker_debug.h"

#include <algorithm>

class CFIShadow {
  static const uintptr_t kShadowGranularity = 12;
  static const uintptr_t kShadowAlign = 1UL << kShadowGranularity;  // 4096

  uintptr_t MemToShadowOffset(uintptr_t x) {
    return (x >> kShadowGranularity) << 1;
  }

  uint16_t* MemToShadow(uintptr_t x) {
    return reinterpret_cast<uint16_t*>(start + MemToShadowOffset(x));
  }

  void AddConstant(uintptr_t begin, uintptr_t end, uint16_t v);
  void AddUnchecked(uintptr_t begin, uintptr_t end);
  void AddInvalid(uintptr_t begin, uintptr_t end);
  void Add(uintptr_t begin, uintptr_t end, uintptr_t cfi_check);
  void AddLibrary(soinfo* si);
  void MapShadow();
  void MaybeInit(soinfo *soinfos[], size_t count, soinfo *solist);

  uintptr_t start;
  size_t size;

  typedef int (*CFICheckFn)(uint64_t, void *, void *);
 public:
  static const uint16_t kInvalidShadow = 0;
  static const uint16_t kUncheckedShadow = 0xFFFFU;

  void AfterLoad(soinfo* soinfos[], size_t count, soinfo *solist);
  void BeforeUnload(soinfo* si);
  void ProcessInitialLibraries(soinfo* solist);

  uint16_t Load(const void *ptr) {
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    uintptr_t ofs = MemToShadowOffset(addr);
    if (ofs > size) return kInvalidShadow;
    return *MemToShadow(addr);
  }

  void CallCfiCheck(uint16_t v, uint64_t CallSiteTypeId, void* Ptr, void *DiagData) {
    uintptr_t addr = reinterpret_cast<uintptr_t>(Ptr);
    uintptr_t aligned_addr = addr & ~(kShadowAlign - 1);
    uintptr_t p = aligned_addr - (static_cast<uintptr_t>(v - 1) << kShadowGranularity);
#ifdef __arm__
    // Assume Thumb encoding. FIXME: force thumb at compile time?
    p++;
#endif
    reinterpret_cast<CFICheckFn>(p)(CallSiteTypeId, Ptr, DiagData);
  }
};

#endif // _LINKER_CFI_H_
