/*
 * Copyright (C) 2008 The Android Open Source Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <linux/auxvec.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dlfcn.h>
#include <sys/stat.h>

#include <pthread.h>

#include <sys/mman.h>

#include <sys/atomics.h>

/* special private C library header - see Android.mk */
#include <bionic_tls.h>

#include "linker.h"
#include "linker_debug.h"

#include "ba.h"

#define SO_MAX 64

/* >>> IMPORTANT NOTE - READ ME BEFORE MODIFYING <<<
 *
 * Do NOT use malloc() and friends or pthread_*() code here.
 * Don't use printf() either; it's caused mysterious memory
 * corruption in the past.
 * The linker runs before we bring up libc and it's easiest
 * to make sure it does not depend on any complex libc features
 *
 * open issues / todo:
 *
 * - should we do anything special for STB_WEAK symbols?
 * - are we doing everything we should for ARM_COPY relocations?
 * - cleaner error reporting
 * - configuration for paths (LD_LIBRARY_PATH?)
 * - after linking, set as much stuff as possible to READONLY
 *   and NOEXEC
 * - linker hardcodes PAGE_SIZE and PAGE_MASK because the kernel
 *   headers provide versions that are negative...
 * - allocate space for soinfo structs dynamically instead of
 *   having a hard limit (64)
 *
 * features to add someday:
 *
 * - dlopen() and friends
 *
*/


static int link_image(soinfo *si, unsigned wr_offset);

static int socount = 0;
static soinfo sopool[SO_MAX];
static soinfo *freelist = NULL;
static soinfo *solist = &libdl_info;
static soinfo *sonext = &libdl_info;

int debug_verbosity;
static int pid;

#if STATS
struct _link_stats linker_stats;
#endif

#if COUNT_PAGES
unsigned bitmask[4096];
#endif

#ifndef PT_ARM_EXIDX
#define PT_ARM_EXIDX    0x70000001      /* .ARM.exidx segment */
#endif

/*
 * This function is an empty stub where GDB locates a breakpoint to get notified
 * about linker activity.
 */
extern void __attribute__((noinline)) rtld_db_dlactivity(void);

static struct r_debug _r_debug = {1, NULL, &rtld_db_dlactivity,
                                  RT_CONSISTENT, 0};
static struct link_map *r_debug_tail = 0;

static pthread_mutex_t _r_debug_lock = PTHREAD_MUTEX_INITIALIZER;

static void insert_soinfo_into_debug_map(soinfo * info)
{
    struct link_map * map;

    /* Copy the necessary fields into the debug structure.
     */
    map = &(info->linkmap);
    map->l_addr = info->base;
    map->l_name = (char*) info->name;

    /* Stick the new library at the end of the list.
     * gdb tends to care more about libc than it does
     * about leaf libraries, and ordering it this way
     * reduces the back-and-forth over the wire.
     */
    if (r_debug_tail) {
        r_debug_tail->l_next = map;
        map->l_prev = r_debug_tail;
        map->l_next = 0;
    } else {
        _r_debug.r_map = map;
        map->l_prev = 0;
        map->l_next = 0;
    }
    r_debug_tail = map;
}

static void remove_soinfo_from_debug_map(soinfo * info)
{
    struct link_map * map = &(info->linkmap);

    if (r_debug_tail == map)
        r_debug_tail = map->l_prev;

    if (map->l_prev) map->l_prev->l_next = map->l_next;
    if (map->l_next) map->l_next->l_prev = map->l_prev;
}

void notify_gdb_of_load(soinfo * info)
{
    if (info->flags & FLAG_EXE) {
        // GDB already knows about the main executable
        return;
    }

    pthread_mutex_lock(&_r_debug_lock);

    _r_debug.r_state = RT_ADD;
    rtld_db_dlactivity();

    insert_soinfo_into_debug_map(info);

    _r_debug.r_state = RT_CONSISTENT;
    rtld_db_dlactivity();

    pthread_mutex_unlock(&_r_debug_lock);
}

void notify_gdb_of_unload(soinfo * info)
{
    if (info->flags & FLAG_EXE) {
        // GDB already knows about the main executable
        return;
    }

    pthread_mutex_lock(&_r_debug_lock);

    _r_debug.r_state = RT_DELETE;
    rtld_db_dlactivity();

    remove_soinfo_from_debug_map(info);

    _r_debug.r_state = RT_CONSISTENT;
    rtld_db_dlactivity();

    pthread_mutex_unlock(&_r_debug_lock);
}

void notify_gdb_of_libraries()
{
    _r_debug.r_state = RT_ADD;
    rtld_db_dlactivity();
    _r_debug.r_state = RT_CONSISTENT;
    rtld_db_dlactivity();
}

static soinfo *alloc_info(const char *name)
{
    soinfo *si;

    if(strlen(name) >= SOINFO_NAME_LEN) {
        ERROR("%5d library name %s too long\n", pid, name);
        return 0;
    }

    /* The freelist is populated when we call free_info(), which in turn is
       done only by dlclose(), which is not likely to be used.
    */
    if (!freelist) {
        if(socount == SO_MAX) {
            ERROR("%5d too many libraries when loading %s\n", pid, name);
            return NULL;
        }
        freelist = sopool + socount++;
        freelist->next = NULL;
    }

    si = freelist;
    freelist = freelist->next;

    /* Make sure we get a clean block of soinfo */
    memset(si, 0, sizeof(soinfo));
    strcpy((char*) si->name, name);
    sonext->next = si;
    si->ba_index = -1; /* by default, prelinked */
    si->next = NULL;
    si->refcount = 0;
    sonext = si;

    TRACE("%5d name %s: allocated soinfo @ %p\n", pid, name, si);
    return si;
}

static void free_info(soinfo *si)
{
    soinfo *prev = NULL, *trav;

    TRACE("%5d name %s: freeing soinfo @ %p\n", pid, si->name, si);

    for(trav = solist; trav != NULL; trav = trav->next){
        if (trav == si)
            break;
        prev = trav;
    }
    if (trav == NULL) {
        /* si was not ni solist */
        ERROR("%5d name %s is not in solist!\n", pid, si->name);
        return;
    }

    /* prev will never be NULL, because the first entry in solist is 
       always the static libdl_info.
    */
    prev->next = si->next;
    if (si == sonext) sonext = prev;
    si->next = freelist;
    freelist = si;
}

#ifndef LINKER_TEXT_BASE
#error "linker's makefile must define LINKER_TEXT_BASE"
#endif
#ifndef LINKER_AREA_SIZE
#error "linker's makefile must define LINKER_AREA_SIZE"
#endif
#define LINKER_BASE ((LINKER_TEXT_BASE) & 0xfff00000)
#define LINKER_TOP  (LINKER_BASE + (LINKER_AREA_SIZE))

const char *addr_to_name(unsigned addr)
{
    soinfo *si;

    for(si = solist; si != 0; si = si->next){
        if((addr >= si->base) && (addr < (si->base + si->size))) {
            return si->name;
        }
    }

    if((addr >= LINKER_BASE) && (addr < LINKER_TOP)){
        return "linker";
    }

    return "";
}

/* For a given PC, find the .so that it belongs to.
 * Returns the base address of the .ARM.exidx section
 * for that .so, and the number of 8-byte entries
 * in that section (via *pcount).
 *
 * Intended to be called by libc's __gnu_Unwind_Find_exidx().
 *
 * This function is exposed via dlfcn.c and libdl.so.
 */
