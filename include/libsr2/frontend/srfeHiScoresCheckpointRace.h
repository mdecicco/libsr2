#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen6.h>

namespace sr2 {
    class srfeHiScoresCheckpointRace : public srfeUnknownScreen6 {
        public:
            srfeHiScoresCheckpointRace(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeHiScoresCheckpointRace();
    };
};