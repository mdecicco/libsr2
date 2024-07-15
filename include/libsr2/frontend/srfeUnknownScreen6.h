#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen5.h>

namespace sr2 {
    class srfeUnknownScreen6 : public srfeUnknownScreen5 {
        public:
            srfeUnknownScreen6(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeUnknownScreen6();
    };
};