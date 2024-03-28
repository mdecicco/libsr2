#pragma once
#include <libsr2/types.h>
#include <libsr2/utilities/DebugDrawer.h>

namespace sr2 {
    class ParticleBirthRule;
    class BaseParticle;

    class ParticleSystem : public IDebugDrawable {
        public:
            ParticleSystem();
            virtual ~ParticleSystem();
            
            void initList();

            virtual void debugDraw(const mat3x4f& transform = mat3x4f::identity);

            virtual void init();
            virtual void reset();
            virtual void updateUnknown();
            virtual void update();
            virtual void updateParticle(BaseParticle* particle);
            virtual void create(u32 count);
            virtual void destroy();

            virtual u32 allocate(u32 count) = 0;
            virtual ParticleBirthRule* genBirthRule() = 0;

            vec3f position;
            u32 freeCount;
            u32 liveCount;
            u32 particleCount;
            u32 particleByteSize;
            f32 timeUntilBlast;
            u8* particleMem;
            BaseParticle* deadList;
            BaseParticle* liveList;
            BaseParticle* someParticle;
            bool addedToManager;
            bool isCulled;
            ParticleSystem* last;
            ParticleSystem* next;
            ParticleBirthRule* rules[10];

            bool field22_0x40;
            vec3f field24_0x48;
    };
};