#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phPolygon {
        public:
            phPolygon();
            ~phPolygon();

            void calculateNormal(vec3f* vertices);

            vec3f normal;
            f32 normalMag;
            u16 indices[4];
            i16 maybeMaterialIndices[4];
    };
};