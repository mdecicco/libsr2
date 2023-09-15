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

            phMaterial* material;
    };
};