#pragma once
#include <libsr2/types.h>
#include <libsr2/mission/Mission.h>

namespace sr2 {
    struct RoundContrabandInfo {
        undefined position_idx;
        undefined contraband_idx;
        undefined2 unk2;
        undefined2 unk3;
        
        bool load(TaggedStream* tsv, u32 unk);
    };

    struct UnkMissionObj2 {
    };

    class ContrabandMissionRound {
        public:
            ContrabandMissionRound();
            ~ContrabandMissionRound();

            bool load(TaggedStream* tsv, u32 l);

            u8 contraband_count;
            RoundContrabandInfo contraband_info[16];
            u8 goal_position_indices[2];
            undefined4 unk2;
            undefined2 unk3;
            u8 unk4_count;
            undefined2* unk4_arr;
    };

    struct UnkMissionObj4 {

    };

    class UnkMissionObj5 {
        public:
            UnkMissionObj5();
            ~UnkMissionObj5();

            static UnkMissionObj5* Data;
    };

    class ContrabandMission : public Mission {
        public:
            ContrabandMission();
            ~ContrabandMission();

            virtual bool load(TaggedStream* tsv);
            virtual void method_0x40();
            virtual void method_0x50();

            void FUN_001a20b0();

            virtual void debugPrintState();
        
        private:
            undefined4 some_unk3_obj_idx;
            UnkMissionObj2* md4_unk10;
            undefined4 md4_unk11;
            UnkMissionObj4 unk4_objs[2];
            f32 md4_unk12;
            char md4_unk0[32];
            undefined4 md4_unk6;
            char md4_unk2[128];
            u16 round_count;
            ContrabandMissionRound* rounds;
            u32 md4_unk7;
            undefined4 md4_unk8;
            undefined4 md4_unk9;
            f32 md4_unk4;
            undefined4 md4_unk5;
            undefined md4_unk3;
            undefined md4_unk1;
            vec3f* contraband_positions;
            vec4f* positions;
            u16 contraband_position_count;
            u16 position_count;
    };
};