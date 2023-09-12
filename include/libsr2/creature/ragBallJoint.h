#pragma once
#include <libsr2/types.h>
#include <libsr2/creature/ragJoint.h>

namespace sr2 {
    class dgLinkData;

    class ragBallJoint : public ragJoint {
        public:
            ragBallJoint();
            ~ragBallJoint();

            virtual void reset(dgLinkData* link);
            virtual void getTransform(mat3x4f& out);
            virtual void setAngleLimit(f32 limit);
            virtual void method_0x18(f32 dt, f32 gravity, dgLinkData* links, const vec3f& velocity, const vec3f& unk, const vec3f& unk1);
            virtual void method_0x20(dgLinkData* links, const vec3f& param_1, const vec3f& param_2);
            virtual void method_0x28(dgLinkData* links, vec3f& param_1, const vec3f& param_2);
            virtual bool method_0x38(f32 unk0, f32 unk1, dgLinkData* link);
            virtual void method_0x48(quat_transform& out);
            virtual void method_0x50(dgLinkData* links, const vec3f& param_2, mat3x4f& param_3, const vec3f& param_4);
            virtual void method_0x58(dgLinkData* links, const vec3f& param_2, const vec3f& param_3, vec3f& param_4);

            quat centerOrient;
            quat initOrient;
            f32 cosHalfAngleLimit;
            f32 sinHalfAngleLimit;
    };
};