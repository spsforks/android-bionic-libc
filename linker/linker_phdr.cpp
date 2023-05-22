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

#include "linker_phdr.h"

#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "linker.h"
#include "linker_dlwarning.h"
#include "linker_globals.h"
#include "linker_debug.h"
#include "linker_utils.h"

#include "private/CFIShadow.h" // For kLibraryAlignment

static int GetTargetElfMachine() {
#if defined(__arm__)
  return EM_ARM;
#elif defined(__aarch64__)
  return EM_AARCH64;
#elif defined(__i386__)
  return EM_386;
#elif defined(__riscv)
  return EM_RISCV;
#elif defined(__x86_64__)
  return EM_X86_64;
#endif
}

/* Code fixup hack!!! */
#include <map>

struct FixUpAction {
  uint8_t reg;
  uint64_t value;
  uint8_t cond;
  bool islink;
  bool bitclear;
  uint64_t bitmask;
  uint8_t bitreg;
};

// TODO make these thread-safe.
std::map<uint64_t, struct FixUpAction> fixupMap;
std::map<uint64_t, uint64_t> pageMoves;

#define PSTATE_V 0x10000000
#define PSTATE_C 0x20000000
#define PSTATE_Z 0x40000000
#define PSTATE_N 0x80000000

bool pstate_cond_holds(uint64_t pstate, uint8_t cond) {
  bool result;

  switch (cond >> 1) {
  case 0b000:
    result = ((pstate & PSTATE_Z) != 0); // EQ or NE
    break;
  case 0b001:
    result = ((pstate & PSTATE_C) != 0); // CS or CC
    break;
  case 0b010:
    result = ((pstate & PSTATE_N) != 0); // MI or PL
    break;
  case 0b011:
    result = ((pstate & PSTATE_V) != 0); // VS or VC
    break;
  case 0b100:
    result = ((pstate & PSTATE_C) != 0 && ((pstate & PSTATE_Z) == 0)); // HI or LS
    break;
  case 0b101:
    result = ((pstate & PSTATE_N) == PSTATE_V); // GE or LT
    break;
  case 0b110:
    result = ((pstate & PSTATE_N) == PSTATE_V && (PSTATE_Z == 0)); // GT or LE
    break;
  case 0b111:
    return true; // AL
  }

  if (cond & 0x1)
    result = !result;

  return result;
}

bool regcompare_holds(bool clear, uint64_t mask, uint64_t value) {
  if (clear)
    return (value & mask) == 0;
  else
    return (value & mask) != 0;
}

// clang-format off
// adrp :
// 31 | 30 29 | 28 | 27 | 26 | 25 | 24 | 23 - 5 |   4 - 0 |
//  1 | immlo |  1 |  0 |  0 |  0 |  0 |  immhi |      Rd |
// adr
// 31 | 30 29 | 28 | 27 | 26 | 25 | 24 | 23 - 5 |   4 - 0 |
//  0 | immlo |  1 |  0 |  0 |  0 |  0 |  immhi |      Rd |
// brk
// 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 | 22 | 21 | 20 - 5 | 4 | 3 | 2 | 1 | 0 |
//  1 |  1 |  0 |  1 |  0 |  1 |  0 |  0 |  0 |  0 |  1 |  imm16 | 0 | 0 | 0 | 0 | 0 |

// b/bl <label>
// 31 | 30 | 29 | 28 | 27 | 26 | 25 -  0 |
// op |  0 |  0 |  1 |  0 |  1 |  imm26  |

// b.cond / bc.cond
// 31 | 30 | 29 | 28 | 27 | 26 | 25 | 24 | 23 - 5 |  4 | 3 - 0 |
//  0 |  1 |  0 |  1 |  0 |  1 |  0 |  0 |  imm19 |  c |  cond |

// clang-format on

const uint32_t adr_mask = 0x1f << 24;
const uint32_t adr_value = 0x10 << 24;
const uint32_t brk_value = 0xd4 << 24 | 1 << 21;
const uint32_t branch_mask = 0x1F << 26;
const uint32_t branch_value = 0x5 << 26;
const uint32_t branchcond_mask = 0xFF << 24;
const uint32_t branchcond_value = 0x54 << 24;
const uint32_t comparebranch_mask = 0x3f << 25;
const uint32_t comparebranch_value = 0x1a << 25;
const uint32_t testbranch_mask = 0x3f << 25;
const uint32_t testbranch_value = 0x1b << 25;


template <unsigned B> constexpr inline int64_t SignExtend64(uint64_t x) {
  static_assert(B > 0, "Bit width can't be 0.");
  static_assert(B <= 64, "Bit width out of range.");
  return int64_t(x << (64 - B)) >> (64 - B);
}

/// Checks if an integer fits into the given bit width.
template <unsigned N> constexpr inline bool isInt(int64_t x) {
  if constexpr (N == 8)
    return static_cast<int8_t>(x) == x;
  if constexpr (N == 16)
    return static_cast<int16_t>(x) == x;
  if constexpr (N == 32)
    return static_cast<int32_t>(x) == x;
  if constexpr (N < 64)
    return -(INT64_C(1) << (N - 1)) <= x && x < (INT64_C(1) << (N - 1));
  (void)x;
  return true;
}

struct Adrp {
  uint8_t destRegister;
  int64_t addend;
  uint32_t shift;
  uint64_t base_address;
  uint64_t getAddress() { return base_address + addend; }
  bool inRange(int64_t fixup) {
    return shift ? isInt<33>(addend + fixup) : isInt<19>(addend + fixup);
  }
};

static bool parseAdrp(uint32_t *inst, Adrp &adrp) {
  if ((*inst & adr_mask) != adr_value)
    return false;
  adrp.destRegister = *inst & 0x1f;
  uint64_t immHi = (*inst >> 5) & 0x7ffff;
  uint64_t immLo = (*inst >> 29) & 0x3;
  if (*inst & (1 << 31)) {
    adrp.addend = SignExtend64<33>((immLo | (immHi << 2)) << 12);
    adrp.shift = 12;
    adrp.base_address = (reinterpret_cast<uint64_t>(inst)) & ~0xFFF;
  } else {
    adrp.addend = SignExtend64<21>(immLo | (immHi << 2));
    adrp.shift = 0;
    adrp.base_address = reinterpret_cast<uint64_t>(inst);
  }
  return true;
}

static uint32_t encodeAdrp(Adrp &adrp) {
  uint32_t inst = adr_value;
  inst |= adrp.destRegister;
  uint64_t immLo = (adrp.addend >> adrp.shift) & 0x3;
  uint64_t immHi = (adrp.addend >> adrp.shift) >> 2 & 0x7ffff;
  inst |= immLo << 29;
  inst |= immHi << 5;
  if (adrp.shift)
    inst |= 1 << 31;
  return inst;
}

struct Branch {
  int64_t addend;
  uint64_t base_address;
  bool islink;
  uint64_t getAddress() { return base_address + addend; }
  bool inRange(int64_t fixup) { return isInt<28>(addend + fixup); }
};

static bool parseBranch(uint32_t *inst, Branch &b) {
  if ((*inst & branch_mask) != branch_value)
    return false;
  b.addend = SignExtend64<28>((*inst & 0x3ffffff) << 2);
  b.islink = *inst & (1 << 31);
  b.base_address = reinterpret_cast<uint64_t>(inst);
  return true;
}

static uint32_t encodeBranch(Branch &b) {
  uint32_t inst = branch_value;
  inst |= (b.addend >> 2) & 0x3ffffff;
  if (b.islink)
    inst |= 1 << 31;
  return inst;
}

struct BranchCond {
  int64_t addend;
  uint64_t base_address;
  uint8_t cond;
  bool consitent;
  uint64_t getAddress() { return base_address + addend; }
  bool inRange(int64_t fixup) { return isInt<21>(addend + fixup); }
};

static bool parseBranchCond(uint32_t *inst, BranchCond &b) {
  if ((*inst & branchcond_mask) != branchcond_value)
    return false;
  b.addend = SignExtend64<21>(((*inst >> 5) & 0x7ffff) << 2);
  b.cond = *inst & (0xf);
  b.base_address = reinterpret_cast<uint64_t>(inst);
  b.consitent = *inst & 0x1 << 4;
  return true;
}

static uint32_t encodeBranchCond(BranchCond &b) {
  uint32_t inst = branchcond_value;
  inst |= ((b.addend >> 2) & 0x7ffff) << 5;
  inst |= b.cond;
  inst |= b.consitent << 4;
  return inst;
}

struct CompareAndBranch {
  int64_t addend;
  uint64_t base_address;
  bool negate;
  uint8_t regsize;
  uint8_t reg;
  uint64_t getAddress() { return base_address + addend; }
  bool inRange(int64_t fixup) { return isInt<19>(addend + fixup); }
};

static bool parseCompareAndBranch(uint32_t *inst, CompareAndBranch &cb) {
  if ((*inst & comparebranch_mask) != comparebranch_value) {
    return false;
  }

  cb.base_address = reinterpret_cast<uint64_t>(inst);
  cb.addend = SignExtend64<21>(((*inst >> 5) & 0x7ffff) << 2);
  cb.negate = *inst & (1 << 24);
  cb.regsize = (*inst >> 31) ? 64 : 32;
  cb.reg = *inst & 0x1f;

  return true;
}

static uint32_t encodeCompareAndBranch(const CompareAndBranch &cb) {
  uint32_t inst = comparebranch_value;
  inst |= (((cb.regsize == 64) ? 0x1 : 0x0) << 31);
  inst |= (cb.negate & 0x1) << 24;
  inst |= ((cb.addend >> 2) & 0x7ffff) << 5;
  inst |= cb.reg & 0x1f;
  return inst;
}

struct TestAndBranch {
  int64_t addend;
  uint64_t base_address;
  bool negate;
  uint8_t regsize;
  uint8_t reg;
  uint8_t bit;
  uint64_t getAddress() { return base_address + addend; }
  bool inRange(int64_t fixup) { return isInt<16>(addend + fixup); }
};

static bool parseTestAndBranch(uint32_t *inst, TestAndBranch &tb) {
  if ((*inst & testbranch_mask) != testbranch_value) {
    return false;
  }

  tb.base_address = reinterpret_cast<uint64_t>(inst);
  tb.addend = SignExtend64<16>(((*inst >> 5) & 0x3fff) << 2);
  tb.negate = *inst & (1 << 24);
  tb.regsize = (*inst >> 31) ? 64 : 32;
  tb.reg = *inst & 0x1f;
  tb.bit = ((*inst >> 19) & 0x1f) | ((*inst >> 26) & (1 << 6)) ;

  return true;
}

