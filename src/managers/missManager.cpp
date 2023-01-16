#include <libsr2/managers/missManager.h>

#include <libsr2/mission/Mission.h>
#include <libsr2/mission/Contraband.h>
#include <libsr2/mission/ContrabandMission.h>
#include <libsr2/mission/FollowMission.h>
#include <libsr2/mission/CinematicMission.h>
#include <libsr2/mission/VehicleAssaultMission.h>
#include <libsr2/mission/UnorderedMission.h>
#include <libsr2/mission/JoyrideMission.h>
#include <libsr2/mission/CheckpointMission.h>

#include <libsr2/io/TaggedStream.h>
#include <libsr2/io/stream.h>
#include <libsr2/io/parCsvFile.h>

#include <libsr2/instances/Allocator.hpp>
#include <libsr2/instances/unknown.h>

#include <libsr2/utilities/utils.h>

namespace sr2 {
    missManager* missManager::Instance = nullptr;

    void missManager::create() {
        if (missManager::Instance) return;
        missManager::Instance = new missManager();
    }

    void missManager::destroy() {
        if (!missManager::Instance) return;
        delete missManager::Instance;
        missManager::Instance = nullptr;
    }

    missManager* missManager::get() {
        return missManager::Instance;
    }

    missManager::missManager() {
        m_scoring.is_loaded_I_guess = 0;
        m_aiCount = m_humanCount = m_parkedCount = 0;
        m_missionCount = 0;
        m_contrabandCount = 0;
        m_contraband = nullptr;
        m_unk0 = nullptr;

        for (u32 i = 0;i < 32;i++) m_missions[i] = nullptr;
    }

    missManager::~missManager() {
        for (u32 i = 0;i < 32;i++) {
            if (m_missions[i]) delete m_missions[i];
            m_missions[i] = nullptr;
        }
    }
    
    missPlayer* missManager::getPlayer(u32 idx) {
        return &m_players[idx & 0xff];
    }

    u32 missManager::getHumanCount() const {
        return m_humanCount;
    }

    u32 missManager::getAICount() const {
        return m_aiCount;
    }

    u32 missManager::getParkedCount() const {
        return m_parkedCount;
    }

    bool missManager::load(const std::string& map_name, const std::string& mission) {
        std::string filename = format("%s_%s", map_name.c_str(), mission.c_str());
        TaggedStream tsv;
        Stream* fp = tsv.open("mission", const_cast<char*>(filename.c_str()), "miss");
        if (!fp) {
            return false;
        }

        i32 iVar4;
        while(true) {
            u16 tag = 0;
            u32 size = 0;

            if (!tsv.readTag(&tag, &size)) {
                tsv.close();
                // FUN_001971e0(this);
                return true;
            }

            switch ((i32)((tag - 1) * 0x10000) >> 0x10) {
                case 0: {
                    if (size != 4) return false;
                    if (!fp->read(&m_field21_0x130, 4)) return false;
                    break;
                }
                case 1: {
                    if (size != 2) return false;
                    if (!loadMissions(&tsv)) return false;
                    break;
                }
                case 6: {
                    if (!loadContraband(&tsv)) return false;
                    break;
                }
                case 8: {
                    missPlayer* pinfo = getPlayer(m_aiCount++);
                    if (!pinfo->read(&tsv, 3)) return false;
                    break;
                }
                case 9: {
                    missPlayer* pinfo = getPlayer(m_aiCount++);
                    if (!pinfo->read(&tsv, 4)) return false;
                    break;
                }
                case 10: {
                    if (size != 32) return false;
                    if (!fp->read(name, 32)) return false;
                    break;
                }
                default: continue;
            }
        }

        return true;
    }

    bool missManager::loadScoring() {
        parCsvFile csv;
        if (!csv.load("mission", "scoring", 1, 1)) return false;

        for (u32 row = 0;row < csv.rowCount();row++) {
            for (u32 col = 0;col < 4;col++) {
                m_scoring.values[row][col] = csv.getInt(row, col);
            }
        }

        m_scoring.is_loaded_I_guess = 1;

        return true;
    }

