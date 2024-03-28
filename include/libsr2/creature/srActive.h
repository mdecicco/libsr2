#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class srCreatureGroup;
    class srMover;
    class crSkeleton;
    class crAnimBlender;
    class dgRagdoll;
    class audImpact;

    class srActive {
        public:
        srActive();
        ~srActive();

        void reset();
        void deactivate();
        void activate(srCreatureGroup* group, i32 idx);
        void setAnimation();
        void calcGroupCenterForce();
        void calcPlayerRepulsion();
        void resolveForces();
        void smoothRepulsion();
        void stopRagdoll();
        void FUN_001aef08(audImpact* aud, f32 u0);
        void FUN_001aef88();
        bool onImpact(const mat3x4f& u0);

        void FUN_001af1d8();
        void updateAnim();
        void startRagdoll();
        void* FUN_001af288();

        u32 ragdollFrames;
        vec3f repulsion;
        vec3f force;
        vec3f worldVelocity;
        mat3x4f transform;
        srCreatureGroup* group;
        srMover* mover;
        crSkeleton* skeleton;
        crAnimBlender* blender;
        dgRagdoll* ragdoll;
        vec3f prevRepulsion[10];
        u32 rollingAverageIdx;

        undefined4 field0_0x0;
        i32 field1_0x4;
        i32 field5_0x14;
    };
};