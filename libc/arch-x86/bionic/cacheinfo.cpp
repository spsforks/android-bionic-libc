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

#include <include/bits/sysconf.h>
#include "private/bionic_cacheinfo.h"

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

void __libc_init_cacheinfo() {
  // L1D
  __x86_data_cache_size = sysconf(_SC_LEVEL1_DCACHE_SIZE);
  __x86_data_cache_size_half = __x86_data_cache_size / 2;

  /* Shared cache size */
  __x86_shared_cache_size = sysconf(_SC_LEVEL2_CACHE_SIZE);
  __x86_shared_cache_size_half = __x86_shared_cache_size / 2;

  __x86_shared_cache_size_memset = __x86_shared_cache_size * 2;
}
