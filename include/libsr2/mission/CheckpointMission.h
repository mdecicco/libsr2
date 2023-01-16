#pragma once
#include <libsr2/types.h>
#include <libsr2/mission/Mission.h>

namespace sr2 {
    struct CheckpointInfo {
        CheckpointInfo();
        bool load(TaggedStream* tsv, undefined4 unk);

        vec3f position;
        f32 unk1;
        char unk2[32];
        f32 bonus_time;
    };

    class CheckpointMission : public Mission {
        public:
            virtual bool load(TaggedStream* tsv);
            virtual void method_0x40();
            virtual void method_0x50();

            virtual void debugPrintState();

            undefined4 unk0;
            undefined unk1, unk2;
            u32 checkpoint_count;
            CheckpointInfo* checkpoints[32];
            undefined4 unk4;
            char some_str_0[32];
            char some_str_1[32];
    };
};