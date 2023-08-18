#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phInertialCS.h>

namespace sr2 {
    class ragUnk0;
    class dgLinkData;

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
    };
};