#include <libsr2/managers/Aud3DObjectManager.h>

namespace sr2 {
    Aud3DObjectManager* Aud3DObjectManager::instance = nullptr;

    Aud3DObjectManager::Aud3DObjectManager() {
        instance = this;
    }

    Aud3DObjectManager::~Aud3DObjectManager() {
        instance = nullptr;
    }

    void Aud3DObjectManager::FUN_001c1370() {
    }

    void Aud3DObjectManager::FUN_001c1428() {
    }

    void Aud3DObjectManager::FUN_001c14b0() {
    }

    Aud3DObjectManager* Aud3DObjectManager::get() {
        return instance;
    }

    bool Aud3DObjectManager::isAlive() {
        return instance != nullptr;
    }
};