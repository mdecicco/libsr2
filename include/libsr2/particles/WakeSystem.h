#pragma once
#include <libsr2/types.h>
#include <libsr2/particles/ParticleSystem.h>

namespace sr2 {
    class WakeBirthRule;

    class WakeSystem : public ParticleSystem {
        public:
            WakeSystem();
            virtual ~WakeSystem();

            virtual void reset();
            virtual void update();
            virtual u32 allocate(u32 count);
            virtual ParticleBirthRule* genBirthRule();

            u32 lastRuleIdx;
            u32 birthRuleCount;

            vec4f field41_0x70;
            undefined* field42_0x80;
            f32 field43_0x84;
            undefined4 field44_0x88;
            undefined4 field45_0x8c;
            undefined4 field46_0x90;
    };
};