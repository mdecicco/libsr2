#include <libsr2/sim/phColliderBase.h>
#include <libsr2/sim/phInertialCS.h>
#include <libsr2/sim/phBound.h>
#include <libsr2/sim/phInst.h>
#include <libsr2/sim/phImpact.h>
#include <libsr2/sim/phContact.h>
#include <libsr2/sim/PhysEntity.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/utilities/datCallback.h>

namespace sr2 {
    phColliderBase::phColliderBase() {
        m_instance = nullptr;
        m_collidingWith = nullptr;
        m_lastCollidingWith = nullptr;
        m_next = nullptr;
        m_last = nullptr;
        m_impactCb = nullptr;
        m_boundCb = nullptr;
        m_bound = nullptr;
        m_ics = nullptr;
        m_readMatrix = nullptr;
        m_sleep = nullptr;

        m_active = true;
        m_needsUpdate = false;
        math::identity(m_matrix);
        m_justReset = false;
        m_barelyMoved = false;
        m_maxMovedDist = FLT_MAX;
        m_unk0 = 0;

        reset();
    }

    phColliderBase::~phColliderBase() {
    }

    void phColliderBase::init(phBound* bnd, phInst* inst, phInertialCS* _ics) {
        m_sleep = nullptr;
        m_instance = inst;
        m_bound = bnd;
        m_ics = _ics;
        reset();

        m_impactCb = nullptr;
        m_boundCb = nullptr;
        m_readMatrix = &m_instance->transform;
        math::copy(m_ics->world_transform, m_instance->transform);
        math::copy(m_matrix, m_instance->transform);
    }

    void phColliderBase::init(phInst* inst, phInertialCS* _ics, phSleep* _sleep) {
        m_impactCb = nullptr;
        m_boundCb = nullptr;
        m_instance = inst;
        m_ics = _ics;
        m_sleep = _sleep;
        m_bound = m_instance->unk->bound;

        if (!m_ics && !m_instance->unk->bound->centroidIsSet) {
            m_ics->world_transform = m_instance->transform;
            m_readMatrix = &m_ics->world_transform;
        } else {
            m_readMatrix = &m_instance->transform;
        }

        reset2();
    }

    void phColliderBase::reset() {
        if (!m_readMatrix) math::identity(m_matrix);
        else math::copy(m_matrix, *m_readMatrix);

        m_justReset = true;
        m_maxMovedDist = FLT_MAX;
        m_barelyMoved = false;
        m_needsUpdate = false;

        m_lastCollidingWith = 0;
        field23_0x74 = 0.0f;
        m_collidingWith = 0;
    }

    void phColliderBase::update() {
        if (!m_needsUpdate) updateMatrix();

        if (m_ics) m_ics->update();

        m_needsUpdate = false;
    }

    void phColliderBase::move() {
        if (!m_ics) return;
        
        m_ics->moveICS();
    }

    void phColliderBase::applyGravity() {
        m_ics->force.y += m_bound->gravityMultiplier * -9.8f * m_ics->mass;
    }
    
    void phColliderBase::method_0x30() {
        m_ics->method_0x40();
    }
    
    void phColliderBase::method_0x38() {
        if (!m_ics) return;

        m_ics->method_0x40();
    }

    void phColliderBase::impact(phImpact* impact, i32 impactIdx, const vec3f& p3, const vec3f& p4, u64 p5) {
        if (m_ics) {
            m_ics->method_0x40();
            m_ics->calcNetPush(p4, impact->impacts[impactIdx].somePosition0);
        }

        phInst* collidingWith = (phInst*)0xFFFFFFFF;
        if (impact->colliderA == this && impact->colliderB) {
            collidingWith = impact->colliderB->m_instance;
        } else if (impact->colliderA) {
            collidingWith = impact->colliderA->m_instance;
        }

        f32 mag = math::magnitude(p4);
        if (mag > field23_0x74) {
            field23_0x74 = mag;
            m_collidingWith = collidingWith;
        }

        if (m_impactCb) {
            phColliderImpactCallbackData data;
            data.field0_0x0 = &p3;
            data.impact = impact;
            data.impactIdx = impactIdx;
            m_impactCb->call(data);
        }
    }

