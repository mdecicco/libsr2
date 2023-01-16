#pragma once
#include <libsr2/types.h>
#include <libsr2/mission/Mission.h>

namespace sr2 {
    class TaggedStream;
    class CinematicMission : public Mission {
        public:
            virtual bool load(TaggedStream* tsv);
            virtual void method_0x40();
            virtual void method_0x50();

            undefined4 unk0;
    };
};