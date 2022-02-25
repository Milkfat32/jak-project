#pragma once

#include <cstring>
#include <cmath>

#include "common/common_types.h"
#include "game/mips2c/mips2c_table.h"
#include "common/util/BitUtils.h"
#include "third-party/fmt/core.h"
#include "common/util/Assert.h"
#include "common/dma/dma.h"
#include "game/common/vu.h"

// This file contains utility functions for code generated by the mips2c pass.
// This is only useful for

extern u8* g_ee_main_mem;

extern "C" {
u64 _call_goal8_asm_linux(void* func, u64* arg_array, u64 zero, u64 pp, u64 st, void* off);
u64 _call_goal8_asm_win32(void* func, u64* arg_array, u64 zero, u64 pp, u64 st, void* off);
}

namespace Mips2C {

// nicknames for GPRs
enum Gpr {
  r0 = 0,  // hardcoded to zero
  at = 1,  // temp, not used by GOAL compiler, but used by GOAL's kernel inline assembly (an other
  // places?)
  v0 = 2,   // return, temp
  v1 = 3,   // temp
  a0 = 4,   // arg0, temp
  a1 = 5,   // arg1, temp
  a2 = 6,   // arg2, temp
  a3 = 7,   // arg3, temp
  t0 = 8,   // arg4, temp
  t1 = 9,   // arg5, temp
  t2 = 10,  // arg6, temp
  t3 = 11,  // arg7, temp
  t4 = 12,  // temp
  t5 = 13,  // temp
  t6 = 14,  // temp
  t7 = 15,  // temp
  s0 = 16,  // saved
  s1 = 17,  // saved
  s2 = 18,  // saved
  s3 = 19,  // saved
  s4 = 20,  // saved
  s5 = 21,  // saved
  s6 = 22,  // process pointer
  s7 = 23,  // symbol table
  t8 = 24,  // temp
  t9 = 25,  // function pointer
  k0 = 26,  // reserved
  k1 = 27,  // reserved
  gp = 28,  // saved (C code uses this a global pointer)
  sp = 29,  // stack pointer
  fp = 30,  // global pointer (address of current function)
  ra = 31,  // return address
  MAX_GPR = 32
};

enum VfName {
  vf00 = 0,
  vf0 = 0,
  vf1 = 1,
  vf01 = 1,
  vf2 = 2,
  vf02 = 2,
  vf3 = 3,
  vf03 = 3,
  vf4 = 4,
  vf04 = 4,
  vf5 = 5,
  vf05 = 5,
  vf6 = 6,
  vf06 = 6,
  vf7 = 7,
  vf07 = 7,
  vf8 = 8,
  vf08 = 8,
  vf9 = 9,
  vf09 = 9,
  vf10 = 10,
  vf11 = 11,
  vf12 = 12,
  vf13 = 13,
  vf14 = 14,
  vf15 = 15,
  vf16 = 16,
  vf17 = 17,
  vf18 = 18,
  vf19 = 19,
  vf20 = 20,
  vf21 = 21,
  vf22 = 22,
  vf23 = 23,
  vf24 = 24,
  vf25 = 25,
  vf26 = 26,
  vf27 = 27,
  vf28 = 28,
  vf29 = 29,
  vf30 = 30,
  vf31 = 31,
};

enum ViName {
  vi00 = 0,
  vi01 = 1,
  vi02 = 2,
  vi03 = 3,
  vi04 = 4,
  vi05 = 5,
  vi06 = 6,
  vi07 = 7,
  vi08 = 8,
  vi09 = 9,
  vi10 = 10,
  vi11 = 11,
  vi12 = 12,
  vi13 = 13,
  vi14 = 14,
};

enum FprName {
  f0 = 0,
  f1 = 1,
  f2 = 2,
  f3 = 3,
  f4 = 4,
  f5 = 5,
  f6 = 6,
  f7 = 7,
  f8 = 8,
  f9 = 9,
  f10 = 10,
  f11 = 11,
  f12 = 12,
  f13 = 13,
  f14 = 14,
  f15 = 15,
  f16 = 16,
  f17 = 17,
  f18 = 18,
  f19 = 19,
  f20 = 20,
  f21 = 21,
  f22 = 22,
  f23 = 23,
  f24 = 24,
  f25 = 25,
  f26 = 26,
  f27 = 27,
  f28 = 28,
  f29 = 29,
  f30 = 30,
  f31 = 31,
};

// note: these are not the same as the ps2 encoding - in these the least significant bit is x.
enum class DEST {
  NONE = 0,
  x = 1,
  y = 2,
  xy = 3,
  z = 4,
  xz = 5,
  yz = 6,
  xyz = 7,
  w = 8,
  xw = 9,
  yw = 10,
  xyw = 11,
  zw = 12,
  xzw = 13,
  yzw = 14,
  xyzw = 15
};

enum class BC { x = 0, y = 1, z = 2, w = 3 };

struct Mips2c_vf {
  union {
    u64 du64[2];
    s64 ds64[2];
    u32 du32[4];
    s32 ds32[4];
    u16 du16[8];
    s16 ds16[8];
    u8 du8[16];
    s8 ds8[16];
    float f[4];
    Vf vf;
  };
};

struct Mips2c_acc {
  union {
    u64 du64[2];
    s64 ds64[2];
    u32 du32[4];
    s32 ds32[4];
    u16 du16[8];
    s16 ds16[8];
    u8 du8[16];
    s8 ds8[16];
    float f[4];
    Accumulator vf;
  };
};

struct ExecutionContext {
  // EE general purpose registers
  u128 gprs[32];
  // EE fprs
  float fprs[32];
  // VU0 vf registers
  Mips2c_vf vfs[32];
  Mips2c_acc acc;

  float Q;
  float I;

  u128 hi, lo;

  void copy_vfs_from_other(const ExecutionContext* other) {
    for (int i = 0; i < 32; i++) {
      vfs[i] = other->vfs[i];
    }
  }

  Mips2c_vf vf_src(int idx) {
    if (idx == 0) {
      Mips2c_vf result;
      result.f[0] = 0;
      result.f[1] = 0;
      result.f[2] = 0;
      result.f[3] = 1.f;
      return result;
    } else {
      return vfs[idx];
    }
  }

