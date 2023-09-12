#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class Stream;
    class crSkeleton;
    class crAnimFrame {
        public:
            crAnimFrame();
            ~crAnimFrame();

            void init(u32 channelCount);
            bool load(Stream* fp, u32 channelCount);
            void addScaled(crAnimFrame* frame, f32 u0, i32 u1, i32 u2);
            void pose(crSkeleton* skel, bool unk);

            u32 channelCount;
            f32* buffer;
    };
};