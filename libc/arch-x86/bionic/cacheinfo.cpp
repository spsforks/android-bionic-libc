/*
Copyright (C) 2023 Intel Corporation.

This software and the related documents are Intel copyrighted materials,
and your use of them is governed by the express license under which they
were provided to you (End User License Agreement for the Intel(R) Software
Development Products). Unless the License provides
otherwise, you may not use, modify, copy, publish, distribute, disclose or
transmit this software or the related documents without Intel's prior
written permission.

This software and the related documents are provided as is, with no
express or implied warranties, other than those that are expressly
stated in the License.
*/

#include <async_safe/log.h>
#include "cpuinfo_x86.h"
#include "private/bionic_cacheinfo.h"

#ifdef _PRIVATE_ARCH_CACHE_H_
/* Data cache size for use in memory and string routines, typically
   L1 size, rounded to multiple of 256 bytes.  */
extern unsigned long int __x86_data_cache_size_half;
extern unsigned long int __x86_data_cache_size;
/* Shared cache size for use in memory and string routines, typically
   L2 or L3 size, rounded to multiple of 256 bytes.  */
extern unsigned long int __x86_shared_cache_size_half;
extern unsigned long int __x86_shared_cache_size;
/* Shared cache size for memset routines*/
extern unsigned long int __x86_shared_cache_size_memset;
#endif

/* Data cache size for use in memory and string routines, typically
   L1 size, rounded to multiple of 256 bytes.  */
unsigned long int __x86_data_cache_size = 24 * 1024;
unsigned long int __x86_data_cache_size_half = __x86_data_cache_size / 2;
/* Shared cache size for use in memory and string routines, typically
   L2 or L3 size, rounded to multiple of 256 bytes.  */
unsigned long int __x86_shared_cache_size = 1024 * 1024;
unsigned long int __x86_shared_cache_size_half = __x86_shared_cache_size / 2;
/* Shared cache size for memset routines*/
unsigned long int __x86_shared_cache_size_memset = 1024 * 1024;

using namespace cpu_features;

void __libc_init_cacheinfo() {
  CacheInfo info = GetX86CacheInfo();
  for (int count = 0, i = 0; count < info.size && i < CPU_FEATURES_MAX_CACHE_LEVEL; ++i) {
    if (info.levels[i].level == 1 && info.levels[i].cache_type == CPU_FEATURE_CACHE_DATA) {
      // L1D
      __x86_data_cache_size = info.levels[i].cache_size;
      __x86_data_cache_size_half = __x86_data_cache_size / 2;
    } else if (info.levels[i].level == 2 &&
               (info.levels[i].cache_type == CPU_FEATURE_CACHE_UNIFIED ||
                info.levels[i].cache_type == CPU_FEATURE_CACHE_DATA)) {
      // Shared cache
      __x86_shared_cache_size = info.levels[i].cache_size;
      __x86_shared_cache_size_half = __x86_shared_cache_size / 2;

      __x86_shared_cache_size_memset = __x86_shared_cache_size * 2;
    }
  }
}
