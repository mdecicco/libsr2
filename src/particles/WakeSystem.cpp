#include <libsr2/particles/WakeSystem.h>
#include <libsr2/particles/WakeBirthRule.h>
#include <libsr2/particles/BaseParticle.h>

namespace sr2 {
    WakeSystem::WakeSystem() {
        field41_0x70.zero();
        field42_0x80 = nullptr;
        field43_0x84 = 1.0f;
        field44_0x88 = 0;
        field45_0x8c = 1;
        field46_0x90 = 0;
        lastRuleIdx = 0;
        birthRuleCount = 0;
    }

    WakeSystem::~WakeSystem() {
    }

    void WakeSystem::reset() {

    }

    void WakeSystem::update() {

    }
    
    u32 WakeSystem::allocate(u32 count) {
        if (count > 0) {
            particleMem = new u8[sizeof(BaseParticle) * count];
            BaseParticle* particles = (BaseParticle*)particleMem;
            for (u32 i = 0;i < count;i++) {
                particles[i].position.zero();
                particles[i].next = nullptr;
                particles[i].life = 0.0f;
            }
        }

        return sizeof(BaseParticle);
    }
    
    ParticleBirthRule* WakeSystem::genBirthRule() {
        return new WakeBirthRule();
    }
};