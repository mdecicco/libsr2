#include <libsr2/managers/ambientManager.h>

namespace sr2 {
    ambientManager* ambientManager::instance = nullptr;

    void ambientManager::create() {
        if (instance) return;
        
        instance = new ambientManager();
    }
    
    void ambientManager::destroy() {
        if (!instance) return;

        delete instance;
        instance = nullptr;
    }

    ambientManager* ambientManager::get() {
        return instance;
    }

    ambientManager::ambientManager() {
        antiGravityEnabled = false;
    }

    ambientManager::~ambientManager() {
    }
    
};