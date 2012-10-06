/*
 * Copyright (C) 2012 The Android Open Source Project
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

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <fcntl.h>

#include <libaprof.h>

#include <sys/gmon.h>
#include <sys/mman.h>
#include <asm/sigcontext.h>
#include <asm/ucontext.h>

#include "linker_environ.h"
#include "linker.h"
#include "linker_debug.h"

#define likely(expr)   __builtin_expect (expr, 1)
#define unlikely(expr) __builtin_expect (expr, 0)

static int aprof_inited;
static int aprof_jni_counter;

static void aprof_init(void *module_address);
static void aprof_fini(void *module_address);
static void aprof_write_header(int fd);
static void aprof_write();
static void aprof_write_jni(soinfo *si);
static void aprof_write_all_jni();
static void aprof_write_bins(int fd, soinfo *si);
static void aprof_write_tos(int fd, soinfo *si);
static uint32_t aprof_count_edges(soinfo *si);
static int aprof_alloc_tos(soinfo *si);
static void aprof_alloc_bin(soinfo *si);
static void aprof_mcount(uintptr_t frompc, uintptr_t selfpc);
static void aprof_control(int control);

static const uint32_t bin_mapped_size = 4;

static soinfo *find_soinfo_by_addr(unsigned addr);

#define ANDROID_LIBAPROF_RUNTIME_STRTAB \
                      "aprof_mcount\0aprof_init\0aprof_fini\0aprof_control\0"

static Elf32_Sym libaprof_symtab[] = {
      // total length of libaprof_runtime_info.strtab, including trailing 0
      // This is actually the the STH_UNDEF entry. Technically, it's
      // supposed to have st_name == 0, but instead, it points to an index
      // in the strtab with a \0 to make iterating through the symtab easier.
    { st_name: sizeof(ANDROID_LIBAPROF_RUNTIME_STRTAB) - 1,
    },
    { st_name: 0,   // starting index of the name in libdl_info.strtab
      st_value: (Elf32_Addr) &aprof_mcount,
      st_info: STB_GLOBAL << 4,
      st_shndx: 1,
    },
    { st_name: 13,
      st_value: (Elf32_Addr) &aprof_init,
      st_info: STB_GLOBAL << 4,
      st_shndx: 1,
    },
    { st_name: 24,
      st_value: (Elf32_Addr) &aprof_fini,
      st_info: STB_GLOBAL << 4,
      st_shndx: 1,
    },
    { st_name: 35,
      st_value: (Elf32_Addr) &aprof_control,
      st_info: STB_GLOBAL << 4,
      st_shndx: 1,
    },
};

static unsigned libaprof_buckets[1] = { 1 };
static unsigned libaprof_chains[5] = { 0, 2, 3, 4, 0 };
soinfo libaprof_runtime_info = {
    name: "libaprof_runtime.so",
    flags: FLAG_LINKED,

    strtab: ANDROID_LIBAPROF_RUNTIME_STRTAB,
    symtab: libaprof_symtab,
    refcount: 1, /* Prevent to unload */

    nbucket: 1,
    nchain: 5,
    bucket: libaprof_buckets,
    chain: libaprof_chains,

    next: &libdl_info,
};

static void aprof_control(int control)
{
    /* Do nothing if non-JNI mode*/
    if (!aprof_jni_counter) return;
    if (control == APROF_OFF) {
        DEBUG("aprof : APROF_OFF\n");
        setitimer(ITIMER_PROF, NULL, NULL);
        aprof_write_all_jni();
    } else {
        DEBUG("aprof : APROF_ON\n");
        struct itimerval timer;
        timer.it_value.tv_sec = 0;
        timer.it_value.tv_usec = 1000000 / (PROFRATE);
        timer.it_interval = timer.it_value;
        if (setitimer(ITIMER_PROF, &timer, NULL) < 0) {
            ERROR("aprof : error on setitimer");
            return;
        }
    }
}

