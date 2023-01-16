#pragma once
#include <libsr2/types.h>
#include <libsr2/mission/Mission.h>

namespace sr2 {
    class TaggedStream;
    class VehicleAssaultMission : public Mission {
        public:
            VehicleAssaultMission();

            virtual bool load(TaggedStream* tsv);
            virtual void method_0x40();
            virtual void method_0x50();
            virtual void debugPrintState();

            undefined4 md0_unk0;
            undefined md0_unk1, md0_unk2, md0_unk3, md0_unk4;
            vec3f md0_unk5;
            undefined2 target_vehicle_idx;
            f32 initial_time; // minutes
            undefined4 md0_unk8;
    };
    
};