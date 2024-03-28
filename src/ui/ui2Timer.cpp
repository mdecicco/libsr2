#include <libsr2/ui/ui2Timer.h>

namespace sr2 {
    ui2Timer::ui2Timer(char* name, f32 time, i32 p3, i32 p4, const WidgetRef<ui2Master>& master) : ui2Widget(name, master, true) {
    }

    ui2Timer::~ui2Timer() {
    }
};