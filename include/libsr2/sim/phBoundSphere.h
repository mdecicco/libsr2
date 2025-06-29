#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBound.h>

namespace sr2 {
    class phMaterial;
    class phBoundSphere : public phBound {
        public:
            phBoundSphere(f32 radius);
            ~phBoundSphere();

            void setRadius(f32 radius);
            bool testSphere(const vec3f& center, f32 radius, vec3f& outProjection, vec3f& outDirNormal, f32& outPenetration);

            phMaterial* material;
    };
};