#ifdef ANDROID_ARM_LINKER
_Unwind_Ptr dl_unwind_find_exidx(_Unwind_Ptr pc, int *pcount)
{
    soinfo *si;
    unsigned addr = (unsigned)pc;

    if ((addr < LINKER_BASE) || (addr >= LINKER_TOP)) {
        for (si = solist; si != 0; si = si->next){
            if ((addr >= si->base) && (addr < (si->base + si->size))) {
                *pcount = si->ARM_exidx_count;
                return (_Unwind_Ptr)(si->base + (unsigned long)si->ARM_exidx);
            }
        }
    }
   *pcount = 0;
    return NULL;
}
#elif defined(ANDROID_X86_LINKER)
/* Here, we only have to provide a callback to iterate across all the
 * loaded libraries. gcc_eh does the rest. */
int
dl_iterate_phdr(int (*cb)(struct dl_phdr_info *info, size_t size, void *data),
                void *data)
{
    soinfo *si;
    struct dl_phdr_info dl_info;
    int rv = 0;

    for (si = solist; si != NULL; si = si->next) {
        dl_info.dlpi_addr = si->linkmap.l_addr;
        dl_info.dlpi_name = si->linkmap.l_name;
        dl_info.dlpi_phdr = si->phdr;
        dl_info.dlpi_phnum = si->phnum;
        rv = cb(&dl_info, sizeof (struct dl_phdr_info), data);
        if (rv != 0)
            break;
    }
    return rv;
}
#endif

static Elf32_Sym *_elf_lookup(soinfo *si, unsigned hash, const char *name)
{
    Elf32_Sym *s;
    Elf32_Sym *symtab = si->symtab;
    const char *strtab = si->strtab;
    unsigned n;

    TRACE_TYPE(LOOKUP, "%5d SEARCH %s in %s@0x%08x %08x %d\n", pid,
               name, si->name, si->base, hash, hash % si->nbucket);
    n = hash % si->nbucket;

    for(n = si->bucket[hash % si->nbucket]; n != 0; n = si->chain[n]){
        s = symtab + n;
        if(strcmp(strtab + s->st_name, name)) continue;

            /* only concern ourselves with global symbols */
        switch(ELF32_ST_BIND(s->st_info)){
        case STB_GLOBAL:
                /* no section == undefined */
            if(s->st_shndx == 0) continue;

        case STB_WEAK:
            TRACE_TYPE(LOOKUP, "%5d FOUND %s in %s (%08x) %d\n", pid,
                       name, si->name, s->st_value, s->st_size);
            return s;
        }
    }

    return 0;
}

static unsigned elfhash(const char *_name)
{
    const unsigned char *name = (const unsigned char *) _name;
    unsigned h = 0, g;

    while(*name) {
        h = (h << 4) + *name++;
        g = h & 0xf0000000;
        h ^= g;
        h ^= g >> 24;
    }
    return h;
}

static Elf32_Sym *
_do_lookup_in_so(soinfo *si, const char *name, unsigned *elf_hash)
{
    if (*elf_hash == 0)
        *elf_hash = elfhash(name);
    return _elf_lookup (si, *elf_hash, name);
}

/* This is used by dl_sym() */
Elf32_Sym *lookup_in_library(soinfo *si, const char *name)
{
    unsigned unused = 0;
    return _do_lookup_in_so(si, name, &unused);
}

static Elf32_Sym *
_do_lookup(soinfo *user_si, const char *name, unsigned *base)
{
    unsigned elf_hash = 0;
    Elf32_Sym *s = NULL;
    soinfo *si;

    /* Look for symbols in the local scope first (the object who is
     * searching). This happens with C++ templates on i386 for some
     * reason. */
    if (user_si) {
        s = _do_lookup_in_so(user_si, name, &elf_hash);
        if (s != NULL)
            *base = user_si->base;
    }

    for(si = solist; (s == NULL) && (si != NULL); si = si->next)
    {
        if((si->flags & FLAG_ERROR) || (si == user_si))
            continue;
        s = _do_lookup_in_so(si, name, &elf_hash);
        if (s != NULL) {
            *base = si->base;
            break;
        }
    }

    if (s != NULL) {
        TRACE_TYPE(LOOKUP, "%5d %s s->st_value = 0x%08x, "
                   "si->base = 0x%08x\n", pid, name, s->st_value, si->base);
        return s;
    }

    return 0;
}

/* This is used by dl_sym() */
Elf32_Sym *lookup(const char *name, unsigned *base)
{
    return _do_lookup(NULL, name, base);
}

#if 0
static void dump(soinfo *si)
{
    Elf32_Sym *s = si->symtab;
    unsigned n;

    for(n = 0; n < si->nchain; n++) {
        TRACE("%5d %04d> %08x: %02x %04x %08x %08x %s\n", pid, n, s,
               s->st_info, s->st_shndx, s->st_value, s->st_size,
               si->strtab + s->st_name);
        s++;
    }
}
#endif

static const char *sopaths[] = {
    "/system/lib",
    "/lib",
    0
};

static int _open_lib(const char *name)
{
    int fd;
    struct stat filestat;

    if ((stat(name, &filestat) >= 0) && S_ISREG(filestat.st_mode)) {
        if ((fd = open(name, O_RDONLY)) >= 0)
            return fd;
    }

    return -1;
}

/* TODO: Need to add support for initializing the so search path with
 * LD_LIBRARY_PATH env variable for non-setuid programs. */
static int open_library(const char *name)
{
    int fd;
    char buf[512];
    const char **path;

    TRACE("[ %5d opening %s ]\n", pid, name);

    if(name == 0) return -1;
    if(strlen(name) > 256) return -1;

    if ((name[0] == '/') && ((fd = _open_lib(name)) >= 0))
        return fd;

    for (path = sopaths; *path; path++) {
        snprintf(buf, sizeof(buf), "%s/%s", *path, name);
        if ((fd = _open_lib(buf)) >= 0)
            return fd;
    }

    return -1;
}

/* temporary space for holding the first page of the shared lib
 * which contains the elf header (with the pht). */
static unsigned char __header[PAGE_SIZE];

typedef struct {
    long mmap_addr;
    char tag[4]; /* 'P', 'R', 'E', ' ' */
} prelink_info_t;

/* Returns the requested base address if the library is prelinked,
 * and 0 otherwise.  */
static unsigned long
is_prelinked(int fd, const char *name)
{
    off_t sz;
    prelink_info_t info;

    sz = lseek(fd, -sizeof(prelink_info_t), SEEK_END);
    if (sz < 0) {
        ERROR("lseek() failed!\n");
        return 0;
    }

    if (read(fd, &info, sizeof(info)) != sizeof(info)) {
        WARN("Could not read prelink_info_t structure for `%s`\n", name);
        return 0;
    }

    if (strncmp(info.tag, "PRE ", 4)) {
        WARN("`%s` is not a prelinked library\n", name);
        return 0;
    }

    return (unsigned long)info.mmap_addr;
}

/* verify_elf_object
 *      Verifies if the object @ base is a valid ELF object
 *
 * Args:
 *
 * Returns:
 *       0 on success
 *      -1 if no valid ELF object is found @ base.
 */
static int
verify_elf_object(void *base, const char *name)
{
    Elf32_Ehdr *hdr = (Elf32_Ehdr *) base;

    if (hdr->e_ident[EI_MAG0] != ELFMAG0) return -1;
    if (hdr->e_ident[EI_MAG1] != ELFMAG1) return -1;
    if (hdr->e_ident[EI_MAG2] != ELFMAG2) return -1;
    if (hdr->e_ident[EI_MAG3] != ELFMAG3) return -1;

    /* TODO: Should we verify anything else in the header? */

    return 0;
}


