#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBasicLayout2.h>

namespace sr2 {
    class srfeVehicleSel : public srfeBasicLayout2 {
        public:
            srfeVehicleSel(const char* name, const char* p2, const char* p3, const WidgetRef<ui2Master>& master);
            ~srfeVehicleSel();
    };
};