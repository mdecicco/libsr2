#include <libsr2/frontend/srfeCredits.h>

namespace sr2 {
    srfeCredits::srfeCredits(const char* name, const WidgetRef<ui2Master>& master) : srfeBasicLayout2(name, master) {
    }

    srfeCredits::~srfeCredits() {
    }

    void srfeCredits::FUN_0011c260() {
        field_0xcc = 0;
        field_0xd0 = 1;
    }

    void srfeCredits::FUN_0011c270() {
        field_0xcc = 1;
        field_0xd0 = 0;
    }
};