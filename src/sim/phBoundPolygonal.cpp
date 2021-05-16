#include <libsr2/sim/phBoundPolygonal.h>

namespace sr2 {
    phBoundPolygonal::phBoundPolygonal(undefined4 param_1) : phBound(param_1) {                                                 
        field_0x78 = 0xffffffff;
        field_0x80 = nullptr;
        field_0x64 = 0;
        field_0x68 = 0xffffffff;
        field_0x70 = 0xffffffff;
        field_0x6c = 0;
        field_0x74 = 0;
        field_0x7c = 0;

        polygons = nullptr;
        field_0x88 = nullptr;
    }

    phBoundPolygonal::~phBoundPolygonal() {
    }
};