#include <libsr2/sim/phSleep.h>
#include <libsr2/sim/phInertialCS.h>
namespace sr2 {
    f32 phSleep::MinVelMagSq = 0.1f;
    f32 phSleep::MinAngVelMagSq = 0.05f;
    undefined4 phSleep::DAT_00363780 = 0x3c;

    phSleep::phSleep() {
    }
    
    phSleep::~phSleep() {
    }

    void phSleep::init(phInertialCS* _ics, phColliderBase* col) {
        m_ics = _ics;
        m_collider = col;
        m_state = SLEEP_STATE_ACTIVE;
        m_totalDisplacement = { 0.0f, 0.0f, 0.0f };
        field_0x20 = { 0.0f, 0.0f, 0.0f };
    }

    void phSleep::reset() {
        wakeUp();
    }

    void phSleep::wakeUp() {
        field_0xc = 0;
        field_0x10 = 0;
        m_state = SLEEP_STATE_ACTIVE;
        if (m_ics) m_ics->active = true;
        m_totalDisplacement = { 0.0f, 0.0f, 0.0f };
        field_0x20 = { 0.0f, 0.0f, 0.0f };
        field_0x48 = 0;
    }

    phSleepState phSleep::getState() const {
        return m_state;
    }

    bool phSleep::isSleeping() const {
        return m_state == SLEEP_STATE_INACTIVE;
    }

    bool phSleep::isActive() const {
        return m_state == SLEEP_STATE_ACTIVE;
    }
};