static uint32_t encodeTestAndBranch(const TestAndBranch &tb) {
  uint32_t inst = testbranch_value;
  inst |= ((tb.regsize == 64) ? 0x1 : 0x0) << 31;
  inst |= (tb.negate & 0x1) << 24;
  inst |= (tb.bit & 0x1f) << 19;
  inst |= ((tb.addend >> 2) & 0x3fff) << 5;
  inst |= (tb.reg) & 0x1f;
  return inst;
}


int scan(uint32_t *inst_p, uint32_t cnt, int64_t fixup) {
#ifdef CODE_COPY_HACK_TRACE
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdate-time"
  DL_WARN("\t[CODE COPY HACK] date: %s, time:%s", __DATE__, __TIME__);
#pragma clang diagnostic pop
#endif

  int adr_t = 0;
  int adr_p = 0;
  int b_t = 0;
  int b_p = 0;
  int bcond_t = 0;
  int bcond_p = 0;
  int cb_t = 0;
  int cb_p = 0;
  int tb_t = 0;
  int tb_p = 0;

  for (uint32_t i = 0; i < cnt; i++) {
    struct Adrp adrp;
    struct Branch b;
    struct BranchCond bc;
    struct CompareAndBranch cb;
    struct TestAndBranch tb;

    if (parseAdrp(&inst_p[i], adrp)) {
#ifdef CODE_COPY_HACK_TRACE
      DL_WARN("\t[CODE COPY HACK] found at (0x%lX) 0x%X (%s)  - base 0%lX, targets 0x%lX - imm 0x%lX\n",
             reinterpret_cast<unsigned long>(&inst_p[i]),
             inst_p[i],
             adrp.shift ? "adrp" : "adr", adrp.base_address,
             adrp.getAddress() + fixup, adrp.addend);
#endif
      // ADR/P always have to be patched, so that they point to the
      // original page.
      adr_t += 1;
      if (!adrp.inRange(fixup)) {
        fixupMap.insert({reinterpret_cast<uint64_t>(&inst_p[i]),
                         FixUpAction{adrp.destRegister,
                                     adrp.getAddress() + fixup, 0b111, false,
                                     true, 0x0, 0}});
        inst_p[i] = brk_value;
      } else {
        adr_p += 1;
        adrp.addend += fixup;
        inst_p[i] = encodeAdrp(adrp);
      }
    } else if (parseBranch(&inst_p[i], b)) {
#ifdef CODE_COPY_HACK_TRACE
      DL_WARN("\t[CODE COPY HACK] branch found 0x%lX (targets 0x%lX), inrange %i (0x%lX)\n",
             reinterpret_cast<unsigned long>(&inst_p[i]),
             b.getAddress() + fixup, b.inRange(fixup), b.addend + fixup);
#endif

      if (page_start(reinterpret_cast<uintptr_t>(inst_p)) == page_start(reinterpret_cast<uintptr_t>(b.getAddress()))) {
#ifdef CODE_COPY_HACK_TRACE
        DL_WARN("\t\t[CODE COPY HACK] skipped -- in same page! ");
#endif
        continue;
      }

      b_t += 1;
      if (b.inRange(fixup)) {
        b_p += 1;
        b.addend += fixup;
        inst_p[i] = encodeBranch(b);
      } else {
        fixupMap.insert({reinterpret_cast<uint64_t>(&inst_p[i]),
                         FixUpAction{33, b.getAddress() + fixup, 0b111, b.islink,
                                     true, 0x0, 0}});
        inst_p[i] = brk_value;
      }
    } else if (parseBranchCond(&inst_p[i], bc)) {
#ifdef CODE_COPY_HACK_TRACE
      DL_WARN("\t[CODE COPY HACK]  branchcond found 0x%X at 0x%lX (targets 0x%lX), inrange %i (0x%lX)\n",
      reinterpret_cast<unsigned>(inst_p[i]),
      reinterpret_cast<unsigned long>(&inst_p[i]),
      bc.getAddress() + fixup, bc.inRange(fixup),
      bc.addend + fixup);
#endif

      if (page_start(reinterpret_cast<uintptr_t>(inst_p)) == page_start(reinterpret_cast<uintptr_t>(bc.getAddress()))) {
#ifdef CODE_COPY_HACK_TRACE
        DL_WARN("\t\t[CODE COPY HACK] skipped -- in same page! ");
#endif
        continue;
      }

      bcond_t += 1;
      if (bc.inRange(fixup)) {
        bcond_p += 1;
        bc.addend += fixup;
        inst_p[i] = encodeBranchCond(bc);
      } else {
        fixupMap.insert({reinterpret_cast<uint64_t>(&inst_p[i]),
                         FixUpAction{33, bc.getAddress() + fixup, bc.cond, false,
                                     true, 0x0, 0}});
        inst_p[i] = brk_value;
      }
    } else if (parseCompareAndBranch(&inst_p[i], cb)) {
#ifdef CODE_COPY_HACK_TRACE
      DL_WARN("\t[CODE COPY HACK]  CB(N)Z found 0x%X at 0x%lX (targets 0x%lX), inrange %i (0x%lX)\n",
      reinterpret_cast<unsigned>(inst_p[i]),
      reinterpret_cast<unsigned long>(&inst_p[i]),
      cb.getAddress() + fixup, cb.inRange(fixup),
      cb.addend + fixup);
#endif

      if (page_start(reinterpret_cast<uintptr_t>(inst_p)) == page_start(reinterpret_cast<uintptr_t>(cb.getAddress()))) {
#ifdef CODE_COPY_HACK_TRACE
        DL_WARN("\t\t[CODE COPY HACK] skipped -- in same page! ");
#endif
        continue;
      }

      cb_t += 1;
      if (cb.inRange(fixup)) {
        cb_p += 1;
        cb.addend += fixup;
        inst_p[i] = encodeCompareAndBranch(cb);
      } else {
        fixupMap.insert({reinterpret_cast<uint64_t>(&inst_p[i]),
                         FixUpAction{33, cb.getAddress() + fixup, 0b111, false,
                                     !cb.negate,
                                     cb.regsize == 64 ? 0xffffffffffffffffUL : 0xffffffffUL,
                                     cb.reg}});
      }
    } else if (parseTestAndBranch(&inst_p[i], tb)) {
#ifdef CODE_COPY_HACK_TRACE
      DL_WARN("\t[CODE COPY HACK] TB(N)Z found 0x%X at 0x%lX (targets 0x%lX), inrange %i (0x%lX)\n",
      reinterpret_cast<unsigned>(inst_p[i]),
      reinterpret_cast<unsigned long>(&inst_p[i]),
      tb.getAddress() + fixup, tb.inRange(fixup),
      tb.addend + fixup);
#endif

      if (page_start(reinterpret_cast<uintptr_t>(inst_p)) == page_start(reinterpret_cast<uintptr_t>(tb.getAddress()))) {
#ifdef CODE_COPY_HACK_TRACE
        DL_WARN("\t\t[CODE COPY HACK] skipped -- in same page! ");
#endif
        continue;
      }

      tb_t += 1;
      if (tb.inRange(fixup)) {
        tb_p += 1;
        tb.addend += fixup;
        inst_p[i] = encodeTestAndBranch(tb);
      } else {
        fixupMap.insert({reinterpret_cast<uint64_t>(&inst_p[i]),
                         FixUpAction{33, tb.getAddress() + fixup, 0b111, false,
                                     !tb.negate, 1UL << tb.bit, tb.reg}});
      }
    }
  }
#ifdef CODE_COPY_HACK_TRACE
  DL_WARN("[CODE COPY STATS] fixup is: %ld", fixup);
  DL_WARN("[CODE COPY STATS] adr total: %d, patched: %d, b total: %d, patched: %d, b.cond total: %d, patched: %d, cb(n)z total: %d, patched: %d, tb(n)z total: %d, patched: %d",
          adr_t, adr_p,
          b_t, b_p,
          bcond_t, bcond_p,
          cb_t, cb_p,
          tb_t, tb_p);
#endif
  return 0;
}

void unblock_signal(int signum) {
  sigset_t sigs;
  sigemptyset(&sigs);
  sigaddset(&sigs, signum);
  sigprocmask(SIG_UNBLOCK, &sigs, NULL);
}

/* Chainging sighandlers */
struct sigaction old_sigsegv;
struct sigaction old_sigtrap;

