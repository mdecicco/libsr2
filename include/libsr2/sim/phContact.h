#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phInst;
    class phColliderBase;

    enum class phContactState {

    };

    class phContact {
        public:
            phContact();
            ~phContact();

            static void disableContacts();
            static bool ContactsEnabled;

            i32 idA;
            i32 idB;
            i32 idC;

            i32 lastIdx;
            i32 nextIdx;
            i32 lastContactIdx;

            phInst* instA;
            phColliderBase* colliderA;
            vec3f velocityA;

            phInst* instB;
            phColliderBase* colliderB;
            vec3f velocityB;

            vec3f maybeContactPoint;
            vec3f contactNormal;
            f32 friction;
            f32 elasticity;
            f32 quarterMassMultGravityDivPenetration;
            f32 halfDampCoefMultMass;

            phContactState state;
            phContactState prevState;
    };
};