/* get_lib_extents
 *      Retrieves the base (*base) address where the ELF object should be
 *      mapped and its overall memory size (*total_sz).
 *
 * Args:
 *      fd: Opened file descriptor for the library
 *      name: The name of the library
 *      _hdr: Pointer to the header page of the library
 *      total_sz: Total size of the memory that should be allocated for
 *                this library
 *
 * Returns:
 *      -1 if there was an error while trying to get the lib extents.
 *         The possible reasons are:
 *             - Could not determine if the library was prelinked.
 *             - The library provided is not a valid ELF object
 *       0 if the library did not request a specific base offset (normal
 *         for non-prelinked libs)
 *     > 0 if the library requests a specific address to be mapped to.
 *         This indicates a pre-linked library.
 */
static unsigned
get_lib_extents(int fd, const char *name, void *__hdr, unsigned *total_sz)
{
    unsigned req_base;
    unsigned min_vaddr = 0xffffffff;
    unsigned max_vaddr = 0;
    unsigned char *_hdr = (unsigned char *)__hdr;
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)_hdr;
    Elf32_Phdr *phdr;
    int cnt;

    TRACE("[ %5d Computing extents for '%s'. ]\n", pid, name);
    if (verify_elf_object(_hdr, name) < 0) {
        ERROR("%5d - %s is not a valid ELF object\n", pid, name);
        return (unsigned)-1;
    }

    req_base = (unsigned) is_prelinked(fd, name);
    if (req_base == (unsigned)-1)
        return -1;
    else if (req_base != 0) {
        TRACE("[ %5d - Prelinked library '%s' requesting base @ 0x%08x ]\n",
              pid, name, req_base);
    } else {
        TRACE("[ %5d - Non-prelinked library '%s' found. ]\n", pid, name);
    }

    phdr = (Elf32_Phdr *)(_hdr + ehdr->e_phoff);

    /* find the min/max p_vaddrs from all the PT_LOAD segments so we can
     * get the range. */
    for (cnt = 0; cnt < ehdr->e_phnum; ++cnt, ++phdr) {
        if (phdr->p_type == PT_LOAD) {
            if ((phdr->p_vaddr + phdr->p_memsz) > max_vaddr)
                max_vaddr = phdr->p_vaddr + phdr->p_memsz;
            if (phdr->p_vaddr < min_vaddr)
                min_vaddr = phdr->p_vaddr;
        }
    }

    if ((min_vaddr == 0xffffffff) && (max_vaddr == 0)) {
        ERROR("%5d - No loadable segments found in %s.\n", pid, name);
        return (unsigned)-1;
    }

    /* truncate min_vaddr down to page boundary */
    min_vaddr &= ~PAGE_MASK;

    /* round max_vaddr up to the next page */
    max_vaddr = (max_vaddr + PAGE_SIZE - 1) & ~PAGE_MASK;

    *total_sz = (max_vaddr - min_vaddr);
    return (unsigned)req_base;
}

/* alloc_mem_region
 *
 *     This function reserves a chunk of memory to be used for mapping in
 *     the shared library. We reserve the entire memory region here, and
 *     then the rest of the linker will relocate the individual loadable
 *     segments into the correct locations within this memory range.
 *
 * Args:
 *     si->base: The requested base of the allocation. If 0, a sane one will be
 *               chosen in the range LIBBASE <= base < LIBLAST.
 *     si->size: The size of the allocation.
 *
 * Returns:
 *     -1 on failure, and 0 on success.  On success, si->base will contain
 *     the virtual address at which the library will be mapped.
 */

static int reserve_mem_region(soinfo *si)
{
    void *base = mmap((void *)si->base, si->size, PROT_READ | PROT_EXEC,
                      MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (base == MAP_FAILED) {
        ERROR("%5d can NOT map (%sprelinked) library '%s' at 0x%08x "
              "as requested, will try general pool: %d (%s)\n",
              pid, (si->base ? "" : "non-"), si->name, si->base,
              errno, strerror(errno));
        return -1;
    } else if (base != (void *)si->base) {
        ERROR("OOPS: %5d %sprelinked library '%s' mapped at 0x%08x, "
              "not at 0x%08x\n", pid, (si->base ? "" : "non-"),
              si->name, (unsigned)base, si->base);
        munmap(base, si->size);
        return -1;
    }
    return 0;
}

static int
alloc_mem_region(soinfo *si)
{
    if (si->base) {
        /* Attempt to mmap a prelinked library. */
        si->ba_index = -1;
        return reserve_mem_region(si);
    }

    /* This is not a prelinked library, so we attempt to allocate space
       for it from the buddy allocator, which manages the area between
       LIBBASE and LIBLAST.
    */
    si->ba_index = ba_allocate(si->size);
    if(si->ba_index >= 0) {
        si->base = ba_start_addr(si->ba_index);
        PRINT("%5d mapping library '%s' at %08x (index %d) " \
              "through buddy allocator.\n",
              pid, si->name, si->base, si->ba_index);
        if (reserve_mem_region(si) < 0) {
            ba_free(si->ba_index);
            si->ba_index = -1;
            si->base = 0;
            goto err;
        }
        return 0;
    }

err:
    ERROR("OOPS: %5d cannot map library '%s'. no vspace available.\n",
          pid, si->name);
    return -1;
}

#define MAYBE_MAP_FLAG(x,from,to)    (((x) & (from)) ? (to) : 0)
#define PFLAGS_TO_PROT(x)            (MAYBE_MAP_FLAG((x), PF_X, PROT_EXEC) | \
                                      MAYBE_MAP_FLAG((x), PF_R, PROT_READ) | \
                                      MAYBE_MAP_FLAG((x), PF_W, PROT_WRITE))
/* load_segments
 *
 *     This function loads all the loadable (PT_LOAD) segments into memory
 *     at their appropriate memory offsets off the base address.
 *
 * Args:
 *     fd: Open file descriptor to the library to load.
 *     header: Pointer to a header page that contains the ELF header.
 *             This is needed since we haven't mapped in the real file yet.
 *     si: ptr to soinfo struct describing the shared object.
 *
 * Returns:
 *     0 on success, -1 on failure.
 */