void sighandler(int sig, siginfo_t *info, void *ucontext) {
  ucontext_t *realcontext = reinterpret_cast<ucontext_t *>(ucontext);
  (void)info;

  if (sig == -SIGSEGV) {
    if (info != nullptr) {
#ifdef CODE_COPY_HACK_TRACE
      DL_WARN("[CODE COPY SIGNAL] returning old sigsegv");
#endif
      struct sigaction * old = reinterpret_cast<struct sigaction *>(info);
      *old = old_sigsegv;
    }
    if (ucontext != nullptr) {
#ifdef CODE_COPY_HACK_TRACE
      DL_WARN("[CODE COPY SIGNAL] updating old sigsegv");
#endif
      struct sigaction * new_sigaction = reinterpret_cast<struct sigaction *>(ucontext);
      old_sigsegv = *new_sigaction;
    }
    return;
  } else if (sig == -SIGTRAP) {
    if (info != nullptr) {
#ifdef CODE_COPY_HACK_TRACE
      DL_WARN("[CODE COPY SIGNAL] returning old sigtrap");
#endif
      struct sigaction * old = reinterpret_cast<struct sigaction *>(info);
      *old = old_sigtrap;
    }
    if (ucontext != nullptr) {
#ifdef CODE_COPY_HACK_TRACE
      DL_WARN("[CODE COPY SIGNAL] updating old sigtrap");
#endif
      struct sigaction * new_sigaction = reinterpret_cast<struct sigaction *>(ucontext);
      old_sigtrap = *new_sigaction;
    }
    return;
  }

#ifdef CODE_COPY_HACK_TRACE
  DL_WARN("\t[CODE COPY HACK] - sighandler start - sig:%d, PC:0x%llX, SP:0x%llX, LR:0x%llX PID:%d",
          sig,
          realcontext->uc_mcontext.pc, realcontext->uc_mcontext.sp,
          realcontext->uc_mcontext.regs[30],
          getpid());
#endif

  if (SIGSEGV == sig) {
    const auto &page_movesmap = pageMoves;
    if (auto mMove =
            page_movesmap.find(realcontext->uc_mcontext.pc & ~(page_size() - 1));
        mMove != page_movesmap.end()) {
      uint64_t diff = mMove->first - mMove->second;
      unblock_signal(SIGSEGV);
#ifdef CODE_COPY_HACK_TRACE
      DL_WARN("\t[CODE COPY HACK]  sighandler(SIGSEGV) move PC from 0x%llX to 0x%llX\n",
             realcontext->uc_mcontext.pc, realcontext->uc_mcontext.pc - diff);
      fflush(stdout);
#endif
      realcontext->uc_mcontext.pc += -diff;
      return;
    } else {
#ifdef CODE_COPY_HACK_TRACE
      DL_WARN("\t[CODE COPY HACK] - unexpected segfault - pageMoves not found, PC:0x%llX, SP:0x%llX, LR:0x%llX, fault_address:0x%llX, PID:%d",
              realcontext->uc_mcontext.pc, realcontext->uc_mcontext.sp,
              realcontext->uc_mcontext.regs[30],
              realcontext->uc_mcontext.fault_address, getpid());
#endif

      if (old_sigsegv.sa_handler != SIG_IGN) {
        // TODO Handle these properly.
        if (old_sigsegv.sa_handler == SIG_DFL) {
          exit(-1);
        }

        unblock_signal(SIGTRAP);
        unblock_signal(SIGSEGV);
        if (old_sigsegv.sa_flags & SA_SIGINFO) {
#ifdef CODE_COPY_HACK_TRACE
          DL_WARN("[CODE COPY HACK] passed to old_sigsegv_sigaction");
#endif
          old_sigsegv.sa_sigaction(sig, info, ucontext);
        } else {
#ifdef CODE_COPY_HACK_TRACE
          DL_WARN("[CODE COPY HACK] passed to old_sigsegv_handler");
#endif
          old_sigsegv.sa_handler(sig);
        }
      }
#ifdef CODE_COPY_HACK_TRACE
      fflush(stdout);
#endif
      return;
    }
  } else if (SIGTRAP == sig) {
    auto const& fmap = fixupMap;
    if (auto fixup = fmap.find(realcontext->uc_mcontext.pc);
        fixup != fmap.end()) {
      unblock_signal(SIGTRAP);
      uint8_t reg = fixup->second.reg;
      // Skip the BRK instruction.
      realcontext->uc_mcontext.pc += 4;
      if (fixup->second.islink) {
#ifdef CODE_COPY_HACK_TRACE
        DL_WARN("[CODE COPY HACK] BL instruction detected, fixup LR");
#endif
        //Fix LR if it's BL
        realcontext->uc_mcontext.regs[30] = realcontext->uc_mcontext.pc;
      }
      if (pstate_cond_holds(realcontext->uc_mcontext.pstate,
                            fixup->second.cond) &&
          regcompare_holds(fixup->second.bitclear, fixup->second.bitmask,
                            realcontext->uc_mcontext.regs[fixup->second.bitreg])) {
        if (reg == 33)
          realcontext->uc_mcontext.pc = fixup->second.value;
        else {
          realcontext->uc_mcontext.regs[reg] = fixup->second.value;
        }
      }
#ifdef CODE_COPY_HACK_TRACE
      DL_WARN(
          "\t[CODE COPY HACK]  sighandler(SIGTRAP) fixup reg %i to 0x%lX (cond is %s for %d)\n",
          fixup->second.reg, fixup->second.value,
          pstate_cond_holds(realcontext->uc_mcontext.pstate, fixup->second.cond) ? "true" : "false",
          fixup->second.cond
      );
      fflush(stdout);
#endif
      return;
    } else {
#ifdef CODE_COPY_HACK_TRACE
      DL_WARN("\t[CODE COPY HACK] - unexpected sigtrap - fixup not found, PC:0x%llX, SP:0x%llX, LR:0x%llX, fault_address:0x%llX, PID:%d",
              realcontext->uc_mcontext.pc, realcontext->uc_mcontext.sp,
              realcontext->uc_mcontext.regs[30],
              realcontext->uc_mcontext.fault_address, getpid());
#endif
      if (old_sigtrap.sa_handler != SIG_IGN) {
        // TODO How to handle this properly?
        if (old_sigtrap.sa_handler == SIG_DFL) {
          exit(-1);
        }

        unblock_signal(SIGTRAP);
        unblock_signal(SIGSEGV);
        if (old_sigtrap.sa_flags & SA_SIGINFO) {
#ifdef CODE_COPY_HACK_TRACE
          DL_WARN("[CODE COPY HACK] passed to old_sigtrap_sigaction");
#endif
          old_sigtrap.sa_sigaction(sig, info, ucontext);
        } else {
#ifdef CODE_COPY_HACK_TRACE
          DL_WARN("[CODE COPY HACK] passed to old_sigtrap_handler");
#endif
          old_sigtrap.sa_handler(sig);
        }
      }
#ifdef CODE_COPY_HACK_TRACE
      fflush(stdout);
#endif
      return;
    }
  }
#ifdef CODE_COPY_HACK_TRACE
  DL_WARN("[CODE COPY HACK] !!!! unexpected signal %i, from: 0x%llX \n", sig,
         realcontext->uc_mcontext.pc);
  fflush(stdout);
#endif
}


/**
  TECHNICAL NOTE ON ELF LOADING.

  An ELF file's program header table contains one or more PT_LOAD
  segments, which corresponds to portions of the file that need to
  be mapped into the process' address space.

  Each loadable segment has the following important properties:

    p_offset  -> segment file offset
    p_filesz  -> segment file size
    p_memsz   -> segment memory size (always >= p_filesz)
    p_vaddr   -> segment's virtual address
    p_flags   -> segment flags (e.g. readable, writable, executable)
    p_align   -> segment's in-memory and in-file alignment

  We will ignore the p_paddr field of ElfW(Phdr) for now.

  The loadable segments can be seen as a list of [p_vaddr ... p_vaddr+p_memsz)
  ranges of virtual addresses. A few rules apply:

  - the virtual address ranges should not overlap.

  - if a segment's p_filesz is smaller than its p_memsz, the extra bytes
    between them should always be initialized to 0.

  - ranges do not necessarily start or end at page boundaries. Two distinct
    segments can have their start and end on the same page. In this case, the
    page inherits the mapping flags of the latter segment.

  Finally, the real load addrs of each segment is not p_vaddr. Instead the
  loader decides where to load the first segment, then will load all others
  relative to the first one to respect the initial range layout.

  For example, consider the following list:

    [ offset:0,      filesz:0x4000, memsz:0x4000, vaddr:0x30000 ],
    [ offset:0x4000, filesz:0x2000, memsz:0x8000, vaddr:0x40000 ],

  This corresponds to two segments that cover these virtual address ranges:

       0x30000...0x34000
       0x40000...0x48000

  If the loader decides to load the first segment at address 0xa0000000
  then the segments' load address ranges will be:

       0xa0030000...0xa0034000
       0xa0040000...0xa0048000

  In other words, all segments must be loaded at an address that has the same
  constant offset from their p_vaddr value. This offset is computed as the
  difference between the first segment's load address, and its p_vaddr value.

  However, in practice, segments do _not_ start at page boundaries. Since we
  can only memory-map at page boundaries, this means that the bias is
  computed as:

       load_bias = phdr0_load_address - page_start(phdr0->p_vaddr)

  (NOTE: The value must be used as a 32-bit unsigned integer, to deal with
          possible wrap around UINT32_MAX for possible large p_vaddr values).

  And that the phdr0_load_address must start at a page boundary, with
  the segment's real content starting at:

       phdr0_load_address + page_offset(phdr0->p_vaddr)

  Note that ELF requires the following condition to make the mmap()-ing work:

      page_offset(phdr0->p_vaddr) == page_offset(phdr0->p_offset)

  The load_bias must be added to any p_vaddr value read from the ELF file to
  determine the corresponding memory address.

 **/

#define MAYBE_MAP_FLAG(x, from, to)  (((x) & (from)) ? (to) : 0)
#define PFLAGS_TO_PROT(x)            (MAYBE_MAP_FLAG((x), PF_X, PROT_EXEC) | \
                                      MAYBE_MAP_FLAG((x), PF_R, PROT_READ) | \
                                      MAYBE_MAP_FLAG((x), PF_W, PROT_WRITE))

// Default PMD size for x86_64 and aarch64 (2MB).
static constexpr size_t kPmdSize = (1UL << 21);

ElfReader::ElfReader()
    : did_read_(false), did_load_(false), fd_(-1), file_offset_(0), file_size_(0), phdr_num_(0),
      phdr_table_(nullptr), shdr_table_(nullptr), shdr_num_(0), dynamic_(nullptr), strtab_(nullptr),
      strtab_size_(0), load_start_(nullptr), load_size_(0), load_bias_(0), loaded_phdr_(nullptr),
      mapped_by_caller_(false) {
}

bool ElfReader::Read(const char* name, int fd, off64_t file_offset, off64_t file_size) {
  if (did_read_) {
    return true;
  }
  name_ = name;
  fd_ = fd;
  file_offset_ = file_offset;
  file_size_ = file_size;

  if (ReadElfHeader() &&
      VerifyElfHeader() &&
      ReadProgramHeaders() &&
      ReadSectionHeaders() &&
      ReadDynamicSection()) {
    did_read_ = true;
  }

  return did_read_;
}

bool ElfReader::Load(address_space_params* address_space) {
  CHECK(did_read_);
  if (did_load_) {
    return true;
  }
  if (ReserveAddressSpace(address_space) && LoadSegments() && FindPhdr() &&
      FindGnuPropertySection()) {
    did_load_ = true;
#if defined(__aarch64__)
    // For Armv8.5-A loaded executable segments may require PROT_BTI.
    if (note_gnu_property_.IsBTICompatible()) {
      did_load_ = (phdr_table_protect_segments(phdr_table_, phdr_num_, load_bias_,
                                               &note_gnu_property_) == 0);
    }
#endif
  }

  return did_load_;
}

const char* ElfReader::get_string(ElfW(Word) index) const {
  CHECK(strtab_ != nullptr);
  CHECK(index < strtab_size_);

  return strtab_ + index;
}

