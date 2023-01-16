#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class PhysInstance;
    struct TriggerMgrInstanceInfo {
        PhysInstance* inst;
        u32 index;
    };

    class TriggerTarget {
        public:
            TriggerTarget();
            virtual ~TriggerTarget();
    };

    class Trigger : public TriggerTarget {
        public:
            Trigger();
            virtual ~Trigger();

            Trigger* next;
    };

    class TriggerMgr {
        public:
            static void Create();
            static void Destroy();
            static TriggerMgr* Get();

        private:
            static TriggerMgr* instance;
            TriggerMgr();
            ~TriggerMgr();

            u32 player_capacity;
            u32 total_player_count;
            u32 local_player_count;
            u32 player_count;
            u32 opponent_count;

            u32 ambient_capacity;
            u32 ambient_count;

            u32 current_mission_idx;
            char mission_name[32];

            TriggerMgrInstanceInfo* players;
            TriggerMgrInstanceInfo* opponents;
            TriggerMgrInstanceInfo* ambients;
            Trigger* trigger_list_a;
            Trigger* trigger_list_b;
    };
};