static int
load_segments(int fd, void *header, soinfo *si)
{
    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)header;
    Elf32_Phdr *phdr = (Elf32_Phdr *)((unsigned char *)header + ehdr->e_phoff);
    unsigned char *base = (unsigned char *)si->base;
    int cnt;
    unsigned len;
    unsigned char *tmp;
    unsigned char *pbase;
    unsigned char *extra_base;
    unsigned extra_len;
    unsigned total_sz = 0;

    si->wrprotect_start = 0xffffffff;
    si->wrprotect_end = 0;

    TRACE("[ %5d - Begin loading segments for '%s' @ 0x%08x ]\n",
          pid, si->name, (unsigned)si->base);
    /* Now go through all the PT_LOAD segments and map them into memory
     * at the appropriate locations. */
    for (cnt = 0; cnt < ehdr->e_phnum; ++cnt, ++phdr) {
        if (phdr->p_type == PT_LOAD) {
            DEBUG_DUMP_PHDR(phdr, "PT_LOAD", pid);
            /* we want to map in the segment on a page boundary */
            tmp = base + (phdr->p_vaddr & (~PAGE_MASK));
            /* add the # of bytes we masked off above to the total length. */
            len = phdr->p_filesz + (phdr->p_vaddr & PAGE_MASK);

            TRACE("[ %d - Trying to load segment from '%s' @ 0x%08x "
                  "(0x%08x). p_vaddr=0x%08x p_offset=0x%08x ]\n", pid, si->name,
                  (unsigned)tmp, len, phdr->p_vaddr, phdr->p_offset);
            pbase = mmap(tmp, len, PFLAGS_TO_PROT(phdr->p_flags),
                         MAP_PRIVATE | MAP_FIXED, fd,
                         phdr->p_offset & (~PAGE_MASK));
            if (pbase == MAP_FAILED) {
                ERROR("%d failed to map segment from '%s' @ 0x%08x (0x%08x). "
                      "p_vaddr=0x%08x p_offset=0x%08x\n", pid, si->name,
                      (unsigned)tmp, len, phdr->p_vaddr, phdr->p_offset);
                goto fail;
            }

            /* If 'len' didn't end on page boundary, and it's a writable
             * segment, zero-fill the rest. */
            if ((len & PAGE_MASK) && (phdr->p_flags & PF_W))
                memset((void *)(pbase + len), 0, PAGE_SIZE - (len & PAGE_MASK));

            /* Check to see if we need to extend the map for this segment to
             * cover the diff between filesz and memsz (i.e. for bss).
             *
             *  base           _+---------------------+  page boundary
             *                  .                     .
             *                  |                     |
             *                  .                     .
             *  pbase          _+---------------------+  page boundary
             *                  |                     |
             *                  .                     .
             *  base + p_vaddr _|                     |
             *                  . \          \        .
             *                  . | filesz   |        .
             *  pbase + len    _| /          |        |
             *     <0 pad>      .            .        .
             *  extra_base     _+------------|--------+  page boundary
             *               /  .            .        .
             *               |  .            .        .
             *               |  +------------|--------+  page boundary
             *  extra_len->  |  |            |        |
             *               |  .            | memsz  .
             *               |  .            |        .
             *               \ _|            /        |
             *                  .                     .
             *                  |                     |
             *                 _+---------------------+  page boundary
             */
            tmp = (unsigned char *)(((unsigned)pbase + len + PAGE_SIZE - 1) &
                                    (~PAGE_MASK));
            if (tmp < (base + phdr->p_vaddr + phdr->p_memsz)) {
                extra_len = base + phdr->p_vaddr + phdr->p_memsz - tmp;
                TRACE("[ %5d - Need to extend segment from '%s' @ 0x%08x "
                      "(0x%08x) ]\n", pid, si->name, (unsigned)tmp, extra_len);
                /* map in the extra page(s) as anonymous into the range.
                 * This is probably not necessary as we already mapped in
                 * the entire region previously, but we just want to be
                 * sure. This will also set the right flags on the region
                 * (though we can probably accomplish the same thing with
                 * mprotect).
                 */
                extra_base = mmap((void *)tmp, extra_len,
                                  PFLAGS_TO_PROT(phdr->p_flags),
                                  MAP_PRIVATE | MAP_FIXED | MAP_ANONYMOUS,
                                  -1, 0);
                if (extra_base == MAP_FAILED) {
                    ERROR("[ %5d - failed to extend segment from '%s' @ 0x%08x "
                          "(0x%08x) ]\n", pid, si->name, (unsigned)tmp,
                          extra_len);
                    goto fail;
                }
                /* TODO: Check if we need to memset-0 this region.
                 * Anonymous mappings are zero-filled copy-on-writes, so we
                 * shouldn't need to. */
                TRACE("[ %5d - Segment from '%s' extended @ 0x%08x "
                      "(0x%08x)\n", pid, si->name, (unsigned)extra_base,
                      extra_len);
            }
            /* set the len here to show the full extent of the segment we
             * just loaded, mostly for debugging */
            len = (((unsigned)base + phdr->p_vaddr + phdr->p_memsz +
                    PAGE_SIZE - 1) & (~PAGE_MASK)) - (unsigned)pbase;
            TRACE("[ %5d - Successfully loaded segment from '%s' @ 0x%08x "
                  "(0x%08x). p_vaddr=0x%08x p_offset=0x%08x\n", pid, si->name,
                  (unsigned)pbase, len, phdr->p_vaddr, phdr->p_offset);
            total_sz += len;
            /* Make the section writable just in case we'll have to write to
             * it during relocation (i.e. text segment). However, we will
             * remember what range of addresses should be write protected.
             *
             */
            if (!(phdr->p_flags & PF_W)) {
                if ((unsigned)pbase < si->wrprotect_start)
                    si->wrprotect_start = (unsigned)pbase;
                if (((unsigned)pbase + len) > si->wrprotect_end)
                    si->wrprotect_end = (unsigned)pbase + len;
                mprotect(pbase, len,
                         PFLAGS_TO_PROT(phdr->p_flags) | PROT_WRITE);
            }
        } else if (phdr->p_type == PT_DYNAMIC) {
            DEBUG_DUMP_PHDR(phdr, "PT_DYNAMIC", pid);
            /* this segment contains the dynamic linking information */
            si->dynamic = (unsigned *)(base + phdr->p_vaddr);
        } else {
#ifdef ANDROID_ARM_LINKER
            if (phdr->p_type == PT_ARM_EXIDX) {
                DEBUG_DUMP_PHDR(phdr, "PT_ARM_EXIDX", pid);
                /* exidx entries (used for stack unwinding) are 8 bytes each.
                 */
                si->ARM_exidx = (unsigned *)phdr->p_vaddr;
                si->ARM_exidx_count = phdr->p_memsz / 8;
            }
#endif
        }

    }

    /* Sanity check */
    if (total_sz > si->size) {
        ERROR("%5d - Total length (0x%08x) of mapped segments from '%s' is "
              "greater than what was allocated (0x%08x). THIS IS BAD!\n",
              pid, total_sz, si->name, si->size);
        goto fail;
    }

    TRACE("[ %5d - Finish loading segments for '%s' @ 0x%08x. "
          "Total memory footprint: 0x%08x bytes ]\n", pid, si->name,
          (unsigned)si->base, si->size);
    return 0;

fail:
    /* We can just blindly unmap the entire region even though some things
     * were mapped in originally with anonymous and others could have been
     * been mapped in from the file before we failed. The kernel will unmap
     * all the pages in the range, irrespective of how they got there.
     */
    munmap((void *)si->base, si->size);
    si->flags |= FLAG_ERROR;
    return -1;
}

/* TODO: Implement this to take care of the fact that Android ARM
 * ELF objects shove everything into a single loadable segment that has the
 * write bit set. wr_offset is then used to set non-(data|bss) pages to be
 * non-writable.
 */
#if 0
static unsigned
get_wr_offset(int fd, const char *name, Elf32_Ehdr *ehdr)
{
    Elf32_Shdr *shdr_start;
    Elf32_Shdr *shdr;
    int shdr_sz = ehdr->e_shnum * sizeof(Elf32_Shdr);
    int cnt;
    unsigned wr_offset = 0xffffffff;

    shdr_start = mmap(0, shdr_sz, PROT_READ, MAP_PRIVATE, fd,
                      ehdr->e_shoff & (~PAGE_MASK));
    if (shdr_start == MAP_FAILED) {
        WARN("%5d - Could not read section header info from '%s'. Will not "
             "not be able to determine write-protect offset.\n", pid, name);
        return (unsigned)-1;
    }

    for(cnt = 0, shdr = shdr_start; cnt < ehdr->e_shnum; ++cnt, ++shdr) {
        if ((shdr->sh_type != SHT_NULL) && (shdr->sh_flags & SHF_WRITE) &&
            (shdr->sh_addr < wr_offset)) {
            wr_offset = shdr->sh_addr;
        }
    }

    munmap(shdr_start, shdr_sz);
    return wr_offset;
}
#endif

