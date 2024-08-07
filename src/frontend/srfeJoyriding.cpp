#include <libsr2/frontend/srfeJoyriding.h>

namespace sr2 {
    srfeJoyriding::srfeJoyriding(const char* name, const WidgetRef<ui2Master>& master) : srfeBasicLayout2(name, master) {
    }

    srfeJoyriding::~srfeJoyriding() {
    }
};