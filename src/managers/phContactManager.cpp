#include <libsr2/managers/phContactManager.h>

namespace sr2 {
    phContactManager::phContactManager() {
    }

    phContactManager::~phContactManager() {
    }

    void phContactManager::setTiming(f32 seconds) {
        m_field10_0x34 = 1;
        m_seconds = seconds;
        m_invSeconds = 1.0f / seconds;
    }

    bool phContactManager::testHeldContact(phColliderBase* bodyA, phColliderBase* bodyB, u32* p3) {
        return false;
    }

    void phContactManager::calcHeldContacts(phColliderBase* bodyA, phColliderBase* bodyB, u32* p3) {
    }

    void phContactManager::addHeldContact(phColliderBase* bodyA, phColliderBase* bodyB) {
    }
};