static soinfo *
load_library(const char *name)
{
    int fd = open_library(name);
    int cnt;
    unsigned ext_sz;
    unsigned req_base;
    soinfo *si = NULL;
    Elf32_Ehdr *hdr;

    if(fd == -1)
        return NULL;

    /* We have to read the ELF header to figure out what to do with this image
     */
    if (lseek(fd, 0, SEEK_SET) < 0) {
        ERROR("lseek() failed!\n");
        goto fail;
    }

    if ((cnt = read(fd, &__header[0], PAGE_SIZE)) < 0) {
        ERROR("read() failed!\n");
        goto fail;
    }

    /* Parse the ELF header and get the size of the memory footprint for
     * the library */
    req_base = get_lib_extents(fd, name, &__header[0], &ext_sz);
    if (req_base == (unsigned)-1)
        goto fail;
    TRACE("[ %5d - '%s' (%s) wants base=0x%08x sz=0x%08x ]\n", pid, name,
          (req_base ? "prelinked" : "not pre-linked"), req_base, ext_sz);

    /* Now configure the soinfo struct where we'll store all of our data
     * for the ELF object. If the loading fails, we waste the entry, but
     * same thing would happen if we failed during linking. Configuring the
     * soinfo struct here is a lot more convenient.
     */
    si = alloc_info(name);
    if (si == NULL)
        goto fail;

    /* Carve out a chunk of memory where we will map in the individual
     * segments */
    si->base = req_base;
    si->size = ext_sz;
    si->flags = 0;
    si->entry = 0;
    si->dynamic = (unsigned *)-1;
    if (alloc_mem_region(si) < 0)
        goto fail;

    TRACE("[ %5d allocated memory for %s @ %p (0x%08x) ]\n",
          pid, name, (void *)si->base, (unsigned) ext_sz);

    /* Now actually load the library's segments into right places in memory */
    if (load_segments(fd, &__header[0], si) < 0) {
        if (si->ba_index >= 0) {
            ba_free(si->ba_index);
            si->ba_index = -1;
        }
        goto fail;
    }

    /* this might not be right. Technically, we don't even need this info
     * once we go through 'load_segments'. */
    hdr = (Elf32_Ehdr *)si->base;
    si->phdr = (Elf32_Phdr *)((unsigned char *)si->base + hdr->e_phoff);
    si->phnum = hdr->e_phnum;
    /**/

    close(fd);
    return si;

fail:
    if (si) free_info(si);
    close(fd);
    return NULL;
}

static soinfo *
init_library(soinfo *si)
{
    unsigned wr_offset = 0xffffffff;

    /* At this point we know that whatever is loaded @ base is a valid ELF
     * shared library whose segments are properly mapped in. */
    TRACE("[ %5d init_library base=0x%08x sz=0x%08x name='%s') ]\n",
          pid, si->base, si->size, si->name);

    if (si->base < LIBBASE || si->base >= LIBLAST)
        si->flags |= FLAG_PRELINKED;

    if(link_image(si, wr_offset)) {
            /* We failed to link.  However, we can only restore libbase
            ** if no additional libraries have moved it since we updated it.
            */
        munmap((void *)si->base, si->size);
        return NULL;
    }

    return si;
}

soinfo *find_library(const char *name)
{
    soinfo *si;

    for(si = solist; si != 0; si = si->next){
        if(!strcmp(name, si->name)) {
            if(si->flags & FLAG_ERROR) return 0;
            if(si->flags & FLAG_LINKED) return si;
            ERROR("OOPS: %5d recursive link to '%s'\n", pid, si->name);
            return 0;
        }
    }

    TRACE("[ %5d '%s' has not been loaded yet.  Locating...]\n", pid, name);
    si = load_library(name);
    if(si == NULL)
        return NULL;
    return init_library(si);
}

/* TODO: 
 *   notify gdb of unload 
 *   for non-prelinked libraries, find a way to decrement libbase
 */
static void call_destructors(soinfo *si);
unsigned unload_library(soinfo *si)
{
    unsigned *d;
    if (si->refcount == 1) {
        TRACE("%5d unloading '%s'\n", pid, si->name);
        call_destructors(si);

        for(d = si->dynamic; *d; d += 2) {
            if(d[0] == DT_NEEDED){
                TRACE("%5d %s needs to unload %s\n", pid,
                      si->name, si->strtab + d[1]);
                soinfo *lsi = find_library(si->strtab + d[1]);
                if(lsi)
                    unload_library(lsi);
                else
                    ERROR("%5d could not unload '%s'\n",
                          pid, si->strtab + d[1]);
            }
        }

        munmap((char *)si->base, si->size);
        if (si->ba_index >= 0) {
            PRINT("%5d releasing library '%s' address space at %08x "\
                  "through buddy allocator.\n",
                  pid, si->name, si->base);
            ba_free(si->ba_index);
        }
        notify_gdb_of_unload(si);
        free_info(si);
        si->refcount = 0;
    }
    else {
        si->refcount--;
        PRINT("%5d not unloading '%s', decrementing refcount to %d\n",
              pid, si->name, si->refcount);
    }
    return si->refcount;
}

/* TODO: don't use unsigned for addrs below. It works, but is not
 * ideal. They should probably be either uint32_t, Elf32_Addr, or unsigned
 * long.
 */
static int reloc_library(soinfo *si, Elf32_Rel *rel, unsigned count)
{
    Elf32_Sym *symtab = si->symtab;
    const char *strtab = si->strtab;
    Elf32_Sym *s;
    unsigned base;
    Elf32_Rel *start = rel;
    unsigned idx;

    for (idx = 0; idx < count; ++idx) {
        unsigned type = ELF32_R_TYPE(rel->r_info);
        unsigned sym = ELF32_R_SYM(rel->r_info);
        unsigned reloc = (unsigned)(rel->r_offset + si->base);
        unsigned sym_addr = 0;
        char *sym_name = NULL;

        DEBUG("%5d Processing '%s' relocation at index %d\n", pid,
              si->name, idx);
        if(sym != 0) {
            s = _do_lookup(si, strtab + symtab[sym].st_name, &base);
            if(s == 0) {
                ERROR("%5d cannot locate '%s'...\n",
				pid, strtab + symtab[sym].st_name);
                return -1;
            }
#if 0
            if((base == 0) && (si->base != 0)){
                    /* linking from libraries to main image is bad */
                ERROR("%5d cannot locate '%s'...\n", 
                       pid, strtab + symtab[sym].st_name);
                return -1;
            }
#endif
            if ((s->st_shndx == SHN_UNDEF) && (s->st_value != 0)) {
                ERROR("%5d In '%s', shndx=%d && value=0x%08x. We do not "
                      "handle this yet\n", pid, si->name, s->st_shndx,
                      s->st_value);
                return -1;
            }
            sym_addr = (unsigned)(s->st_value + base);
            sym_name = (char *)(strtab + symtab[sym].st_name);
            COUNT_RELOC(RELOC_SYMBOL);
        } else {
            s = 0;
        }

/* TODO: This is ugly. Split up the relocations by arch into
 * different files.
 */
        switch(type){
#if defined(ANDROID_ARM_LINKER)
        case R_ARM_JUMP_SLOT:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO JMP_SLOT %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) = sym_addr;
            break;
        case R_ARM_GLOB_DAT:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO GLOB_DAT %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) = sym_addr;
            break;
        case R_ARM_ABS32:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO ABS %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) += sym_addr;
            break;