bool ElfReader::ReadElfHeader() {
  ssize_t rc = TEMP_FAILURE_RETRY(pread64(fd_, &header_, sizeof(header_), file_offset_));
  if (rc < 0) {
    DL_ERR("can't read file \"%s\": %s", name_.c_str(), strerror(errno));
    return false;
  }

  if (rc != sizeof(header_)) {
    DL_ERR("\"%s\" is too small to be an ELF executable: only found %zd bytes", name_.c_str(),
           static_cast<size_t>(rc));
    return false;
  }
  return true;
}

static const char* EM_to_string(int em) {
  if (em == EM_386) return "EM_386";
  if (em == EM_AARCH64) return "EM_AARCH64";
  if (em == EM_ARM) return "EM_ARM";
  if (em == EM_RISCV) return "EM_RISCV";
  if (em == EM_X86_64) return "EM_X86_64";
  return "EM_???";
}

bool ElfReader::VerifyElfHeader() {
  if (memcmp(header_.e_ident, ELFMAG, SELFMAG) != 0) {
    DL_ERR("\"%s\" has bad ELF magic: %02x%02x%02x%02x", name_.c_str(),
           header_.e_ident[0], header_.e_ident[1], header_.e_ident[2], header_.e_ident[3]);
    return false;
  }

  // Try to give a clear diagnostic for ELF class mismatches, since they're
  // an easy mistake to make during the 32-bit/64-bit transition period.
  int elf_class = header_.e_ident[EI_CLASS];
#if defined(__LP64__)
  if (elf_class != ELFCLASS64) {
    if (elf_class == ELFCLASS32) {
      DL_ERR("\"%s\" is 32-bit instead of 64-bit", name_.c_str());
    } else {
      DL_ERR("\"%s\" has unknown ELF class: %d", name_.c_str(), elf_class);
    }
    return false;
  }
#else
  if (elf_class != ELFCLASS32) {
    if (elf_class == ELFCLASS64) {
      DL_ERR("\"%s\" is 64-bit instead of 32-bit", name_.c_str());
    } else {
      DL_ERR("\"%s\" has unknown ELF class: %d", name_.c_str(), elf_class);
    }
    return false;
  }
#endif

  if (header_.e_ident[EI_DATA] != ELFDATA2LSB) {
    DL_ERR("\"%s\" not little-endian: %d", name_.c_str(), header_.e_ident[EI_DATA]);
    return false;
  }

  if (header_.e_type != ET_DYN) {
    DL_ERR("\"%s\" has unexpected e_type: %d", name_.c_str(), header_.e_type);
    return false;
  }

  if (header_.e_version != EV_CURRENT) {
    DL_ERR("\"%s\" has unexpected e_version: %d", name_.c_str(), header_.e_version);
    return false;
  }

  if (header_.e_machine != GetTargetElfMachine()) {
    DL_ERR("\"%s\" is for %s (%d) instead of %s (%d)",
           name_.c_str(),
           EM_to_string(header_.e_machine), header_.e_machine,
           EM_to_string(GetTargetElfMachine()), GetTargetElfMachine());
    return false;
  }

  if (header_.e_shentsize != sizeof(ElfW(Shdr))) {
    // Fail if app is targeting Android O or above
    if (get_application_target_sdk_version() >= 26) {
      DL_ERR_AND_LOG("\"%s\" has unsupported e_shentsize: 0x%x (expected 0x%zx)",
                     name_.c_str(), header_.e_shentsize, sizeof(ElfW(Shdr)));
      return false;
    }
    DL_WARN_documented_change(26,
                              "invalid-elf-header_section-headers-enforced-for-api-level-26",
                              "\"%s\" has unsupported e_shentsize 0x%x (expected 0x%zx)",
                              name_.c_str(), header_.e_shentsize, sizeof(ElfW(Shdr)));
    add_dlwarning(name_.c_str(), "has invalid ELF header");
  }

  if (header_.e_shstrndx == 0) {
    // Fail if app is targeting Android O or above
    if (get_application_target_sdk_version() >= 26) {
      DL_ERR_AND_LOG("\"%s\" has invalid e_shstrndx", name_.c_str());
      return false;
    }

    DL_WARN_documented_change(26,
                              "invalid-elf-header_section-headers-enforced-for-api-level-26",
                              "\"%s\" has invalid e_shstrndx", name_.c_str());
    add_dlwarning(name_.c_str(), "has invalid ELF header");
  }

  return true;
}

bool ElfReader::CheckFileRange(ElfW(Addr) offset, size_t size, size_t alignment) {
  off64_t range_start;
  off64_t range_end;

  // Only header can be located at the 0 offset... This function called to
  // check DYNSYM and DYNAMIC sections and phdr/shdr - none of them can be
  // at offset 0.

  return offset > 0 &&
         safe_add(&range_start, file_offset_, offset) &&
         safe_add(&range_end, range_start, size) &&
         (range_start < file_size_) &&
         (range_end <= file_size_) &&
         ((offset % alignment) == 0);
}

// Loads the program header table from an ELF file into a read-only private
// anonymous mmap-ed block.
bool ElfReader::ReadProgramHeaders() {
  phdr_num_ = header_.e_phnum;

  // Like the kernel, we only accept program header tables that
  // are smaller than 64KiB.
  if (phdr_num_ < 1 || phdr_num_ > 65536/sizeof(ElfW(Phdr))) {
    DL_ERR("\"%s\" has invalid e_phnum: %zd", name_.c_str(), phdr_num_);
    return false;
  }

  // Boundary checks
  size_t size = phdr_num_ * sizeof(ElfW(Phdr));
  if (!CheckFileRange(header_.e_phoff, size, alignof(ElfW(Phdr)))) {
    DL_ERR_AND_LOG("\"%s\" has invalid phdr offset/size: %zu/%zu",
                   name_.c_str(),
                   static_cast<size_t>(header_.e_phoff),
                   size);
    return false;
  }

  if (!phdr_fragment_.Map(fd_, file_offset_, header_.e_phoff, size)) {
    DL_ERR("\"%s\" phdr mmap failed: %s", name_.c_str(), strerror(errno));
    return false;
  }

  phdr_table_ = static_cast<ElfW(Phdr)*>(phdr_fragment_.data());
  return true;
}

bool ElfReader::ReadSectionHeaders() {
  shdr_num_ = header_.e_shnum;

  if (shdr_num_ == 0) {
    DL_ERR_AND_LOG("\"%s\" has no section headers", name_.c_str());
    return false;
  }

  size_t size = shdr_num_ * sizeof(ElfW(Shdr));
  if (!CheckFileRange(header_.e_shoff, size, alignof(const ElfW(Shdr)))) {
    DL_ERR_AND_LOG("\"%s\" has invalid shdr offset/size: %zu/%zu",
                   name_.c_str(),
                   static_cast<size_t>(header_.e_shoff),
                   size);
    return false;
  }

  if (!shdr_fragment_.Map(fd_, file_offset_, header_.e_shoff, size)) {
    DL_ERR("\"%s\" shdr mmap failed: %s", name_.c_str(), strerror(errno));
    return false;
  }

  shdr_table_ = static_cast<const ElfW(Shdr)*>(shdr_fragment_.data());
  return true;
}

bool ElfReader::ReadDynamicSection() {
  // 1. Find .dynamic section (in section headers)
  const ElfW(Shdr)* dynamic_shdr = nullptr;
  for (size_t i = 0; i < shdr_num_; ++i) {
    if (shdr_table_[i].sh_type == SHT_DYNAMIC) {
      dynamic_shdr = &shdr_table_ [i];
      break;
    }
  }

  if (dynamic_shdr == nullptr) {
    DL_ERR_AND_LOG("\"%s\" .dynamic section header was not found", name_.c_str());
    return false;
  }

  // Make sure dynamic_shdr offset and size matches PT_DYNAMIC phdr
  size_t pt_dynamic_offset = 0;
  size_t pt_dynamic_filesz = 0;
  for (size_t i = 0; i < phdr_num_; ++i) {
    const ElfW(Phdr)* phdr = &phdr_table_[i];
    if (phdr->p_type == PT_DYNAMIC) {
      pt_dynamic_offset = phdr->p_offset;
      pt_dynamic_filesz = phdr->p_filesz;
    }
  }

  if (pt_dynamic_offset != dynamic_shdr->sh_offset) {
    if (get_application_target_sdk_version() >= 26) {
      DL_ERR_AND_LOG("\"%s\" .dynamic section has invalid offset: 0x%zx, "
                     "expected to match PT_DYNAMIC offset: 0x%zx",
                     name_.c_str(),
                     static_cast<size_t>(dynamic_shdr->sh_offset),
                     pt_dynamic_offset);
      return false;
    }
    DL_WARN_documented_change(26,
                              "invalid-elf-header_section-headers-enforced-for-api-level-26",
                              "\"%s\" .dynamic section has invalid offset: 0x%zx "
                              "(expected to match PT_DYNAMIC offset 0x%zx)",
                              name_.c_str(),
                              static_cast<size_t>(dynamic_shdr->sh_offset),
                              pt_dynamic_offset);
    add_dlwarning(name_.c_str(), "invalid .dynamic section");
  }

  if (pt_dynamic_filesz != dynamic_shdr->sh_size) {
    if (get_application_target_sdk_version() >= 26) {
      DL_ERR_AND_LOG("\"%s\" .dynamic section has invalid size: 0x%zx, "
                     "expected to match PT_DYNAMIC filesz: 0x%zx",
                     name_.c_str(),
                     static_cast<size_t>(dynamic_shdr->sh_size),
                     pt_dynamic_filesz);
      return false;
    }
    DL_WARN_documented_change(26,
                              "invalid-elf-header_section-headers-enforced-for-api-level-26",
                              "\"%s\" .dynamic section has invalid size: 0x%zx "
                              "(expected to match PT_DYNAMIC filesz 0x%zx)",
                              name_.c_str(),
                              static_cast<size_t>(dynamic_shdr->sh_size),
                              pt_dynamic_filesz);
    add_dlwarning(name_.c_str(), "invalid .dynamic section");
  }

  if (dynamic_shdr->sh_link >= shdr_num_) {
    DL_ERR_AND_LOG("\"%s\" .dynamic section has invalid sh_link: %d",
                   name_.c_str(),
                   dynamic_shdr->sh_link);
    return false;
  }

  const ElfW(Shdr)* strtab_shdr = &shdr_table_[dynamic_shdr->sh_link];

  if (strtab_shdr->sh_type != SHT_STRTAB) {
    DL_ERR_AND_LOG("\"%s\" .dynamic section has invalid link(%d) sh_type: %d (expected SHT_STRTAB)",
                   name_.c_str(), dynamic_shdr->sh_link, strtab_shdr->sh_type);
    return false;
  }

  if (!CheckFileRange(dynamic_shdr->sh_offset, dynamic_shdr->sh_size, alignof(const ElfW(Dyn)))) {
    DL_ERR_AND_LOG("\"%s\" has invalid offset/size of .dynamic section", name_.c_str());
    return false;
  }

  if (!dynamic_fragment_.Map(fd_, file_offset_, dynamic_shdr->sh_offset, dynamic_shdr->sh_size)) {
    DL_ERR("\"%s\" dynamic section mmap failed: %s", name_.c_str(), strerror(errno));
    return false;
  }

  dynamic_ = static_cast<const ElfW(Dyn)*>(dynamic_fragment_.data());

  if (!CheckFileRange(strtab_shdr->sh_offset, strtab_shdr->sh_size, alignof(const char))) {
    DL_ERR_AND_LOG("\"%s\" has invalid offset/size of the .strtab section linked from .dynamic section",
                   name_.c_str());
    return false;
  }

  if (!strtab_fragment_.Map(fd_, file_offset_, strtab_shdr->sh_offset, strtab_shdr->sh_size)) {
    DL_ERR("\"%s\" strtab section mmap failed: %s", name_.c_str(), strerror(errno));
    return false;
  }

  strtab_ = static_cast<const char*>(strtab_fragment_.data());
  strtab_size_ = strtab_fragment_.size();
  return true;
}

