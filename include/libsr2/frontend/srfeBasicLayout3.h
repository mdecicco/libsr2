#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBackground.h>

namespace sr2 {
    class srfeBasicLayout3 : public srfeBackground {
        public:
            srfeBasicLayout3(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeBasicLayout3();
    };
};