  u128 gpr_src(int idx) {
    if (idx == 0) {
      u128 result;
      result.du64[0] = 0;
      result.du64[1] = 0;
      return result;
    } else {
      return gprs[idx];
    }
  }

  u64 sgpr64(int idx) { return gpr_src(idx).du64[0]; }

  u32 gpr_addr(int idx) { return gpr_src(idx).du32[0]; }

  void load_symbol(int gpr, void* sym_addr) {
    s32 val;
    memcpy(&val, sym_addr, 4);
    gprs[gpr].ds64[0] = val;  // sign extend and set
  }

  void load_symbol_addr(int gpr, void* sym_addr) {
    gprs[gpr].du64[0] = ((const u8*)sym_addr) - g_ee_main_mem;
  }

  void lbu(int dst, int offset, int src) {
    u8 val;
    memcpy(&val, g_ee_main_mem + gpr_src(src).du32[0] + offset, 1);
    gprs[dst].du64[0] = val;
  }

  void lb(int dst, int offset, int src) {
    s8 val;
    memcpy(&val, g_ee_main_mem + gpr_src(src).du32[0] + offset, 1);
    gprs[dst].du64[0] = val;
  }

  void lqc2(int vf, int offset, int gpr) {
    ASSERT(((gpr_src(gpr).du32[0] + offset) & 0xf) == 0);
    memcpy(&vfs[vf], g_ee_main_mem + gpr_src(gpr).du32[0] + offset, 16);
  }

  void lwc1(int dst, int offset, int gpr) {
    memcpy(&fprs[dst], g_ee_main_mem + gpr_src(gpr).du32[0] + offset, 4);
  }

  void lw(int dst, int offset, int src) {
    s32 val;
    memcpy(&val, g_ee_main_mem + gpr_src(src).du32[0] + offset, 4);
    gprs[dst].ds64[0] = val;
  }

  void lw_float_constant(int dst, u32 src) {
    s32 val = src;
    gprs[dst].ds64[0] = val;
  }

  void lh(int dst, int offset, int src) {
    s16 val;
    memcpy(&val, g_ee_main_mem + gpr_src(src).du32[0] + offset, 2);
    gprs[dst].ds64[0] = val;
  }

  void lhu(int dst, int offset, int src) {
    u16 val;
    memcpy(&val, g_ee_main_mem + gpr_src(src).du32[0] + offset, 2);
    gprs[dst].du64[0] = val;
  }

  void lwu(int dst, int offset, int src) {
    u32 val;
    memcpy(&val, g_ee_main_mem + gpr_src(src).du32[0] + offset, 4);
    gprs[dst].du64[0] = val;
  }

  void lq(int dst, int offset, int src) {
    memcpy(&gprs[dst].du64[0], g_ee_main_mem + ((gpr_addr(src) + offset) & (~15)), 16);
  }

  void ld(int dst, int offset, int src) {
    memcpy(&gprs[dst].du64[0], g_ee_main_mem + gpr_addr(src) + offset, 8);
  }

  void sw(int src, int offset, int addr) {
    auto s = gpr_src(src);
    memcpy(g_ee_main_mem + gpr_addr(addr) + offset, &s.du32[0], 4);
  }

  void jalr(u32 addr) {
    // u64 _call_goal8_asm_linux(u64 func, u64* arg_array, u64 zero, u64 pp, u64 st, u64 off);
    u64 args[8] = {gprs[a0].du64[0], gprs[a1].du64[0], gprs[a2].du64[0], gprs[a3].du64[0],
                   gprs[t0].du64[0], gprs[t1].du64[0], gprs[t2].du64[0], gprs[t3].du64[0]};
#ifdef __linux__
    gprs[v0].du64[0] = _call_goal8_asm_linux(g_ee_main_mem + addr, args, 0, gprs[s6].du64[0],
                                             gprs[s7].du64[0], g_ee_main_mem);
#elif _WIN32
    gprs[v0].du64[0] = _call_goal8_asm_win32(g_ee_main_mem + addr, args, 0, gprs[s6].du64[0],
                                             gprs[s7].du64[0], g_ee_main_mem);
#endif
  }

  void sb(int src, int offset, int addr) {
    auto s = gpr_src(src);
    memcpy(g_ee_main_mem + gpr_addr(addr) + offset, &s.du32[0], 1);
  }

  void sh(int src, int offset, int addr) {
    auto s = gpr_src(src);
    memcpy(g_ee_main_mem + gpr_addr(addr) + offset, &s.du32[0], 2);
  }

  void sd(int src, int offset, int addr) {
    auto s = gpr_src(src);
    memcpy(g_ee_main_mem + gpr_addr(addr) + offset, &s.du32[0], 8);
  }

  void sq(int src, int offset, int addr) {
    auto s = gpr_src(src);
    ASSERT((offset & 15) == 0);
    memcpy(g_ee_main_mem + gpr_addr(addr) + offset, &s.du32[0], 16);
  }

  void sqc2(int src, int offset, int addr) {
    auto s = vf_src(src);
    ASSERT(((gpr_addr(addr) + offset) & 0xf) == 0);
    memcpy(g_ee_main_mem + gpr_addr(addr) + offset, &s.du32[0], 16);
  }

  void swc1(int src, int offset, int addr) {
    memcpy(g_ee_main_mem + gpr_addr(addr) + offset, &fprs[src], 4);
  }