/* Returns the size of the extent of all the possibly non-contiguous
 * loadable segments in an ELF program header table. This corresponds
 * to the page-aligned size in bytes that needs to be reserved in the
 * process' address space. If there are no loadable segments, 0 is
 * returned.
 *
 * If out_min_vaddr or out_max_vaddr are not null, they will be
 * set to the minimum and maximum addresses of pages to be reserved,
 * or 0 if there is nothing to load.
 */
size_t phdr_table_get_load_size(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                ElfW(Addr)* out_min_vaddr,
                                ElfW(Addr)* out_max_vaddr) {
  ElfW(Addr) min_vaddr = UINTPTR_MAX;
  ElfW(Addr) max_vaddr = 0;

  bool found_pt_load = false;
  for (size_t i = 0; i < phdr_count; ++i) {
    const ElfW(Phdr)* phdr = &phdr_table[i];

    if (phdr->p_type != PT_LOAD) {
      continue;
    }
    found_pt_load = true;

    if (phdr->p_vaddr < min_vaddr) {
      min_vaddr = phdr->p_vaddr;
    }

    if (phdr->p_vaddr + phdr->p_memsz > max_vaddr) {
      max_vaddr = phdr->p_vaddr + phdr->p_memsz;
    }
  }
  if (!found_pt_load) {
    min_vaddr = 0;
  }

  min_vaddr = page_start(min_vaddr);
  max_vaddr = page_end(max_vaddr);

  if (out_min_vaddr != nullptr) {
    *out_min_vaddr = min_vaddr;
  }
  if (out_max_vaddr != nullptr) {
    *out_max_vaddr = max_vaddr;
  }
  return max_vaddr - min_vaddr;
}

// Returns the maximum p_align associated with a loadable segment in the ELF
// program header table. Used to determine whether the file should be loaded at
// a specific virtual address alignment for use with huge pages.
size_t phdr_table_get_maximum_alignment(const ElfW(Phdr)* phdr_table, size_t phdr_count) {
  size_t maximum_alignment = page_size();

  for (size_t i = 0; i < phdr_count; ++i) {
    const ElfW(Phdr)* phdr = &phdr_table[i];

    // p_align must be 0, 1, or a positive, integral power of two.
    if (phdr->p_type != PT_LOAD || ((phdr->p_align & (phdr->p_align - 1)) != 0)) {
      continue;
    }

    if (phdr->p_align > maximum_alignment) {
      maximum_alignment = phdr->p_align;
    }
  }

#if defined(__LP64__)
  return maximum_alignment;
#else
  return page_size();
#endif
}

// Reserve a virtual address range such that if it's limits were extended to the next 2**align
// boundary, it would not overlap with any existing mappings.
static void* ReserveWithAlignmentPadding(size_t size, size_t mapping_align, size_t start_align,
                                         void** out_gap_start, size_t* out_gap_size) {
  int mmap_flags = MAP_PRIVATE | MAP_ANONYMOUS;
  // Reserve enough space to properly align the library's start address.
  mapping_align = std::max(mapping_align, start_align);
  if (mapping_align == page_size()) {
    void* mmap_ptr = mmap(nullptr, size, PROT_NONE, mmap_flags, -1, 0);
    if (mmap_ptr == MAP_FAILED) {
      return nullptr;
    }
    return mmap_ptr;
  }

  // Minimum alignment of shared library gap. For efficiency, this should match the second level
  // page size of the platform.
#if defined(__LP64__)
  constexpr size_t kGapAlignment = 1ul << 21;  // 2MB
#else
  constexpr size_t kGapAlignment = 0;
#endif
  // Maximum gap size, in the units of kGapAlignment.
  constexpr size_t kMaxGapUnits = 32;
  // Allocate enough space so that the end of the desired region aligned up is still inside the
  // mapping.
  size_t mmap_size = align_up(size, mapping_align) + mapping_align - page_size();
  uint8_t* mmap_ptr =
      reinterpret_cast<uint8_t*>(mmap(nullptr, mmap_size, PROT_NONE, mmap_flags, -1, 0));
  if (mmap_ptr == MAP_FAILED) {
    return nullptr;
  }
  size_t gap_size = 0;
  size_t first_byte = reinterpret_cast<size_t>(align_up(mmap_ptr, mapping_align));
  size_t last_byte = reinterpret_cast<size_t>(align_down(mmap_ptr + mmap_size, mapping_align) - 1);
  if (kGapAlignment && first_byte / kGapAlignment != last_byte / kGapAlignment) {
    // This library crosses a 2MB boundary and will fragment a new huge page.
    // Lets take advantage of that and insert a random number of inaccessible huge pages before that
    // to improve address randomization and make it harder to locate this library code by probing.
    munmap(mmap_ptr, mmap_size);
    mapping_align = std::max(mapping_align, kGapAlignment);
    gap_size =
        kGapAlignment * (is_first_stage_init() ? 1 : arc4random_uniform(kMaxGapUnits - 1) + 1);
    mmap_size = align_up(size + gap_size, mapping_align) + mapping_align - page_size();
    mmap_ptr = reinterpret_cast<uint8_t*>(mmap(nullptr, mmap_size, PROT_NONE, mmap_flags, -1, 0));
    if (mmap_ptr == MAP_FAILED) {
      return nullptr;
    }
  }

  uint8_t *gap_end, *gap_start;
  if (gap_size) {
    gap_end = align_down(mmap_ptr + mmap_size, kGapAlignment);
    gap_start = gap_end - gap_size;
  } else {
    gap_start = gap_end = mmap_ptr + mmap_size;
  }

  uint8_t* first = align_up(mmap_ptr, mapping_align);
  uint8_t* last = align_down(gap_start, mapping_align) - size;

  // arc4random* is not available in first stage init because /dev/urandom hasn't yet been
  // created. Don't randomize then.
  size_t n = is_first_stage_init() ? 0 : arc4random_uniform((last - first) / start_align + 1);
  uint8_t* start = first + n * start_align;
  // Unmap the extra space around the allocation.
  // Keep it mapped PROT_NONE on 64-bit targets where address space is plentiful to make it harder
  // to defeat ASLR by probing for readable memory mappings.
  munmap(mmap_ptr, start - mmap_ptr);
  munmap(start + size, gap_start - (start + size));
  if (gap_end != mmap_ptr + mmap_size) {
    munmap(gap_end, mmap_ptr + mmap_size - gap_end);
  }
  *out_gap_start = gap_start;
  *out_gap_size = gap_size;
  return start;
}

// Reserve a virtual address range big enough to hold all loadable
// segments of a program header table. This is done by creating a
// private anonymous mmap() with PROT_NONE.
bool ElfReader::ReserveAddressSpace(address_space_params* address_space) {
  ElfW(Addr) min_vaddr;
  load_size_ = phdr_table_get_load_size(phdr_table_, phdr_num_, &min_vaddr);
  if (load_size_ == 0) {
    DL_ERR("\"%s\" has no loadable segments", name_.c_str());
    return false;
  }

  uint8_t* addr = reinterpret_cast<uint8_t*>(min_vaddr);
  void* start;

  if (load_size_ > address_space->reserved_size) {
    if (address_space->must_use_address) {
      DL_ERR("reserved address space %zd smaller than %zd bytes needed for \"%s\"",
             load_size_ - address_space->reserved_size, load_size_, name_.c_str());
      return false;
    }
    size_t start_alignment = page_size();
    if (get_transparent_hugepages_supported() && get_application_target_sdk_version() >= 31) {
      size_t maximum_alignment = phdr_table_get_maximum_alignment(phdr_table_, phdr_num_);
      // Limit alignment to PMD size as other alignments reduce the number of
      // bits available for ASLR for no benefit.
      start_alignment = maximum_alignment == kPmdSize ? kPmdSize : page_size();
    }
    start = ReserveWithAlignmentPadding(load_size_, kLibraryAlignment, start_alignment, &gap_start_,
                                        &gap_size_);
    if (start == nullptr) {
      DL_ERR("couldn't reserve %zd bytes of address space for \"%s\"", load_size_, name_.c_str());
      return false;
    }
  } else {
    start = address_space->start_addr;
    gap_start_ = nullptr;
    gap_size_ = 0;
    mapped_by_caller_ = true;

    // Update the reserved address space to subtract the space used by this library.
    address_space->start_addr = reinterpret_cast<uint8_t*>(address_space->start_addr) + load_size_;
    address_space->reserved_size -= load_size_;
  }

  load_start_ = start;
  load_bias_ = reinterpret_cast<uint8_t*>(start) - addr;
  return true;
}

