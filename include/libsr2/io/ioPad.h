#pragma once
#include <libsr2/types.h>

namespace sr2 {
    enum class PAD_MODE : u8 {
        UNK0 = 0xf0
    };

    enum class IO_PAD_STATE : u32 {
        UNK0 = 0,
        Error = 0x2,
        Disconnected = 0x3,
        Stable = 0x4,
        FindPadOrExecCmd = 0x5
    };

    class ioPad {
        public:
            ioPad();
            ~ioPad();

            u32 attach();
            void update();
            void read();

            undefined4 mainModeLock;
            undefined4 actuatorStatus;
            char actAlign[6];
            undefined4 buttonsAnalog;
            undefined4 actuatorsActivation;
            undefined ok;
            PAD_MODE mode;
            u16 buttonsUnk;
            vec2ub rightStick;
            vec2ub leftStick;
            u8 padUnk[12];
            i32 port;
            i32 slot;
            IO_PAD_STATE state;
            Button buttons;
            Button lastButtons;

            undefined4 field_0x178;
        
            static ioPad All[4];
            static bool MultitapConnected;
            static void UpdateAll();
    };

    f32 ioAddDeadZone(f32 p1, f32 p2);
};