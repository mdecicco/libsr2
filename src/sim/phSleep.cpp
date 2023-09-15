#include <libsr2/sim/phSleep.h>

namespace sr2 {
    phSleep::phSleep() {
    }
    
    phSleep::~phSleep() {
    }

    void phSleep::init(phInertialCS* _ics, phColliderBase* col) {
        ics = _ics;
        collider = col;
        state = SLEEP_STATE_ACTIVE;
        totalDisplacement = { 0.0f, 0.0f, 0.0f };
        field6_0x20 = { 0.0f, 0.0f, 0.0f };
    }
};