static bool is_misaligned(const ElfW(Phdr)* phdr, size_t phdr_count, off64_t file_offset) {
  const ElfW(Phdr)* phdr_limit = phdr + phdr_count;

  if (file_offset % page_size() != 0) {
    return true;
  }

  for (; phdr < phdr_limit; ++phdr) {
    if (phdr->p_type == PT_LOAD && phdr->p_align < page_size()) {
      return true;
    }
  }
  return false;
}

bool ElfReader::LoadSegments() {
  bool misaligned = is_misaligned(phdr_table_, phdr_num_, file_offset_);
  ElfW(Addr) start = 0;
  size_t full_size = phdr_table_get_load_size(phdr_table_, phdr_num_, &start);

  if (misaligned) {
    if (!full_size) {
      DL_ERR("Can't find misaligned size for \"%s\"", name_.c_str());
      return false;
    }
    void* map_addr = mmap(reinterpret_cast<void*>(start + load_bias_),
                          full_size,
                          PROT_READ|PROT_WRITE|PROT_EXEC,
                          MAP_FIXED|MAP_PRIVATE|MAP_ANONYMOUS,
                          -1,
                          0);
    if (map_addr == MAP_FAILED) {
      DL_ERR("couldn't map misaligned segments for \"%s\": %s",
             name_.c_str(), strerror(errno));
      return false;
    }
  }

  for (size_t i = 0; i < phdr_num_; ++i) {
    const ElfW(Phdr)* phdr = &phdr_table_[i];

    if (phdr->p_type != PT_LOAD) {
      continue;
    }

    // Segment addresses in memory.
    ElfW(Addr) seg_start = phdr->p_vaddr + load_bias_;
    ElfW(Addr) seg_end   = seg_start + phdr->p_memsz;
    ElfW(Addr) seg_page_start = page_start(seg_start);
    ElfW(Addr) seg_page_end = page_end(seg_end);
    ElfW(Addr) seg_file_end   = seg_start + phdr->p_filesz;

    // File offsets.
    ElfW(Addr) file_start = phdr->p_offset;
    ElfW(Addr) file_end   = file_start + phdr->p_filesz;

    ElfW(Addr) file_page_start = page_start(file_start);
    ElfW(Addr) file_length = file_end - file_page_start;

    if (file_size_ <= 0) {
      DL_ERR("\"%s\" invalid file size: %" PRId64, name_.c_str(), file_size_);
      return false;
    }

    if (file_end > static_cast<size_t>(file_size_)) {
      DL_ERR("invalid ELF file \"%s\" load segment[%zd]:"
          " p_offset (%p) + p_filesz (%p) ( = %p) past end of file (0x%" PRIx64 ")",
          name_.c_str(), i, reinterpret_cast<void*>(phdr->p_offset),
          reinterpret_cast<void*>(phdr->p_filesz),
          reinterpret_cast<void*>(file_end), file_size_);
      return false;
    }

    if (file_length != 0) {
      int prot = PFLAGS_TO_PROT(phdr->p_flags);
      if ((prot & (PROT_EXEC | PROT_WRITE)) == (PROT_EXEC | PROT_WRITE)) {
        // W + E PT_LOAD segments are not allowed in O.
        if (get_application_target_sdk_version() >= 26) {
          DL_ERR_AND_LOG("\"%s\": W+E load segments are not allowed", name_.c_str());
          return false;
        }
        DL_WARN_documented_change(26,
                                  "writable-and-executable-segments-enforced-for-api-level-26",
                                  "\"%s\" has load segments that are both writable and executable",
                                  name_.c_str());
        add_dlwarning(name_.c_str(), "W+E load segments");
      }

      void* seg_addr;
      if (!misaligned) {
        seg_addr = mmap64(reinterpret_cast<void*>(seg_page_start),
                          file_length,
                          prot,
                          MAP_FIXED|MAP_PRIVATE,
                          fd_,
                          file_offset_ + file_page_start);
        if (seg_addr == MAP_FAILED) {
          DL_ERR("couldn't map \"%s\" segment %zd: %s", name_.c_str(), i, strerror(errno));
          return false;
        }
      } else {
        off64_t file_offset = page_start(file_offset_);
        ElfW(Addr) file_correction = file_offset_ - file_offset;
        seg_addr = mmap64(nullptr,
                          file_length + file_correction,
                          prot,
                          MAP_PRIVATE,
                          fd_,
                          file_offset + file_page_start);
        if (seg_addr == MAP_FAILED) {
          DL_ERR("couldn't map \"%s\" segment %zd: %s", name_.c_str(), i, strerror(errno));
          return false;
        }
        char* seg_ptr = reinterpret_cast<char*>(seg_addr) + (file_start - file_page_start);
        memcpy(reinterpret_cast<void*>(seg_start),
               reinterpret_cast<void*>(seg_ptr + file_correction),
               phdr->p_filesz);
        munmap(seg_addr, file_length + file_correction);
        seg_addr = reinterpret_cast<void*>(seg_page_start);
      }

      // Mark segments as huge page eligible if they meet the requirements
      // (executable and PMD aligned).
      if ((phdr->p_flags & PF_X) && phdr->p_align == kPmdSize &&
          get_transparent_hugepages_supported()) {
        madvise(seg_addr, file_length, MADV_HUGEPAGE);
      }
    }

    // if the segment is writable, and does not end on a page boundary,
    // zero-fill it until the page limit.
    if (!misaligned && ((phdr->p_flags & PF_W) != 0 && page_offset(seg_file_end) > 0)) {
      memset(reinterpret_cast<void*>(seg_file_end), 0, page_size() - page_offset(seg_file_end));
    }

    seg_file_end = page_end(seg_file_end);

    // seg_file_end is now the first page address after the file
    // content. If seg_end is larger, we need to zero anything
    // between them. This is done by using a private anonymous
    // map for all extra pages.
    if (!misaligned && seg_page_end > seg_file_end) {
      size_t zeromap_size = seg_page_end - seg_file_end;
      void* zeromap = mmap(reinterpret_cast<void*>(seg_file_end),
                           zeromap_size,
                           PFLAGS_TO_PROT(phdr->p_flags),
                           MAP_FIXED|MAP_ANONYMOUS|MAP_PRIVATE,
                           -1,
                           0);
      if (zeromap == MAP_FAILED) {
        DL_ERR("couldn't zero fill \"%s\" gap: %s", name_.c_str(), strerror(errno));
        return false;
      }

      prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, zeromap, zeromap_size, ".bss");
    }
  }

  // Set protection for copied misaligned segments.
  if (misaligned) {
    ElfW(Addr) prev_seg_page_start = 0;
    ElfW(Addr) prev_seg_page_end = 0;
    int prev_prot = 0;

    for (size_t i = 0; i < phdr_num_; ++i) {
      const ElfW(Phdr)* phdr = &phdr_table_[i];

      if (phdr->p_type != PT_LOAD) {
        continue;
      }

      ElfW(Addr) seg_start = phdr->p_vaddr + load_bias_;
      ElfW(Addr) seg_end   = seg_start + phdr->p_memsz;
      ElfW(Addr) seg_page_start = page_start(seg_start);
      ElfW(Addr) seg_page_end = page_end(seg_end);

      ElfW(Addr) orig_code_seg_page_start = 0;
      ElfW(Addr) orig_code_seg_page_end = 0;

      int orig_prot = PFLAGS_TO_PROT(phdr->p_flags);
      int approved_prot = orig_prot;

#ifdef CODE_COPY_HACK_TRACE
      DL_WARN("[CODE COPY HACK]:%zu. misaligned load:\"%s\" ", i, name_.c_str());
      DL_WARN("[CODE COPY HACK] %zu.seg_start: 0x%llX", i, (seg_start));
      DL_WARN("[CODE COPY HACK] %zu.seg_end: 0x%llX", i, (seg_end));
      DL_WARN("[CODE COPY HACK] %zu.prot: %d", i, orig_prot);
      DL_WARN("[CODE COPY HACK] %zu.seg_page_start: 0x%llX", i, (seg_page_start));
      DL_WARN("[CODE COPY HACK] %zu.seg_page_end: 0x%llX", i, (seg_page_end));
#endif

      //Misaligned segments should agree on most secure protections.
      if ((seg_page_start < prev_seg_page_end) &&
         (seg_page_start >= prev_seg_page_start) &&
         (prev_prot != orig_prot)) {
#ifdef CODE_COPY_HACK_TRACE
          DL_WARN("[CODE COPY HACK] overlapping segment found with conflicting prot!");
          DL_WARN("[CODE COPY HACK] prev segment 0x%llX - 0x%llX, prot:%d",
                  prev_seg_page_start, prev_seg_page_end, prev_prot);
          DL_WARN("[CODE COPY HACK] current segment 0x%llX - 0x%llX, prot:%d",
                  seg_page_start, seg_page_end, orig_prot);

#endif
        if ((prev_prot & PROT_WRITE) || (orig_prot & PROT_WRITE)) {
          approved_prot = PROT_READ | PROT_WRITE;
        } else if ((prev_prot & PROT_EXEC) || (orig_prot & PROT_EXEC)) {
          approved_prot = PROT_READ | PROT_EXEC;
        }

        if ((orig_prot & PROT_EXEC) && !(approved_prot & PROT_EXEC)) {
          orig_code_seg_page_end = seg_page_end;
        }
        if ((prev_prot & PROT_EXEC) && !(approved_prot & PROT_EXEC)) {
          orig_code_seg_page_end = prev_seg_page_end;
        }
        if (orig_code_seg_page_end) {
          orig_code_seg_page_start = page_start(orig_code_seg_page_end-1);
#ifdef CODE_COPY_HACK_TRACE
          DL_WARN("[CODE COPY HACK] segment 0x%llX - 0x%llX lost PROT_EXEC permission",
                         orig_code_seg_page_start, orig_code_seg_page_end);
#endif
          //1.mmap that region into anon
          void* copied_code_seg_page_start = mmap64(reinterpret_cast<void*>(start + full_size + load_bias_),
                      3 * page_size(),
                      PROT_NONE,
                      MAP_PRIVATE|MAP_ANONYMOUS,
                      -1,
                      0);
          if (copied_code_seg_page_start == MAP_FAILED) {
            DL_ERR("couldn't map \"%s\" segment %zd: %s", name_.c_str(), i, strerror(errno));
            return false;
          }
          copied_code_seg_page_start = reinterpret_cast<void*>(reinterpret_cast<char*>(copied_code_seg_page_start) + page_size());

          int ret = mprotect(copied_code_seg_page_start, page_size(), PROT_READ | PROT_WRITE);
          if (ret) {
            DL_ERR("mprotect failed: %d", ret);
            return false;
          }

#ifdef CODE_COPY_HACK_TRACE
          DL_WARN("[CODE COPY HACK] code segment copied from 0x%llX to 0x%llX",
                         orig_code_seg_page_start, reinterpret_cast<unsigned long long>(copied_code_seg_page_start));
#endif
          // TODO copy only the actual code part(s)
          memcpy(copied_code_seg_page_start, reinterpret_cast<void*>(orig_code_seg_page_start),
                 page_size());

          //2.fixup code
          int64_t diff =
            reinterpret_cast<unsigned long long>(orig_code_seg_page_start) -
            reinterpret_cast<unsigned long long>(copied_code_seg_page_start);
          pageMoves.insert({
            reinterpret_cast<unsigned long long>(copied_code_seg_page_start) - page_size(),
            reinterpret_cast<unsigned long long>(orig_code_seg_page_start) - page_size()});
          pageMoves.insert({
            reinterpret_cast<unsigned long long>(orig_code_seg_page_start),
            reinterpret_cast<unsigned long long>(copied_code_seg_page_start)});
          pageMoves.insert({
            reinterpret_cast<unsigned long long>(copied_code_seg_page_start) + page_size(),
            reinterpret_cast<unsigned long long>(orig_code_seg_page_start) + page_size()});

#ifdef CODE_COPY_HACK_TRACE
          DL_WARN("[CODE COPY HACK] distance %llx, %llx, %lld",
                  reinterpret_cast<unsigned long long>(orig_code_seg_page_start),
                  reinterpret_cast<unsigned long long>(copied_code_seg_page_start),
                  reinterpret_cast<unsigned long long>(orig_code_seg_page_start) -
                  reinterpret_cast<unsigned long long>(copied_code_seg_page_start));
#endif

          scan(reinterpret_cast<uint32_t *>(copied_code_seg_page_start),
                page_size() / sizeof(uint32_t), diff);

          //3.set PROT_READ | PROT_EXEC permission on the copied code
          ret = mprotect(reinterpret_cast<void*>(copied_code_seg_page_start),
                            page_size(),
                            PROT_READ | PROT_EXEC);

          prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME,
                reinterpret_cast<void*>(reinterpret_cast<char*>(copied_code_seg_page_start) - page_size()),
                page_size(), "HACK guard before");
          prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME,
                reinterpret_cast<void*>(copied_code_seg_page_start),
                page_size(), "HACK copied code segment");
          prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME,
                reinterpret_cast<void*>(reinterpret_cast<char*>(copied_code_seg_page_start) + page_size()),
                page_size(), "HACK guard after");


          if (ret < 0) {
            DL_ERR("mprotect failed: %d", ret);
            return false;
          }

          //4.setup fault handler
          static bool is_sighandler_inited = false;
          if (is_sighandler_inited == false) {
            struct sigaction act;
            act.sa_sigaction = sighandler;
            sigemptyset(&act.sa_mask);
            sigaddset(&act.sa_mask, 41);
            act.sa_flags = SA_SIGINFO;

            sigaction(SIGSEGV, &act, &old_sigsegv);
            sigaction(SIGTRAP, &act, &old_sigtrap);

            is_sighandler_inited = true;
          }
        }
      }
      prev_seg_page_start = seg_page_start;
      prev_seg_page_end = seg_page_end;
      prev_prot = approved_prot;

      int ret = mprotect(reinterpret_cast<void*>(seg_page_start),
                        seg_page_end - seg_page_start,
                        approved_prot);
      if (ret < 0) {
        DL_ERR("mprotect failed: %d", ret);
        return false;
      }
    }
  }

  return true;
}

