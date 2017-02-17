/*
 * Copyright (C) 2017 The Android Open Source Project
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

#include <vector>

#include <sys/prctl.h>

#include <linux/unistd.h>
#include <linux/audit.h>
#include <linux/filter.h>
#include <assert.h>

#include "../private/libc_logging.h"

#include "seccomp_policy.h"
#include "seccomp_bpfs.h"

static const char* TAG = "SECCOMP";

#if defined TARGET_ARCH_arm
#ifdef ONE_ARCH_ONLY
#error Two architectures defined!
#endif
#define ONE_ARCH_ONLY
#define PRIMARY_ARCH AUDIT_ARCH_ARM
static const struct sock_filter *primary_filter = arm_filter;
static const size_t primary_filter_size = arm_filter_size;
#endif


#if defined TARGET_ARCH_arm64
#ifdef ONE_ARCH_ONLY
#error Two architectures defined!
#endif
#define ONE_ARCH_ONLY
#define DUAL_ARCH
#define PRIMARY_ARCH AUDIT_ARCH_AARCH64
static const struct sock_filter *primary_filter = arm64_filter;
static const size_t primary_filter_size = arm64_filter_size;
#define SECONDARY_ARCH AUDIT_ARCH_ARM
static const struct sock_filter *secondary_filter = arm_filter;
static const size_t secondary_filter_size = arm_filter_size;
#endif


#if defined TARGET_ARCH_x86
#ifdef ONE_ARCH_ONLY
#error Two architectures defined!
#endif
#define ONE_ARCH_ONLY
#define PRIMARY_ARCH AUDIT_ARCH_I386
static const struct sock_filter *primary_filter = x86_filter;
static const size_t primary_filter_size = x86_filter_size;
#endif


#if defined TARGET_ARCH_x86_64
#ifdef ONE_ARCH_ONLY
#error Two architectures defined!
#endif
#define ONE_ARCH_ONLY
#define DUAL_ARCH
#define PRIMARY_ARCH AUDIT_ARCH_X86_64
static const struct sock_filter *primary_filter = x86_64_filter;
static const size_t primary_filter_size = x86_64_filter_size;
#define SECONDARY_ARCH AUDIT_ARCH_I386
static const struct sock_filter *secondary_filter = x86_filter;
static const size_t secondary_filter_size = x86_filter_size;
#endif


#if defined TARGET_ARCH_mips
#ifdef ONE_ARCH_ONLY
#error Two architectures defined!
#endif
#define ONE_ARCH_ONLY
#define PRIMARY_ARCH AUDIT_ARCH_MIPS
static const struct sock_filter *primary_filter = mips_filter;
static const size_t primary_filter_size = mips_filter_size;
#endif


#if defined TARGET_ARCH_mips64
#ifdef ONE_ARCH_ONLY
#error Two architectures defined!
#endif
#define ONE_ARCH_ONLY
#define PRIMARY_ARCH AUDIT_ARCH_MIPS64
static const struct sock_filter *primary_filter = mips64_filter;
static const size_t primary_filter_size = mips64_filter_size;
#define SECONDARY_ARCH AUDIT_ARCH_MIPS
static const struct sock_filter *secondary_filter = mips_filter;
static const size_t secondary_filter_size = mips_filter_size;
#endif


#if !defined ONE_ARCH_ONLY
#error No architecture was defined!
#endif


#define syscall_nr (offsetof(struct seccomp_data, nr))
#define arch_nr (offsetof(struct seccomp_data, arch))

typedef std::vector<sock_filter> filter;

inline void Disallow(filter& f) {
    f.push_back(BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_TRAP));
}

static void ExamineSyscall(filter& f) {
    f.push_back(BPF_STMT(BPF_LD|BPF_W|BPF_ABS, syscall_nr));
}

#ifdef DUAL_ARCH
static bool SetValidateArchitectureJumpTarget(size_t offset, filter& f) {
    size_t jump_length = f.size() - offset - 1;
    auto u8_jump_length = (__u8) jump_length;
    if (u8_jump_length != jump_length) {
        __libc_format_log(ANDROID_LOG_FATAL, TAG,
                          "Can't set jump greater than 255 - actual jump is %zu",
                          jump_length);
        return false;
    }
    f[offset] = BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K, SECONDARY_ARCH, u8_jump_length, 0);
    return true;
}

static size_t ValidateArchitectureAndJumpIfNeeded(filter& f) {
    f.push_back(BPF_STMT(BPF_LD|BPF_W|BPF_ABS, arch_nr));
    f.push_back(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K, PRIMARY_ARCH, 2, 0));
    f.push_back(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K, SECONDARY_ARCH, 1, 0));
    Disallow(f);
    return f.size() - 2;
}
#else
static void ValidateArchitecture(filter& f) {
    f.push_back(BPF_STMT(BPF_LD|BPF_W|BPF_ABS, arch_nr));
    f.push_back(BPF_JUMP(BPF_JMP|BPF_JEQ|BPF_K, PRIMARY_ARCH, 1, 0));
    f.push_back(BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW));
    // Disallow(f);
}
#endif

static bool install_filter(filter const& f) {
    struct sock_fprog prog = {
        (unsigned short) f.size(),
        (struct sock_filter*) &f[0],
    };

    if (prctl(PR_SET_SECCOMP, SECCOMP_MODE_FILTER, &prog) < 0) {
        __libc_format_log(ANDROID_LOG_FATAL, TAG,
                          "Could not set seccomp filter of size %zu: %s",
                          f.size(), strerror(errno));
        return false;
    }

    __libc_format_log(ANDROID_LOG_INFO, TAG,
                      "Global filter of size %zu installed",
                      f.size());
    return true;
}

bool set_seccomp_filter() {
    return true;
    filter f;
#ifdef DUAL_ARCH
    // Note that for mixed 64/32 bit architectures, ValidateArchitecture inserts a
    // jump that must be changed to point to the start of the 32-bit policy
    // 32 bit syscalls will not hit the policy between here and the call to SetJump
    auto offset_to_secondary_filter =
        ValidateArchitectureAndJumpIfNeeded(f);
#else
    ValidateArchitecture(f);
#endif

    ExamineSyscall(f);

    for (size_t i = 0; i < primary_filter_size; ++i)
        f.push_back(primary_filter[i]);
    f.push_back(BPF_STMT(BPF_RET|BPF_K, SECCOMP_RET_ALLOW));
    Disallow(f);

#ifdef DUAL_ARCH
    if (!SetValidateArchitectureJumpTarget(offset_to_secondary_filter, f))
        return false;

    ExamineSyscall(f);

    for (size_t i = 0; i < secondary_filter_size; ++i)
        f.push_back(secondary_filter[i]);
    Disallow(f);
#endif

    return install_filter(f);
}
