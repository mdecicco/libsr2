#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeHiScoresBase.h>

namespace sr2 {
    class srfeUnknownScreen6 : public srfeHiScoresBase {
        public:
            srfeUnknownScreen6(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeUnknownScreen6();
    };
};