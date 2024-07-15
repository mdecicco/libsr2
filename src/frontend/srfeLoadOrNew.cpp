#include <libsr2/frontend/srfeLoadOrNew.h>

namespace sr2 {
    srfeLoadOrNew::srfeLoadOrNew(const char* name, const WidgetRef<ui2Master>& master) : srfeUnknownScreen4(name, master) {
    }

    srfeLoadOrNew::~srfeLoadOrNew() {
    }
};