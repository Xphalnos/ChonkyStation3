#pragma once

#include <BitField.hpp>


namespace SPUTypes {

union GPR {
    u8 b[16];
    u16 h[8];
    u32 w[4];
    u64 dw[2];
    float f[4];
    double d[2];
};

struct State {
    GPR gprs[128] = { 0 };
    u32 pc = 0;
};

union SPUInstruction {
    u32 raw;
    BitField<0,  7,  u32> rt0;
    BitField<0,  7,  u32> rc;
    BitField<7,  7,  u32> ra;
    BitField<7,  16, u32> i16;
    BitField<7,  18, u32> i18;
    BitField<14, 2,  u32> roh;
    BitField<14, 7,  u32> rb;
    BitField<14, 7,  u32> i7;
    BitField<14, 7,  u32> si7;
    BitField<14, 8,  u32> i8;
    BitField<14, 10, u32> si10;
    BitField<18, 1,  u32> e;
    BitField<19, 1,  u32> d;
    BitField<20, 1,  u32> c;
    BitField<21, 7,  u32> rt;
    BitField<23, 2,  u32> r0h;
};

}   // End namespace SPUTypes