static void aprof_mcount(uintptr_t frompc, uintptr_t selfpc)
{
    soinfo *si;
    unsigned short *frompcindex;
    struct tostruct *top, *prevtop;
    unsigned long toindex;
    unsigned long frompc_offset;
    if (!aprof_inited) return;
    si = find_soinfo_by_addr(frompc);
    if (si == NULL) {
        ERROR("aprof : unkwnon frompc %x", frompc);
        return;
    }
    if (si->froms == NULL) {
        if (aprof_alloc_tos (si) == -1)
            return;
    }
    frompc_offset = frompc -si->base;
    frompcindex =
        &si->froms[frompc_offset / (HASHFRACTION * sizeof(*si->froms))];
    toindex = *frompcindex;
    if (toindex == 0) {
        /*
         *  first time traversing this arc
         */
        toindex = ++si->tos[0].link;
        if (toindex >= si->tolimit)
            /* halt further profiling */
            goto overflow;

        *frompcindex = toindex;
        top = &si->tos[toindex];
        top->selfpc = selfpc;
        top->count = 1;
        top->link = 0;
        return;
    }

    top = &si->tos[toindex];
    if (top->selfpc == selfpc) {
        /*
         * arc at front of chain; usual case.
         */
        top->count++;
        return;
    }

    /*
     * have to go looking down chain for it.
     * top points to what we are looking at,
     * prevtop points to previous top.
     * we know it is not at the head of the chain.
     */
    for (; /* goto done */; ) {
        if (top->link == 0) {
            /*
             * top is end of the chain and none of the chain
             * had top->selfpc == selfpc.
             * so we allocate a new tostruct
             * and link it to the head of the chain.
             */
            toindex = ++si->tos[0].link;
            if (toindex >= si->tolimit)
                goto overflow;

            top = &si->tos[toindex];
            top->selfpc = selfpc;
            top->count = 1;
            top->link = *frompcindex;
            *frompcindex = toindex;
            return;
        }
        /*
         * otherwise, check the next arc on the chain.
         */
        prevtop = top;
        top = &si->tos[top->link];
        if (top->selfpc == selfpc) {
            /*
             * there it is.
             * increment its count
             * move it to the head of the chain.
             */
            top->count++;
            toindex = prevtop->link;
            prevtop->link = top->link;
            top->link = *frompcindex;
            *frompcindex = toindex;
        }
    }
    return;

overflow:
    ERROR("aprof : overflow @ %s\n", si->name);
    return;
}

static int aprof_alloc_tos(soinfo *si)
{
    unsigned textsize = si->size;
    void *addr;
    DEBUG("aproc : alloc tos for %s\n", si->name);
    si->fromssize = textsize / HASHFRACTION;
    si->tolimit = textsize * ARCDENSITY / 100;
    if (si->tolimit < MINARCS)
        si->tolimit = MINARCS;
    else if (si->tolimit > MAXARCS)
        si->tolimit = MAXARCS;
    si->tossize = si->tolimit * sizeof(struct tostruct);

    addr = mmap((void *)0, si->fromssize,  PROT_READ|PROT_WRITE,
        MAP_ANON|MAP_PRIVATE, -1, (off_t)0);
    if (addr == MAP_FAILED)
        goto mapfailed;
    si->froms = addr;

    addr = mmap((void *)0, si->tossize,  PROT_READ|PROT_WRITE,
        MAP_ANON|MAP_PRIVATE, -1, (off_t)0);
    if (addr == MAP_FAILED)
        goto mapfailed;
    si->tos = addr;
    si->tos[0].link = 0;

    return 0;

mapfailed:
    if (si->froms != NULL) {
        munmap(si->froms, si->fromssize);
        si->froms = NULL;
    }
    if (si->tos != NULL) {
        munmap(si->tos, si->tossize);
        si->tos = NULL;
    }
    ERROR("aprof : Out of memory !");
    return -1;
}

static void aprof_alloc_bin(soinfo *si)
{
    unsigned textsize = si->size;
    void *addr;
    si->bin_size = textsize / bin_mapped_size;
    addr = mmap((void *)0, si->bin_size * sizeof(uint16_t),
                PROT_READ|PROT_WRITE, MAP_ANON|MAP_PRIVATE, -1, (off_t)0);

    if (addr == MAP_FAILED) {
        ERROR("aprof : out of memory\n");
        return;
    }

    si->bins = addr;
}

static void aprof_sampler (int sig __attribute__((unused)),
                           siginfo_t *sii __attribute__((unused)),
                           void *context)
{
    struct ucontext *uc = (struct ucontext*)context;
#if defined(ANDROID_ARM_LINKER)
    uintptr_t pc = uc->uc_mcontext.arm_pc;
#else
#error "Aprof not supported"
#endif

    /*
     * Exclude the time for profiling
     */
    if (pc >= linker_soinfo.base &&
        pc <= linker_soinfo.base + linker_soinfo.size) {
        return;
    }
    soinfo *si = find_soinfo_by_addr(pc);
    if (unlikely(si == NULL)) {
        ERROR("aprof : unknown pc @ %x\n", pc);
        ERROR("aprof : linker_soinfo.base :%x linker_soinfo.size %x\n", linker_soinfo.base, linker_soinfo.size);
        return;
    }
    /*
     * Skip non-profiling shared libraries under aprof jni mode
     */
    if (aprof_jni_counter &&
        !si->aprof_jni_mode) {
        return;
    }
    if (unlikely(si->bins == NULL)) {
        aprof_alloc_bin(si);
    }
    size_t i = ((uintptr_t)pc - (uintptr_t)si->base) / bin_mapped_size;
    if (i < si->bin_size) {
        si->bins[i]++;
    } else {
        ERROR("aprof : out of range %u (bin size : %u)\n", i, si->bin_size);
    }
}

