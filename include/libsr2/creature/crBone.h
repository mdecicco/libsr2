#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class crBone {
        public:
            crBone();
            ~crBone();

            // XZY
            vec3f eulers;
            mat3x4f transform;
            mat3x4f* transformPtr;
    };
};