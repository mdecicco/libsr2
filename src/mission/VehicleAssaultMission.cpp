#include <libsr2/mission/VehicleAssaultMission.h>
#include <libsr2/io/TaggedStream.h>
#include <libsr2/io/stream.h>

namespace sr2 {
    VehicleAssaultMission::VehicleAssaultMission() {
        md0_unk0 = 0;
        md0_unk1 = 0;
        md0_unk2 = 0;
        md0_unk3 = 1;                                                       
        md0_unk4 = 1;
        md0_unk5.x = 0;
        md0_unk5.y = 0;
        md0_unk5.z = 0;
        md0_unk8 = 0;
    }

    bool VehicleAssaultMission::load(TaggedStream* tsv) {
        Stream* fp = tsv->getStream();
        u32 i = 0;
        while (true) {
            u16 tag;
            u32 size;
            if (!tsv->readTag(&tag,&size)) return false;
            if (tag == 0x706) break;

            if (tag < 0x707) {
                if (tag == 0x703 && !fp->read(&md0_unk5.x, 12)) return false;
                else if (tag < 0x704) {
                    if (tag == 0x701) {
                        fp->read(&vehicle_positions[i++], 16);
                        if (i > 16) return false;
                    } else if (tag == 0x702 && !fp->read(&md0_unk2, 2)) return false;
                } else if (tag == 0x704 && !fp->read(&target_vehicle_idx, 2)) return false;
                else if (tag == 0x705 && !fp->read(&initial_time, 4)) return false;
            }
            else if (tag == 0x709) {
                u32 readSz = size / 8 << 2;
                if ((size / 8) > 16) return false;
                fp->read(b_unk3, readSz);
                fp->read(b_unk4, readSz);
            }
            else if (tag < 0x70a) {
                if (tag == 0x707 && !fp->read(&b_unk5, 4)) return false;
                else if (tag == 0x708 && !fp->read(&md0_unk8, 4)) return false;
            }
            else if (tag == 0x70b && !fp->read(&md0_unk4, 1)) return false;
            else if (tag < 0x70b && !fp->read(&md0_unk3,1)) return false;
            else if (tag == 0x7ff) {
                method_0x50();
                return true;
            }
        
            if (size != 32) return false;
            if (!fp->read(b_unk6, 32)) return false;
        }

        return false;
    }
    void VehicleAssaultMission::method_0x40() { }
    void VehicleAssaultMission::method_0x50() {
        b_unk1 = new UnkMissionObj0();
        b_unk1->setUnk2(md0_unk3 == 1);
    }
    void VehicleAssaultMission::debugPrintState() {
        debugPrintBaseState();
        printf("            derived:\n");
        printf("                unk0: 0x%X\n", md0_unk0);
        printf("                unk1: 0x%X\n", md0_unk1);
        printf("                unk2: 0x%X\n", md0_unk2);
        printf("                unk3: 0x%X\n", md0_unk3);
        printf("                unk4: 0x%X\n", md0_unk4);
        printf("                unk5: %f, %f, %f\n", md0_unk5.x, md0_unk5.y, md0_unk5.z);
        printf("                target_vehicle_idx: %d\n", target_vehicle_idx);
        printf("                initial_time: %f\n", initial_time);
        printf("                unk8: 0x%X\n", md0_unk8);
    }

};