static void aprof_init(void *module_address)
{
    struct sigaction sigact;
    struct itimerval timer;
    if (module_address != NULL) {
        if (aprof_inited && aprof_jni_counter == 0) {
            ERROR("aprof : aprof already start with non-jni mode!");
            return;
        }

        soinfo *si = find_soinfo_by_addr((unsigned)module_address);
        aprof_jni_counter++;
        si->aprof_jni_mode = 1;
        if (aprof_jni_counter != 1) {
            return;
        }
    }
    aprof_inited = 1;
    DEBUG("aprof : init\n");
    sigact.sa_sigaction =  &aprof_sampler;
    sigact.sa_flags = SA_RESTART | SA_SIGINFO;
    sigfillset(&sigact.sa_mask);

    if (sigaction(SIGPROF, &sigact, NULL) < 0) {
        ERROR("aprof : error on set sigaction");
        return;
    }

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 1000000 / (PROFRATE);
    timer.it_interval = timer.it_value;
    if (setitimer(ITIMER_PROF, &timer, NULL) < 0) {
        ERROR("aprof : error on setitimer");
        return;
    }
}

static void aprof_fini(void *module_address)
{
    DEBUG("aprof : fini\n");
    soinfo *si = NULL;
    if (module_address != NULL) {
        si = find_soinfo_by_addr((unsigned)module_address);
        if (si == NULL) {
            ERROR("aprof : error on aprof_fini, unknown module address %x\n",
                  (unsigned)module_address);
            return;
        }
        --aprof_jni_counter;
        /* Release memory */
        if (si->froms != NULL) {
            munmap(si->froms, si->fromssize);
            si->froms = NULL;
        }
        if (si->tos != NULL) {
            munmap(si->tos, si->tossize);
            si->tos = NULL;
        }
    }
    if (!aprof_jni_counter) {
        /* Trun off aprof_sampler */
        sigaction(SIGPROF, NULL, NULL);
        setitimer(ITIMER_PROF, NULL, NULL);
    }
    if (module_address != NULL) {
        aprof_write_jni(si);
    } else {
        aprof_write();
    }
}

static void aprof_write_header(int fd)
{
    const char aprof_tag[] = APROF_TAG;
    uint32_t aprof_version = APROF_VERSION;
    uint32_t sample_rate = PROFRATE;
    uint32_t pointer_size = sizeof(uintptr_t);
    /* Write aprof profiling file header. */
    write(fd, aprof_tag, APROF_TAG_LENGTH);
    write(fd, &aprof_version, sizeof(uint32_t));
    write(fd, &sample_rate, sizeof(uint32_t));
    write(fd, &pointer_size, sizeof(uint32_t));
}

static const char *aprof_basename(const char *begin, const char *end)
{
    const char *itr = end-1;
    while (itr != begin) {
        if (*itr == '/') {
            return itr+1;
        } else {
            itr--;
        }
    }
    return itr;
}

static int aprof_open_file(const char *filename)
{
    int fd;
    const char path[] = "/mnt/sdcard/";
    const char suffix[] = ".aprof";
    char output[PATH_MAX] = "";
    size_t max_len, env_len;
    const char* env;
    env = linker_env_get("EXTERNAL_STORAGE");
    if (env) {
        env_len = strlen(env);
        max_len = PATH_MAX - (sizeof(suffix) + env_len +
                              1 /* for '/' */ + 1 /* for '\0' */);
        strcpy(output, env);
        strcat(output, "/");
    } else {
        max_len = PATH_MAX - (sizeof(suffix) + sizeof(path) + 1);
        strcpy(output, path);
    }
    strncat(output, filename, max_len);
    strcat(output, suffix);

    fd = open(output, O_CREAT|O_TRUNC|O_WRONLY, 0664);
    DEBUG("aprof : open %s fd = %d\n", output, fd);
    return fd;
}

static void aprof_write_jni(soinfo *si)
{
    int fd = aprof_open_file(si->name);
    uint32_t header_type;
    uint32_t num_edges = aprof_count_edges(si);

    if (fd < 0) {
      ERROR("aprof : open file fail for aprof_write_jni!\n");
      return;
    }

    aprof_write_header(fd);

    /* Write histogram records */
    header_type = APROF_HISTOGRAM_HEADER;
    write(fd, &header_type, sizeof(uint32_t));
    DEBUG("aprof : write %s ...\n", si->name);
    aprof_write_bins(fd, si);

    /* Write call graph records */
    header_type = APROF_CALL_GRAPH_HEADER;
    write(fd, &header_type, sizeof(uint32_t));
    write(fd, &num_edges, sizeof(uint32_t));
    aprof_write_tos(fd, si);
    close(fd);
}

