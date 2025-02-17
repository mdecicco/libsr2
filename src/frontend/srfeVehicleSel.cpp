#include <libsr2/frontend/srfeVehicleSel.h>

namespace sr2 {
    srfeVehicleSel::Widgets* srfeVehicleSel::WidgetsInstance = nullptr;
    u32 srfeVehicleSel::ActiveCount = 0;
    u32 srfeVehicleSel::InitializedCount = 0;

    srfeVehicleSel::srfeVehicleSel(
        const char* name,
        const char* prevScreenName,
        const char* nextScreenName,
        const WidgetRef<ui2Master>& master
    ) : srfeBasicLayout2(name, master) {
        m_prevScreenName = prevScreenName;
        m_nextScreenName = nextScreenName;

        field_0xb8 = 0;
        field_0xc4 = 0;
        field_0xc8 = 0;

        if (m_isActive) ActiveCount++;
        if (WidgetsInstance) {
            if (ActiveCount == 1) WidgetsInstance->master->setActive(true);
        }
    }

    srfeVehicleSel::~srfeVehicleSel() {
        deinitScreen();

        if (m_isActive) ActiveCount--;
    }
};