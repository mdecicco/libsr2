#pragma once
#include <libsr2/types.h>
#include <libsr2/mission/missOpponent.h>
#include <string>

namespace sr2 {
    class TaggedStream;
    class Mission;
    class ContrabandInfo;
    class Cls_00395078;

    struct MissionScoringData {
        i32 is_loaded_I_guess;
        i32 values[39][4];
    };

    class missManager {
        public:
            static void create();
            static void destroy();
            static missManager* get();
            missPlayer* getPlayer(u32 idx);
            u32 getHumanCount() const;
            u32 getAICount() const;
            u32 getParkedCount() const;

            bool load(const std::string& map_name, const std::string& mission);
            bool loadScoring();

            void debugPrintState(bool summarize);

        protected:
            missManager();
            ~missManager();
            static missManager* Instance;

            bool loadMissions(TaggedStream* tsv);
            bool loadContraband(TaggedStream* tsv);

            u32 m_aiCount;
            u32 m_humanCount;
            u32 m_parkedCount;
            missPlayer m_players[16];
            u32 m_missionCount;
            Mission* m_missions[32];
            u32 m_missionTypes[32];
            MissionScoringData m_scoring;
            u8 m_contrabandCount;
            ContrabandInfo* m_contraband;
            Cls_00395078* m_unk0;
            Cls_00395078* m_unk1;

            undefined4 m_field21_0x130;
            char name[32];
    };
};