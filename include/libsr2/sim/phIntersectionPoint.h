#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phInst;
    class phColliderBase;

    class phIntersectionPoint {
        public:
            void transform(const mat3x4f& transform);

            phInst* instance;
            u32 someCellIdx;
            vec3f contactPoint;

            vec3f normal;
            f32 field4_0x20;
            f32 penetration;
            u16 field6_0x28;
            phColliderBase* field9_0x2c;
            i32 field10_0x30;
            
            // more to come...
    };
};