  void vadd_bc(DEST mask, BC bc, int dest, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dest].f[i] = s0.f[i] + s1.f[(int)bc];
      }
    }
  }

  void vmini_bc(DEST mask, BC bc, int dest, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dest].f[i] = std::min(s0.f[i], s1.f[(int)bc]);
      }
    }
  }

  void vmax_bc(DEST mask, BC bc, int dest, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dest].f[i] = std::max(s0.f[i], s1.f[(int)bc]);
      }
    }
  }

  void pextuh(int dst, int src0, int src1) {
    auto s0 = gpr_src(src0);
    auto s1 = gpr_src(src1);
    gprs[dst].du16[0] = s1.du16[4];
    gprs[dst].du16[1] = s0.du16[4];
    gprs[dst].du16[2] = s1.du16[5];
    gprs[dst].du16[3] = s0.du16[5];
    gprs[dst].du16[4] = s1.du16[6];
    gprs[dst].du16[5] = s0.du16[6];
    gprs[dst].du16[6] = s1.du16[7];
    gprs[dst].du16[7] = s0.du16[7];
  }

  void pextuw(int dst, int src0, int src1) {
    auto s0 = gpr_src(src0);
    auto s1 = gpr_src(src1);
    gprs[dst].du32[0] = s1.du32[2];
    gprs[dst].du32[1] = s0.du32[2];
    gprs[dst].du32[2] = s1.du32[3];
    gprs[dst].du32[3] = s0.du32[3];
  }

  void pextlw(int dst, int src0, int src1) {
    auto s0 = gpr_src(src0);
    auto s1 = gpr_src(src1);
    gprs[dst].du32[0] = s1.du32[0];
    gprs[dst].du32[1] = s0.du32[0];
    gprs[dst].du32[2] = s1.du32[1];
    gprs[dst].du32[3] = s0.du32[1];
  }

  void pcpyud(int dst, int src0, int src1) {
    auto s0 = gpr_src(src0);
    auto s1 = gpr_src(src1);
    gprs[dst].du64[0] = s0.du64[1];
    gprs[dst].du64[1] = s1.du64[1];
  }

  void pcpyld(int dst, int src0, int src1) {
    auto s0 = gpr_src(src0);
    auto s1 = gpr_src(src1);
    gprs[dst].du64[1] = s0.du64[0];
    gprs[dst].du64[0] = s1.du64[0];
  }

  void pexew(int dst, int src) {
    auto s = gpr_src(src);
    gprs[dst].du32[0] = s.du32[2];
    gprs[dst].du32[1] = s.du32[1];
    gprs[dst].du32[2] = s.du32[0];
    gprs[dst].du32[3] = s.du32[3];
  }

  void pcgtw(int rd, int rs, int rt) {
    auto s = gpr_src(rs);
    auto t = gpr_src(rt);
    for (int i = 0; i < 4; i++) {
      if (s.ds32[i] > t.ds32[i]) {
        gprs[rd].du32[i] = 0xffffffff;
      } else {
        gprs[rd].du32[i] = 0;
      }
    }
  }

  void ppach(int rd, int rs, int rt) {
    auto s = gpr_src(rs);
    auto t = gpr_src(rt);
    gprs[rd].du16[0] = t.du16[0];
    gprs[rd].du16[1] = t.du16[2];
    gprs[rd].du16[2] = t.du16[4];
    gprs[rd].du16[3] = t.du16[6];
    gprs[rd].du16[4] = s.du16[0];
    gprs[rd].du16[5] = s.du16[2];
    gprs[rd].du16[6] = s.du16[4];
    gprs[rd].du16[7] = s.du16[6];
  }

  void pinteh(int rd, int rs, int rt) {
    auto s = gpr_src(rs);
    auto t = gpr_src(rt);
    gprs[rd].du16[0] = t.du16[0];
    gprs[rd].du16[1] = s.du16[0];
    gprs[rd].du16[2] = t.du16[2];
    gprs[rd].du16[3] = s.du16[2];
    gprs[rd].du16[4] = t.du16[4];
    gprs[rd].du16[5] = s.du16[4];
    gprs[rd].du16[6] = t.du16[6];
    gprs[rd].du16[7] = s.du16[6];
  }

  void ppacb(int rd, int rs, int rt) {
    auto s = gpr_src(rs);
    auto t = gpr_src(rt);
    gprs[rd].du8[0] = t.du8[0];
    gprs[rd].du8[1] = t.du8[2];
    gprs[rd].du8[2] = t.du8[4];
    gprs[rd].du8[3] = t.du8[6];

    gprs[rd].du8[4] = t.du8[8];
    gprs[rd].du8[5] = t.du8[10];
    gprs[rd].du8[6] = t.du8[12];
    gprs[rd].du8[7] = t.du8[14];

    gprs[rd].du8[8] = s.du8[0];
    gprs[rd].du8[9] = s.du8[2];
    gprs[rd].du8[10] = s.du8[4];
    gprs[rd].du8[11] = s.du8[6];

    gprs[rd].du8[12] = s.du8[8];
    gprs[rd].du8[13] = s.du8[10];
    gprs[rd].du8[14] = s.du8[12];
    gprs[rd].du8[15] = s.du8[14];
  }

  void paddw(int rd, int rs, int rt) {
    auto s = gpr_src(rs);
    auto t = gpr_src(rt);
    for (int i = 0; i < 4; i++) {
      gprs[rd].du32[i] = s.du32[i] + t.du32[i];
    }
  }

  void pextub(int rd, int rs, int rt) {
    auto s = gpr_src(rs);
    auto t = gpr_src(rt);
    gprs[rd].du8[0] = t.du8[8];
    gprs[rd].du8[1] = s.du8[8];
    gprs[rd].du8[2] = t.du8[9];
    gprs[rd].du8[3] = s.du8[9];
    gprs[rd].du8[4] = t.du8[10];
    gprs[rd].du8[5] = s.du8[10];
    gprs[rd].du8[6] = t.du8[11];
    gprs[rd].du8[7] = s.du8[11];
    gprs[rd].du8[8] = t.du8[12];
    gprs[rd].du8[9] = s.du8[12];
    gprs[rd].du8[10] = t.du8[13];
    gprs[rd].du8[11] = s.du8[13];
    gprs[rd].du8[12] = t.du8[14];
    gprs[rd].du8[13] = s.du8[14];
    gprs[rd].du8[14] = t.du8[15];
    gprs[rd].du8[15] = s.du8[15];
  }

  void pmulth(int rd, int rs, int rt) {
    // ASSERT(rd == 0);
    s32 temp;

    auto s = gpr_src(rs);
    auto t = gpr_src(rt);

    temp = (s32)s.ds16[0] * (s32)t.ds16[0];
    lo.du32[0] = temp;
    if (rd)
      gprs[rd].du32[0] = temp;

    temp = (s32)s.ds16[1] * (s32)t.ds16[1];
    lo.du32[1] = temp;

    temp = (s32)s.ds16[2] * (s32)t.ds16[2];
    hi.du32[0] = temp;
    if (rd)
      gprs[rd].du32[1] = temp;

    temp = (s32)s.ds16[3] * (s32)t.ds16[3];
    hi.du32[1] = temp;

    temp = (s32)s.ds16[4] * (s32)t.ds16[4];
    lo.du32[2] = temp;
    if (rd)
      gprs[rd].du32[2] = temp;

    temp = (s32)s.ds16[5] * (s32)t.ds16[5];
    lo.du32[3] = temp;

    temp = (s32)s.ds16[6] * (s32)t.ds16[6];
    hi.du32[2] = temp;
    if (rd)
      gprs[rd].du32[3] = temp;

    temp = (s32)s.ds16[7] * (s32)t.ds16[7];
    hi.du32[3] = temp;
  }

  void pmaddh(int rd, int rs, int rt) {
    s32 temp;

    auto s = gpr_src(rs);
    auto t = gpr_src(rt);
    temp = lo.du32[0] + (s32)s.ds16[0] * (s32)t.ds16[0];
    lo.du32[0] = temp;
    if (rd)
      gprs[rd].du32[0] = temp;

    temp = lo.du32[1] + (s32)s.ds16[1] * (s32)t.ds16[1];
    lo.du32[1] = temp;

    temp = hi.du32[0] + (s32)s.ds16[2] * (s32)t.ds16[2];
    hi.du32[0] = temp;
    if (rd)
      gprs[rd].du32[1] = temp;

    temp = hi.du32[1] + (s32)s.ds16[3] * (s32)t.ds16[3];
    hi.du32[1] = temp;

    temp = lo.du32[2] + (s32)s.ds16[4] * (s32)t.ds16[4];
    lo.du32[2] = temp;
    if (rd)
      gprs[rd].du32[2] = temp;

    temp = lo.du32[3] + (s32)s.ds16[5] * (s32)t.ds16[5];
    lo.du32[3] = temp;

    temp = hi.du32[2] + (s32)s.ds16[6] * (s32)t.ds16[6];
    hi.du32[2] = temp;
    if (rd)
      gprs[rd].du32[3] = temp;

    temp = hi.du32[3] + (s32)s.ds16[7] * (s32)t.ds16[7];
    hi.du32[3] = temp;
  }

  void psrlh(int dest, int src, int sa) {
    auto s = gpr_src(src);
    for (int i = 0; i < 8; i++) {
      gprs[dest].du16[i] = s.du16[i] >> (sa & 0xf);
    }
  }

  void psraw(int dest, int src, int sa) {
    auto s = gpr_src(src);
    for (int i = 0; i < 4; i++) {
      gprs[dest].du32[i] = s.ds32[i] >> (sa & 0x1f);
    }
  }

  void psllw(int dest, int src, int sa) {
    auto s = gpr_src(src);
    for (int i = 0; i < 4; i++) {
      gprs[dest].ds32[i] = s.ds32[i] << (sa & 0x1f);
    }
  }

  void prot3w(int dst, int src) {
    auto s = gpr_src(src);
    gprs[dst].du32[0] = s.du32[1];
    gprs[dst].du32[1] = s.du32[2];
    gprs[dst].du32[2] = s.du32[0];
    gprs[dst].du32[3] = s.du32[3];
  }

  void paddh(int dest, int rs, int rt) {
    auto s = gpr_src(rs);
    auto t = gpr_src(rt);
    for (int i = 0; i < 8; i++) {
      gprs[dest].du16[i] = s.du16[i] + t.du16[i];
    }
  }

  void pminh(int dest, int rs, int rt) {
    auto s = gpr_src(rs);
    auto t = gpr_src(rt);
    for (int i = 0; i < 8; i++) {
      if (s.ds16[i] < t.ds16[i])
        gprs[dest].du16[i] = s.ds16[i];
      else
        gprs[dest].du16[i] = t.ds16[i];
    }
  }

  void pand(int dst, int rs, int rt) {
    auto s = gpr_src(rs);
    auto t = gpr_src(rt);
    gprs[dst].du64[0] = s.du64[0] & t.du64[0];
    gprs[dst].du64[1] = s.du64[1] & t.du64[1];
  }

  void pceqb(int dst, int rs, int rt) {
    auto s = gpr_src(rs);
    auto t = gpr_src(rt);
    for (int i = 0; i < 16; i++) {
      gprs[dst].du8[i] = (s.du8[i] == t.du8[i]) ? 0xff : 0;
    }
  }

  void pmfhl_lh(int dest) {
    gprs[dest].du16[0] = lo.du16[0];
    gprs[dest].du16[1] = lo.du16[2];
    gprs[dest].du16[2] = hi.du16[0];
    gprs[dest].du16[3] = hi.du16[2];
    gprs[dest].du16[4] = lo.du16[4];
    gprs[dest].du16[5] = lo.du16[6];
    gprs[dest].du16[6] = hi.du16[4];
    gprs[dest].du16[7] = hi.du16[6];
  }

  void vsub_bc(DEST mask, BC bc, int dest, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dest].f[i] = s0.f[i] - s1.f[(int)bc];
      }
    }
  }

  void vmul_bc(DEST mask, BC bc, int dest, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dest].f[i] = s0.f[i] * s1.f[(int)bc];
      }
    }
  }

  void vmul(DEST mask, int dest, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dest].f[i] = s0.f[i] * s1.f[i];
      }
    }
  }

  void vadd(DEST mask, int dest, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dest].f[i] = s0.f[i] + s1.f[i];
      }
    }
  }

  void vmini(DEST mask, int dest, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dest].f[i] = std::min(s0.f[i], s1.f[i]);
      }
    }
  }

  void vmax(DEST mask, int dest, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dest].f[i] = std::max(s0.f[i], s1.f[i]);
      }
    }
  }

  void vsub(DEST mask, int dest, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dest].f[i] = s0.f[i] - s1.f[i];
      }
    }
  }

  void vmula_bc(DEST mask, BC bc, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        acc.f[i] = s0.f[i] * s1.f[(int)bc];
      }
    }
  }

  void vadda_bc(DEST mask, BC bc, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        acc.f[i] = s0.f[i] + s1.f[(int)bc];
      }
    }
  }

  void vmadda_bc(DEST mask, BC bc, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        acc.f[i] += s0.f[i] * s1.f[(int)bc];
      }
    }
  }

  void vmadda(DEST mask, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        acc.f[i] += s0.f[i] * s1.f[i];
      }
    }
  }

  void vmsuba_bc(DEST mask, BC bc, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        acc.f[i] -= s0.f[i] * s1.f[(int)bc];
      }
    }
  }

  void vmadd_bc(DEST mask, BC bc, int dst, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].f[i] = acc.f[i] + s0.f[i] * s1.f[(int)bc];
      }
    }
  }

  void vmadd(DEST mask, int dst, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].f[i] = acc.f[i] + s0.f[i] * s1.f[i];
      }
    }
  }

  void vmsub_bc(DEST mask, BC bc, int dst, int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].f[i] = acc.f[i] - s0.f[i] * s1.f[(int)bc];
      }
    }
  }

  void vdiv(int src0, BC bc0, int src1, BC bc1) {
    Q = vf_src(src0).f[(int)bc0] / vf_src(src1).f[(int)bc1];
  }

  void vrsqrt(int src0, BC bc0, int src1, BC bc1) {
    Q = vf_src(src0).f[(int)bc0] / std::sqrt(std::abs(vf_src(src1).f[(int)bc1]));
  }

  void vsqrt(int src, BC bc) { Q = std::sqrt(std::abs(vf_src(src).f[(int)bc])); }

  void sqrts(int src, int dst) { fprs[dst] = std::sqrt(std::abs(fprs[src])); }

  void vmulq(DEST mask, int dst, int src) {
    auto s0 = vf_src(src);
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].f[i] = s0.f[i] * Q;
      }
    }
  }

  void vrget(DEST mask, int dst) {
    float r = gRng.R;
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].f[i] = r;
      }
    }
  }

  void vrxor(int src, BC bc) { gRng.rxor(vf_src(src).du32[(int)bc]); }

  void vaddq(DEST mask, int dst, int src) {
    auto s = vf_src(src);
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].f[i] = s.f[i] + Q;
      }
    }
  }

  void vabs(DEST mask, int dst, int src) {
    auto s = vf_src(src);
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].f[i] = std::abs(s.f[i]);
      }
    }
  }

  void vrnext(DEST mask, int dst) {
    gRng.advance();
    float r = gRng.R;
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].f[i] = r;
      }
    }
  }

  void mov64(int dest, int src) { gprs[dest].ds64[0] = gpr_src(src).du64[0]; }

  void vmove(DEST mask, int dest, int src) {
    auto s = vf_src(src);
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dest].f[i] = s.f[i];
      }
    }
  }

  void slt(int dst, int src0, int src1) {
    gprs[dst].du64[0] = (gpr_src(src0).ds64[0] < gpr_src(src1).ds64[0]) ? 1 : 0;
  }

  void sltu(int dst, int src0, int src1) {
    gprs[dst].du64[0] = (gpr_src(src0).du64[0] < gpr_src(src1).du64[0]) ? 1 : 0;
  }

  void sll(int dst, int src, int sa) {
    u32 value = gpr_src(src).du32[0] << sa;
    s32 value_signed = value;
    gprs[dst].ds64[0] = value_signed;
  }

  void srl(int dst, int src, int sa) {
    u32 value = gpr_src(src).du32[0] >> sa;
    s32 value_signed = value;
    gprs[dst].ds64[0] = value_signed;
  }

  void dsra(int dst, int src, int sa) { gprs[dst].ds64[0] = gpr_src(src).ds64[0] >> sa; }
  void dsrl(int dst, int src, int sa) { gprs[dst].du64[0] = gpr_src(src).du64[0] >> sa; }
  void dsrav(int dst, int src, int sa) {
    gprs[dst].ds64[0] = gpr_src(src).ds64[0] >> gpr_src(sa).du32[0];
  }
  void dsllv(int dst, int src, int sa) {
    gprs[dst].ds64[0] = gpr_src(src).ds64[0] << (gpr_src(sa).du32[0] & 0b111111);
  }
  void dsra32(int dst, int src, int sa) { gprs[dst].ds64[0] = gpr_src(src).ds64[0] >> (32 + sa); }
  void dsrl32(int dst, int src, int sa) { gprs[dst].du64[0] = gpr_src(src).du64[0] >> (32 + sa); }
  void sra(int dst, int src, int sa) { gprs[dst].ds64[0] = gpr_src(src).ds32[0] >> sa; }
  void dsll(int dst, int src0, int sa) { gprs[dst].du64[0] = gpr_src(src0).du64[0] << sa; }
  void dsll32(int dst, int src0, int sa) { gprs[dst].du64[0] = gpr_src(src0).du64[0] << (32 + sa); }

  void daddu(int dst, int src0, int src1) { gprs[dst].du64[0] = sgpr64(src0) + sgpr64(src1); }
  void daddiu(int dst, int src0, s64 imm) { gprs[dst].du64[0] = sgpr64(src0) + imm; }
  void addiu(int dst, int src0, s64 imm) {
    s32 temp = sgpr64(src0) + imm;
    gprs[dst].ds64[0] = temp;
  }

  void lui(int dst, u32 src) {
    s32 val = (src << 16);
    gprs[dst].ds64[0] = val;
  }

  void addu(int dst, int src0, int src1) {
    s32 temp = sgpr64(src0) + sgpr64(src1);
    gprs[dst].ds64[0] = temp;
  }

  void dsubu(int dst, int src0, int src1) { gprs[dst].du64[0] = sgpr64(src0) - sgpr64(src1); }
  void subu(int dst, int src0, int src1) {
    gprs[dst].ds64[0] = gpr_src(src0).ds32[0] - gpr_src(src1).ds32[0];
  }
  void xor_(int dst, int src0, int src1) { gprs[dst].du64[0] = sgpr64(src0) ^ sgpr64(src1); }
  void or_(int dst, int src0, int src1) { gprs[dst].du64[0] = sgpr64(src0) | sgpr64(src1); }

  void movz(int dst, int src0, int src1) {
    if (sgpr64(src1) == 0) {
      gprs[dst].du64[0] = sgpr64(src0);
    }
  }

  void movn(int dst, int src0, int src1) {
    if (sgpr64(src1) != 0) {
      gprs[dst].du64[0] = sgpr64(src0);
    }
  }

  void mult3(int dst, int src0, int src1) {
    u32 result = gpr_src(src0).ds32[0] * gpr_src(src1).ds32[0];
    s32 sresult = result;
    gprs[dst].ds64[0] = sresult;
  }

  void multu3(int dst, int src0, int src1) {
    u32 result = gpr_src(src0).du32[0] * gpr_src(src1).du32[0];
    s32 sresult = result;
    gprs[dst].ds64[0] = sresult;
  }

  void xori(int dest, int src, u64 imm) { gprs[dest].du64[0] = gpr_src(src).du64[0] ^ imm; }
  void andi(int dest, int src, u64 imm) { gprs[dest].du64[0] = gpr_src(src).du64[0] & imm; }
  void ori(int dest, int src, u64 imm) { gprs[dest].du64[0] = gpr_src(src).du64[0] | imm; }
  void and_(int dest, int src0, int src1) {
    gprs[dest].du64[0] = gpr_src(src0).du64[0] & gpr_src(src1).du64[0];
  }

  void pextlb(int dst, int src0, int src1) {
    auto s0 = gpr_src(src0);
    auto s1 = gpr_src(src1);
    gprs[dst].du8[0] = s1.du8[0];
    gprs[dst].du8[1] = s0.du8[0];
    gprs[dst].du8[2] = s1.du8[1];
    gprs[dst].du8[3] = s0.du8[1];
    gprs[dst].du8[4] = s1.du8[2];
    gprs[dst].du8[5] = s0.du8[2];
    gprs[dst].du8[6] = s1.du8[3];
    gprs[dst].du8[7] = s0.du8[3];
    gprs[dst].du8[8] = s1.du8[4];
    gprs[dst].du8[9] = s0.du8[4];
    gprs[dst].du8[10] = s1.du8[5];
    gprs[dst].du8[11] = s0.du8[5];
    gprs[dst].du8[12] = s1.du8[6];
    gprs[dst].du8[13] = s0.du8[6];
    gprs[dst].du8[14] = s1.du8[7];
    gprs[dst].du8[15] = s0.du8[7];
  }

  void pextlh(int dst, int src0, int src1) {
    auto s0 = gpr_src(src0);
    auto s1 = gpr_src(src1);
    gprs[dst].du16[0] = s1.du16[0];
    gprs[dst].du16[1] = s0.du16[0];
    gprs[dst].du16[2] = s1.du16[1];
    gprs[dst].du16[3] = s0.du16[1];
    gprs[dst].du16[4] = s1.du16[2];
    gprs[dst].du16[5] = s0.du16[2];
    gprs[dst].du16[6] = s1.du16[3];
    gprs[dst].du16[7] = s0.du16[3];
  }

  u32 lzocw(s32 in) {
    if (in < 0) {
      in = ~in;
    }
    if (in == 0) {
      return 32;
    }
    return count_leading_zeros_u32(in);
  }

  void plzcw(int dst, int src) {
    gprs[dst].du32[0] = lzocw(gpr_src(src).ds32[0]) - 1;
    gprs[dst].du32[1] = lzocw(gpr_src(src).ds32[1]) - 1;
  }

  void por(int dst, int src0, int src1) {
    auto s0 = gpr_src(src0);
    auto s1 = gpr_src(src1);
    gprs[dst].du64[0] = s0.du64[0] | s1.du64[0];
    gprs[dst].du64[1] = s0.du64[1] | s1.du64[1];
  }

  void pmaxw(int dst, int src0, int src1) {
    auto s0 = gpr_src(src0);
    auto s1 = gpr_src(src1);
    for (int i = 0; i < 4; i++) {
      gprs[dst].ds32[i] = std::max(s0.ds32[i], s1.ds32[i]);
    }
  }

  void pminw(int dst, int src0, int src1) {
    auto s0 = gpr_src(src0);
    auto s1 = gpr_src(src1);
    for (int i = 0; i < 4; i++) {
      gprs[dst].ds32[i] = std::min(s0.ds32[i], s1.ds32[i]);
    }
  }

  void pmaxh(int dst, int src0, int src1) {
    auto s0 = gpr_src(src0);
    auto s1 = gpr_src(src1);
    for (int i = 0; i < 8; i++) {
      gprs[dst].ds16[i] = std::max(s0.ds16[i], s1.ds16[i]);
    }
  }

  void pcpyh(int dst, int src) {
    auto s0 = gpr_src(src);
    gprs[dst].du16[0] = s0.du16[0];
    gprs[dst].du16[1] = s0.du16[0];
    gprs[dst].du16[2] = s0.du16[0];
    gprs[dst].du16[3] = s0.du16[0];
    gprs[dst].du16[4] = s0.du16[4];
    gprs[dst].du16[5] = s0.du16[4];
    gprs[dst].du16[6] = s0.du16[4];
    gprs[dst].du16[7] = s0.du16[4];
  }

  void psrah(int dest, int src, int sa) {
    auto s = gpr_src(src);
    for (int i = 0; i < 8; i++) {
      gprs[dest].ds16[i] = s.ds16[i] >> (sa & 0xf);
    }
  }

  void mov128_vf_gpr(int dst, int src) { memcpy(vfs[dst].f, gpr_src(src).f, 16); }
  void mov128_gpr_vf(int dst, int src) { memcpy(gprs[dst].f, vf_src(src).f, 16); }
  void mov128_gpr_gpr(int dst, int src) { gprs[dst] = gpr_src(src); }

  void vitof0(DEST mask, int dst, int src) {
    auto s = vf_src(src);
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].f[i] = s.ds32[i];
      }
    }
  }

  void vitof12(DEST mask, int dst, int src) {
    auto s = vf_src(src);
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].f[i] = ((float)s.ds32[i]) * (1.f / 4096.f);
      }
    }
  }

  void vitof15(DEST mask, int dst, int src) {
    auto s = vf_src(src);
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].f[i] = ((float)s.ds32[i]) * (1.f / 32768.f);
      }
    }
  }

  void vftoi12(DEST mask, int dst, int src) {
    auto s = vf_src(src);
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].ds32[i] = s.f[i] * 4096.f;
      }
    }
  }

  void vftoi4(DEST mask, int dst, int src) {
    auto s = vf_src(src);
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].ds32[i] = s.f[i] * 16.f;
      }
    }
  }

  void vftoi0(DEST mask, int dst, int src) {
    auto s = vf_src(src);
    for (int i = 0; i < 4; i++) {
      if ((u64)mask & (1 << i)) {
        vfs[dst].ds32[i] = s.f[i];
      }
    }
  }

  void mfc1(int dst, int src) {
    s32 val;
    memcpy(&val, &fprs[src], 4);
    gprs[dst].ds64[0] = val;
  }

  void mtc1(int dst, int src) {
    u32 val = gpr_src(src).du32[0];
    memcpy(&fprs[dst], &val, 4);
  }

  void muls(int dst, int src0, int src1) { fprs[dst] = fprs[src0] * fprs[src1]; }
  void adds(int dst, int src0, int src1) { fprs[dst] = fprs[src0] + fprs[src1]; }
  void subs(int dst, int src0, int src1) { fprs[dst] = fprs[src0] - fprs[src1]; }
  void divs(int dst, int src0, int src1) {
    // ASSERT(fprs[src1] != 0);
    fprs[dst] = fprs[src0] / fprs[src1];
  }
  void negs(int dst, int src) {
    u32 v;
    memcpy(&v, &fprs[src], 4);
    v ^= 0x80000000;
    memcpy(&fprs[dst], &v, 4);
  }

  void movs(int dst, int src) { fprs[dst] = fprs[src]; }
  void abss(int dst, int src) { fprs[dst] = std::abs(fprs[src]); }

  void cvtws(int dst, int src) {
    // float to int
    s32 value = fprs[src];
    memcpy(&fprs[dst], &value, 4);
  }

  void cvtsw(int dst, int src) {
    // int to float
    s32 value;
    memcpy(&value, &fprs[src], 4);
    fprs[dst] = value;
  }

  void vwaitq() {}

  void vopmula(int src0, int src1) {
    auto s0 = vf_src(src0);
    auto s1 = vf_src(src1);

    acc.f[0] = s0.f[1] * s1.f[2];
    acc.f[1] = s0.f[2] * s1.f[0];
    acc.f[2] = s0.f[0] * s1.f[1];
  }

  void vopmsub(int dst, int src0, int src1) {
    auto s0 = vf_src(src0);  // fs
    auto s1 = vf_src(src1);  // ft

    vfs[dst].f[0] = acc.f[0] - s0.f[1] * s1.f[2];
    vfs[dst].f[1] = acc.f[1] - s0.f[2] * s1.f[0];
    vfs[dst].f[2] = acc.f[2] - s0.f[0] * s1.f[1];
  }

  // copied from PCSX2, to handle the weirdo special case in lwr.
  static constexpr u32 LWL_MASK[4] = {0xffffff, 0x0000ffff, 0x000000ff, 0x00000000};
  static constexpr u32 LWR_MASK[4] = {0x000000, 0xff000000, 0xffff0000, 0xffffff00};
  static constexpr u8 LWL_SHIFT[4] = {24, 16, 8, 0};
  static constexpr u8 LWR_SHIFT[4] = {0, 8, 16, 24};

  void lwl(int dst, int offset, int addr_reg) {
    s32 addr = sgpr64(addr_reg) + offset;
    u32 shift = addr & 3;

    u32 mem;
    memcpy(&mem, g_ee_main_mem + (addr & ~3), 4);

    if (!dst)
      return;

    // ensure the compiler does correct sign extension into 64 bits by using s32
    gprs[dst].ds64[0] = (s32)((gprs[dst].du32[0] & LWL_MASK[shift]) | (mem << LWL_SHIFT[shift]));

    /*
    Mem = 1234.  Reg = abcd
    (result is always sign extended into the upper 32 bits of the Rt)

    0   4bcd   (mem << 24) | (reg & 0x00ffffff)
    1   34cd   (mem << 16) | (reg & 0x0000ffff)
    2   234d   (mem <<  8) | (reg & 0x000000ff)
    3   1234   (mem      ) | (reg & 0x00000000)
    */
  }

  void lwr(int dst, int offset, int addr_reg) {
    s32 addr = sgpr64(addr_reg) + offset;
    u32 shift = addr & 3;

    u32 mem;
    memcpy(&mem, g_ee_main_mem + (addr & ~3), 4);

    if (!dst)
      return;

    // Use unsigned math here, and conditionally sign extend below, when needed.
    mem = (gprs[dst].du32[0] & LWR_MASK[shift]) | (mem >> LWR_SHIFT[shift]);

    if (shift == 0) {
      // This special case requires sign extension into the full 64 bit dest.
      gprs[dst].ds64[0] = (s32)mem;
    } else {
      // This case sets the lower 32 bits of the target register.  Upper
      // 32 bits are always preserved.
      gprs[dst].du32[0] = mem;
    }
  }

  static constexpr u64 LDL_MASK[8] = {
      0x00ffffffffffffffULL, 0x0000ffffffffffffULL, 0x000000ffffffffffULL, 0x00000000ffffffffULL,
      0x0000000000ffffffULL, 0x000000000000ffffULL, 0x00000000000000ffULL, 0x0000000000000000ULL};
  static constexpr u64 LDR_MASK[8] = {
      0x0000000000000000ULL, 0xff00000000000000ULL, 0xffff000000000000ULL, 0xffffff0000000000ULL,
      0xffffffff00000000ULL, 0xffffffffff000000ULL, 0xffffffffffff0000ULL, 0xffffffffffffff00ULL};

  static constexpr u8 LDR_SHIFT[8] = {0, 8, 16, 24, 32, 40, 48, 56};
  static constexpr u8 LDL_SHIFT[8] = {56, 48, 40, 32, 24, 16, 8, 0};

  void ldl(int dst, int offset, int addr_reg) {
    u32 addr = sgpr64(addr_reg) + offset;
    u32 shift = addr & 7;

    u64 mem;
    // memRead64(addr & ~7, &mem);
    memcpy(&mem, g_ee_main_mem + (addr & ~7), 8);

    if (!dst)
      return;
    gprs[dst].du64[0] = (gprs[dst].du64[0] & LDL_MASK[shift]) | (mem << LDL_SHIFT[shift]);
  }

  void ldr(int dst, int offset, int addr_reg) {
    u32 addr = sgpr64(addr_reg) + offset;
    u32 shift = addr & 7;

    u64 mem;
    // memRead64(addr & ~7, &mem);
    memcpy(&mem, g_ee_main_mem + (addr & ~7), 8);

    if (!dst)
      return;
    gprs[dst].du64[0] = (gprs[dst].du64[0] & LDR_MASK[shift]) | (mem >> LDR_SHIFT[shift]);
  }

  std::string print_vf_float(int vf) {
    auto src = vf_src(vf);
    return fmt::format("{} {} {} {}", src.f[0], src.f[1], src.f[2], src.f[3]);
  }
};

