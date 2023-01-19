#include <libsr2/sim/phBoundPolygonal.h>

namespace sr2 {
    phBoundPolygonal::phBoundPolygonal(BOUND_TYPE type) : phBound(type) {
        vertices = nullptr;
        polygons = nullptr;
        edges = nullptr;
        unk0 = nullptr;
        unk1 = -1;
        unk2 = 0;
        unk3 = 0xffffffff;
        unk4 = 0;
        unk5 = 0xffffffff;
        unk6 = 0;
    }

    phBoundPolygonal::~phBoundPolygonal() {
    }
};