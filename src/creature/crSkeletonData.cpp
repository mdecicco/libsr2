#include <libsr2/creature/crSkeletonData.h>
#include <libsr2/creature/crBoneData.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/io/datAsciiTokenizer.h>
#include <libsr2/io/stream.h>

#include <string.h>
#include <stdio.h>

namespace sr2 {
    crSkeletonData::crSkeletonData() {
    }

    crSkeletonData::~crSkeletonData() {
    }
    
    bool crSkeletonData::load(const char* name) {
        return load("anim", name);
    }

    bool crSkeletonData::load(const char* prefix, const char* name) {
        Stream* fp = datAssetManager::open(prefix, name, "skel", 0, true);
        if (!fp) return false;

        datAsciiTokenizer tok;
        tok.init(name, fp);
        
        tok.matchToken("NumBones");
        boneCount = tok.readInt32();

        char token[256] = { 0 };
        tok.getToken(token, 256);

        if (stricmp(token, "bone") == 0) {
            boneData = new crBoneData[boneCount];

            i32 count = 0;
            crBoneData* next = &boneData[1];
            if (boneData->load(tok, &next, &count)) initMirror();
        }

        delete fp;
        return true;
    }

    void crSkeletonData::initMirror() {
        for (u32 i = 0;i < boneCount;i++) {
            u32 len = strlen(boneData[i].name);
            if (len < 3 || boneData[i].name[len - 1] != 'l' || boneData[i].name[len - 2] != '_') continue;

            crBoneData* left = &boneData[i];
            crBoneData* right = nullptr;

            char buf[129] = { 0 };
            strncpy(&buf[1], left->name, 128);
            buf[len] = 'r';

            for (u32 j = 0;j < boneCount;j++) {
                if (stricmp(boneData[j].name, &buf[1]) == 0) {
                    right = &boneData[j];
                }
            }

            if (right) {
                left->index1 = right->index0;
                right->index1 = left->index0;
            }
        }
    }
    
    crBoneData* crSkeletonData::findBone(const char* name, char postfix, u32 nameIndex) {
        char buf[128];
        if (nameIndex) snprintf(buf, 128, "%s_%02d", name, nameIndex);
        else strncpy(buf, name, 128);

        if (postfix == 'l') strncat(buf, "_l", 128);
        else if (postfix == 'r') strncat(buf, "_r", 128);

        for (u32 i = 0;i < boneCount;i++) {
            if (strcmp(boneData[i].name, buf) == 0) return &boneData[i];
        }

        return nullptr;
    }
};