static_assert(sizeof(ExecutionContext) <= 1280);

inline void get_fake_spad_addr(int dst, void* sym_addr, u32 offset, ExecutionContext* c) {
  u32 val;
  memcpy(&val, sym_addr, 4);
  c->gprs[dst].du64[0] = val + offset;
}

inline void spad_to_dma(void* spad_sym_addr, u32 madr, u32 sadr, u32 qwc) {
  u32 spad_addr_goal;
  memcpy(&spad_addr_goal, spad_sym_addr, 4);
  sadr -= spad_addr_goal;

  ASSERT((madr & 0xf) == 0);
  ASSERT((sadr & 0xf) == 0);
  ASSERT(sadr < 0x4000);
  ASSERT((sadr + 16 * qwc) <= 0x4000);
  ASSERT(qwc <= 0x4000);

  void* spad_addr_c = g_ee_main_mem + spad_addr_goal + sadr;

  memcpy(spad_addr_c, g_ee_main_mem + madr, qwc * 16);
}

inline void spad_to_dma_no_sadr_off(void* spad_sym_addr, u32 madr, u32 sadr, u32 qwc) {
  u32 spad_addr_goal;
  memcpy(&spad_addr_goal, spad_sym_addr, 4);

  ASSERT((madr & 0xf) == 0);
  ASSERT((sadr & 0xf) == 0);
  ASSERT(sadr < 0x4000);
  ASSERT((sadr + 16 * qwc) <= 0x4000);
  ASSERT(qwc <= 0x4000);

  void* spad_addr_c = g_ee_main_mem + spad_addr_goal + sadr;

  memcpy(spad_addr_c, g_ee_main_mem + madr, qwc * 16);
}

