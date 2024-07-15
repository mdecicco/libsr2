#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen2.h>

namespace sr2 {
    class srfeVehicleSel2Pl : public srfeUnknownScreen2 {
        public:
            srfeVehicleSel2Pl(const char* name, const char* p2, const char* p3, const WidgetRef<ui2Master>& master);
            ~srfeVehicleSel2Pl();
    };
};