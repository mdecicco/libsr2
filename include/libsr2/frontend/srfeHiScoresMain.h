#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBasicLayout1.h>

namespace sr2 {
    class srfeHiScoresMain : public srfeBasicLayout1 {
        public:
            srfeHiScoresMain(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeHiScoresMain();
    };
};