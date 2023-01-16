#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class datTimeManager {
        public:
            datTimeManager();
            ~datTimeManager();

            f32 InvSeconds;
            f32 Seconds;
    };

    extern datTimeManager g_datTimeManager;
};