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

#ifndef _LINKER_CFI_H_
#define _LINKER_CFI_H_

#include "linker.h"
#include "linker_debug.h"

#include <algorithm>

// This class implements shared library support for Control Flow Integrity in Clang.
// See documentation in http://clang.llvm.org/docs/ControlFlowIntegrityDesign.html#shared-library-support.
//
// This is effectively a very fast and specialized implementation of dladdr: given an address that
// belongs to a shared library or an executable, it finds the address of a specific export in that
// library (a function called "__cfi_check") and calls it. This is only guaranteed to work for
// addresses of possible CFI targets inside a library: indirectly called functions and virtual
// tables. A random address inside a library may not work in the future (but it does in the current
// implementation).
//
// Implementation is a sparse array of uint16_t where each element describes the location of
// __cfi_check for a 2**kShadowGranularity range of memory. Array elements (called "shadow values"
// below) are interpreted as follows.
//
// For an address P and corresponding shadow value V, the address of __cfi_check is calculated as
//   align_up(P, 2**kShadowGranularity) - (V - 2) * (2 ** kCfiCheckGranularity)
//
// Special shadow values:
//        0 = kInvalidShadow, this memory range has no valid CFI targets.
//        1 = kUncheckedShadow, any address is this memory range is a valid CFI target
//
// Loader requirement: each aligned 2**kShadowGranularity region of address space may contain at
// most one DSO.
// Compiler requirement: __cfi_check is aligned at kCfiCheckGranularity.
// Compiler requirement: __cfi_check for a given DSO is located below any CFI target for that DSO.
//
// Shadow is mapped and initialized lazily as soon as the first CFI-enabled DSO is loaded.
class CFIShadow {
  static constexpr uintptr_t kShadowGranularity = kLibraryAlignmentBits;
  static constexpr uintptr_t kCfiCheckGranularity = 12;

  // Each uint16_t element of the shadow corresponds to this much application memory.
  static constexpr uintptr_t kShadowAlign = 1UL << kShadowGranularity;

  // Alignment of __cfi_check.
  static constexpr uintptr_t kCfiCheckAlign = 1UL << kCfiCheckGranularity;  // 4K

#if defined(__aarch64__)
  static constexpr uintptr_t kMaxTargetAddr = 0x7fffffffff;
#elif defined (__LP64__)
  static constexpr uintptr_t kMaxTargetAddr = 0x7fffffffffff;
#else
  static constexpr uintptr_t kMaxTargetAddr = 0xffffffff;
#endif

  // Shadow is 2 -> 2**kShadowGranularity.
  static constexpr uintptr_t kShadowSize =
      align_up((kMaxTargetAddr >> (kShadowGranularity - 1)), PAGE_SIZE);

  // Returns offset inside the shadow region for an address.
  constexpr uintptr_t MemToShadowOffset(uintptr_t x) {
    return (x >> kShadowGranularity) << 1;
  }

  // Returns pointer to the shadow element for an address.
  uint16_t* MemToShadow(uintptr_t x) {
    return reinterpret_cast<uint16_t*>(start + MemToShadowOffset(x));
  }

  // Update shadow for the address range to the given constant value.
  void AddConstant(uintptr_t begin, uintptr_t end, uint16_t v);

  // Update shadow for the address range to kUncheckedShadow.
  void AddUnchecked(uintptr_t begin, uintptr_t end);

  // Update shadow for the address range to kInvalidShadow.
  void AddInvalid(uintptr_t begin, uintptr_t end);

  // Update shadow for the address range to the given __cfi_check value.
  void Add(uintptr_t begin, uintptr_t end, uintptr_t cfi_check);

  // Add a DSO to CFI shadow.
  void AddLibrary(soinfo* si);

  // Initialize CFI shadow.
  void MapShadow();

  // If any of the soinfos[] are CFI-enabled, initialize the shadow and update it for everything in
  // solist.
  void MaybeInit(soinfo *soinfos[], size_t count, soinfo *solist);

  // Set a human readable name for the entire shadow region.
  void FixupVmaName();

  uintptr_t start;

  typedef int (*CFICheckFn)(uint64_t, void *, void *);
 public:
  enum ShadowValues : uint16_t {
    kInvalidShadow = 0,    // Not a valid CFI target.
    kUncheckedShadow = 1,  // Unchecked, valid CFI target.
    kRegularShadowMin = 2  // This and all higher values encode a negative offset to __cfi_check in
                           // the units of kCfiCheckGranularity, starting with 0 at
                           // kRegularShadowMin.
  };

  // Update shadow after loading a set of DSOs.
  // This function will initialize the shadow if it sees a CFI-enabled DSO for the first time.
  // In that case it will retroactively update shadow for all previously loaded DSOs. "solist" is a
  // pointer to the global list.
  // This function must be called before any user code has observed the newly loaded DSO.
  void AfterLoad(soinfo* soinfos[], size_t count, soinfo *solist);

  // Update shadow before unloading a DSO.
  void BeforeUnload(soinfo* si);

  // Load a shadow value for a target address. The result is either kInvalidShadow,
  // kUncheckedShadow, or an opaque value that can only be used by passing it to CallCfiCheck.
  uint16_t Load(const void *ptr) {
    uintptr_t addr = reinterpret_cast<uintptr_t>(ptr);
    if (MemToShadowOffset(addr) > kShadowSize) return kInvalidShadow;
    return *MemToShadow(addr);
  }

  static uintptr_t CfiCheckAddr(uint16_t v, void *Ptr) {
    uintptr_t addr = reinterpret_cast<uintptr_t>(Ptr);
    uintptr_t aligned_addr = align_up(addr, kShadowAlign);
    uintptr_t p =
        aligned_addr - (static_cast<uintptr_t>(v - kRegularShadowMin) << kCfiCheckGranularity);
#ifdef __arm__
    // Assume Thumb encoding. FIXME: force thumb at compile time?
    p++;
#endif
    return p;
  }

  // Call __cfi_check. "v" is a shadow value returned from Load(), the rest of the arguments are
  // forwarded to __cfi_check. "v" must not be one of the special shadow value.
  static void CallCfiCheck(uint16_t v, uint64_t CallSiteTypeId, void* Ptr, void *DiagData) {
    uintptr_t p = CfiCheckAddr(v, Ptr);
    reinterpret_cast<CFICheckFn>(p)(CallSiteTypeId, Ptr, DiagData);
  }

  // Handle failure to locate __cfi_check for a target address.
  static void CfiFail(uint64_t CallSiteTypeId, void* Ptr, void* DiagData, void *caller_pc);
};

CFIShadow* get_cfi_shadow();

#endif // _LINKER_CFI_H_
