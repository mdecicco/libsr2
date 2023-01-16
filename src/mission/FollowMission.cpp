#include <libsr2/mission/FollowMission.h>
#include <libsr2/managers/missManager.h>
#include <libsr2/io/TaggedStream.h>
#include <libsr2/io/stream.h>
#include <libsr2/utilities/color.h>

namespace sr2 {
    FollowMission::FollowMission() {
        md1_unk0 = 1;
        md1_unk1 = 1;
        md1_unk2 = 0;
        md1_unk3 = 0;
        md1_unk4 = 0;
        md1_unk5 = 0;
        md1_unk6 = 0;
        follow_marker = new giColorRGB();
        follow_marker->set_name("FollowMarker");
        follow_marker->loadAndUpdate();
    }
    bool FollowMission::load(TaggedStream* tsv) {
        Stream* fp = tsv->getStream();
        u16 tag;
        u32 size;
        u32 i = 0;

        while (true) {
            while (true) {
                while (true) {
                    while (true) {
                        if (!tsv->readTag(&tag, &size)) return false;
                        if (tag != 0x506) break;

                        if (!fp->read(b_unk6, 32)) return false;
                    }

                    if (tag > 0x506) break;
                    if (tag == 0x503) {
                        if (!fp->read(&some_dist_1, 4)) return false;
                    } else if (tag < 0x504) {
                        if (tag == 0x501) {
                            fp->read(&vehicle_positions[i++], 16);
                            if (i > 16) return false;
                        }
                        else if (tag == 0x502 && !fp->read(&some_dist_0, 4)) return false;
                    }
                    else if (tag == 0x504 && !fp->read(&md1_unk9, 2)) return false;
                    else if (tag == 0x505 && !fp->read(&md1_unk4, 12)) return false;
                }

                if (tag == 0x509) break;
                if (tag < 0x50a) {
                    if (tag == 0x507 && !fp->read(&b_unk5, 4)) return false;
                    else if (tag == 0x508 && !fp->read(&md1_unk3, 4)) return false;
                }
                else if (tag == 0x50b && !fp->read(&md1_unk0, 1)) return false;
                else if (tag < 0x50b && !fp->read(&md1_unk1, 1)) return false;
                else if (tag == 0x5ff) {
                    method_0x50();
                    return true;
                }
            }
            
            u32 readSz = size / 8 << 2;
            if (size / 8 > 16) break;
            fp->read(b_unk3, readSz);
            fp->read(b_unk4, readSz);
        }

        return false;
    }
    void FollowMission::method_0x40() { }
    void FollowMission::method_0x50() {
        b_unk1 = new UnkMissionObj0();
        b_unk1->setUnk2(md1_unk1 == 1);

        missManager* mgr = missManager::get();
        // for (u32 i = 0;i < mgr->getHumanCount();i++) {
        //     *(bool*)(get_vehicle(i)->field6_0x18 + 0x38)) = md1_unk0 == 1;
        // }
    }
    void FollowMission::debugPrintState() {
        debugPrintBaseState();
        printf("            derived:\n");
        printf("                unk0: 0x%X\n", md1_unk0);
        printf("                unk1: 0x%X\n", md1_unk1);
        printf("                unk2: 0x%X\n", md1_unk2);
        printf("                unk3: 0x%X\n", md1_unk3);
        printf("                unk4: 0x%X\n", md1_unk4);
        printf("                unk5: 0x%X\n", md1_unk5);
        printf("                unk6: 0x%X\n", md1_unk6);
        printf("                unk7: %f\n", some_dist_1);
        printf("                unk8: %f\n", some_dist_0);
        printf("                unk9: 0x%X\n", md1_unk9);
        if (follow_marker) {
            printf("                follow_marker: %f, %f, %f\n", follow_marker->r, follow_marker->g, follow_marker->b);
        } else {
            printf("                follow_marker: null\n");
        }
    }
};