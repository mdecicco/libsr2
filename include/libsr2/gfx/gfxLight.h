#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class gfxLight {
        public:
            gfxLight();
            ~gfxLight();

            void reset();

            vec4f color;
            vec3f direction;
    };
};