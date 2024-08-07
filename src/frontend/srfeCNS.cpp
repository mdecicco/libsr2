#include <libsr2/frontend/srfeCNS.h>

namespace sr2 {
    srfeCNS::srfeCNS(const char* name, const WidgetRef<ui2Master>& master) : srfeBasicLayout2(name, master) {
    }

    srfeCNS::~srfeCNS() {
    }
};