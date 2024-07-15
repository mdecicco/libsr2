#include <libsr2/ui/srui2Clock.h>

namespace sr2 {
    srui2Clock::srui2Clock(const char* name, i32 x, i32 y, i32 p4, const WidgetRef<ui2Master>& master) : ui2DynamicText(name, x, y, p4, "", 0xc, master) {
    }

    srui2Clock::~srui2Clock() {
    }
};