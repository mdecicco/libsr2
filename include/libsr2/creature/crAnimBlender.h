#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class crSkeleton;
    class crSkeletonData;
    class crAnimPlayer;

    class crAnimBlender {
        public:
            crAnimBlender(crSkeleton* skel, u32 animCount, bool u0, i32 u1);
            ~crAnimBlender();

            void reset();
            void init(crSkeletonData* data);

            crAnimPlayer* players;
    };
};