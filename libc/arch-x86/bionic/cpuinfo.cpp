#include <async_safe/log.h>
#include "../cache.h"
#include "cpuinfo_x86.h"
#include "private/bionic_cpuinfo.h"

/* Data cache size for use in memory and string routines, typically
   L1 size, rounded to multiple of 256 bytes.  */
unsigned long int __x86_data_cache_size = 24 * 1024;
unsigned long int __x86_data_cache_size_half = __x86_data_cache_size / 2;
/* Shared cache size for use in memory and string routines, typically
   L2 or L3 size, rounded to multiple of 256 bytes.  */
unsigned long int __x86_shared_cache_size = 1024 * 1024;
unsigned long int __x86_shared_cache_size_half = __x86_shared_cache_size / 2;

using namespace cpu_features;

void __libc_init_cpuinfo() {
  __libc_init_cacheinfo();
}

void __libc_init_cacheinfo() {
  CacheInfo info = GetX86CacheInfo();
  bool bL3Obtained = false;
  for (int count = 0, i = 0; count < info.size && i < CPU_FEATURES_MAX_CACHE_LEVEL; ++i) {
    if (info.levels[i].level == 1 && info.levels[i].cache_type == CPU_FEATURE_CACHE_DATA) {
      // L1D
      __x86_data_cache_size = info.levels[i].cache_size;
      __x86_data_cache_size_half = __x86_data_cache_size / 2;
    } else if (info.levels[i].level == 3 &&
               info.levels[i].cache_type == CPU_FEATURE_CACHE_UNIFIED) {
      // L3Unified
      __x86_shared_cache_size = info.levels[i].cache_size;
      __x86_shared_cache_size_half = __x86_shared_cache_size / 2;
      bL3Obtained = true;
    } else if (info.levels[i].level == 2 &&
               (info.levels[i].cache_type == CPU_FEATURE_CACHE_UNIFIED ||
                info.levels[i].cache_type == CPU_FEATURE_CACHE_DATA) &&
               !bL3Obtained) {
      // Fallback to L2 Data or Unified
      // If no L3, this must be the shared cache
      __x86_shared_cache_size = info.levels[i].cache_size;
      __x86_shared_cache_size_half = __x86_shared_cache_size / 2;
    }
  }
}