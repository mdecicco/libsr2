#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBasicLayout1.h>

namespace sr2 {
    class srfeTurfWars : public srfeBasicLayout1 {
        public:
            srfeTurfWars(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeTurfWars();
    };
};