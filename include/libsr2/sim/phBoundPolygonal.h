#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBound.h>

namespace sr2 {
    class phPolygon;
    class phBoundPolygonal : public phBound {
        public:
            phBoundPolygonal(BOUND_TYPE type);
            ~phBoundPolygonal();

            phPolygon* polygons;
            undefined* field_0x88;
    };
};