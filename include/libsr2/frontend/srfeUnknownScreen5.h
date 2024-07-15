#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen3.h>

namespace sr2 {
    class srfeUnknownScreen5 : public srfeUnknownScreen3 {
        public:
            srfeUnknownScreen5(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeUnknownScreen5();
    };
};