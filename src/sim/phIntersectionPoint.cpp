#include <libsr2/sim/phIntersectionPoint.h>

namespace sr2 {
    void phIntersectionPoint::transform(const mat3x4f& transform) {
        contactPoint = transform.basis() * contactPoint;
        normal = transform.basis() * normal;
    }
};