#pragma once
#include <libsr2/types.h>
#include <unordered_map>
#include <string>

namespace sr2 {
    class ParticleBirthRule;
    class ParticleSystem;

    class ParticleManager {
        public:
            static void init();
            static void shutdown();
            static void destroySystems();
            static ParticleBirthRule* getBirthRule(const std::string& name);
            static void addBirthRule(const std::string& name, ParticleBirthRule* rule);
            static void addSystem(ParticleSystem* sys);
            static void reset();
            static void update();
            static ParticleManager* get();

        private:
            static ParticleManager* instance;
            ParticleSystem* m_systems;
            u32 m_systemCount;
            std::unordered_map<std::string, ParticleBirthRule*> m_rules;
    };
};