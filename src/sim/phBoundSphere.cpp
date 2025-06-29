#include <libsr2/sim/phBoundSphere.h>

#include <cmath>

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

    bool phBoundSphere::testSphere(const vec3f& center, f32 radius, vec3f& outProjection, vec3f& outDirNormal, f32& outPenetration) {
        vec3f v = center;
        if (centroidIsSet) v -= centroid;

        f32 intersectionDist = aabbRadius + radius;
        f32 distSq = v.lengthSq();

        if (distSq > intersectionDist * intersectionDist) return false;

        f32 dist = sqrtf(distSq);
        outDirNormal = v * (1.0f / dist);
        outPenetration = intersectionDist - dist;

        f32 projectionDist = radius - outPenetration;
        outProjection = center - projectionDist * outDirNormal;

        return true;
    }
};