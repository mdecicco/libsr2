#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen1.h>

namespace sr2 {
    class srfeUnknownScreen2 : public srfeUnknownScreen1 {
        public:
            srfeUnknownScreen2(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeUnknownScreen2();
    };
};