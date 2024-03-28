#include <libsr2/managers/ParticleManager.h>
#include <libsr2/particles/ParticleSystem.h>

namespace sr2 {
    ParticleManager* ParticleManager::instance = nullptr;

    void ParticleManager::init() {
        instance->m_systems = nullptr;
        instance->m_systemCount = 0;
    }

    void ParticleManager::shutdown() {
        if (!instance) return;
        destroySystems();
        instance->m_systems = nullptr;
        instance->m_systemCount = 0;
        
        delete instance;
        instance = nullptr;
    }

    void ParticleManager::destroySystems() {
        for (auto sys = instance->m_systems;sys != nullptr;sys = sys->next) {
            sys->destroy();
            if (sys->field22_0x40) delete sys;
        }
    }

    ParticleBirthRule* ParticleManager::getBirthRule(const std::string& name) {
        auto it = instance->m_rules.find(name);
        if (it == instance->m_rules.end()) {
            return nullptr;
        }

        return it->second;
    }

    void ParticleManager::addBirthRule(const std::string& name, ParticleBirthRule* rule) {
        instance->m_rules[name] = rule;
    }

    void ParticleManager::addSystem(ParticleSystem* sys) {
        if (sys->addedToManager) return;
        sys->addedToManager = true;

        sys->last = nullptr;
        sys->next = instance->m_systems;

        if (instance->m_systems) instance->m_systems->last = sys;
        instance->m_systems = sys;

        instance->m_systemCount++;
    }

    void ParticleManager::reset() {
        // DisableGlobalSeed();
        for (auto sys = instance->m_systems;sys != nullptr;sys = sys->next) {
            sys->reset();

            if (!sys->field22_0x40) continue;

            // Why though?
            for (u32 i = 0;i < 500;i++) {
                sys->update();
            }
        }
        // EnableGlobalSeed();
    }

    void ParticleManager::update() {
        for (auto sys = instance->m_systems;sys != nullptr;sys = sys->next) {
            sys->update();
            sys->isCulled = true;
        }
    }

    ParticleManager* ParticleManager::get() {
        if (!instance) instance = new ParticleManager();
        return instance;
    }
};