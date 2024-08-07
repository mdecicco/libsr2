#include <libsr2/frontend/srfeControllerSetup.h>

namespace sr2 {
    srfeControllerSetup::srfeControllerSetup(const char* name, const WidgetRef<ui2Master>& master) : srfeBasicLayout2(name, master) {
    }

    srfeControllerSetup::~srfeControllerSetup() {
    }
};