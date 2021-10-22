#pragma once
#include <libsr2/types.h>

namespace sr2 {
    namespace timer {
        // 0x00337c18
        u64 Ticks();

        // 0x00337c28
        u64 QuickTicks();
    };
};