#pragma once
#include <libsr2/types.h>

namespace sr2 {
    enum BOUND_TYPE : u32 {
        BOUND_SPHERE = 0,
        BOUND_HOTDOG,
        BOUND_BOX,
        BOUND_GEOMETRY,
        BOUND_TERRAIN,
        BOUND_UNK0,
        BOUND_UNK1,
        BOUND_UNK2,
        BOUND_UNK3,
        BOUND_OTGRID,
        BOUND_ORTERRAINGRID,
        BOUND_FORCE_SPHERE,
        BOUND_UNK4,
        BOUND_COMPOSITE
    };

    class phBound {
        public:
            phBound(BOUND_TYPE param_1);
            ~phBound();

            virtual void calculateBoundingBox();
            void calculateSphereFromBoundingBox();
            void setPenetration();

            BOUND_TYPE type;
            u32 material_count;
            vec3f aabb_min;
            vec3f aabb_max;
            bool centroid_is_set;
            vec3f centroid;
            bool center_of_gravity_is_set;
            vec3f center_of_gravity;
            f32 radius;
            f32 longest_extent_mag;
            f32 penetration;
            f32 penetration_barely_moved;
            f32 bound_gravity;
    };
};