#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class crSkeletonData;
    class crBone;

    class crSkeleton {
        public:
            crSkeleton(u32 boneCount);
            ~crSkeleton();

            void init(crSkeletonData* data, mat3x4f* transform);
            void update();
            void attach(mat4x4f* transforms);

            crSkeletonData* data;
            crBone* bones;
            mat3x4f* boneTransforms;
            mat3x4f* transform;
    };
};