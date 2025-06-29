#include <libsr2/ui/srui2VehicleDisplay.h>

namespace sr2 {
    srui2VehicleDisplay::srui2VehicleDisplay(
        const char* name,
        i32 x,
        i32 y,
        i32 p4,
        const WidgetRef<ui2Master>& master
    ) : ui2Visual(name, x, y, master) {
        m_elem = nullptr;
    }

    srui2VehicleDisplay::~srui2VehicleDisplay() {
        if (m_elem) uiRenderer::destroyElement(m_elem);
    }
};