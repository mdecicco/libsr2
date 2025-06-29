#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phImpact;

    namespace phCollision {
        bool testBoundGeneric(phImpact* impact);
    };
};