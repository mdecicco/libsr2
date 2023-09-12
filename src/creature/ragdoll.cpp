#include <libsr2/creature/ragdoll.h>
#include <libsr2/creature/ragBallJoint.h>
#include <libsr2/creature/ragElbowJoint.h>
#include <libsr2/creature/crAnimation.h>
#include <libsr2/creature/crAnimFrame.h>
#include <libsr2/creature/crSkeletonData.h>
#include <libsr2/creature/crSkeleton.h>
#include <libsr2/creature/crBone.h>
#include <libsr2/sim/dgLinkData.h>
#include <libsr2/math/math.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/quat.h>

#include <math.h>
#include <stdio.h>

namespace sr2 {
    /*
     * ragUnk0
     */
    ragUnk0::ragUnk0() {
        links = nullptr;
        boneLinkIndices = nullptr;
        unk0 = 0.0f;
        unk1 = 0.0f;
        count = 0;
    }

    ragUnk0::~ragUnk0() {
        if (links) {
            for (u32 i = 0;i < count;i++) delete links[i];
            delete [] links;
            links = nullptr;
        }

        if (boneLinkIndices) {
            delete [] boneLinkIndices;
            boneLinkIndices = nullptr;
        }
    }
    
    void ragUnk0::init(i32 linkCount, i32 boneCount) {
        count = linkCount;
        links = new ragJoint*[linkCount];
        for (u32 i = 0;i < linkCount;i++) links[i] = nullptr;

        boneLinkIndices = new i32[boneCount];
        for (u32 i = 0;i < boneCount;i++) boneLinkIndices[i] = -1;

        unk0 = 1.0f;
        unk1 = 1.0f;
    }

    void ragUnk0::initBallJoint(f32 angleLimit, f32 length, f32 inertia, f32 unk1, f32 unk2, i32 linkIdx, i32 boundIdx, const vec3f& centerOrient, const vec3f &initOrient, const vec3f& pos, i32 parentIdx) {
        ragBallJoint* joint = new ragBallJoint();
        links[linkIdx] = joint;
        boneLinkIndices[boundIdx] = linkIdx;
        joint->boneIdx = boundIdx;
        joint->linkIdx = linkIdx;
        
        math::from_length_encoded(joint->centerOrient, centerOrient);
        math::from_length_encoded(joint->initOrient, initOrient);

        FUN_00282b78(angleLimit, length, inertia, unk1, unk2, joint, pos, parentIdx);
    }

    void ragUnk0::initElbowJoint(f32 angleLimit, f32 centerAngle, f32 initAngle, f32 length, f32 inertia, f32 unk1, f32 unk2, i32 linkIdx, i32 boundIdx, const vec3f& pos, i32 parentIdx) {
        ragElbowJoint* joint = new ragElbowJoint();
        links[linkIdx] = joint;
        boneLinkIndices[boundIdx] = linkIdx;
        joint->boneIdx = boundIdx;
        joint->linkIdx = linkIdx;
        joint->centerAngle = centerAngle;
        joint->initAngle = initAngle;

        FUN_00282b78(angleLimit, length, inertia, unk1, unk2, joint, pos, parentIdx);
    }

    void ragUnk0::FUN_00282b78(f32 angleLimit, f32 length, f32 inertia, f32 unk1, f32 unk2, ragJoint* joint, const vec3f& pos, i32 parentIdx) {
        joint->setAngleLimit(angleLimit);
        joint->FUN_00284238(length, inertia);
        joint->position = pos;
        joint->unk1 = unk1;
        joint->unk2 = unk2;
        if (parentIdx >= 0) joint->parent = links[parentIdx];
    }



    /*
     * ragUnk1
     */
    ragUnk1::ragUnk1() {
        bones = nullptr;
        unk0 = nullptr;
        filename[0] = 0;
    }

    ragUnk1::~ragUnk1() {
    }

    bool ragUnk1::load(crSkeletonData* skel, char* filename) {
        // unk0 = LoadBounds(0.9f, filename, 0, nullptr);
        if (loadRagdoll(filename, skel)) {
            char standAnim[64] = { 0 };
            snprintf(standAnim, 64, "%s_stand", filename);

            crAnimation* stand = crAnimation::getAnim(standAnim, true, 1, nullptr, 0, 0xffffffff);
            if (stand) {
                 setPose(skel, stand->frames);
                 return true;
            }
        }

        return false;
    }

    void ragUnk1::initBone(i32 boundIdx, i32 numBones) {
        bones[boundIdx].count = numBones;
        bones[boundIdx].mats = new mat3x4f[numBones];
        bones[boundIdx].links = new i32[numBones];
    }

    void ragUnk1::attachBone(i32 boneIdx, i32 linkIdx, crSkeletonData* skel, char* name) {

    }

    void ragUnk1::setPose(crSkeletonData* skelData, crAnimFrame* frame) {
        crSkeleton skel(100);
        skel.init(skelData, nullptr);
        frame->pose(&skel, true);
        skel.update();
    }

    phBoundComposite* ragUnk1::getBound() {
        return nullptr;
    }

    dgRagdoll* ragUnk1::getRagdoll() {
        return nullptr;
    }

    bool ragUnk1::loadRagdoll(char* filename, crSkeletonData* skel) {
        return false;
    }
};