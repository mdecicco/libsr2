#include <libsr2/frontend/srfeNameInput.h>

namespace sr2 {
    srfeNameInput::srfeNameInput(const char* name, const WidgetRef<ui2Master>& master) : srfeUnknownScreen4(name, master) {
    }

    srfeNameInput::~srfeNameInput() {
    }
};