/*
 * Copyright (C) 2015 The Android Open Source Project
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

#ifndef TOOLS_RELOCATION_PACKER_SRC_LIBELF_SUPPORT_H_
#define TOOLS_RELOCATION_PACKER_SRC_LIBELF_SUPPORT_H_

#include <elf.h>
#include "libelf.h"

// TODO: android host toolchain does not have these constants yet.
#if !defined(R_ARM_IRELATIVE)
#define R_ARM_IRELATIVE   160
#endif
#if !defined(R_AARCH64_IRELATIVE)
#define R_AARCH64_IRELATIVE 1032
#endif
#if !defined(R_AARCH64_RELATIVE)
#define R_AARCH64_RELATIVE 1027
#endif

namespace relocation_packer {

template <typename ELF>
class RelocationHelper {
 public:
  virtual bool is_relative(const typename ELF::Rela& relocation) = 0;
  static RelocationHelper<ELF>* make_relocation_helper(typename ELF::Half machine);
};

template <typename ELF>
class RelocationHelperArm : public RelocationHelper<ELF> {
 public:
  virtual bool is_relative(const typename ELF::Rela& relocation) {
    typename ELF::Xword type = ELF::elf_r_type(relocation.r_info);
    return (type == R_ARM_RELATIVE) ||
           (type == R_ARM_IRELATIVE);
  }
};

template <typename ELF>
class RelocationHelperArm64 : public RelocationHelper<ELF> {
 public:
  virtual bool is_relative(const typename ELF::Rela& relocation) {
    typename ELF::Xword type = ELF::elf_r_type(relocation.r_info);
    return (type == R_AARCH64_RELATIVE) ||
           (type == R_AARCH64_IRELATIVE);
  }
};

template <typename ELF>
class RelocationHelper386 : public RelocationHelper<ELF> {
 public:
  virtual bool is_relative(const typename ELF::Rela& relocation) {
    typename ELF::Xword type = ELF::elf_r_type(relocation.r_info);
    return (type == R_386_RELATIVE) ||
           (type == R_386_IRELATIVE);
  }
};

template <typename ELF>
class RelocationHelper86_64 : public RelocationHelper<ELF> {
 public:
  virtual bool is_relative(const typename ELF::Rela& relocation) {
    typename ELF::Xword type = ELF::elf_r_type(relocation.r_info);
    return (type == R_X86_64_RELATIVE) ||
           (type == R_X86_64_IRELATIVE);
  }
};

template <typename ELF>
class RelocationHelperMips : public RelocationHelper<ELF> {
 public:
  virtual bool is_relative(const typename ELF::Rela& relocation) {
    return ELF::elf_r_type(relocation.r_info) == R_MIPS_REL32 &&
           ELF::elf_r_sym(relocation.r_info) == 0;
  }
}

template <typename ELF>
RelocationHelper<ELF>* RelocationHelper<ELF>::make_relocation_helper(typename ELF::Half machine) {
  switch(machine) {
  case EM_386:
    return new RelocationHelper386<ELF>();
  case EM_X86_64:
    return new RelocationHelper86_64<ELF>();
  case EM_ARM:
    return new RelocationHelperArm<ELF>();
  case EM_AARCH64:
    return new RelocationHelperArm64<ELF>();
  case EM_MIPS:
    return new RelocationHelperMips<ELF>();
  // TODO: mips64?
  }
  return nullptr;
}

};  // namespace relocation_packer

#endif  // TOOLS_RELOCATION_PACKER_SRC_LIBELF_SUPPORT_H_
