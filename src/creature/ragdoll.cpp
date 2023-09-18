#include <libsr2/creature/ragdoll.h>
#include <libsr2/creature/ragBallJoint.h>
#include <libsr2/creature/ragElbowJoint.h>
#include <libsr2/creature/crAnimation.h>
#include <libsr2/creature/crAnimFrame.h>
#include <libsr2/creature/crSkeletonData.h>
#include <libsr2/creature/crSkeleton.h>
#include <libsr2/creature/crBoneData.h>
#include <libsr2/creature/crBone.h>
#include <libsr2/sim/dgLinkData.h>
#include <libsr2/sim/PhysEntity.h>
#include <libsr2/sim/phBoundComposite.h>
#include <libsr2/sim/phBoundHotdog.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/managers/PhysEntityManager.h>
#include <libsr2/io/datAsciiTokenizer.h>
#include <libsr2/io/stream.h>
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

    void ragUnk0::initBallJoint(f32 angleLimit, f32 length, f32 inertia, f32 flex, f32 damping, i32 linkIdx, i32 boundIdx, const vec3f& centerOrient, const vec3f &initOrient, const vec3f& pos, i32 parentIdx) {
        ragBallJoint* joint = new ragBallJoint();
        links[linkIdx] = joint;
        boneLinkIndices[boundIdx] = linkIdx;
        joint->boneIdx = boundIdx;
        joint->linkIdx = linkIdx;
        
        math::from_length_encoded(joint->centerOrient, centerOrient);
        math::from_length_encoded(joint->initOrient, initOrient);

        FUN_00282b78(angleLimit, length, inertia, flex, damping, joint, pos, parentIdx);
    }

    void ragUnk0::initElbowJoint(f32 angleLimit, f32 centerAngle, f32 initAngle, f32 length, f32 inertia, f32 flex, f32 damping, i32 linkIdx, i32 boundIdx, const vec3f& pos, i32 parentIdx) {
        ragElbowJoint* joint = new ragElbowJoint();
        links[linkIdx] = joint;
        boneLinkIndices[boundIdx] = linkIdx;
        joint->boneIdx = boundIdx;
        joint->linkIdx = linkIdx;
        joint->centerAngle = centerAngle;
        joint->initAngle = initAngle;

        FUN_00282b78(angleLimit, length, inertia, flex, damping, joint, pos, parentIdx);
    }

    void ragUnk0::FUN_00282b78(f32 angleLimit, f32 length, f32 inertia, f32 flex, f32 damping, ragJoint* joint, const vec3f& pos, i32 parentIdx) {
        joint->setAngleLimit(angleLimit);
        joint->FUN_00284238(length, inertia);
        joint->position = pos;
        joint->flex = flex;
        joint->damping = damping;
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

    bool ragUnk1::load(crSkeletonData* skel, const char* filename) {
        PhysEntityManager* m = PhysEntityManager::get();
        unk0 = m->getEntity(0.9f, filename, 0, nullptr);

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
        crBoneData* bone = skel->findBone(name);
        if (bone) bones[boneIdx].links[linkIdx] = bone->index0;
    }

    void ragUnk1::setPose(crSkeletonData* skelData, crAnimFrame* frame) {
        crSkeleton skel(100);
        skel.init(skelData, nullptr);
        frame->pose(&skel, true);
        skel.update();

        phBoundComposite* bound = (phBoundComposite*)unk0->bound;

        for (u32 i = 0;i < bound->boundCount;i++) {
            ragBone& bone = bones[i];
            for (u32 j = 0;j < bone.count;j++) {
                bone.mats[j] = skel.bones[bone.links[j]].transform;
            }

            mat3x4f* firstMtx = &bone.mats[0];
            mat3x4f& boundTrans = bound->boundTransforms[i];
            i32 firstLinkIdx = bone.links[0];
            i32 linkIdx = unk1.boneLinkIndices[i];

            if (linkIdx < 0) {
                *firstMtx = skel.bones[firstLinkIdx].transform;
            } else {
                mat3x4f& posedMtx = skel.boneTransforms[firstLinkIdx];
                math::mult_transposed(*firstMtx, boundTrans, posedMtx);
                boundTrans = posedMtx;
                math::sub(boundTrans.w, offset);
                //math::sub(boundTrans.w, posedMtx.w, offset);

                ragJoint* joint = unk1.links[linkIdx];
                if (joint->parent) {
                    mat3x4f tform;
                    joint->parent->getTransform(tform);
                    vec3f deltaPos;
                    math::sub(deltaPos, boundTrans.w, tform.w);
                    math::mult_transposed(joint->position, tform, deltaPos);
                } else {
                    joint->position = boundTrans.w;
                }
            }
        }
    }

    phBoundComposite* ragUnk1::getBound() {
        return (phBoundComposite*)unk0->bound;
    }

    dgRagdoll* ragUnk1::getRagdoll() {
        return nullptr;
    }

    bool ragUnk1::loadRagdoll(const char* _filename, crSkeletonData* skel) {
        Stream* fp = datAssetManager::open("tune/creature", _filename, "rag", 1, true);
        if (!fp) return false;

        phBoundComposite* bound = (phBoundComposite*)unk0->bound;

        datAsciiTokenizer tok;
        tok.init(_filename, fp);

        tok.matchToken("flex:");
        f32 flex = tok.readFloat();

        tok.matchToken("damping:");
        f32 damping = tok.readFloat();

        tok.matchToken("mass:");
        unk0->mass = tok.readFloat();
        if (unk0->mass != 0.0f) unk0->invMass = 1.0f / unk0->mass;

        tok.matchToken("angInertia:");
        unk0->angInertia = tok.readVec3();
        unk0->invAngInertia = {
            1.0f / unk0->angInertia.x,
            1.0f / unk0->angInertia.y,
            1.0f / unk0->angInertia.z
        };

        tok.matchToken("offset:");
        offset = tok.readVec3();

        tok.matchToken("numLinks:");
        u32 linkCount = tok.readInt32();

        bones = new ragBone[bound->boundCount];
        for (u32 i = 0;i < bound->boundCount;i++) {
            bones[i].mats = nullptr;
            bones[i].links = nullptr;
            bones[i].count = 0;
        }

        unk1.init(linkCount, bound->boundCount);
        for (u32 i = 0;i < bound->boundCount;i++) {
            tok.matchToken("bound:");
            i32 boundIdx = tok.readInt32();
            
            tok.matchToken("numBones:");
            u32 boneCount = tok.readInt32();

            initBone(boundIdx, boneCount);

            tok.matchToken("bones:");
            for (u32 j = 0;j < boneCount;j++) {
                char boneName[32] = { 0 };
                tok.getToken(boneName, 32);
                attachBone(i, j, skel, boneName);
            }

            char type[32] = { 0 };
            tok.matchToken("type:");
            tok.getToken(type, 32);

            if (stricmp(type, "none") == 0) {
                tok.matchToken("offset:");
                vec3f pos = tok.readVec3();

                tok.matchToken("initOrient");
                vec3f initOrient = tok.readVec3();

                math::from_position_and_length_encoded(bound->boundTransforms[boundIdx], pos, initOrient);
            } else if (stricmp(type, "ball") == 0) {
                tok.matchToken("link:");
                i32 linkIdx = tok.readInt32();

                tok.matchToken("parent:");
                i32 parentIdx = tok.readInt32();

                tok.matchToken("initOrient:");
                vec3f initOrient = tok.readVec3();

                tok.matchToken("centOrient:");
                vec3f centerOrient = tok.readVec3();

                tok.matchToken("angleLimit:");
                f32 angleLimit = tok.readFloat();

                tok.matchToken("inertia:");
                f32 inertia = tok.readFloat();

                phBoundHotdog* boneBound = ((phBoundHotdog*)bound->bounds[boundIdx]);
                boneBound->centerBound({ 0.0f, boneBound->length * 0.5f, 0.0f });
                
                unk1.initBallJoint(
                    angleLimit,
                    boneBound->length,
                    inertia,
                    flex,
                    damping,
                    linkIdx,
                    boundIdx,
                    centerOrient,
                    initOrient,
                    { 0.0f, 0.0f, 0.0f },
                    parentIdx
                );

                unk1.links[linkIdx]->getTransform(bound->boundTransforms[boundIdx]);
            } else if (stricmp(type, "elbow") == 0) {
                tok.matchToken("link:");
                i32 linkIdx = tok.readInt32();

                tok.matchToken("parent:");
                i32 parentIdx = tok.readInt32();

                tok.matchToken("initOrient:");
                f32 initAngle = tok.readFloat();

                tok.matchToken("centOrient:");
                f32 centerAngle = tok.readFloat();

                tok.matchToken("angleLimit:");
                f32 angleLimit = tok.readFloat();

                tok.matchToken("inertia:");
                f32 inertia = tok.readFloat();

                phBoundHotdog* boneBound = ((phBoundHotdog*)bound->bounds[boundIdx]);
                boneBound->centerBound({ 0.0f, boneBound->length * 0.5f, 0.0f });
                
                unk1.initElbowJoint(
                    angleLimit,
                    centerAngle,
                    initAngle,
                    boneBound->length,
                    inertia,
                    flex,
                    damping,
                    linkIdx,
                    boundIdx,
                    { 0.0f, 0.0f, 0.0f },
                    parentIdx
                );

                unk1.links[linkIdx]->getTransform(bound->boundTransforms[boundIdx]);
            }
        }

        fp->close();
        strcpy(filename, _filename);

        return true;
    }
};