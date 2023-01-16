#pragma once
#include <libsr2/types.h>

namespace sr2 {
    enum BOUND_TYPE : u32 {
        BOUND_SPHERE = 0,
        BOUND_HOTDOG,
        BOUND_BOX,
        BOUND_GEOMETRY,
        BOUND_TERRAIN,
        BOUND_UNK0,
        BOUND_UNK1,
        BOUND_UNK2,
        BOUND_UNK3,
        BOUND_OTGRID,
        BOUND_ORTERRAINGRID,
        BOUND_FORCE_SPHERE,
        BOUND_UNK4,
        BOUND_COMPOSITE
    };

    class phBound {
        public:
            phBound(BOUND_TYPE param_1);
            ~phBound();

            // knowns
            f32 penetration;
            f32 penetration_barely_moved;

            // unknowns
            BOUND_TYPE type;
            undefined4 field_0x4;
            undefined field_0x8;
            undefined field_0x9;
            undefined field_0xa;
            undefined field_0xb;
            undefined field_0xc;
            undefined field_0xd;
            undefined field_0xe;
            undefined field_0xf;
            undefined field_0x10;
            undefined field_0x11;
            undefined field_0x12;
            undefined field_0x13;
            undefined field_0x14;
            undefined field_0x15;
            undefined field_0x16;
            undefined field_0x17;
            undefined field_0x18;
            undefined field_0x19;
            undefined field_0x1a;
            undefined field_0x1b;
            undefined field_0x1c;
            undefined field_0x1d;
            undefined field_0x1e;
            undefined field_0x1f;
            undefined4 field_0x20;
            vec3f field_0x24;
            undefined4 field_0x30;
            vec3f field_0x34;
            undefined4 field_0x40;
            undefined4 field_0x44;
            f32 field_0x50;
            i32 field_0x58;
            undefined4 field_0x5c;
            undefined4 field_0x60;
            undefined4 field_0x64;
            undefined4 field_0x68;
            undefined4 field_0x6c;
            undefined4 field_0x70;
            undefined4 field_0x74;
            undefined4 field_0x78;
            undefined4 field_0x7c;
            f32* field_0x80;
    };
};