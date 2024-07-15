#include <libsr2/frontend/srfeVehicleSel.h>

namespace sr2 {
    srfeVehicleSel::srfeVehicleSel(const char* name, const char* p2, const char* p3, const WidgetRef<ui2Master>& master) : srfeUnknownScreen3(name, master) {
    }

    srfeVehicleSel::~srfeVehicleSel() {
    }
};