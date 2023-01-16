#include <libsr2/sim/phBound.h>
#include <libsr2/sim/phBoundCollision.h>

namespace sr2 {
    phBound::phBound(BOUND_TYPE tp) {    
        type = tp;
        field_0x4 = 0;
        field_0x20 = 0;
        field_0x24.x = 0.0f;
        field_0x24.z = 0.0f;
        field_0x24.y = 0.0f;
        field_0x30 = 0;
        field_0x34.x = 0.0f;
        field_0x34.z = 0.0f;
        field_0x34.y = 0.0f;
        field_0x50 = 1.0f;
        penetration = phBoundCollision::Penetration;
        penetration_barely_moved = phBoundCollision::PenetrationBarelyMoved;
        field_0x40 = 0;
        field_0x44 = 0;
    }

    phBound::~phBound() {
    }
};