/* Used internally. Used to set the protection bits of all loaded segments
 * with optional extra flags (i.e. really PROT_WRITE). Used by
 * phdr_table_protect_segments and phdr_table_unprotect_segments.
 */
static int _phdr_table_set_load_prot(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                     ElfW(Addr) load_bias, int extra_prot_flags) {
  const ElfW(Phdr)* phdr = phdr_table;
  const ElfW(Phdr)* phdr_limit = phdr + phdr_count;

  for (; phdr < phdr_limit; phdr++) {
    if (phdr->p_type != PT_LOAD || (phdr->p_flags & PF_W) != 0) {
      continue;
    }

    ElfW(Addr) seg_page_start = page_start(phdr->p_vaddr) + load_bias;
    ElfW(Addr) seg_page_end = page_end(phdr->p_vaddr + phdr->p_memsz) + load_bias;

    int prot = PFLAGS_TO_PROT(phdr->p_flags) | extra_prot_flags;
    if ((prot & PROT_WRITE) != 0) {
      // make sure we're never simultaneously writable / executable
      prot &= ~PROT_EXEC;
    }
#if defined(__aarch64__)
    if ((prot & PROT_EXEC) == 0) {
      // Though it is not specified don't add PROT_BTI if segment is not
      // executable.
      prot &= ~PROT_BTI;
    }
#endif

    int ret =
        mprotect(reinterpret_cast<void*>(seg_page_start), seg_page_end - seg_page_start, prot);
    if (ret < 0) {
      return -1;
    }
  }
  return 0;
}

/* Restore the original protection modes for all loadable segments.
 * You should only call this after phdr_table_unprotect_segments and
 * applying all relocations.
 *
 * AArch64: also called from linker_main and ElfReader::Load to apply
 *     PROT_BTI for loaded main so and other so-s.
 *
 * Input:
 *   phdr_table  -> program header table
 *   phdr_count  -> number of entries in tables
 *   load_bias   -> load bias
 *   prop        -> GnuPropertySection or nullptr
 * Return:
 *   0 on error, -1 on failure (error code in errno).
 */
int phdr_table_protect_segments(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                ElfW(Addr) load_bias, const GnuPropertySection* prop __unused) {
  int prot = 0;
#if defined(__aarch64__)
  if ((prop != nullptr) && prop->IsBTICompatible()) {
    prot |= PROT_BTI;
  }
#endif
  return _phdr_table_set_load_prot(phdr_table, phdr_count, load_bias, prot);
}

/* Change the protection of all loaded segments in memory to writable.
 * This is useful before performing relocations. Once completed, you
 * will have to call phdr_table_protect_segments to restore the original
 * protection flags on all segments.
 *
 * Note that some writable segments can also have their content turned
 * to read-only by calling phdr_table_protect_gnu_relro. This is no
 * performed here.
 *
 * Input:
 *   phdr_table  -> program header table
 *   phdr_count  -> number of entries in tables
 *   load_bias   -> load bias
 * Return:
 *   0 on error, -1 on failure (error code in errno).
 */
int phdr_table_unprotect_segments(const ElfW(Phdr)* phdr_table,
                                  size_t phdr_count, ElfW(Addr) load_bias) {
  return _phdr_table_set_load_prot(phdr_table, phdr_count, load_bias, PROT_WRITE);
}

/* Used internally by phdr_table_protect_gnu_relro and
 * phdr_table_unprotect_gnu_relro.
 */
static int _phdr_table_set_gnu_relro_prot(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                          ElfW(Addr) load_bias, int prot_flags) {
  const ElfW(Phdr)* phdr = phdr_table;
  const ElfW(Phdr)* phdr_limit = phdr + phdr_count;

  for (phdr = phdr_table; phdr < phdr_limit; phdr++) {
    if (phdr->p_type != PT_GNU_RELRO) {
      continue;
    }

    // Tricky: what happens when the relro segment does not start
    // or end at page boundaries? We're going to be over-protective
    // here and put every page touched by the segment as read-only.

    // This seems to match Ian Lance Taylor's description of the
    // feature at http://www.airs.com/blog/archives/189.

    //    Extract:
    //       Note that the current dynamic linker code will only work
    //       correctly if the PT_GNU_RELRO segment starts on a page
    //       boundary. This is because the dynamic linker rounds the
    //       p_vaddr field down to the previous page boundary. If
    //       there is anything on the page which should not be read-only,
    //       the program is likely to fail at runtime. So in effect the
    //       linker must only emit a PT_GNU_RELRO segment if it ensures
    //       that it starts on a page boundary.
    ElfW(Addr) seg_page_start = page_start(phdr->p_vaddr) + load_bias;
    ElfW(Addr) seg_page_end = page_end(phdr->p_vaddr + phdr->p_memsz) + load_bias;

    int ret = mprotect(reinterpret_cast<void*>(seg_page_start),
                       seg_page_end - seg_page_start,
                       prot_flags);
    if (ret < 0) {
      return -1;
    }
  }
  return 0;
}

/* Apply GNU relro protection if specified by the program header. This will
 * turn some of the pages of a writable PT_LOAD segment to read-only, as
 * specified by one or more PT_GNU_RELRO segments. This must be always
 * performed after relocations.
 *
 * The areas typically covered are .got and .data.rel.ro, these are
 * read-only from the program's POV, but contain absolute addresses
 * that need to be relocated before use.
 *
 * Input:
 *   phdr_table  -> program header table
 *   phdr_count  -> number of entries in tables
 *   load_bias   -> load bias
 * Return:
 *   0 on error, -1 on failure (error code in errno).
 */
int phdr_table_protect_gnu_relro(const ElfW(Phdr)* phdr_table,
                                 size_t phdr_count, ElfW(Addr) load_bias) {
  // Avoid warnings about unused parameters and functions.
  (void)phdr_table;
  (void)phdr_count;
  (void)load_bias;
  (void)&_phdr_table_set_gnu_relro_prot;
  return 0;
}

/* Serialize the GNU relro segments to the given file descriptor. This can be
 * performed after relocations to allow another process to later share the
 * relocated segment, if it was loaded at the same address.
 *
 * Input:
 *   phdr_table  -> program header table
 *   phdr_count  -> number of entries in tables
 *   load_bias   -> load bias
 *   fd          -> writable file descriptor to use
 *   file_offset -> pointer to offset into file descriptor to use/update
 * Return:
 *   0 on error, -1 on failure (error code in errno).
 */