    void phColliderBase::impact(phImpact* impact, i32 impactIdx, const vec3f& p3) {
        if (m_ics) m_ics->method_0x40();

        if (m_impactCb) {
            phColliderImpactCallbackData data;
            data.field0_0x0 = &p3;
            data.impact = impact;
            data.impactIdx = impactIdx;
            m_impactCb->call(data);
        }
    }
    
    void phColliderBase::contact(phImpactBase* impact, i32 p2, const vec3f& force, const vec3f& maybeContactPoint, const vec3f& p5, const mat3x4f& p6) {
        if (m_ics) {
            m_ics->applyContactForce(force, maybeContactPoint, p6, nullptr);
            m_ics->calcNetPush(p5, vec3f());
        }

        phInst* collidingWith = (phInst*)0xFFFFFFFF;
        if (impact->colliderA == this && impact->colliderB) {
            collidingWith = impact->colliderB->m_instance;
        } else if (impact->colliderA) {
            collidingWith = impact->colliderA->m_instance;
        }

        f32 mag = math::magnitude(p5);
        if (mag > field23_0x74) {
            field23_0x74 = mag;
            m_collidingWith = collidingWith;
        }
    }
    
    void phColliderBase::contact(const vec3f& force, phContact* contact, const mat3x4f& p3) {
        if (!m_ics) return;

        m_ics->applyContactForce(force, contact->maybeContactPoint, p3, nullptr);
    }
    
    void phColliderBase::contact(const vec3f& force, phContact* contact) {
        if (!m_ics) return;

        vec3f deltaPos = contact->maybeContactPoint - m_ics->world_transform.w;
        vec3f torque = deltaPos.cross(force);

        m_ics->oversampleForce += force;
        m_ics->oversampleTorque += torque;
    }



    void phColliderBase::reset2() {
        reset();

        if (m_ics) {
            if (m_readMatrix != &m_ics->world_transform) {
                math::copy(m_ics->world_transform, *m_readMatrix);

                if (m_bound->centroidIsSet) {
                    vec3f centroidWorldSpace;
                    math::mult(centroidWorldSpace, m_ics->world_transform, m_bound->centroid);
                    math::add(m_ics->world_transform.w, centroidWorldSpace);
                }
            }
        }

        math::copy(m_matrix, m_instance->transform);
    }

    void phColliderBase::updateMatrix() {
        if (m_ics) m_ics->moveICS();

        math::copy(m_matrix, *m_readMatrix);

        m_justReset = false;
        field23_0x74 = 0.0f;
        m_lastCollidingWith = m_collidingWith;
        m_collidingWith = nullptr;
    }

    phInst* phColliderBase::getInstance() const {
        return m_instance;
    }

    undefined2 phColliderBase::getUnk2() const {
        return m_unk0;
    }

    phInertialCS* phColliderBase::getICS() const {
        return m_ics;
    }

    phBound* phColliderBase::getBound() const {
        return m_bound;
    }

    mat3x4f* phColliderBase::getReadMatrix() const {
        return m_readMatrix;
    }

    mat3x4f& phColliderBase::getMatrix() const {
        return m_matrix;
    }

    phSleep* phColliderBase::getSleep() const {
        return m_sleep;
    }

    void phColliderBase::setSleep(phSleep* sleep) {
        m_sleep = sleep;
    }

    bool phColliderBase::isActive() const {
        return m_active;
    }

    void phColliderBase::setActive(bool active) {
        m_active = active;
    }

    void phColliderBase::setMaxMovedDist(f32 maxMovedDist) {
        m_maxMovedDist = maxMovedDist;
    }

    void phColliderBase::setBarelyMoved(bool barelyMoved) {
        m_barelyMoved = barelyMoved;
    }

    bool phColliderBase::barelyMoved() const {
        return m_barelyMoved;
    }

    void phColliderBase::getSphereCenterAndRadius(vec3f& center, f32& radius) {
        center = m_bound->getCenter(*m_readMatrix);
        radius = m_bound->aabbRadius;
    }
};