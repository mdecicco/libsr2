#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen3.h>

namespace sr2 {
    class srfePlayerSel : public srfeUnknownScreen3 {
        public:
            srfePlayerSel(const char* name, const char* p2, const char* p3, undefined4 p4, const WidgetRef<ui2Master>& master);
            ~srfePlayerSel();
    };
};