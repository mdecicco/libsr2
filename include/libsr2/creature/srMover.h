#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class srType;

    class srMover {
        public:
            srMover();
            ~srMover();

            void reset();
            void init(srType* type, const mat3x4f& transform);
            void setElevationFromPosition(const vec3f& position);

            mat3x4f field1_0x4;
    };
};