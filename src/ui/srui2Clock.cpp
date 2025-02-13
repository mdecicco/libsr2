#include <libsr2/ui/srui2Clock.h>
#include <libsr2/managers/datTimeManager.h>

#include <time.h>


namespace sr2 {
    srui2Clock::srui2Clock(const char* name, i32 x, i32 y, i32 p4, const WidgetRef<ui2Master>& master) : ui2DynamicText(name, x, y, p4, "", 0xc, master) {
    }

    srui2Clock::~srui2Clock() {
    }

    bool srui2Clock::updateTime() {
        time_t now = time(nullptr);
        struct tm* tm = localtime(&now);
        setTime(tm->tm_hour, tm->tm_min, tm->tm_sec, 0);
        return true;
    }

    void srui2Clock::setTime(u32 hour, u32 minute, u32 second, u32 centisecond) {
        setTimeSeconds(f32((((hour * 60) + minute) * 60) + second) + (f32(centisecond) / 100.0f));
    }

    void srui2Clock::setTimeSeconds(f32 seconds) {
        if (currentSeconds == seconds) return;
        currentSeconds = seconds;
        currentElapsedTime = datTimeManager::ElapsedTime;
    }
};