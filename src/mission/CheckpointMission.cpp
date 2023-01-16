#include <libsr2/mission/CheckpointMission.h>
#include <libsr2/managers/missManager.h>
#include <libsr2/io/TaggedStream.h>
#include <libsr2/io/stream.h>

namespace sr2 {
    CheckpointInfo::CheckpointInfo() {
        memset(&unk2, 0, 32);
        bonus_time = 0;
    }

    bool CheckpointInfo::load(TaggedStream* tsv, undefined4 unk) {
        Stream* fp = tsv->getStream();
        if (!fp->read(&position.x, 12)) return false;
        if (!fp->read(&unk1, 4)) return false;
        if (!fp->read(&unk2, 32)) return false;

        if (unk < 2) return true;
        if (!fp->read(&bonus_time, 4)) return false;
        return true;
    }


    
    bool CheckpointMission::load(TaggedStream* tsv) {
        Stream* fp = tsv->getStream();
        u16 tag;
        u32 size;
        u32 n = 0;
        checkpoint_count = 0;
        while (true) {
            while (true) {
                while (true) {
                    while (true) {
                        if (!tsv->readTag(&tag, &size)) return false;
                        if (tag != 0x306) break;
                        if (!fp->read(&unk0, 4)) return false;
                    }

                    if (tag < 0x307) break;
                    if (tag == 0x309) {
                        u32 sz = size / 8 << 2;
                        if (size / 8 > 16) return false;
                        if (!fp->read(&b_unk3, sz)) return false;
                        if (!fp->read(&b_unk4, sz)) return false;
                    }
                    else if (tag < 0x30a) {
                        if (tag == 0x307) {
                            if (size != 32) return false;
                            if (!fp->read(&b_unk6, 32)) return false;
                        }
                        else if ((tag == 0x308) && !fp->read(&b_unk5, 4)) return false;
                    }
                    else if (tag == 0x30b) {
                        if (!fp->read(&unk1, 1)) return false;
                    }
                    else if (tag < 0x30b) {
                        if (!fp->read(&unk1, 2)) return false;
                    }
                    else if (tag == 0x3ff) {
                        method_0x50();
                        missManager* mgr = missManager::get();
                        return mgr->getHumanCount() + mgr->getAICount() + mgr->getParkedCount() <= n;
                    }
                }
                if (tag != 0x303) break;
                if (!fp->read(&some_str_0, 32)) return false;
                if (!fp->read(&some_str_1, 32)) return false;
            }

            if (tag > 0x303) {
                if (tag != 0x304) {
                    if (tag == 0x305) {
                        checkpoints[checkpoint_count] = new CheckpointInfo();
                        if (!checkpoints[checkpoint_count]->load(tsv, n)) return false;
                        checkpoint_count++;
                        if (checkpoint_count > 32) return false;
                    }

                    continue;
                }

                if (!fp->read(&unk4, 2)) return false;
                continue;
            }
            
            if (tag == 0x301) {
                fp->read(&vehicle_positions[n++], 16);
                if (n > 16) return false;
            } else if (tag == 0x302) {
                checkpoints[checkpoint_count] = new CheckpointInfo();
                if (!checkpoints[checkpoint_count]->load(tsv, n)) return false;
                checkpoint_count++;
                if (checkpoint_count > 32) return false;
            }
        }
    }
    void CheckpointMission::method_0x40() { }
    void CheckpointMission::method_0x50() { }
    void CheckpointMission::debugPrintState() {
        debugPrintBaseState();
        printf("            derived:\n");
        printf("                unk0: 0x%X\n", unk0);
        printf("                unk1: %d\n", unk1);
        printf("                unk2: %d\n", unk2);
        printf("                unk4: %d\n", unk4);
        printf("                some_str_0: '%s'\n", some_str_0);
        printf("                some_str_1: '%s'\n", some_str_1);

        if (checkpoint_count > 0) {
            printf("                unk3:\n");
            for (u32 i = 0;i < checkpoint_count;i++) {
                printf("                    [%d]\n", i);
                printf("                        position: %f, %f, %f\n", checkpoints[i]->position.x, checkpoints[i]->position.y, checkpoints[i]->position.z);
                printf("                        unk1: %f\n", checkpoints[i]->unk1);
                printf("                        unk2: '%s'\n", checkpoints[i]->unk2);
                printf("                        bonus_time: %f\n", checkpoints[i]->bonus_time);
            }
        } else {
            printf("                checkpoints: none\n");
        }
    }
};