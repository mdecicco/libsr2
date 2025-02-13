#include <libsr2/mission/Mission.h>
#include <libsr2/managers/missManager.h>
#include <libsr2/io/TaggedStream.h>
#include <libsr2/io/parCsvFile.h>
#include <libsr2/io/stream.h>
#include <libsr2/utilities/utils.h>

#include <string.h>

namespace sr2 {
    UnkMissionObj0::UnkMissionObj0() {
        unk2 = 1;
        unk0 = -3.0f;
    }
    
    void UnkMissionObj0::setUnk2(undefined4 value) {
        unk2 = value;
    }



    Mission::Mission() {
        for (u32 i = 0;i < 16;i++) {
            vehicle_positions[i].position.x = 10.0f;
            vehicle_positions[i].position.y = 100.0f;
            vehicle_positions[i].position.z = 10.0f;
            vehicle_positions[i].rot_y = 0.0f;
            b_unk3[i] = 1;
            b_unk4[i] = 0.0f;
        }

        memset(b_unk6, 0, 32);
        b_unk0 = 0;
        b_unk1 = nullptr;
        b_unk5 = 0;
    }

    Mission::~Mission() {
    }

    void Mission::FUN_0019c698() {

    }

    void Mission::debugPrintBaseState() {
        printf("            base:\n");
        printf("                unk0: 0x%X\n", b_unk0);
        if (b_unk1) {
            printf("                unk1:\n");
            printf("                    unk0: %f\n", b_unk1->unk0);
            printf("                    unk1: 0x%X\n", b_unk1->unk1);
            printf("                    unk2: 0x%X\n", b_unk1->unk2);
        } else {
            printf("                unk1: null\n");
        }
        
        printf("                initial vehicle positions:\n");
        u32 vc = missManager::get()->getAICount();
        vc += missManager::get()->getHumanCount();
        vc += missManager::get()->getParkedCount();
        for (u32 i = 0;i < vc;i++) {
            auto& o = vehicle_positions[i];
            printf("                    [%d]: %f, %f, %f (rotY %f)\n", i, o.position.x, o.position.y, o.position.z, o.rot_y);
        }
        
        for (u32 i = 0;i < vc;i++) {
            printf("                unk3[%d]: 0x%X\n", i, b_unk3[i]);
        }
        
        for (u32 i = 0;i < vc;i++) {
            printf("                unk4[%d]: %f\n", i, b_unk4[i]);
        }
        printf("                unk5: 0x%X\n", b_unk5);
        printf("                unk6: '%s'\n", b_unk6);
    }

    void Mission::debugPrintState() {
        debugPrintBaseState();
        printf("            derived: (not implemented)\n");
    }
};