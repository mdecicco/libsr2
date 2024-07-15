#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen3.h>

namespace sr2 {
    class srfeLootGrab : public srfeUnknownScreen3 {
        public:
            srfeLootGrab(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeLootGrab();
    };
};