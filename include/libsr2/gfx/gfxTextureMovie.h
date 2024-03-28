#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class gfxTextureMovie {
        public:
            gfxTextureMovie();
            ~gfxTextureMovie();

            static void updateAll(f32 dt);
    };
};