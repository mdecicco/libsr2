#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen3.h>

namespace sr2 {
    class srfeVehicleSel : public srfeUnknownScreen3 {
        public:
            srfeVehicleSel(const char* name, const char* p2, const char* p3, const WidgetRef<ui2Master>& master);
            ~srfeVehicleSel();
    };
};