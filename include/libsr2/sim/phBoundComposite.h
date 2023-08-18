#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBound.h>

namespace sr2 {
    class phBoundComposite : public phBound {
        public:
            phBoundComposite();
            ~phBoundComposite();

            u32 bound_count_0;
            u32 bound_count_1;
    };
};