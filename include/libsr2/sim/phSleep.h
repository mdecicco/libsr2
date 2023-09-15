#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phInertialCS;
    class phColliderBase;

    enum phSleepState {
        SLEEP_STATE_INACTIVE,
        SLEEP_STATE_ACTIVE,
        SLEEP_STATE_UNKNOWN
    };

    class phSleep {
        public:
            phSleep();
            ~phSleep();

            void init(phInertialCS* ics, phColliderBase* col);

            phInertialCS* ics;
            phColliderBase* collider;
            phSleepState state;
            vec3f totalDisplacement;
            vec3f field6_0x20;
    };
};