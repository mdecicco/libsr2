#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBasicLayout1.h>

namespace sr2 {
    class srfeSavegameSelect : public srfeBasicLayout1 {
        public:
            srfeSavegameSelect(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeSavegameSelect();
    };
};