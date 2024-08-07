#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBasicLayout2.h>

namespace sr2 {
    class srfePlayerSel : public srfeBasicLayout2 {
        public:
            srfePlayerSel(const char* name, const char* p2, const char* p3, undefined4 p4, const WidgetRef<ui2Master>& master);
            ~srfePlayerSel();
    };
};