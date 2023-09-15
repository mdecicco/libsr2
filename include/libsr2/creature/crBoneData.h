#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class crBone;
    class datAsciiTokenizer;
    
    class crBoneData {
        public:
            crBoneData();
            ~crBoneData();

            void transform(mat3x4f* transform, crBone* bones);
            bool load(datAsciiTokenizer& tok, crBoneData** next, i32* count);
            void addChild(crBoneData* bone);

            char name[64];
            crBoneData* next;
            crBoneData* children;
            crBoneData* parent;
            vec3f offset;
            u32 index0;
            u32 index1;
            vec3f rotationMin;
            vec3f rotationMax;
            vec3f unkPos;
    };
};