    void missManager::debugPrintState(bool summarize) {
        const char* vehicleStrs[] = {
            "vpsupbug",
            "vphondo",
            "vpatv",
            "vprally2",
            "vptrophy",
            "vpsuv2",
            "vpmil2",
            "vphalftrack",
            "vpmass2",
            "vpsnowcycle"
        };

        const char* mdtstrs[] = {
            "JoyrideMission",
            "ContrabandMission",
            "CheckpointMission",
            "UnorderedMission",
            "FollowMission",
            "VehicleAssaultMission",
            "CinematicMission",
            "invalid type"
        };

        if (summarize) {
            printf("    opponents: ");
            if (m_aiCount == 0) printf("none\n");
            else {
                for (u32 i = 0;i < m_aiCount;i++) {
                    if (i != 0) printf(", ");
                    printf("%s", vehicleStrs[m_players[i].vehicle_tp]);
                }
                printf("\n");
            }

            printf("    mission types: ");
            if (m_missionCount == 0) printf("none\n");
            else {
                for (u32 i = 0;i < m_missionCount;i++) {
                    if (i != 0) printf(", ");
                    printf("%d", m_missionTypes[i]);
                }
                printf("\n");
            }
            return;
        }

        
        printf("    m_field21_0x130: 0x%X\n", m_field21_0x130);
        printf("    name: '%s'\n", name);
        printf("    human count: %d\n", m_humanCount);
        printf("    ai count: %d\n", m_aiCount);
        printf("    opponents:\n");
        for (u32 i = 0;i < m_aiCount;i++) {
            printf("        opponent %d:\n", i);

            missPlayer* o = getPlayer(i);
            printf("            vehicle: %s\n", vehicleStrs[o->vehicle_tp]);
            printf("            team_idx: %d\n", o->team_idx);
            printf("            geom_idx: %d\n", o->geom_idx);
            printf("            agent_idx: %d\n", o->agent_idx);
            printf("            unk3: %d\n", o->unk3);
            printf("            is_human: 0x%X\n", o->is_human);
        }

        printf("    mission data count: %d\n", m_missionCount);
        for (u32 i = 0;i < m_missionCount;i++) {
            u32 tp = m_missionTypes[i];
            printf("        mission data %d:\n", i);
            printf("            type: %d (%s)\n", tp, mdtstrs[tp < 7 ? tp : 7]);
            if (m_missions[i]) m_missions[i]->debugPrintState();
            else printf("            (data not available)\n");
        }

        if (m_contrabandCount > 0) {
            printf("    contraband:\n");
            for (u32 i = 0;i < m_contrabandCount;i++) {
                ContrabandInfo& c = m_contraband[i];
                printf("        [%d]:\n", i);
                printf("            name: '%s'\n", c.name);
                printf("            geom_file_0: '%s'\n", c.geom_file_0);
                printf("            geom_file_1: '%s'\n", c.geom_file_1);
                printf("            unk2: '%s'\n", c.unk2);
                printf("            is_bomb: %s\n", c.is_bomb ? "yes" : "no");
                printf("            unk4: 0x%X\n", c.unk4);
                printf("            unk5: 0x%X\n", c.unk5);
                printf("            unk6: %d\n", c.time_to_detonate);
                printf("            unk7: %d\n", c.unk7);
                printf("            cash_value: %d\n", c.cash_value);
                printf("            unk0:\n");
                printf("                room_id: %d\n", c.unk0.room_id);
                printf("                unk0: %f\n", c.unk0.unk0);
                printf("                unk1: %d\n", c.unk0.unk1);
                printf("                unk2: %f\n", c.unk0.unk2);
                printf("                geomIdx: %d\n", c.unk0.geomIdxPlus1);
                printf("                flags: 0x%X\n", c.unk0.flags);
                printf("                unk_mat:\n");
                printf("                    x: %f, %f, %f\n", c.unk0.unk_mat.x.x, c.unk0.unk_mat.x.y, c.unk0.unk_mat.x.z);
                printf("                    y: %f, %f, %f\n", c.unk0.unk_mat.y.x, c.unk0.unk_mat.y.y, c.unk0.unk_mat.y.z);
                printf("                    z: %f, %f, %f\n", c.unk0.unk_mat.z.x, c.unk0.unk_mat.z.y, c.unk0.unk_mat.z.z);
                printf("                    w: %f, %f, %f\n", c.unk0.unk_mat.w.x, c.unk0.unk_mat.w.y, c.unk0.unk_mat.w.z);
                printf("            unk1:\n");
                printf("                room_id: %d\n", c.unk1.room_id);
                printf("                unk0: %f\n", c.unk1.unk0);
                printf("                unk1: %d\n", c.unk1.unk1);
                printf("                unk2: %f\n", c.unk1.unk2);
                printf("                geomIdx: %d\n", c.unk1.geomIdxPlus1);
                printf("                flags: 0x%X\n", c.unk1.flags);
                printf("                unk_mat:\n");
                printf("                    x: %f, %f, %f\n", c.unk1.unk_mat.x.x, c.unk1.unk_mat.x.y, c.unk1.unk_mat.x.z);
                printf("                    y: %f, %f, %f\n", c.unk1.unk_mat.y.x, c.unk1.unk_mat.y.y, c.unk1.unk_mat.y.z);
                printf("                    z: %f, %f, %f\n", c.unk1.unk_mat.z.x, c.unk1.unk_mat.z.y, c.unk1.unk_mat.z.z);
                printf("                    w: %f, %f, %f\n", c.unk1.unk_mat.w.x, c.unk1.unk_mat.w.y, c.unk1.unk_mat.w.z);
            }
        } else {
            printf("    contraband: none\n");
        }
    }

