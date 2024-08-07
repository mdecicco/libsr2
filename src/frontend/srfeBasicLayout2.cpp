#include <libsr2/frontend/srfeBasicLayout2.h>

namespace sr2 {
    srfeBasicLayout2::srfeBasicLayout2(const char* name, const WidgetRef<ui2Master>& master) : srfeBackground(name, master) {
    }

    srfeBasicLayout2::~srfeBasicLayout2() {
    }
};