inline void spad_to_dma_no_sadr_off_bones_interleave(void* spad_sym_addr,
                                                     u32 madr,
                                                     u32 sadr,
                                                     u32 qwc) {
  u32 spad_addr_goal;
  memcpy(&spad_addr_goal, spad_sym_addr, 4);

  ASSERT((madr & 0xf) == 0);
  ASSERT((sadr & 0xf) == 0);
  ASSERT(sadr < 0x4000);
  ASSERT((sadr + 16 * qwc) <= 0x4000);
  ASSERT(qwc <= 0x4000);

  u8* spad_addr_c = g_ee_main_mem + spad_addr_goal + sadr;
  const u8* mem_addr = g_ee_main_mem + madr;
  ASSERT((qwc & 3) == 0);
  while (qwc > 0) {
    // transfer 4.
    memcpy(spad_addr_c, mem_addr, 4 * 16);
    spad_addr_c += (4 * 16);
    sadr += 4 * 16;
    // but skip 5
    mem_addr += (5 * 16);
    qwc -= 4;
  }
}

inline void spad_from_dma(void* spad_sym_addr, u32 madr, u32 sadr, u32 qwc) {
  u32 spad_addr_goal;
  memcpy(&spad_addr_goal, spad_sym_addr, 4);
  sadr -= spad_addr_goal;
  ASSERT((madr & 0xf) == 0);
  ASSERT((sadr & 0xf) == 0);
  ASSERT(sadr < 0x4000);
  ASSERT((sadr + 16 * qwc) <= 0x4000);
  ASSERT(qwc <= 0x4000);

  void* spad_addr_c = g_ee_main_mem + spad_addr_goal + sadr;

  memcpy(g_ee_main_mem + madr, spad_addr_c, qwc * 16);
}

