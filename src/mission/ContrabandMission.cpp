#include <libsr2/mission/ContrabandMission.h>
#include <libsr2/managers/missManager.h>
#include <libsr2/io/TaggedStream.h>
#include <libsr2/io/stream.h>
#include <libsr2/vehicle/vehicle.h>

namespace sr2 {
    bool RoundContrabandInfo::load(TaggedStream* tsv, u32 unk) {
        Stream* fp = tsv->getStream();
        if (!fp->read(&position_idx, 1)) return false;
        if (!fp->read(&contraband_idx, 1)) return false;
        if (!fp->read(&unk2, 2)) return false;
        if (unk < 3) return true;

        if (!fp->read(&unk3, 2)) return false;
        return true;
    }


    ContrabandMissionRound::ContrabandMissionRound() {}
    ContrabandMissionRound::~ContrabandMissionRound() {}
    bool ContrabandMissionRound::load(TaggedStream* tsv, u32 unk) {
        Stream* fp = tsv->getStream();
        int iVar1;
        int iVar2;
        undefined2 *puVar3;
        undefined4 uVar4;
        ContrabandMissionRound *pUVar5;
        int iVar6;
        undefined (*pauVar7) [6];
        
        if (!fp->read(&contraband_count, 1)) return false;
        for (u32 i = 0;i < contraband_count;i++) {
            if (!contraband_info[i].load(tsv, unk)) return false;
        }

        if (!fp->read(&goal_position_indices[0], 1)) return false;
        if (!fp->read(&goal_position_indices[1], 1)) return false;

        if (((unk < 2) || fp->read(&unk2, 4)) && ((unk < 4 || fp->read(&unk3, 2)))) {
            if (unk < 5) return true;
            if (!fp->read(&unk4_count, 1)) return false;
            if (unk4_count > 0) unk4_arr = new u16[unk4_count];
            for (u32 i = 0;i < unk4_count;i++) {
                if (!fp->read(&unk4_arr[i], 2)) return false;
            }

            return true;
        }

        return false;
    }

    UnkMissionObj5* UnkMissionObj5::Data = nullptr;
    UnkMissionObj5::UnkMissionObj5() {}
    UnkMissionObj5::~UnkMissionObj5() {}

    ContrabandMission::ContrabandMission() {
        md4_unk0[0] = 0;
        md4_unk1 = 1;
        md4_unk2[0] = 0;
        md4_unk3 = 1;
        md4_unk4 = 100.0f;
        md4_unk5 = 5;
        md4_unk6 = 0;
        round_count = 0;
        rounds = nullptr;
        md4_unk7 = 0;
        md4_unk8 = 0;
        md4_unk9 = 0;
        position_count = 0;
        positions = nullptr;
        contraband_position_count = 0;
        contraband_positions = nullptr;
        md4_unk10 = nullptr;
        md4_unk11 = 0;
        some_unk3_obj_idx = 0;
        b_unk1 = nullptr;
        md4_unk12 = 0.0f;

        UnkMissionObj5::Data = new UnkMissionObj5[16];
    }
    ContrabandMission::~ContrabandMission() {
        if (rounds) {
            delete [] rounds;
            rounds = nullptr;
        }

        if (contraband_positions) delete [] contraband_positions;
        contraband_positions = nullptr;

        if (positions) delete [] positions;
        positions = nullptr;

        if (md4_unk10) delete [] md4_unk10;
        md4_unk10 = nullptr;

        delete [] UnkMissionObj5::Data;
        UnkMissionObj5::Data = nullptr;
    }
    bool ContrabandMission::load(TaggedStream* tsv) {
        Stream* fp = tsv->getStream();
        u16 tag;
        u32 size;
        u32 n = 0;
        u32 x = 0;
        while (true) {
            while (true) {
                if (!tsv->readTag(&tag, &size)) return false;
                if (tag != 0x40b) break;
                if (!fp->read(&md4_unk9, 4)) return false;
            }

            if (tag < 0x40c) {
                if (tag == 0x405) {
                    if (!fp->read(&position_count, 2)) return false;
                    if (position_count > 0) {
                        positions = new vec4f[position_count];
                        for (u32 i = 0;i < position_count;i++) {
                            if (!fp->read(&positions[i], 16)) return false;
                        }
                    }
                    continue;
                }
                if (tag < 0x406) {
                    if (tag == 0x402) {
                        if (!fp->read(&round_count, 2)) return false;
                        if (round_count > 0) {
                            rounds = new ContrabandMissionRound[round_count];
                        }
                        continue;
                    }
                    
                    if (tag < 0x403) {
                        if (tag == 0x401) {
                            u8 u = 0;
                            if (!fp->read(&u, 1)) return false;
                            if (!fp->read(&md4_unk0, u)) return false;
                            md4_unk0[u] = 0;

                            u16 u1 = 0;
                            if (!fp->read(&u1, 2)) return false;
                            if (!fp->read(&md4_unk2, u1)) return false;
                            md4_unk2[u1] = 0;

                            if (tag == 0x40d && !fp->read(&md4_unk6, 4)) return false;
                        }
                        continue;
                    }

                    if (tag != 0x403) {
                        if (tag == 0x404) {
                            if (!fp->read(&contraband_position_count, 2)) return false;
                            if (contraband_position_count > 0) {
                                contraband_positions = new vec3f[contraband_position_count];
                                for (u32 i = 0;i < contraband_position_count;i++) {
                                    if (!fp->read(&contraband_positions[i], 12)) return false;
                                }
                            }
                        }
                        continue;
                    }
                }
                else {
                    if (tag == 0x408) {
                        if (!fp->read(&md4_unk8, 4)) return false;
                        continue;
                    }

                    if (tag < 0x409) {
                        if (tag == 0x406) {
                            fp->read(&vehicle_positions[n++], 16);
                            if (n > 16) return false;
                        }
                        else if ((tag == 0x407) && !fp->read(&md4_unk7, 4)) return false;
                        continue;
                    }

                    if (tag != 0x409) {
                        if (tag == 0x40a) {
                            u32 sz = (size / 8) << 2;
                            if ((sz / 8) >= 16) return false;
                            fp->read(&b_unk3, sz);
                            fp->read(&b_unk4, sz);
                        }
                        continue;
                    }
                }
            } else {
                if (tag != 0x411) {
                    if (0x411 < tag) {
                        if (tag == 0x414) {
                            if (!fp->read(&md4_unk3, 1)) return false;
                        }
                        else if (tag < 0x415) {
                            if (tag == 0x412 && !fp->read(&b_unk6, 32)) return false;
                            else if ((tag == 0x413) && !fp->read(&b_unk5, 4)) return false;
                        }
                        else if (tag == 0x415) {
                            if (!fp->read(&md4_unk1, 1)) return false;
                        }
                        else if (tag == 0x4ff) {
                            method_0x50();
                            return true;
                        }
                        continue;
                    }
                    
                    if (tag != 0x40e) {
                        if (0x40e < tag) {
                            if (tag == 0x40f) {
                                if (!fp->read(&md4_unk4, 4)) return false;
                            }
                            else if ((tag == 0x410) && !fp->read(&md4_unk5, 4)) return false;
                            continue;
                        }

                        if (tag != 0x40c) {
                            if (tag == 0x40d) {
                                u8 u = 0;
                                if (!fp->read(&u, 1)) return false;
                                if (!fp->read(&md4_unk0, u)) return false;
                                md4_unk0[u] = 0;

                                u16 u1 = 0;
                                if (!fp->read(&u1, 2)) return false;
                                if (!fp->read(&md4_unk2, u1)) return false;
                                md4_unk2[u1] = 0;

                                if (tag == 0x40d && !fp->read(&md4_unk6, 4)) return false;
                            }
                            continue;
                        }
                    }
                }
            }

            if (this->rounds == NULL) return false;

            u32 unk = 1;
            switch (((tag - 0x403) * 0x10000) >> 0x10) {
                case 6  : { unk = 2; break; }
                case 9  : { unk = 3; break; }
                case 0xb: { unk = 4; break; }
                case 0xe: { unk = 5; break; }
                default : { unk = 1; break; }
            }

            rounds[x++].load(tsv, unk);
        }

        return true;
    }
    void ContrabandMission::method_0x40() { }
    void ContrabandMission::method_0x50() { }
    void ContrabandMission::FUN_001a20b0() {
        for (u32 i = 0;i < missManager::get()->getHumanCount();i++) {
            Vehicle* v = Vehicle::GetVehicle(i);
            // v->cam_data_field_0x38 = md4_unk1 == 1;
        }

        FUN_0019c698();
        method_0x40();
    }


