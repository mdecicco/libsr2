#include <libsr2/particles/ParticleSystem.h>
#include <libsr2/particles/ParticleBirthRule.h>
#include <libsr2/particles/BaseParticle.h>

namespace sr2 {
    ParticleSystem::ParticleSystem() {
        position.zero();
        field24_0x48.zero();
        
        isCulled = false;
        field22_0x40 = 0;
        addedToManager = false;
        freeCount = 0;
        particleCount = 0;
        liveCount = 0;
        particleByteSize = 0;
        timeUntilBlast = 0.0f;

        particleMem = nullptr;
        deadList = liveList = someParticle = nullptr;
        last = next = nullptr;
    }

    ParticleSystem::~ParticleSystem() {
        destroy();
    }
    
    void ParticleSystem::initList() {
        u32 endOffset = (freeCount - 1) * particleByteSize;
        for (u32 off = 0;off < endOffset;off += particleByteSize) {
            u32 nextOff = off + particleByteSize;
            BaseParticle* cur = (BaseParticle*)(particleMem + off);
            cur->next = (BaseParticle*)(particleMem + nextOff);
        }

        BaseParticle* last = (BaseParticle*)(particleMem + endOffset);
        last->next = nullptr;

        timeUntilBlast = 0.0f;
        liveList = nullptr;
        deadList = (BaseParticle*)particleMem;
        liveCount = 0;
        someParticle = nullptr;
    }

    void ParticleSystem::debugDraw(const mat3x4f& transform) {

    }
    
    void ParticleSystem::init() {
    }

    void ParticleSystem::reset() {
        initList();
    }

    void ParticleSystem::updateUnknown() {
        BaseParticle* particle = liveList;
        BaseParticle* last = particle;
        while (particle) {
            BaseParticle* next = particle->next;

            if (particle->life > 0.0f) {
                updateParticle(particle);
                last = particle;
            } else {
                last->next = next;

                if (liveList == particle) {
                    liveList = next;
                    last = next;
                }

                particle->next = deadList;
                deadList = particle;
                liveCount--;
            }
            
            particle = next;
        }
    }

    void ParticleSystem::update() {
        BaseParticle* particle = liveList;
        BaseParticle* last = particle;
        while (particle) {
            BaseParticle* next = particle->next;

            if (particle->life > 0.0f) {
                updateParticle(particle);
                last = particle;
            } else {
                last->next = next;

                if (next) {
                    next->last = particle->last;
                }

                if (liveList == particle) {
                    liveList = next;
                    last = next;
                }

                if (someParticle == particle) someParticle = particle->last;

                particle->last = nullptr;
                particle->next = deadList;
                deadList = particle;
                liveCount--;
            }
            
            particle = next;
        }
    }

    void ParticleSystem::updateParticle(BaseParticle* particle) {
    }

    void ParticleSystem::create(u32 count) {
        for (u32 i = 0;i < 10;i++) rules[i] = nullptr;
        particleByteSize = allocate(count);
        particleCount = count;
        freeCount = count;

        initList();
        init();
    }

    void ParticleSystem::destroy() {
        if (particleMem) delete [] particleMem;
        particleMem = nullptr;
    }
};