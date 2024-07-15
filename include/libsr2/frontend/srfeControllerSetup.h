#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen3.h>

namespace sr2 {
    class srfeControllerSetup : public srfeUnknownScreen3 {
        public:
            srfeControllerSetup(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeControllerSetup();
    };
};