    void ContrabandMission::debugPrintState() {
        debugPrintBaseState();
        printf("            derived:\n");
        printf("                unk0: %s\n", md4_unk0);
        printf("                unk1: %d\n", md4_unk1);
        printf("                unk2: %s\n", md4_unk2);
        printf("                unk3: %d\n", md4_unk3);
        printf("                unk4: %f\n", md4_unk4);
        printf("                unk5: %d\n", md4_unk5);
        printf("                unk6: %d\n", md4_unk6);
        printf("                unk7: %d\n", md4_unk7);
        printf("                unk8: %d\n", md4_unk8);
        printf("                unk9: %d\n", md4_unk9);
        
        if (position_count == 0) {
            printf("                unk10: null\n");
        } else {
            printf("                unk10:\n");
            for (u32 i = 0;i < position_count;i++) {
                printf("                    [%d]: %f, %f, %f, %f\n", i, positions[i].x, positions[i].y, positions[i].z, positions[i].w);
            }
        }
    
        if (contraband_position_count == 0) {
            printf("                unk11: null\n");
        } else {
            printf("                unk11:\n");
            for (u32 i = 0;i < contraband_position_count;i++) {
                vec3f& v = contraband_positions[i];
                printf("                    [%d]: %f, %f, %f\n", i, v.x, v.y, v.z);
            }
        }
        
        if (round_count == 0) {
            printf("                rounds: null\n");
        } else {
            printf("                rounds:\n");
            for (u32 i = 0;i < round_count;i++) {
                ContrabandMissionRound& a = rounds[i];
                printf("                    [%d]:\n", i);
                if (a.contraband_count == 0) {
                    printf("                        contraband: empty\n");
                } else {
                    printf("                        contraband:\n");
                    for (u32 j = 0;j < a.contraband_count;j++) {
                        RoundContrabandInfo& b = a.contraband_info[j];
                        printf("                            [%d]: %d, %d, %d, %d\n", j, b.position_idx, b.contraband_idx, b.unk2, b.unk3);
                    }
                }
                printf("                        goal_position_indices: %d, %d\n", a.goal_position_indices[0], a.goal_position_indices[1]);
                printf("                        unk2: %d\n", a.unk2);
                printf("                        unk3: %d\n", a.unk3);
                
                if (a.unk4_count == 0) {
                    printf("                        unk4: empty\n");
                } else {
                    printf("                        unk4:");
                    for (u32 j = 0;j < a.unk4_count;j++) {
                        printf(" %d", a.unk4_arr[j]);
                    }
                    printf("\n");
                }
            }
        }
    }
};