int phdr_table_serialize_gnu_relro(const ElfW(Phdr)* phdr_table,
                                   size_t phdr_count,
                                   ElfW(Addr) load_bias,
                                   int fd,
                                   size_t* file_offset) {
  const ElfW(Phdr)* phdr = phdr_table;
  const ElfW(Phdr)* phdr_limit = phdr + phdr_count;

  for (phdr = phdr_table; phdr < phdr_limit; phdr++) {
    if (phdr->p_type != PT_GNU_RELRO) {
      continue;
    }

    ElfW(Addr) seg_page_start = page_start(phdr->p_vaddr) + load_bias;
    ElfW(Addr) seg_page_end = page_end(phdr->p_vaddr + phdr->p_memsz) + load_bias;
    ssize_t size = seg_page_end - seg_page_start;

    ssize_t written = TEMP_FAILURE_RETRY(write(fd, reinterpret_cast<void*>(seg_page_start), size));
    if (written != size) {
      return -1;
    }
    void* map = mmap(reinterpret_cast<void*>(seg_page_start), size, PROT_READ,
                     MAP_PRIVATE|MAP_FIXED, fd, *file_offset);
    if (map == MAP_FAILED) {
      return -1;
    }
    *file_offset += size;
  }
  return 0;
}

/* Where possible, replace the GNU relro segments with mappings of the given
 * file descriptor. This can be performed after relocations to allow a file
 * previously created by phdr_table_serialize_gnu_relro in another process to
 * replace the dirty relocated pages, saving memory, if it was loaded at the
 * same address. We have to compare the data before we map over it, since some
 * parts of the relro segment may not be identical due to other libraries in
 * the process being loaded at different addresses.
 *
 * Input:
 *   phdr_table  -> program header table
 *   phdr_count  -> number of entries in tables
 *   load_bias   -> load bias
 *   fd          -> readable file descriptor to use
 *   file_offset -> pointer to offset into file descriptor to use/update
 * Return:
 *   0 on error, -1 on failure (error code in errno).
 */
int phdr_table_map_gnu_relro(const ElfW(Phdr)* phdr_table,
                             size_t phdr_count,
                             ElfW(Addr) load_bias,
                             int fd,
                             size_t* file_offset) {
  // Map the file at a temporary location so we can compare its contents.
  struct stat file_stat;
  if (TEMP_FAILURE_RETRY(fstat(fd, &file_stat)) != 0) {
    return -1;
  }
  off_t file_size = file_stat.st_size;
  void* temp_mapping = nullptr;
  if (file_size > 0) {
    temp_mapping = mmap(nullptr, file_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (temp_mapping == MAP_FAILED) {
      return -1;
    }
  }

  // Iterate over the relro segments and compare/remap the pages.
  const ElfW(Phdr)* phdr = phdr_table;
  const ElfW(Phdr)* phdr_limit = phdr + phdr_count;

  for (phdr = phdr_table; phdr < phdr_limit; phdr++) {
    if (phdr->p_type != PT_GNU_RELRO) {
      continue;
    }

    ElfW(Addr) seg_page_start = page_start(phdr->p_vaddr) + load_bias;
    ElfW(Addr) seg_page_end = page_end(phdr->p_vaddr + phdr->p_memsz) + load_bias;

    char* file_base = static_cast<char*>(temp_mapping) + *file_offset;
    char* mem_base = reinterpret_cast<char*>(seg_page_start);
    size_t match_offset = 0;
    size_t size = seg_page_end - seg_page_start;

    if (file_size - *file_offset < size) {
      // File is too short to compare to this segment. The contents are likely
      // different as well (it's probably for a different library version) so
      // just don't bother checking.
      break;
    }

    while (match_offset < size) {
      // Skip over dissimilar pages.
      while (match_offset < size &&
             memcmp(mem_base + match_offset, file_base + match_offset, page_size()) != 0) {
        match_offset += page_size();
      }

      // Count similar pages.
      size_t mismatch_offset = match_offset;
      while (mismatch_offset < size &&
             memcmp(mem_base + mismatch_offset, file_base + mismatch_offset, page_size()) == 0) {
        mismatch_offset += page_size();
      }

      // Map over similar pages.
      if (mismatch_offset > match_offset) {
        void* map = mmap(mem_base + match_offset, mismatch_offset - match_offset,
                         PROT_READ, MAP_PRIVATE|MAP_FIXED, fd, *file_offset + match_offset);
        if (map == MAP_FAILED) {
          munmap(temp_mapping, file_size);
          return -1;
        }
      }

      match_offset = mismatch_offset;
    }

    // Add to the base file offset in case there are multiple relro segments.
    *file_offset += size;
  }
  munmap(temp_mapping, file_size);
  return 0;
}


#if defined(__arm__)

#  ifndef PT_ARM_EXIDX
#    define PT_ARM_EXIDX    0x70000001      /* .ARM.exidx segment */
#  endif

/* Return the address and size of the .ARM.exidx section in memory,
 * if present.
 *
 * Input:
 *   phdr_table  -> program header table
 *   phdr_count  -> number of entries in tables
 *   load_bias   -> load bias
 * Output:
 *   arm_exidx       -> address of table in memory (null on failure).
 *   arm_exidx_count -> number of items in table (0 on failure).
 * Return:
 *   0 on error, -1 on failure (_no_ error code in errno)
 */
int phdr_table_get_arm_exidx(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                             ElfW(Addr) load_bias,
                             ElfW(Addr)** arm_exidx, size_t* arm_exidx_count) {
  const ElfW(Phdr)* phdr = phdr_table;
  const ElfW(Phdr)* phdr_limit = phdr + phdr_count;

  for (phdr = phdr_table; phdr < phdr_limit; phdr++) {
    if (phdr->p_type != PT_ARM_EXIDX) {
      continue;
    }

    *arm_exidx = reinterpret_cast<ElfW(Addr)*>(load_bias + phdr->p_vaddr);
    *arm_exidx_count = phdr->p_memsz / 8;
    return 0;
  }
  *arm_exidx = nullptr;
  *arm_exidx_count = 0;
  return -1;
}
#endif

/* Return the address and size of the ELF file's .dynamic section in memory,
 * or null if missing.
 *
 * Input:
 *   phdr_table  -> program header table
 *   phdr_count  -> number of entries in tables
 *   load_bias   -> load bias
 * Output:
 *   dynamic       -> address of table in memory (null on failure).
 *   dynamic_flags -> protection flags for section (unset on failure)
 * Return:
 *   void
 */
void phdr_table_get_dynamic_section(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                    ElfW(Addr) load_bias, ElfW(Dyn)** dynamic,
                                    ElfW(Word)* dynamic_flags) {
  *dynamic = nullptr;
  for (size_t i = 0; i<phdr_count; ++i) {
    const ElfW(Phdr)& phdr = phdr_table[i];
    if (phdr.p_type == PT_DYNAMIC) {
      *dynamic = reinterpret_cast<ElfW(Dyn)*>(load_bias + phdr.p_vaddr);
      if (dynamic_flags) {
        *dynamic_flags = phdr.p_flags;
      }
      return;
    }
  }
}

/* Return the program interpreter string, or nullptr if missing.
 *
 * Input:
 *   phdr_table  -> program header table
 *   phdr_count  -> number of entries in tables
 *   load_bias   -> load bias
 * Return:
 *   pointer to the program interpreter string.
 */
const char* phdr_table_get_interpreter_name(const ElfW(Phdr)* phdr_table, size_t phdr_count,
                                            ElfW(Addr) load_bias) {
  for (size_t i = 0; i<phdr_count; ++i) {
    const ElfW(Phdr)& phdr = phdr_table[i];
    if (phdr.p_type == PT_INTERP) {
      return reinterpret_cast<const char*>(load_bias + phdr.p_vaddr);
    }
  }
  return nullptr;
}

// Sets loaded_phdr_ to the address of the program header table as it appears
// in the loaded segments in memory. This is in contrast with phdr_table_,
// which is temporary and will be released before the library is relocated.
bool ElfReader::FindPhdr() {
  const ElfW(Phdr)* phdr_limit = phdr_table_ + phdr_num_;

  // If there is a PT_PHDR, use it directly.
  for (const ElfW(Phdr)* phdr = phdr_table_; phdr < phdr_limit; ++phdr) {
    if (phdr->p_type == PT_PHDR) {
      return CheckPhdr(load_bias_ + phdr->p_vaddr);
    }
  }

  // Otherwise, check the first loadable segment. If its file offset
  // is 0, it starts with the ELF header, and we can trivially find the
  // loaded program header from it.
  for (const ElfW(Phdr)* phdr = phdr_table_; phdr < phdr_limit; ++phdr) {
    if (phdr->p_type == PT_LOAD) {
      if (phdr->p_offset == 0) {
        ElfW(Addr)  elf_addr = load_bias_ + phdr->p_vaddr;
        const ElfW(Ehdr)* ehdr = reinterpret_cast<const ElfW(Ehdr)*>(elf_addr);
        ElfW(Addr)  offset = ehdr->e_phoff;
        return CheckPhdr(reinterpret_cast<ElfW(Addr)>(ehdr) + offset);
      }
      break;
    }
  }

  DL_ERR("can't find loaded phdr for \"%s\"", name_.c_str());
  return false;
}

// Tries to find .note.gnu.property section.
// It is not considered an error if such section is missing.
bool ElfReader::FindGnuPropertySection() {
#if defined(__aarch64__)
  note_gnu_property_ = GnuPropertySection(phdr_table_, phdr_num_, load_start(), name_.c_str());
#endif
  return true;
}

// Ensures that our program header is actually within a loadable
// segment. This should help catch badly-formed ELF files that
// would cause the linker to crash later when trying to access it.
bool ElfReader::CheckPhdr(ElfW(Addr) loaded) {
  const ElfW(Phdr)* phdr_limit = phdr_table_ + phdr_num_;
  ElfW(Addr) loaded_end = loaded + (phdr_num_ * sizeof(ElfW(Phdr)));
  for (const ElfW(Phdr)* phdr = phdr_table_; phdr < phdr_limit; ++phdr) {
    if (phdr->p_type != PT_LOAD) {
      continue;
    }
    ElfW(Addr) seg_start = phdr->p_vaddr + load_bias_;
    ElfW(Addr) seg_end = phdr->p_filesz + seg_start;
    if (seg_start <= loaded && loaded_end <= seg_end) {
      loaded_phdr_ = reinterpret_cast<const ElfW(Phdr)*>(loaded);
      return true;
    }
  }
  DL_ERR("\"%s\" loaded phdr %p not in loadable segment",
         name_.c_str(), reinterpret_cast<void*>(loaded));
  return false;
}
