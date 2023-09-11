#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class dgLinkData;

    class ragJoint {
        public:
            ragJoint();
            virtual ~ragJoint();

            virtual void reset(dgLinkData* data);
            virtual void getTransform(mat3x4f& out);
            virtual void method_0x50(dgLinkData* links, const vec3f& param_2, mat3x4f& param_3, const vec3f& param_4);
            virtual void method_0x58(dgLinkData* links, const vec3f& param_2, const vec3f& param_3, vec3f& param_4);
            virtual void method_0x20(dgLinkData* links, const vec3f& param_1, const vec3f& param_2);
            virtual void method_0x28(dgLinkData* links, vec3f& param_1, const vec3f& param_2);
            virtual void method_0x18(f32 dt, f32 gravity, dgLinkData* links, const vec3f& velocity, const vec3f& unk, const vec3f& unk1) = 0;
            virtual void setAngleLimit(f32 limit) = 0;
            virtual bool method_0x38(f32 unk0, f32 unk1, dgLinkData* link) = 0;
            virtual void method_0x48(quat_transform& out) = 0;

            void resetSomething(dgLinkData* data);
            void FUN_00284238(f32 length, f32 inertia);

            ragJoint* parent;

            /** Object space */
            vec3f position;

            f32 unk0;
            f32 unk1;
            f32 unk2;
            i32 boneIdx;
            i32 linkIdx;
    };

    class ragElbowJoint : public ragJoint {
        public:
            ragElbowJoint();
            ~ragElbowJoint();

            virtual void reset(dgLinkData* link);
            virtual void method_0x18(f32 dt, f32 gravity, dgLinkData* links, const vec3f& velocity, const vec3f& unk, const vec3f& unk1);
            virtual void method_0x20(dgLinkData* links, const vec3f& param_1, const vec3f& param_2);
            virtual void method_0x28(dgLinkData* links, vec3f& param_1, const vec3f& param_2);
            virtual void setAngleLimit(f32 limit);
            virtual bool method_0x38(f32 unk0, f32 unk1, dgLinkData* link);
            virtual void getTransform(mat3x4f& out);
            virtual void method_0x48(quat_transform& out);
            virtual void method_0x50(dgLinkData* links, const vec3f& param_2, mat3x4f& param_3, const vec3f& param_4);
            virtual void method_0x58(dgLinkData* links, const vec3f& param_2, const vec3f& param_3, vec3f& param_4);

            void FUN_00285810(dgLinkData* link, f32 param_1);

            f32 angleLimit;
            f32 centerAngle;
            f32 initAngle;
    };

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

    class ragUnk0 {
        public:
            ragUnk0();
            ~ragUnk0();

            void init(i32 linkCount, i32 boneCount);

            void initBallJoint(f32 angleLimit, f32 length, f32 inertia, f32 unk1, f32 unk2, i32 linkIdx, i32 boundIdx, const vec3f& centerOrient, const vec3f &initOrient, const vec3f& pos, i32 parentIdx);
            void initElbowJoint(f32 angleLimit, f32 centerAngle, f32 initAngle, f32 length, f32 inertia, f32 unk1, f32 unk2, i32 linkIdx, i32 boundIdx, const vec3f& pos, i32 parentIdx);
            void FUN_00282b78(f32 angleLimit, f32 length, f32 inertia, f32 unk1, f32 unk2, ragJoint* joint, const vec3f& pos, i32 parentIdx);

            f32 unk0;
            f32 unk1;

            u32 count;
            ragJoint** links;

            /** bone index -> link index */
            i32* boneLinkIndices;
    };
};