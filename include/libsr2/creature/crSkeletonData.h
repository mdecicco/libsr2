#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class crAnimFrame;
    class crBoneData;

    class crSkeletonData {
        public:
            crSkeletonData();
            ~crSkeletonData();

            bool load(const char* name);
            bool load(const char* prefix, const char* name);
            void initMirror();
            crBoneData* findBone(const char* name, char postfix = 0, u32 nameIndex = 0);

            u32 boneCount;
            crBoneData* boneData;
    };
};