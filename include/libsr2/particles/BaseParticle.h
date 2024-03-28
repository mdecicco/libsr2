#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class BaseParticle {
        public:
            BaseParticle();
            ~BaseParticle();

            f32 life;
            vec3f position;
            vec3f velocity;
            u32 birthRuleIdx;
            BaseParticle* last;
            BaseParticle* next;
    };
};