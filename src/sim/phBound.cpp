#include <libsr2/sim/phBound.h>
#include <libsr2/sim/phBoundCollision.h>
#include <libsr2/math/vec3f.h>

#include <math.h>

namespace sr2 {
    phBound::phBound(BOUND_TYPE tp) {    
        type = tp;
        materialCount = 0;
        centroid_is_set = false;
        centroid.x = 0.0f;
        centroid.z = 0.0f;
        centroid.y = 0.0f;
        center_of_gravity_is_set = false;
        center_of_gravity.x = 0.0f;
        center_of_gravity.z = 0.0f;
        center_of_gravity.y = 0.0f;
        bound_gravity = 1.0f;
        penetration = phBoundCollision::Penetration;
        penetration_barely_moved = phBoundCollision::PenetrationBarelyMoved;
        radius = 0.0f;
        longest_extent_mag = 0.0f;
    }

    phBound::~phBound() {
    }

    void phBound::calculateBoundingBox() {
    }

    void phBound::calculateSphereFromBoundingBox() {
        centroid.x = aabb_min.x + (aabb_max.x - aabb_min.x) * 0.5;
        centroid.y = aabb_min.y + (aabb_max.y - aabb_min.y) * 0.5;
        centroid.z = aabb_min.z + (aabb_max.z - aabb_min.z) * 0.5;

        centroid_is_set = true;
        if (centroid.x == 0.0f && centroid.y == 0.0f && centroid.z == 0.0f) {
            centroid_is_set = false;
        }

        vec3f delta;
        math::sub(delta, centroid, aabb_max);
        radius = math::magnitude(delta);
        
        vec3f abs_min = {
            fabsf(aabb_min.x),
            fabsf(aabb_min.y),
            fabsf(aabb_min.z)
        };
        vec3f abs_max = {
            fabsf(aabb_max.x),
            fabsf(aabb_max.y),
            fabsf(aabb_max.z)
        };
        vec3f max_extent = {
            abs_max.x > abs_min.x ? abs_max.x : abs_min.x,
            abs_max.y > abs_min.y ? abs_max.y : abs_min.y,
            abs_max.z > abs_min.z ? abs_max.z : abs_min.z
        };

        this->longest_extent_mag = math::magnitude(max_extent);
    }

    void phBound::setPenetration() {
        penetration = phBoundCollision::Penetration;
        penetration_barely_moved = phBoundCollision::PenetrationBarelyMoved;
    }
};