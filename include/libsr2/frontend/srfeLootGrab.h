#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBasicLayout2.h>

namespace sr2 {
    class srfeLootGrab : public srfeBasicLayout2 {
        public:
            srfeLootGrab(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeLootGrab();
    };
};