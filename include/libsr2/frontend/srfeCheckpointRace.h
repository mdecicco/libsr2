#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBasicLayout2.h>

namespace sr2 {
    class srfeCheckpointRace : public srfeBasicLayout2 {
        public:
            srfeCheckpointRace(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeCheckpointRace();
    };
};