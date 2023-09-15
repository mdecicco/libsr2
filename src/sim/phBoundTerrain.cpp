#include <libsr2/sim/phBoundTerrain.h>

namespace sr2 {
    phBoundTerrain::phBoundTerrain() : phBoundGeometry(BOUND_TERRAIN) {
    }

    phBoundTerrain::~phBoundTerrain() {
    }

    bool phBoundTerrain::load(const char* name) {
        return false;
    }
};