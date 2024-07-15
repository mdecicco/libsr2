#include <libsr2/frontend/srfeOptions.h>

namespace sr2 {
    srfeOptions::srfeOptions(const char* name, const WidgetRef<ui2Master>& master) : srfeUnknownScreen3(name, master) {
    }

    srfeOptions::~srfeOptions() {
    }
};