#include <libsr2/frontend/srfeMissionSelect.h>

namespace sr2 {
    srfeMissionSelect::srfeMissionSelect(const char* name, const WidgetRef<ui2Master>& master) : srfeBasicLayout2(name, master) {
    }

    srfeMissionSelect::~srfeMissionSelect() {
    }
};