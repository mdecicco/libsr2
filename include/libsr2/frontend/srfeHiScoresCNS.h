#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen6.h>

namespace sr2 {
    class srfeHiScoresCNS : public srfeUnknownScreen6 {
        public:
            srfeHiScoresCNS(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeHiScoresCNS();
    };
};