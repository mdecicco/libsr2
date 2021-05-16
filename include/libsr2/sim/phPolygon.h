#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phPolygon {
        public:
            phPolygon();
            ~phPolygon();

            vec3f field_0x0;
            f32 field_0xc;
            u16 indices[4];
            i16 maybe_material_idx;
            undefined field_0x1a;
            undefined field_0x1b;
            undefined field_0x1c;
            undefined field_0x1d;
            undefined field_0x1e;
            undefined field_0x1f;
    };
};