inline void spad_from_dma_no_sadr_off(void* spad_sym_addr, u32 madr, u32 sadr, u32 qwc) {
  u32 spad_addr_goal;
  memcpy(&spad_addr_goal, spad_sym_addr, 4);
  ASSERT((madr & 0xf) == 0);
  ASSERT((sadr & 0xf) == 0);
  ASSERT(sadr < 0x4000);
  ASSERT((sadr + 16 * qwc) <= 0x4000);
  ASSERT(qwc <= 0x4000);

  void* spad_addr_c = g_ee_main_mem + spad_addr_goal + sadr;

  memcpy(g_ee_main_mem + madr, spad_addr_c, qwc * 16);
}

inline void load_vfs_from_tf_regs(const void* tf_regs_sym, ExecutionContext* c) {
  u32 goal_addr_of_vf1;
  memcpy(&goal_addr_of_vf1, tf_regs_sym, 4);
  u8* c_addr_of_vf1 = g_ee_main_mem + goal_addr_of_vf1;
  memcpy(&c->vfs[1], c_addr_of_vf1, 31 * 16);
}

inline void spad_to_dma_blerc_chain(void* spad_sym_addr, u32 sadr, u32 tadr) {
  u32 spad_addr_goal;
  memcpy(&spad_addr_goal, spad_sym_addr, 4);
  void* spad_addr_c = g_ee_main_mem + spad_addr_goal;
  ASSERT(sadr < 0x4000);
  emulate_dma(g_ee_main_mem, spad_addr_c, tadr, sadr);
}
}  // namespace Mips2C
