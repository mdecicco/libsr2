#pragma once
#include <libsr2/types.h>

namespace sr2 {
    enum EXPLOSION_TYPE {
        unk = 0x0
    };

    class dgBomb {
        public:
            dgBomb();
            ~dgBomb();

            void enablePhysics(bool unk);
            void setExplosionType(EXPLOSION_TYPE tp);
            void setStrengthAlt();

            static dgBomb* g_SomeBomb;
    };
};