#include <libsr2/sim/lvlMaterial.h>

namespace sr2 {
    lvlMaterial::lvlMaterial() {
        width = 1.0;
        friction = 1.0f;
        elasticity = 0.5f;
        ptx_index[0] = -1;
        ptx_index[1] = -1;
        ptx_threshold[0] = 0.25f;
        ptx_threshold[1] = 0.5f;

        field_0x30 = 0.0f;
        field_0x38 = 0.0f;
        field_0x3c = 0.0f;
    }

    lvlMaterial::~lvlMaterial() {
    }
};