#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phBound;
    class phInst {
        public:
            phInst();
            ~phInst();

            mat3x4f transform;
            phBound** bound;
    };
};