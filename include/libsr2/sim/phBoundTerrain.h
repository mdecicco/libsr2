#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBoundGeometry.h>

namespace sr2 {
    class phBoundTerrain : public phBoundGeometry {
        public:
            phBoundTerrain();
            ~phBoundTerrain();

            bool load(const char* name);
    };
};