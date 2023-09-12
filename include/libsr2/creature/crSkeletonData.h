#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class crAnimFrame;
    class crBoneData;

    class crSkeletonData {
        public:
            crSkeletonData();
            ~crSkeletonData();

            u32 boneCount;
            crBoneData* boneData;
    };
};