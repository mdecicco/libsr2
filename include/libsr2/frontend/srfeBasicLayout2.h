#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBackground.h>

namespace sr2 {
    class srfeBasicLayout2 : public srfeBackground {
        public:
            srfeBasicLayout2(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeBasicLayout2();
    };
};