/*
 * Copyright (C) 2011 The Android Open Source Project
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <limits.h>
#include <unistd.h>
#include <signal.h>

#include <sys/param.h>
#include <sys/time.h>
#include <sys/gmon.h>
#include <sys/mman.h>
#include <asm/sigcontext.h>
#include <asm/ucontext.h>

#include <libaprof.h>

static const uint32_t bin_mapped_size = 4;

extern char etext[];
extern char *__progname;

static uintptr_t textsize;

static uint32_t bin_size;
static uint16_t *bins;

static size_t fromssize;
static uint16_t *froms;

static struct tostruct *tos;
static size_t tolimit;
static size_t tossize;

void aprof_init(void *module_address);
void aprof_fini(void *module_address);

static void aprof_sampler (int sig __attribute__((unused)),
                           siginfo_t *sii __attribute__((unused)),
                           void *context)
{
    struct ucontext *uc = (struct ucontext*)context;
    uintptr_t pc = uc->uc_mcontext.arm_pc;
    size_t i = (pc / 2) / HASHFRACTION;
    bins[i]++;
}

void aprof_init(void *module_address __attribute__((unused))) {
    struct sigaction sigact;
    struct itimerval timer;
    void *addr;
    size_t tossize;
    /*
     *  Allocate memory
     */
    textsize = (uintptr_t)(&etext[0]);
    bin_size =  textsize / bin_mapped_size;
    tolimit = textsize * ARCDENSITY / 100;
    if (tolimit < MINARCS)
        tolimit = MINARCS;
    else if (tolimit > MAXARCS)
        tolimit = MAXARCS;
    tossize = tolimit * sizeof(struct tostruct);

    addr = mmap((void *)0, bin_size * sizeof(uint16_t), PROT_READ|PROT_WRITE,
                MAP_ANON|MAP_PRIVATE, -1, (off_t)0);
    if (addr == MAP_FAILED)
        goto mapfailed;
    bins = addr;

    fromssize = textsize / HASHFRACTION;
    addr = mmap((void *)0, fromssize,  PROT_READ|PROT_WRITE,
        MAP_ANON|MAP_PRIVATE, -1, (off_t)0);
    if (addr == MAP_FAILED)
        goto mapfailed;
    froms = addr;

    addr = mmap((void *)0, tossize,  PROT_READ|PROT_WRITE,
                MAP_ANON|MAP_PRIVATE, -1, (off_t)0);
    if (addr == MAP_FAILED)
        goto mapfailed;
    tos = addr;
    tos[0].link = 0;

    /*
     * Setup timer and hander for sample
     */
    sigact.sa_sigaction =  &aprof_sampler;
    sigact.sa_flags = SA_RESTART | SA_SIGINFO;
    sigfillset(&sigact.sa_mask);

    if (sigaction(SIGPROF, &sigact, NULL) < 0) {
        /* Error */
        return;
    }

    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 1000000 / (PROFRATE*10);
    timer.it_interval = timer.it_value;
    if (setitimer(ITIMER_PROF, &timer, NULL) < 0) {
        /* Error */
        return;
    }
    return;

mapfailed:
    if (bins != NULL) {
        munmap(bins, bin_size * sizeof(uint16_t));
        bins = NULL;
    }
    if (froms != NULL) {
        munmap(froms, fromssize);
        froms = NULL;
    }
    if (tos != NULL) {
        munmap(tos, tossize);
        tos = NULL;
    }
}

