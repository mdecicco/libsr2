#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBound.h>

namespace sr2 {
    class phMaterial;
    class phBoundOTGrid : public phBound {
        public:
            phBoundOTGrid();
            ~phBoundOTGrid();

            phMaterial* material;
    };
};