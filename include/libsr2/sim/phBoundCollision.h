#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phBoundCollision {
        public:
            phBoundCollision();
            ~phBoundCollision();

            static void SetPenetration(f32 penetration);

            static f32 PenetrationBarelyMoved;
            static f32 Penetration;
    };
};