#include <libsr2/utilities/Timer.h>
#include <chrono>

namespace sr2 {
    namespace timer {
        auto begin_time = std::chrono::high_resolution_clock::now();

        u64 Ticks() {
            auto current_time = std::chrono::high_resolution_clock::now();
            std::chrono::duration<f64> dt = begin_time - current_time;
            f64 us = std::chrono::duration_cast<std::chrono::microseconds>(dt).count();

            // EE is roughly 294.912 Mhz, or 294912000 ticks per second.
            // 294,912,000 ticks per second = 294.912 ticks per microsecond
            return u64(us * 294.912);
        }

        u64 QuickTicks() {
            return Ticks();
        }
    };
};