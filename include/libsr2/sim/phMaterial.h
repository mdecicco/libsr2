#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phMaterial {
        public:
            phMaterial();
            ~phMaterial();

            char name[32];
            short effect;
            short sound;
            float elasticity;
            float friction;
            float width;
            short ptx_index[2];
            float ptx_threshold[2];

            float field_0x30;
            float field_0x38;
            float field_0x3c;
    };
};