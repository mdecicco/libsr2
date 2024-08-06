#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeHiScoresBase.h>

namespace sr2 {
    class srfeHiScoresSmuggler : public srfeHiScoresBase {
        public:
            srfeHiScoresSmuggler(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeHiScoresSmuggler();
    };
};