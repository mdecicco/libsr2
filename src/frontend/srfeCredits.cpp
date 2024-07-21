#include <libsr2/frontend/srfeCredits.h>

namespace sr2 {
    srfeCredits::srfeCredits(const char* name, const WidgetRef<ui2Master>& master) : srfeUnknownScreen3(name, master) {
    }

    srfeCredits::~srfeCredits() {
    }

    void srfeCredits::FUN_0011c270() {
        field_0xd0 = 0;
        field_0xcc = 1;
    }
};