#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phImpactBase.h>

namespace sr2 {
    class phImpact : public phImpactBase {
        public:
            phImpact();
            ~phImpact();

            bool hasBothColliders;
    };
};