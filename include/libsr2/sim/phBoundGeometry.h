#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBoundPolygonal.h>

namespace sr2 {
    class phMaterial;
    class phBoundGeometry : public phBoundPolygonal {
        public:
            phBoundGeometry(BOUND_TYPE type);
            ~phBoundGeometry();

            phMaterial* materials;

            pointer field_0x90;
            pointer field_0x94;
            phMaterial* material;
            pointer field_0x9c;
            f32 field_0xa0;
            f32 field_0xa4;
            f32 field_0xa8;
            f32 field_0xac;
            f32 field_0xb0;
            f32 field_0xb4;
            undefined field_0xb8;
            undefined field_0xb9;
            undefined field_0xba;
            undefined field_0xbb;
            f32 field_0xbc;
            f32 field_0xc0;
    };
};