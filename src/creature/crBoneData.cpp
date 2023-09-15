#include <libsr2/creature/crBoneData.h>
#include <libsr2/creature/crBone.h>
#include <libsr2/io/datAsciiTokenizer.h>
#include <libsr2/math/mat3x4f.h>

#include <string.h>
#include <stdio.h>

namespace sr2 {
    crBoneData::crBoneData() {
        snprintf(name, 64, "default");
        parent = nullptr;
        next = nullptr;
        children = nullptr;
        offset = { 0.0f, 0.0f, 0.0f };
        unkPos = { 0.0f, 0.0f, 0.0f };
        index0 = 0;
        index1 = 0xffffffff;
        rotationMin = { -3.141593f, -3.141593f, -3.141593f };
        rotationMax = {  3.141593f,  3.141593f,  3.141593f };
    }

    crBoneData::~crBoneData() {
    }

    void crBoneData::transform(mat3x4f* transform, crBone* bones) {
        if (!transform) *bones[index0].transformPtr = bones[index0].transform;
        else math::mult_with_translation(*bones[index0].transformPtr, bones[index0].transform, *transform);
        
        if (children) children->transform(bones[index0].transformPtr, bones);
        if (next) next->transform(transform, bones);
    }

    bool crBoneData::load(datAsciiTokenizer& tok, crBoneData** next, i32* count) {
        index0 = *count;
        tok.getToken(name, 64);
        tok.matchToken("{");
        offset = tok.matchVec3("offset");
        if (tok.checkToken("rotmin", true)) rotationMin = tok.readVec3();
        if (tok.checkToken("rotmax", true)) rotationMax = tok.readVec3();

        char buf[128] = { 0 };
        bool matched = false;
        while (true) {
            tok.getToken(buf, 128);
            if (stricmp(buf, "bone") != 0) break;

            crBoneData* bone = *next;
            addChild(bone);
            
            (*count)++;
            (*next)++;

            bone->load(tok, next, count);
        }

        return strcmp(buf, "}") == 0;
    }

    void crBoneData::addChild(crBoneData* bone) {
        if (bone) bone->parent = this;
        if (!children) {
            children = bone;
            return;
        }

        crBoneData* c = children;
        while (c->next) c = c->next;

        c->next = bone;
    }
};