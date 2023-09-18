#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class audImpact {
        public:
            audImpact();
            ~audImpact();

            void play(f32 u0, f32 u1, u32 idx);
    };
};