#elif defined(ANDROID_X86_LINKER)
        case R_386_JUMP_SLOT:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO JMP_SLOT %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) = sym_addr;
            break;
        case R_386_GLOB_DAT:
            COUNT_RELOC(RELOC_ABSOLUTE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO GLOB_DAT %08x <- %08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned*)reloc) = sym_addr;
            break;
#endif /* ANDROID_*_LINKER */

#if defined(ANDROID_ARM_LINKER)
        case R_ARM_RELATIVE:
#elif defined(ANDROID_X86_LINKER)
        case R_386_RELATIVE:
#endif /* ANDROID_*_LINKER */
            COUNT_RELOC(RELOC_RELATIVE);
            MARK(rel->r_offset);
            if(sym){
                ERROR("%5d odd RELATIVE form...\n", pid);
                return -1;
            }
            TRACE_TYPE(RELO, "%5d RELO RELATIVE %08x <- +%08x\n", pid,
                       reloc, si->base);
            *((unsigned*)reloc) += si->base;
            break;

#if defined(ANDROID_X86_LINKER)
        case R_386_32:
            COUNT_RELOC(RELOC_RELATIVE);
            MARK(rel->r_offset);

            TRACE_TYPE(RELO, "%5d RELO R_386_32 %08x <- +%08x %s\n", pid,
                       reloc, sym_addr, sym_name);
            *((unsigned *)reloc) += (unsigned)sym_addr;
            break;

        case R_386_PC32:
            COUNT_RELOC(RELOC_RELATIVE);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO R_386_PC32 %08x <- "
                       "+%08x (%08x - %08x) %s\n", pid, reloc,
                       (sym_addr - reloc), sym_addr, reloc, sym_name);
            *((unsigned *)reloc) += (unsigned)(sym_addr - reloc);
            break;
#endif /* ANDROID_X86_LINKER */

#ifdef ANDROID_ARM_LINKER
        case R_ARM_COPY:
            COUNT_RELOC(RELOC_COPY);
            MARK(rel->r_offset);
            TRACE_TYPE(RELO, "%5d RELO %08x <- %d @ %08x %s\n", pid,
                       reloc, s->st_size, sym_addr, sym_name);
            memcpy((void*)reloc, (void*)sym_addr, s->st_size);
            break;
        case R_ARM_NONE:
            break;
#endif /* ANDROID_ARM_LINKER */

        default:
            ERROR("%5d unknown reloc type %d @ %p (%d)\n",
                  pid, type, rel, (int) (rel - start));
            return -1;
        }
        rel++;
    }
    return 0;
}

static void call_array(unsigned *ctor, int count)
{
    int n;
    for(n = count; n > 0; n--){
        TRACE("[ %5d Looking at ctor *0x%08x == 0x%08x ]\n", pid,
              (unsigned)ctor, (unsigned)*ctor);
        void (*func)() = (void (*)()) *ctor++;
        if(((int) func == 0) || ((int) func == -1)) continue;
        TRACE("[ %5d Calling func @ 0x%08x ]\n", pid, (unsigned)func);
        func();
    }
}

static void call_constructors(soinfo *si)
{
    /* TODO: THE ORIGINAL CODE SEEMED TO CALL THE INIT FUNCS IN THE WRONG ORDER.
     *       Old order: init, init_array, preinit_array..
     *       Correct order: preinit_array, init, init_array.
     *       Verify WHY.
     */

    if (si->flags & FLAG_EXE) {
        TRACE("[ %5d Calling preinit_array @ 0x%08x [%d] for '%s' ]\n",
              pid, (unsigned)si->preinit_array, si->preinit_array_count,
              si->name);
        call_array(si->preinit_array, si->preinit_array_count);
        TRACE("[ %5d Done calling preinit_array for '%s' ]\n", pid, si->name);
    } else {
        if (si->preinit_array) {
            ERROR("%5d Shared library '%s' has a preinit_array table @ 0x%08x."
                  " This is INVALID.\n", pid, si->name,
                  (unsigned)si->preinit_array);
        }
    }

    // If we have an init section, then we should call it now, to make sure
    // that all the funcs in the .ctors section get run.
    // Note: For ARM, we shouldn't have a .ctor section (should be empty)
    // when we have an (pre)init_array section, but let's be compatible with
    // old (non-eabi) binaries and try the _init (DT_INIT) anyway.
    if (si->init_func) {
        TRACE("[ %5d Calling init_func @ 0x%08x for '%s' ]\n", pid,
              (unsigned)si->init_func, si->name);
        si->init_func();
        TRACE("[ %5d Done calling init_func for '%s' ]\n", pid, si->name);
    }

    if (si->init_array) {
        TRACE("[ %5d Calling init_array @ 0x%08x [%d] for '%s' ]\n", pid,
              (unsigned)si->init_array, si->init_array_count, si->name);
        call_array(si->init_array, si->init_array_count);
        TRACE("[ %5d Done calling init_array for '%s' ]\n", pid, si->name);
    }
}

static void call_destructors(soinfo *si)
{
    if (si->fini_array) {
        TRACE("[ %5d Calling fini_array @ 0x%08x [%d] for '%s' ]\n", pid,
              (unsigned)si->fini_array, si->fini_array_count, si->name);
        call_array(si->fini_array, si->fini_array_count);
        TRACE("[ %5d Done calling fini_array for '%s' ]\n", pid, si->name);
    }

    // If we have an fini section, then we should call it now, to make sure
    // that all the funcs in the .dtors section get run.
    // Note: For ARM, we shouldn't have a .dtor section (should be empty)
    // when we have an fini_array section, but let's be compatible with
    // old (non-eabi) binaries and try the _fini (DT_FINI) anyway.
    if (si->fini_func) {
        TRACE("[ %5d Calling fini_func @ 0x%08x for '%s' ]\n", pid,
              (unsigned)si->fini_func, si->name);
        si->fini_func();
        TRACE("[ %5d Done calling fini_func for '%s' ]\n", pid, si->name);
    }
}

/* Force any of the closed stdin, stdout and stderr to be associated with
   /dev/null. */
static int nullify_closed_stdio (void)
{
    int dev_null, i, status;
    int return_value = 0;

    dev_null = open("/dev/null", O_RDWR);
    if (dev_null < 0) {
        ERROR("Cannot open /dev/null.\n");
        return -1;
    }
    TRACE("[ %5d Opened /dev/null file-descriptor=%d]\n", pid, dev_null);

    /* If any of the stdio file descriptors is valid and not associated
       with /dev/null, dup /dev/null to it.  */
    for (i = 0; i < 3; i++) {
        /* If it is /dev/null already, we are done. */
        if (i == dev_null)
            continue;

        TRACE("[ %5d Nullifying stdio file descriptor %d]\n", pid, i);
        /* The man page of fcntl does not say that fcntl(..,F_GETFL)
           can be interrupted but we do this just to be safe. */
        do {
          status = fcntl(i, F_GETFL);
        } while (status < 0 && errno == EINTR);

        /* If file is openned, we are good. */
        if (status >= 0)
          continue;

        /* The only error we allow is that the file descriptor does not
           exist, in which case we dup /dev/null to it. */
        if (errno != EBADF) {
            ERROR("nullify_stdio: unhandled error %s\n", strerror(errno));
            return_value = -1;
            continue;
        }

        /* Try dupping /dev/null to this stdio file descriptor and
           repeat if there is a signal.  Note that any errors in closing
           the stdio descriptor are lost.  */
        do {
            status = dup2(dev_null, i);
        } while (status < 0 && errno == EINTR);
        
        if (status < 0) {
            ERROR("nullify_stdio: dup2 error %s\n", strerror(errno));
            return_value = -1;
            continue;
        }
    }

    /* If /dev/null is not one of the stdio file descriptors, close it. */
    if (dev_null > 2) {
        TRACE("[ %5d Closing /dev/null file-descriptor=%d]\n", pid, dev_null);
	do {
            status = close(dev_null);
        } while (status < 0 && errno == EINTR);

        if (status < 0) {
            ERROR("nullify_stdio: close error %s\n", strerror(errno));
            return_value = -1;
        }
    }

    return return_value;
}

