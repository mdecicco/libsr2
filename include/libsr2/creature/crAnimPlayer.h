#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class crAnimation;

    class crAnimPlayer {
        public:
            crAnimPlayer();
            ~crAnimPlayer();

            void playAnimation(crAnimation* anim);

            bool doLoop;
            f32 percentPerSec;
            f32 time;
            
            f32 field10_0x28;
    };
};