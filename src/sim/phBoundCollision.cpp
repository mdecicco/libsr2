#include <libsr2/sim/phBoundCollision.h>

namespace sr2 {
    phBoundCollision::phBoundCollision() {
    }

    phBoundCollision::~phBoundCollision() {
    }

    void phBoundCollision::SetPenetration(f32 penetration) {
        Penetration = penetration;
        PenetrationBarelyMoved = penetration * 0.3f;
    }

    f32 phBoundCollision::PenetrationBarelyMoved = 0.0f;
    f32 phBoundCollision::Penetration = 0.0f;
};