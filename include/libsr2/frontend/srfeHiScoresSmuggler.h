#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen5.h>

namespace sr2 {
    class srfeHiScoresSmuggler : public srfeUnknownScreen5 {
        public:
            srfeHiScoresSmuggler(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeHiScoresSmuggler();
    };
};