void aprof_fini(void *module_address __attribute__((unused))) {
    extern char *__progname;
    int fd;
    uint32_t len = strlen(__progname);
    uint32_t header_type;
    const char aprof_tag[] = APROF_TAG;
    uint32_t version = APROF_VERSION;
    uint32_t pointer_size = sizeof(intptr_t);
    uint32_t sample_rate = PROFRATE;
    uint32_t base = 0;
    char path[] = "/mnt/sdcard/";
    char suffix[] = ".aprof";
    char output[PATH_MAX] = "";
    size_t len_progname = strlen(__progname);
    size_t max_len, env_len;
    const char* env;
    env = getenv("EXTERNAL_STORAGE");
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

    strncat(output, __progname, max_len);
    strcat(output, suffix);

    fd = open(output, O_CREAT|O_TRUNC|O_WRONLY, 0664);

    /* Trun off aprof_sampler */
    sigaction(SIGPROF, NULL, NULL);
    setitimer(ITIMER_PROF, NULL, NULL);

    /* Write aprof profiling file header */
    write(fd, aprof_tag, APROF_TAG_LENGTH);
    write(fd, &version, sizeof(uint32_t));
    write(fd, &sample_rate, sizeof(uint32_t));
    write(fd, &pointer_size, sizeof(uint32_t));

    /* Write histogram records */
    header_type = APROF_EXECUTABLE_HISTOGRAM_HEADER;
    write(fd, &header_type, sizeof(uint32_t));
    write(fd, &len, sizeof(uint32_t));
    write(fd, __progname, len);
    write(fd, &base, sizeof(uintptr_t));
    write(fd, &textsize, sizeof(uintptr_t));
    write(fd, &bin_size, sizeof(uint32_t));
    write(fd, &bin_mapped_size, sizeof(uint32_t));
    write(fd, bins, bin_size*sizeof(uint16_t));
    /* Write call graph records */
    header_type = APROF_CALL_GRAPH_HEADER;
    write(fd, &header_type, sizeof(uint32_t));
    int fromindex, endfrom, toindex;
    uintptr_t frompc;
    uint32_t num_edges = 0;
    /* Go through for count how many edges. */
    endfrom = fromssize / sizeof(*froms);
    for (fromindex = 0; fromindex < endfrom; fromindex++) {
        if (froms[fromindex] == 0) continue;
        for (toindex = froms[fromindex]; toindex != 0;
             toindex = tos[toindex].link) {
            num_edges++;
        }
    }

    write(fd, &num_edges, sizeof(uint32_t));
    endfrom = fromssize / sizeof(*froms);
    for (fromindex = 0; fromindex < endfrom; fromindex++) {
        if (froms[fromindex] == 0)
            continue;

        frompc = fromindex * HASHFRACTION * sizeof(*froms);
        for (toindex = froms[fromindex]; toindex != 0;
             toindex = tos[toindex].link) {
            uintptr_t selfpc = tos[toindex].selfpc;
            uint32_t count = tos[toindex].count;
            write(fd, &frompc, sizeof(uintptr_t));
            write(fd, &selfpc, sizeof(uintptr_t));
            write(fd, &count, sizeof(uint32_t));
        }
    }
}

void aprof_mcount(uintptr_t frompc, uintptr_t selfpc) {
    unsigned short *frompcindex;
    struct tostruct *top, *prevtop;
    unsigned long toindex;
    uintptr_t frompc_offset;
    frompc_offset = frompc;
    frompcindex =
        &froms[frompc_offset / (HASHFRACTION * sizeof(*froms))];
    toindex = *frompcindex;
    if (toindex == 0) {
        /*  first time traversing this arc */
        toindex = ++tos[0].link;
        if (toindex >= tolimit)
            /* halt further profiling */
            goto overflow;

        *frompcindex = toindex;
        top = &tos[toindex];
        top->selfpc = selfpc;
        top->count = 1;
        top->link = 0;
        return;
    }

    top = &tos[toindex];
    if (top->selfpc == selfpc) {
        /* arc at front of chain; usual case. */
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
            toindex = ++tos[0].link;
            if (toindex >= tolimit)
                goto overflow;

            top = &tos[toindex];
            top->selfpc = selfpc;
            top->count = 1;
            top->link = *frompcindex;
            top->link = *frompcindex;
            *frompcindex = toindex;
            return;
        }
        /* otherwise, check the next arc on the chain. */
        prevtop = top;
        top = &tos[top->link];
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
    return;
}
