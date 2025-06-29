#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class orTerrainGrid;

    class ambientWheel {
        public:
            ambientWheel();
            ~ambientWheel();

            void FUN_001818a0();

            u32 spatialPartitionerIdx;
            orTerrainGrid* terrain;
    };
};