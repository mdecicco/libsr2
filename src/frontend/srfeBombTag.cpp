#include <libsr2/frontend/srfeBombTag.h>

namespace sr2 {
    srfeBombTag::srfeBombTag(const char* name, const WidgetRef<ui2Master>& master) : srfeUnknownScreen3(name, master) {
    }

    srfeBombTag::~srfeBombTag() {
    }
};