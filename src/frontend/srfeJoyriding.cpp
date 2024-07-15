#include <libsr2/frontend/srfeJoyriding.h>

namespace sr2 {
    srfeJoyriding::srfeJoyriding(const char* name, const WidgetRef<ui2Master>& master) : srfeUnknownScreen3(name, master) {
    }

    srfeJoyriding::~srfeJoyriding() {
    }
};