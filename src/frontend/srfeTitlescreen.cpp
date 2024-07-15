#include <libsr2/frontend/srfeTitlescreen.h>

namespace sr2 {
    srfeTitlescreen::srfeTitlescreen(const char* name, const WidgetRef<ui2Master>& master) : srui2SRScreen(name, master) {
    }

    srfeTitlescreen::~srfeTitlescreen() {
    }
};