#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen4.h>

namespace sr2 {
    class srfeHiScoresMain : public srfeUnknownScreen4 {
        public:
            srfeHiScoresMain(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeHiScoresMain();
    };
};