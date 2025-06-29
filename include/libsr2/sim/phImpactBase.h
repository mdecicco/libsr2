#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phColliderBase;
    class phBound;
    class phInst;

    struct phImpactData {
        // These three fields are used to identify the impact data
        i32 idA;
        i32 idB;
        i32 idC;
        vec3f somePosition0;
        vec3f contactNormal;
        f32 penetration;
        f32 friction;
        f32 elasticity;
        vec3f field_0x40;
        vec3f field_0x4c;
        vec3f field_0x58;
    };

    class phImpactBase {
        public:
            phImpactBase();
            ~phImpactBase();

            void reset();
            void reset(phColliderBase* bodyA, phColliderBase* bodyB);
            void reset(phColliderBase* bodyA, phInst* instB, phBound* boundB);
            phInst* setCollider(phColliderBase* body);
            phInst* setColliderA(phColliderBase* body);
            phInst* setColliderB(phColliderBase* body);
            void setBoundA(phBound* bound);
            void setBoundB(phBound* bound);
            void setInstanceA(phInst* inst, phBound* bound);
            void setInstanceB(phInst* inst, phBound* bound);
            void setPenetration();
            phBound* checkCompositeBound(phBound* bound);
            bool hitImpactLimit();
            bool correctBoundOrder();
            void swapBoundOrder();
            void resetStartIndex();
            vec3f getImpulse(u32 idx);
            vec3f getTotalImpulse(phInst* inst);
            void applyImpetus();

            u32 impactCount;
            u32 startIndex;
            phImpactData impacts[64];
            f32 penetration;

            phColliderBase* colliderA;
            mat3x4f matrixA;
            mat3x4f* readMatrixA;
            phBound* boundA;
            phInst* instA;
            
            phColliderBase* colliderB;
            mat3x4f matrixB;
            mat3x4f* readMatrixB;
            phBound* boundB;
            phInst* instB;

            bool hasBothColliders;

            undefined4 field2_0x8;
    };
};