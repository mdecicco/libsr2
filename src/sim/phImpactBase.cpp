#include <libsr2/sim/phImpactBase.h>
#include <libsr2/sim/phColliderBase.h>
#include <libsr2/sim/phBound.h>
#include <libsr2/sim/phInst.h>
#include <libsr2/math/math.h>

namespace sr2 {
    phImpactBase::phImpactBase() {
    }

    phImpactBase::~phImpactBase() {
    }

    void phImpactBase::reset() {
        // todo
    }

    void phImpactBase::reset(phColliderBase* bodyA, phColliderBase* bodyB) {
        // todo
    }

    void phImpactBase::reset(phColliderBase* bodyA, phInst* instB, phBound* boundB) {
        // todo
    }

    phInst* phImpactBase::setCollider(phColliderBase* body) {
        if (!colliderA) return setColliderA(body);
        
        return setColliderB(body);
    }

    phInst* phImpactBase::setColliderA(phColliderBase* body) {
        phInst* oldInstA = instA;
        
        colliderA = body;
        instA = nullptr;
        setBoundA(body->getBound());
        readMatrixA = body->getReadMatrix();
        matrixA = body->getMatrix();

        hasBothColliders = colliderB != nullptr;
        return oldInstA;
    }

    phInst* phImpactBase::setColliderB(phColliderBase* body) {
        phInst* oldInstB = instB;

        colliderB = body;
        instB = nullptr;
        setBoundB(body->getBound());
        readMatrixB = body->getReadMatrix();
        matrixB = body->getMatrix();

        hasBothColliders = colliderA != nullptr;
        return oldInstB;
    }

    void phImpactBase::setBoundA(phBound* bound) {
        boundA = bound;
        setPenetration();
        boundA = checkCompositeBound(bound);
    }

    void phImpactBase::setBoundB(phBound* bound) {
        boundB = bound;
        setPenetration();
        boundB = checkCompositeBound(bound);
    }

    void phImpactBase::setInstanceA(phInst* inst, phBound* bound) {
        instA = inst;
        colliderA = nullptr;

        setBoundA(bound);
        readMatrixA = &inst->transform;
        matrixA = *inst->method_0x10();
    }

    void phImpactBase::setInstanceB(phInst* inst, phBound* bound) {
        instB = inst;
        colliderB = nullptr;

        setBoundB(bound);
        readMatrixB = &inst->transform;
        matrixB = *inst->method_0x10();
    }

    void phImpactBase::setPenetration() {
        if (!boundA || !boundB) {
            penetration = 0.0f;
            return;
        }

        penetration = math::min(boundA->penetration, boundB->penetration);
    }

    phBound* phImpactBase::checkCompositeBound(phBound* bound) {
        // todo
        return bound;
    }

    bool phImpactBase::hitImpactLimit() {
        // todo
        return false;
    }

    bool phImpactBase::correctBoundOrder() {
        // todo
        return false;
    }

    void phImpactBase::swapBoundOrder() {
        // todo
    }

    void phImpactBase::resetStartIndex() {
        // todo
    }

    vec3f phImpactBase::getImpulse(u32 idx) {
        // todo
        return vec3f(0.0f, 0.0f, 0.0f);
    }

    vec3f phImpactBase::getTotalImpulse(phInst* inst) {
        vec3f impulse;
        for (u32 i = 0;i < impactCount;i++) {
            impulse += getImpulse(i);
        }

        if (inst == instB) return -impulse;
        return impulse;
    }

    void phImpactBase::applyImpetus() {
        // todo
    }
};