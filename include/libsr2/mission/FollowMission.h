#pragma once
#include <libsr2/types.h>
#include <libsr2/mission/Mission.h>

namespace sr2 {
    class TaggedStream;
    class giColorRGB;
    class FollowMission : public Mission {
        public:
            FollowMission();
            virtual bool load(TaggedStream* tsv);
            virtual void method_0x40();
            virtual void method_0x50();
            virtual void debugPrintState();
            
            undefined md1_unk0, md1_unk1;
            undefined4 md1_unk2, md1_unk3, md1_unk4, md1_unk5, md1_unk6;
            f32 some_dist_1, some_dist_0;
            undefined2 md1_unk9;
            giColorRGB* follow_marker;
    };
};