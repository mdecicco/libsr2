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
            void reset();
            void wakeUp();

            phSleepState getState() const;
            bool isSleeping() const;
            bool isActive() const;

            static f32 MinVelMagSq;
            static f32 MinAngVelMagSq;
            static undefined4 DAT_00363780;

        private:
            phInertialCS* m_ics;
            phColliderBase* m_collider;
            phSleepState m_state;
            vec3f m_totalDisplacement;

            u32 field_0xc;
            u32 field_0x10;
            vec3f field_0x20;
            u32 field_0x48;
    };
};