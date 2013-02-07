/*
 * Copyright (C) 2013 The Android Open Source Project
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

#ifndef ELF_DATA_H
#define ELF_DATA_H

#include <elf.h>
#include <stdint.h>
#include <sys/auxv.h>

class ElfData {
 public:
  ElfData(void* raw_elf_data) {
    uint32_t* elf_data = reinterpret_cast<uint32_t*>(raw_elf_data);
    argc = static_cast<int>(*elf_data);
    argv = reinterpret_cast<char**>(elf_data + 1);
    envp = argv + argc + 1;

    // Skip over all environment variable definitions to find aux vector.
    // The end of the environment block is marked by two NULL pointers.
    char** p = envp;
    while (*p != NULL) {
      ++p;
    }
    ++p; // Skip second NULL;

    auxv = (Elf32_auxv_t*) p;
  }

  unsigned long getauxval(unsigned long type) {
    for (Elf32_auxv_t* v = auxv; v->a_type != AT_NULL; ++v) {
      if (v->a_type == type) {
        return v->a_un.a_val;
      }
    }
    return 0;
  }

  int argc;
  char** argv;
  char** envp;
  Elf32_auxv_t* auxv;

 private:
  // Disallow copy and assignment.
  ElfData(const ElfData&);
  void operator=(const ElfData&);
};

#endif // ELF_DATA_H