static void aprof_write_all_jni(){
    soinfo *si;
    for (si = solist; si != 0; si = si->next) {
        if (si != &libdl_info &&
            si != &libaprof_runtime_info &&
            si != somain &&
            si->aprof_jni_mode) {
            aprof_write_jni(si);
        }
    }
}

static void aprof_write()
{
    size_t soname_length = strlen(somain->name);
    int fd = aprof_open_file(
      aprof_basename(somain->name, somain->name+soname_length));
    soinfo *si;
    uint32_t header_type;
    uint32_t num_edges = 0;

    if (fd < 0) {
      ERROR("aprof : open file fail for aprof_write!\n");
      return;
    }

    aprof_write_header(fd);
    /* Write histogram records */
    header_type = APROF_EXECUTABLE_HISTOGRAM_HEADER;
    write(fd, &header_type, sizeof(uint32_t));
    aprof_write_bins(fd, somain);

    for (si = solist; si != 0; si = si->next) {
        if (si != &libdl_info &&
            si != &libaprof_runtime_info &&
            si != somain) {
            header_type = APROF_HISTOGRAM_HEADER;
            write(fd, &header_type, sizeof(uint32_t));
            DEBUG("aprof : write %s ...\n", si->name);
            aprof_write_bins(fd, si);
        }
    }

    /* Write call graph records */
    header_type = APROF_CALL_GRAPH_HEADER;
    write(fd, &header_type, sizeof(uint32_t));
    /* Count the # of edges */
    for (si = solist; si != 0; si = si->next) {
        if (si != &libdl_info &&
            si != &libaprof_runtime_info) {
            num_edges += aprof_count_edges(si);
        }
    }
    write(fd, &num_edges, sizeof(uint32_t));

    /* Write edge info*/
    for (si = solist; si != 0; si = si->next) {
        if (si != &libdl_info &&
            si != &libaprof_runtime_info) {
            aprof_write_tos(fd, si);
        }
    }
    close(fd);
}

static void aprof_write_bins(int fd, soinfo *si)
{
    unsigned len = strlen(si->name);
    write(fd, &len, sizeof(len));
    write(fd, &si->name, len);
    write(fd, &si->base, sizeof(si->base));
    write(fd, &si->size, sizeof(si->size));
    write(fd, &si->bin_size, sizeof(si->bin_size));
    write(fd, &bin_mapped_size, sizeof(uint32_t));
    if (si->bins != NULL) {
        write(fd, si->bins, si->bin_size*sizeof(uint16_t));
    }
}

static uint32_t aprof_count_edges(soinfo *si)
{
    int fromindex, endfrom, toindex;
    uint32_t num_edges = 0;

    endfrom = si->fromssize / sizeof(*si->froms);
    for (fromindex = 0; fromindex < endfrom; fromindex++) {
        if (si->froms[fromindex] == 0)
            continue;
        for (toindex = si->froms[fromindex]; toindex != 0;
             toindex = si->tos[toindex].link) {
            num_edges++;
        }
    }
    return num_edges;
}

static void aprof_write_tos(int fd, soinfo *si)
{
    int fromindex, endfrom, toindex;
    uintptr_t frompc;
    uintptr_t selfpc;
    uint32_t count;

    endfrom = si->fromssize / sizeof(*si->froms);
    for (fromindex = 0; fromindex < endfrom; fromindex++) {
        if (si->froms[fromindex] == 0)
            continue;

        frompc = si->base;
        frompc += fromindex * HASHFRACTION * sizeof(*si->froms);
        for (toindex = si->froms[fromindex]; toindex != 0;
             toindex = si->tos[toindex].link) {
            selfpc = si->tos[toindex].selfpc;
            count = si->tos[toindex].count;
            DEBUG("%x->%x : %d\n", frompc, selfpc, count);
            write(fd, &frompc, sizeof(uintptr_t));
            write(fd, &selfpc, sizeof(uintptr_t));
            write(fd, &count, sizeof(uint32_t));
        }
    }
}

static soinfo *find_soinfo_by_addr(unsigned addr)
{
    soinfo *si;
    for (si = solist; si != 0; si = si->next) {
        if (addr >= si->base &&
            addr <= si->base + si->size) {
            return si;
        }
    }
    return NULL;
}
