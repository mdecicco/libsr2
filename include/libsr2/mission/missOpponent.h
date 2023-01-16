#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class TaggedStream;
    
    class missPlayer {
        public:
            missPlayer();
            bool read(TaggedStream* tsv, u16 unk);

            i32 team_idx;
            VEHICLE_TYPE vehicle_tp;
            i32 geom_idx;
            u16 agent_idx;
            undefined2 unk3;
            bool is_human;
    };
};