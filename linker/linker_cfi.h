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

#include "private/CFIShadow.h"

// This class keeps the contents of CFI shadow up-to-date with the current set of loaded libraries.
// See the comment in CFIShadow.h for more context.
// See documentation in http://clang.llvm.org/docs/ControlFlowIntegrityDesign.html#shared-library-support.
//
// Shadow is mapped and initialized lazily as soon as the first CFI-enabled DSO is loaded.
// It is updated after a set of libraries is loaded (but before any constructors are ran), and
// before any library is unloaded.
class CFIShadowWriter : private CFIShadow {
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

  void NotifyLibDl(soinfo *solist);

  uintptr_t start;
  bool initial_link_done;

 public:
  // Update shadow after loading a set of DSOs.
  // This function will initialize the shadow if it sees a CFI-enabled DSO for the first time.
  // In that case it will retroactively update shadow for all previously loaded DSOs. "solist" is a
  // pointer to the global list.
  // This function must be called before any user code has observed the newly loaded DSO.
  void AfterLoad(soinfo* soinfos[], size_t count, soinfo *solist);

  // Update shadow before unloading a DSO.
  void BeforeUnload(soinfo* si);

  void InitialLinkDone(soinfo *solist);

  // Handle failure to locate __cfi_check for a target address.
  static void CfiFail(uint64_t CallSiteTypeId, void* Ptr, void* DiagData, void *caller_pc);
};

CFIShadowWriter* get_cfi_shadow();

#endif // _LINKER_CFI_H_