    bool missManager::loadMissions(TaggedStream* tsv) {
        Stream* fp = tsv->getStream();
        if (!fp->read(&m_missionCount, 2)) return false;
        if (m_missionCount == 0) return true;

        for (u32 i = 0;i < m_missionCount;i++) {
            u16 tag;
            u32 size;
            if (!tsv->readTag(&tag, &size)) return false;

            if (tag == 0x500) {
                m_missions[i] = new FollowMission();
                m_missionTypes[i] = mission_unk4;
            } else if (tag < 0x501) {
                if (tag == 0x300) {
                    m_missions[i] = new CheckpointMission();
                    m_missionTypes[i] = mission_unk2;
                } else if (tag < 0x301) {
                    if (tag != 0x100) return false;
                    m_missions[i] = new JoyrideMission();
                    m_missionTypes[i] = mission_unk0;
                } else {
                    if (tag != 0x400) return false;
                    m_missions[i] = new ContrabandMission();
                    m_missionTypes[i] = mission_contraband;
                }
            } else if (tag == 0x700) {
                m_missions[i] = new VehicleAssaultMission();
                m_missionTypes[i] = mission_unk5;
            } else if (tag < 0x701) {
                if (tag != 0x600) return false;
                m_missions[i] = new UnorderedMission();
                m_missionTypes[i] = mission_unk3;
            } else {
                if (tag != 0x800) {
                    // TODO: UNCOMMENT THIS AFTER IMPLEMENTING OTHER MISSIONS
                    //       THIS IS AN ERROR CASE
                    // return false;
                    return true;
                }
                m_missions[i] = new CinematicMission();
                m_missionTypes[i] = mission_unk6;
            }

            if (!m_missions[i]->load(tsv)) return false;
        }

        return true;
    }

    bool missManager::loadContraband(TaggedStream* tsv) {
        Stream* fp = tsv->getStream();

        if (!fp->read(&m_contrabandCount, 1)) return false;
        
        if (m_contrabandCount > 0) {
            m_contraband = new ContrabandInfo[m_contrabandCount];
            for (u32 i = 0;i < m_contrabandCount;i++) {
                if (!m_contraband[i].load(tsv)) return false;
                if (m_unk0 || !m_contraband[i].is_bomb) continue;
                m_unk0 = InstanceAllocator::Alloc<Cls_00395078>();
                m_unk0->FUN_001a6890(2);
            }
        }

        m_unk1 = InstanceAllocator::Alloc<Cls_00395078>();
        m_unk1->FUN_001a6890(0);        
        return true;
    }
};