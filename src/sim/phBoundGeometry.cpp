#include <libsr2/sim/phBoundGeometry.h>

namespace sr2 {
    phBoundGeometry::phBoundGeometry(BOUND_TYPE type) : phBoundPolygonal(type) {                                                                            
        field_0x58 = 0;
        field_0x4 = 0;
        field_0x60 = 0;
        field_0x5c = 0;
        field_0x80 = nullptr;
        polygons = nullptr;
        field_0x88 = 0;
        field_0x90 = nullptr;
        field_0x94 = nullptr;
        materials = nullptr;
    }

    phBoundGeometry::~phBoundGeometry() {
        if (polygons != nullptr) delete [] polygons;
        if (field_0x80 != nullptr) delete [] field_0x80;
        if (field_0x88 != nullptr) delete [] field_0x88;
        if (materials == nullptr) delete [] materials;
        if (field_0x90 == nullptr) delete [] field_0x90;
        if (field_0x94 == nullptr) delete [] field_0x94;
    }
};