#include <libsr2/sim/phColliderBase.h>
#include <libsr2/sim/phInertialCS.h>
#include <libsr2/sim/phBound.h>
#include <libsr2/sim/phInst.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>

namespace sr2 {
    phColliderBase::phColliderBase() {

    }

    phColliderBase::~phColliderBase() {

    }

    void phColliderBase::init(phBound* bnd, phInst* inst, phInertialCS* _ics) {
        sleep = nullptr;
        instance = inst;
        bound = bnd;
        ics = _ics;
        reset();

        impact_cb = nullptr;
        bound_cb = nullptr;
        read_matrix = &instance->transform;
        math::copy(ics->world_transform, instance->transform);
        math::copy(matrix, instance->transform);
    }

    void phColliderBase::reset() {
        if (!read_matrix) math::identity(matrix);
        else math::copy(matrix, *read_matrix);

        just_reset = true;
        max_moved_dist = FLT_MAX;
        barely_moved = false;
        field25_0x80 = 0;
        field20_0x68 = 0;
        field23_0x74 = 0.0f;
        field24_0x78 = 0;
    }

    void phColliderBase::update() {
    }

    void phColliderBase::move() {
    }

    void phColliderBase::reset() {
        reset();

        if (ics) {
            if (read_matrix != &ics->world_transform) {
                math::copy(ics->world_transform, *read_matrix);

                if (bound->centroid_is_set) {
                    vec3f centroidWorldSpace;
                    math::mult(centroidWorldSpace, ics->world_transform, bound->centroid);
                    math::add(ics->world_transform.w, centroidWorldSpace);
                }
            }
        }

        math::copy(matrix, instance->transform);
    }

    void phColliderBase::applyGravity() {
        ics->force.y += bound->gravity_multiplier * -9.8f * ics->mass;
    }
    
    void phColliderBase::method_0x30() {
        // ics->method_0x44();
    }
    
    void phColliderBase::method_0x38() {
        if (!ics) return;

        // ics->method_0x44();
    }

    void phColliderBase::impact(phImpact* impact, i32 p1, const vec3f& p2, u64 p3) {
    }

    void phColliderBase::impact(i32 p1, i32 p2, u64 p3) {
    }
    
    void phColliderBase::contact(phImpactBase* impact, i32 p1, const vec3f& force, const vec3f& p2, const vec3f& p3, const mat3x4f& p4) {
    }
    
    void phColliderBase::contact(const vec3f& force, phContact* contact, const mat3x4f& p1) {
    }
    
    void phColliderBase::contact(const vec3f& force, phContact* contact) {
    }
};