static int link_image(soinfo *si, unsigned wr_offset)
{
    unsigned *d;
    Elf32_Phdr *phdr = si->phdr;
    int phnum = si->phnum;

    INFO("[ %5d linking %s ]\n", pid, si->name);
    DEBUG("%5d si->base = 0x%08x si->flags = 0x%08x\n", pid,
          si->base, si->flags);

    if (si->flags & FLAG_EXE) {
        /* Locate the needed program segments (DYNAMIC/ARM_EXIDX) for
         * linkage info if this is the executable. If this was a
         * dynamic lib, that would have been done at load time.
         *
         * TODO: It's unfortunate that small pieces of this are
         * repeated from the load_library routine. Refactor this just
         * slightly to reuse these bits.
         */
        si->size = 0;
        for(; phnum > 0; --phnum, ++phdr) {
#ifdef ANDROID_ARM_LINKER
            if(phdr->p_type == PT_ARM_EXIDX) {
                /* exidx entries (used for stack unwinding) are 8 bytes each.
                 */
                si->ARM_exidx = (unsigned *)phdr->p_vaddr;
                si->ARM_exidx_count = phdr->p_memsz / 8;
            }
#endif
            if (phdr->p_type == PT_LOAD) {
                /* For the executable, we use the si->size field only in 
                   dl_unwind_find_exidx(), so the meaning of si->size 
                   is not the size of the executable; it is the last 
                   virtual address of the loadable part of the executable;
                   since si->base == 0 for an executable, we use the
                   range [0, si->size) to determine whether a PC value 
                   falls within the executable section.  Of course, if
                   a value is below phdr->p_vaddr, it's not in the 
                   executable section, but a) we shouldn't be asking for
                   such a value anyway, and b) if we have to provide
                   an EXIDX for such a value, then the executable's
                   EXIDX is probably the better choice.
                */
                DEBUG_DUMP_PHDR(phdr, "PT_LOAD", pid);
                if (phdr->p_vaddr + phdr->p_memsz > si->size)
                    si->size = phdr->p_vaddr + phdr->p_memsz;
                /* try to remember what range of addresses should be write
                 * protected */
                if (!(phdr->p_flags & PF_W)) {
                    unsigned _end;

                    if (phdr->p_vaddr < si->wrprotect_start)
                        si->wrprotect_start = phdr->p_vaddr;
                    _end = (((phdr->p_vaddr + phdr->p_memsz + PAGE_SIZE - 1) &
                             (~PAGE_MASK)));
                    if (_end > si->wrprotect_end)
                        si->wrprotect_end = _end;
                }
            } else if (phdr->p_type == PT_DYNAMIC) {
                if (si->dynamic != (unsigned *)-1) {
                    ERROR("%5d multiple PT_DYNAMIC segments found in '%s'. "
                          "Segment at 0x%08x, previously one found at 0x%08x\n",
                          pid, si->name, si->base + phdr->p_vaddr,
                          (unsigned)si->dynamic);
                    goto fail;
                }
                DEBUG_DUMP_PHDR(phdr, "PT_DYNAMIC", pid);
                si->dynamic = (unsigned *) (si->base + phdr->p_vaddr);
            }
        }
    }

    if (si->dynamic == (unsigned *)-1) {
        ERROR("%5d missing PT_DYNAMIC?!\n", pid);
        goto fail;
    }

    DEBUG("%5d dynamic = %p\n", pid, si->dynamic);

    /* extract useful information from dynamic section */
    for(d = si->dynamic; *d; d++){
        DEBUG("%5d d = %p, d[0] = 0x%08x d[1] = 0x%08x\n", pid, d, d[0], d[1]);
        switch(*d++){
        case DT_HASH:
            si->nbucket = ((unsigned *) (si->base + *d))[0];
            si->nchain = ((unsigned *) (si->base + *d))[1];
            si->bucket = (unsigned *) (si->base + *d + 8);
            si->chain = (unsigned *) (si->base + *d + 8 + si->nbucket * 4);
            break;
        case DT_STRTAB:
            si->strtab = (const char *) (si->base + *d);
            break;
        case DT_SYMTAB:
            si->symtab = (Elf32_Sym *) (si->base + *d);
            break;
        case DT_PLTREL:
            if(*d != DT_REL) {
                ERROR("DT_RELA not supported\n");
                goto fail;
            }
            break;
        case DT_JMPREL:
            si->plt_rel = (Elf32_Rel*) (si->base + *d);
            break;
        case DT_PLTRELSZ:
            si->plt_rel_count = *d / 8;
            break;
        case DT_REL:
            si->rel = (Elf32_Rel*) (si->base + *d);
            break;
        case DT_RELSZ:
            si->rel_count = *d / 8;
            break;
        case DT_PLTGOT:
            /* Save this in case we decide to do lazy binding. We don't yet. */
            si->plt_got = (unsigned *)(si->base + *d);
            break;
        case DT_DEBUG:
            // Set the DT_DEBUG entry to the addres of _r_debug for GDB
            *d = (int) &_r_debug;
            break;
        case DT_RELA:
            ERROR("%5d DT_RELA not supported\n", pid);
            goto fail;
        case DT_INIT:
            si->init_func = (void (*)(void))(si->base + *d);
            DEBUG("%5d %s constructors (init func) found at %p\n",
                  pid, si->name, si->init_func);
            break;
        case DT_FINI:
            si->fini_func = (void (*)(void))(si->base + *d);
            DEBUG("%5d %s destructors (fini func) found at %p\n",
                  pid, si->name, si->fini_func);
            break;
        case DT_INIT_ARRAY:
            si->init_array = (unsigned *)(si->base + *d);
            DEBUG("%5d %s constructors (init_array) found at %p\n",
                  pid, si->name, si->init_array);
            break;
        case DT_INIT_ARRAYSZ:
            si->init_array_count = ((unsigned)*d) / sizeof(Elf32_Addr);
            break;
        case DT_FINI_ARRAY:
            si->fini_array = (unsigned *)(si->base + *d);
            DEBUG("%5d %s destructors (fini_array) found at %p\n",
                  pid, si->name, si->fini_array);
            break;
        case DT_FINI_ARRAYSZ:
            si->fini_array_count = ((unsigned)*d) / sizeof(Elf32_Addr);
            break;
        case DT_PREINIT_ARRAY:
            si->preinit_array = (unsigned *)(si->base + *d);
            DEBUG("%5d %s constructors (preinit_array) found at %p\n",
                  pid, si->name, si->preinit_array);
            break;
        case DT_PREINIT_ARRAYSZ:
            si->preinit_array_count = ((unsigned)*d) / sizeof(Elf32_Addr);
            break;
        case DT_TEXTREL:
            /* TODO: make use of this. */
            /* this means that we might have to write into where the text
             * segment was loaded during relocation... Do something with
             * it.
             */
            DEBUG("%5d Text segment should be writable during relocation.\n",
                  pid);
            break;
        }
    }

    DEBUG("%5d si->base = 0x%08x, si->strtab = %p, si->symtab = %p\n", 
           pid, si->base, si->strtab, si->symtab);

    if((si->strtab == 0) || (si->symtab == 0)) {
        ERROR("%5d missing essential tables\n", pid);
        goto fail;
    }

    for(d = si->dynamic; *d; d += 2) {
        if(d[0] == DT_NEEDED){
            DEBUG("%5d %s needs %s\n", pid, si->name, si->strtab + d[1]);
            soinfo *lsi = find_library(si->strtab + d[1]);            
            if(lsi == 0) {
                ERROR("%5d could not load '%s'\n", pid, si->strtab + d[1]);
                goto fail;
            }
            lsi->refcount++;
        }
    }

    if(si->plt_rel) {
        DEBUG("[ %5d relocating %s plt ]\n", pid, si->name );
        if(reloc_library(si, si->plt_rel, si->plt_rel_count))
            goto fail;
    }
    if(si->rel) {
        DEBUG("[ %5d relocating %s ]\n", pid, si->name );
        if(reloc_library(si, si->rel, si->rel_count))
            goto fail;
    }

    si->flags |= FLAG_LINKED;
    DEBUG("[ %5d finished linking %s ]\n", pid, si->name);

#if 0
    /* This is the way that the old dynamic linker did protection of
     * non-writable areas. It would scan section headers and find where
     * .text ended (rather where .data/.bss began) and assume that this is
     * the upper range of the non-writable area. This is too coarse,
     * and is kept here for reference until we fully move away from single
     * segment elf objects. See the code in get_wr_offset (also #if'd 0)
     * that made this possible.
     */
    if(wr_offset < 0xffffffff){
        mprotect((void*) si->base, wr_offset, PROT_READ | PROT_EXEC);
    }
#else
    /* TODO: Verify that this does the right thing in all cases, as it
     * presently probably does not. It is possible that an ELF image will
     * come with multiple read-only segments. What we ought to do is scan
     * the program headers again and mprotect all the read-only segments.
     * To prevent re-scanning the program header, we would have to build a
     * list of loadable segments in si, and then scan that instead. */
    if (si->wrprotect_start != 0xffffffff && si->wrprotect_end != 0) {
        mprotect((void *)si->wrprotect_start,
                 si->wrprotect_end - si->wrprotect_start,
                 PROT_READ | PROT_EXEC);
    }
#endif

    /* If this is a SET?ID program, dup /dev/null to opened stdin,
       stdout and stderr to close a security hole described in:

    ftp://ftp.freebsd.org/pub/FreeBSD/CERT/advisories/FreeBSD-SA-02:23.stdio.asc

     */
    if (getuid() != geteuid() || getgid() != getegid())
        nullify_closed_stdio ();
    call_constructors(si);
    notify_gdb_of_load(si);
    return 0;

fail:
    ERROR("failed to link %s\n", si->name);
    si->flags |= FLAG_ERROR;
    return -1;
}

