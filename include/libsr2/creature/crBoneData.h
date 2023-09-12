#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class crBone;
    
    class crBoneData {
        public:
            crBoneData();
            ~crBoneData();

            void transform(mat3x4f* transform, crBone* bones);

            char name[64];
            crBoneData* next;
            crBoneData* children;
            crBoneData* parent;
            vec3f offset;
            u32 index;
            vec3f rotationMin;
            vec3f rotationMax;
            vec3f unkPos;
    };
};