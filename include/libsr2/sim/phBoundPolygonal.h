#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBound.h>

namespace sr2 {
    class phPolygon;
    class phBoundPolygonal : public phBound {
        public:
            phBoundPolygonal(undefined4 param_1);
            ~phBoundPolygonal();

            phPolygon* polygons;
            undefined* field_0x88;
    };
};