int main(int argc, char **argv)
{
    return 0;
}

#define ANDROID_TLS_SLOTS  BIONIC_TLS_SLOTS

static void * __tls_area[ANDROID_TLS_SLOTS];

unsigned __linker_init(unsigned **elfdata)
{
    static soinfo linker_soinfo;

    int argc = (int) *elfdata;
    char **argv = (char**) (elfdata + 1);
    unsigned *vecs = (unsigned*) (argv + argc + 1);
    soinfo *si;
    struct link_map * map;

    pid = getpid();

#if TIMING
    struct timeval t0, t1;
    gettimeofday(&t0, 0);
#endif

    __set_tls(__tls_area);
    ((unsigned *)__get_tls())[TLS_SLOT_THREAD_ID] = gettid();

    debugger_init();

        /* skip past the environment */
    while(vecs[0] != 0) {
        if(!strncmp((char*) vecs[0], "DEBUG=", 6)) {
            debug_verbosity = atoi(((char*) vecs[0]) + 6);
        }
        vecs++;
    }
    vecs++;

    INFO("[ android linker & debugger ]\n");
    DEBUG("%5d elfdata @ 0x%08x\n", pid, (unsigned)elfdata);

    si = alloc_info(argv[0]);
    if(si == 0) {
        exit(-1);
    }

        /* bootstrap the link map, the main exe always needs to be first */
    si->flags |= FLAG_EXE;
    map = &(si->linkmap);

    map->l_addr = 0;
    map->l_name = argv[0];
    map->l_prev = NULL;
    map->l_next = NULL;

    _r_debug.r_map = map;
    r_debug_tail = map;

        /* gdb expects the linker to be in the debug shared object list,
         * and we need to make sure that the reported load address is zero.
         * Without this, gdb gets the wrong idea of where rtld_db_dlactivity()
         * is.  Don't use alloc_info(), because the linker shouldn't
         * be on the soinfo list.
         */
    strcpy((char*) linker_soinfo.name, "/system/bin/linker");
    linker_soinfo.flags = 0;
    linker_soinfo.base = 0;     // This is the important part; must be zero.
    insert_soinfo_into_debug_map(&linker_soinfo);

        /* extract information passed from the kernel */
    while(vecs[0] != 0){
        switch(vecs[0]){
        case AT_PHDR:
            si->phdr = (Elf32_Phdr*) vecs[1];
            break;
        case AT_PHNUM:
            si->phnum = (int) vecs[1];
            break;
        case AT_ENTRY:
            si->entry = vecs[1];
            break;
        }
        vecs += 2;
    }

    ba_init();

    si->base = 0;
    si->dynamic = (unsigned *)-1;
    si->wrprotect_start = 0xffffffff;
    si->wrprotect_end = 0;

    if(link_image(si, 0)){
        ERROR("CANNOT LINK EXECUTABLE '%s'\n", argv[0]);
        exit(-1);
    }

#if TIMING
    gettimeofday(&t1,NULL);
    PRINT("LINKER TIME: %s: %d microseconds\n", argv[0], (int) (
               (((long long)t1.tv_sec * 1000000LL) + (long long)t1.tv_usec) -
               (((long long)t0.tv_sec * 1000000LL) + (long long)t0.tv_usec)
               ));
#endif
#if STATS
    PRINT("RELO STATS: %s: %d abs, %d rel, %d copy, %d symbol\n", argv[0],
           linker_stats.reloc[RELOC_ABSOLUTE],
           linker_stats.reloc[RELOC_RELATIVE],
           linker_stats.reloc[RELOC_COPY],
           linker_stats.reloc[RELOC_SYMBOL]);
#endif
#if COUNT_PAGES
    {
        unsigned n;
        unsigned i;
        unsigned count = 0;
        for(n = 0; n < 4096; n++){
            if(bitmask[n]){
                unsigned x = bitmask[n];
                for(i = 0; i < 8; i++){
                    if(x & 1) count++;
                    x >>= 1;
                }
            }
        }
        PRINT("PAGES MODIFIED: %s: %d (%dKB)\n", argv[0], count, count * 4);
    }
#endif

#if TIMING || STATS || COUNT_PAGES
    fflush(stdout);
#endif

    TRACE("[ %5d Ready to execute '%s' @ 0x%08x ]\n", pid, si->name,
          si->entry);
    return si->entry;
}
