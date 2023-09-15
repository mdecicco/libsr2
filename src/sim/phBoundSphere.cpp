#include <libsr2/sim/phBoundSphere.h>

namespace sr2 {
    phBoundSphere::phBoundSphere(f32 _radius) : phBound(BOUND_SPHERE) {
        aabbRadius = _radius;
    }

    phBoundSphere::~phBoundSphere() {
    }

    void phBoundSphere::setRadius(f32 _radius) {
        aabbRadius = _radius;
        calculateBoundingBox();
    }
};