#pragma once
#include <libsr2/types.h>

namespace sr2 {
    struct phSegment;
    
    class phCollisionPrim {
        public:
            phCollisionPrim();

            bool segmentSphereTest(const phSegment& seg, const vec3f& center, f32 radius);

            vec3f center;
    }
};