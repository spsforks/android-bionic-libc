/*
Copyright (c) 2010, Intel Corporation
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
    * this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above copyright notice,
    * this list of conditions and the following disclaimer in the documentation
    * and/or other materials provided with the distribution.

    * Neither the name of Intel Corporation nor the names of its contributors
    * may be used to endorse or promote products derived from this software
    * without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

# undef USE_GLIBC_CACHE_SIZE
# ifdef USE_GLIBC_CACHE_SIZE
    /*
     * These are normally stored in a library symbol referenced off of the GOT.
     * This minimal library does not define them, so we hard wire them.
     */
#    define X86_64_data_cache_size		(32 * 1024)		/* Values from glibc */
#    define X86_64_data_cache_size_half		(X86_64_data_cache_size / 2)
#    define X86_64_shared_cache_size		(1024 * 1024)
#    define X86_64_shared_cache_size_half	(X86_64_shared_cache_size / 2)

#    define SHARED_CACHE_SIZE		X86_64_shared_cache_size
#    define DATA_CACHE_SIZE		X86_64_data_cache_size
#    define SHARED_CACHE_SIZE_HALF	X86_64_shared_cache_size_half
#    define DATA_CACHE_SIZE_HALF	X86_64_data_cache_size_half
# else

#    define SHARED_CACHE_SIZE		(512*1024)			/* Atom L2 Cache */
#    define DATA_CACHE_SIZE		(24*1024)			/* Atom L1 Data Cache */
#    define SHARED_CACHE_SIZE_HALF	(SHARED_CACHE_SIZE / 2)
#    define DATA_CACHE_SIZE_HALF	(DATA_CACHE_SIZE / 2)
# endif
