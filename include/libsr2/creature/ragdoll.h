#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class dgLinkData;
    class dgRagdoll;
    class Unknown0;
    class crSkeletonData;
    class crAnimFrame;
    class phBoundComposite;
    class ragJoint;
    
    class ragBone {
        public:
            mat3x4f* mats;
            i32* links;
            u32 count;
    };

    class ragUnk0 {
        public:
            ragUnk0();
            virtual ~ragUnk0();

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

    class ragUnk1 {
        public:
            ragUnk1();
            ~ragUnk1();

            bool load(crSkeletonData* skel, char* filename);
            void initBone(i32 boundIdx, i32 numBones);
            void attachBone(i32 boneIdx, i32 linkIdx, crSkeletonData* skel, char* name);
            void setPose(crSkeletonData* skel, crAnimFrame* frames);
            phBoundComposite* getBound();
            dgRagdoll* getRagdoll();
            bool loadRagdoll(char* filename, crSkeletonData* skel);

            Unknown0* unk0;
            ragUnk0 unk1;
            ragBone* bones;
            vec3f offset;
            char filename[32];
    };
};