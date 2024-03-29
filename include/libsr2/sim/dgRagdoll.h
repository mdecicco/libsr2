#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phInertialCS.h>
#include <libsr2/sim/phInst.h>
#include <libsr2/sim/phColliderBase.h>
#include <libsr2/sim/phSleep.h>

namespace sr2 {
    class ragUnk0;
    class ragUnk1;
    class crSkeleton;
    class dgLinkData;
    class PhysEntity;

    class dgRagdollData : public phInertialCS {
        public:
        dgRagdollData();
        virtual ~dgRagdollData();

        virtual void resetTransform();
        virtual void zero();
        virtual void freeze();
        virtual void updateInner(f32 dt);
        virtual void method_0x40();
        virtual void calcNetPush(const vec3f& unk);
        virtual void moveICS();
        virtual void getLocalVelocity(vec3f& some_position, vec3f& velocity);
        virtual void method_0x68();
        void init(u32 linkCount);
        void FUN_00282e08();
        void updateTransforms();
        void FUN_00282f80(f32 dt, mat3x4f* bones);

        ragUnk0* unk0;
        dgLinkData* links;
        quat_transform worldTrans;
        quat_transform prevWorldTrans;
        undefined4 unk1;
        undefined4 unk2;
    };

    class dgRagdoll {
        public:
        dgRagdoll();
        ~dgRagdoll();

        void reset();
        void init(u32 boneCount, u32 linkCount);
        void setBoundsFromUnk1(ragUnk1* unk);
        void FUN_001ab2d8();
        void startRagdoll(crSkeleton* skeleton, const mat3x4f& transform);

        ragUnk1* unk1;
        PhysEntity* entity;
        phInst instance0;
        phInst instance1;
        mat3x4f* bones0;
        mat3x4f* bones1;
        dgRagdollData* data;
        phColliderBase col;
        phSleep sleep;
        u16 spIndex;
        i32 field28_0x190;
    };
};