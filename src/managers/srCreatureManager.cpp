#include <libsr2/managers/srCreatureManager.h>
#include <libsr2/managers/RagdollManager.h>
#include <libsr2/creature/srCreatureGroup.h>
#include <libsr2/creature/srActivePool.h>
#include <libsr2/creature/srActive.h>
#include <libsr2/creature/srType.h>
#include <libsr2/creature/crSkeleton.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/vehicle/vehicle.h>
#include <libsr2/sim/phInst.h>
#include <libsr2/io/TaggedStream.h>
#include <libsr2/io/Stream.h>

#include <math.h>
#include <string.h>

namespace sr2 {
    srCreatureMgr::srCreatureMgr() {
        groupCount = 0;
        groups = nullptr;
        pool = nullptr;
    }

    srCreatureMgr::~srCreatureMgr() {
        if (groups) {
            delete [] groups;
            groups = nullptr;
        }

        if (pool) {
            delete pool;
            pool = nullptr;
        }

        for (u32 i = 0;i < 10;i++) {
            if (types[i].type) {
                delete types[i].type;
                types[i].type = nullptr;
                types[i].name[0] = 0;
            }
        }
    }

    void srCreatureMgr::reset() {
        pool->reset();
        for (u32 i = 0;i < groupCount;i++) {
            if (groups[i].field1_0x20) groups[i].reset();
        }
    }

    void srCreatureMgr::update() {
        for (u32 i = 0;i < groupCount;i++) {
            u32 a = 0;
            u32 inRange = 0;
            u32 outOfRange = 0;

            for (u32 j = 0;j < Vehicle::GetCount();j++) {
                Vehicle* veh = Vehicle::GetVehicle(j);

                if (veh->type == PLAYER_TYPE::Human) {
                    phInst* inst = nullptr; // veh->instance->base.base.car.sim->veh.sim.col.instance (lol)
                    vec3f dp = groups[i].currentPosition - inst->transform.w;
                    f32 distXZ = sqrtf(dp.x * dp.x + dp.z * dp.z);
                    if (distXZ <= 175.0f) {
                        inRange++;

                        if (groups[i].field1_0x20) {
                            a = 1;
                            break;
                        }
                    } else if (distXZ > 175.0f && !groups[i].field1_0x20) {
                        outOfRange++;
                    }
                }
            }

            if (outOfRange > 0 && inRange == 0) a = 2;

            if (a == 1) {
                pool->addGroup(&groups[i]);
                groups[i].field1_0x20 = 1;
            } else if (a == 2) {
                pool->removeGroup(&groups[i]);
                groups[i].field1_0x20 = 0;
            }

            if (groups[i].field1_0x20) {
                groups[i].traversePath();
            }
        }

        pool->update();
    }

    srType* srCreatureMgr::getType(srCreatureGroup* group) {
        for (u32 i = 0;i < 10;i++) {
            if (strcmp(types[i].name, group->name) == 0) {
                return types[i].type;
            }
        }

        for (u32 i = 0;i < 10;i++) {
            if (types[i].name[0] != 0) continue;
            types[i].type = new srType();
            strcpy(types[i].name, group->name);
            types[i].type->load(group);
            types[i].type->initData();
            return types[i].type;
        }

        return nullptr;
    }

    void srCreatureMgr::FUN_001ac6b0() {
        for (u32 i = 0;i < 10;i++) {
            if (!types[i].type) continue;

            crSkeleton skel(100);
            mat3x4f m = mat3x4f::identity;
            skel.init(types[i].type->skel, &m);
            types[i].type->FUN_001ad528(types[i].name);
        }
    }

    void srCreatureMgr::FUN_001ac750() {
        for (u32 i = 0;i < 10;i++) {
            if (!types[i].type) continue;
            RagdollManager::get()->load(types[i].type, types[i].name);
        }

        RagdollManager::get()->initRagdolls(24);
    }

    bool srCreatureMgr::load(const char* levelName, const char* missionName) {
        pool = new srActivePool();
        groupCount = 0;

        for (u32 i = 0;i < 10;i++) types[i].type = nullptr;

        char missionCrgFilename[128] = { 0 };
        snprintf(missionCrgFilename, 128, "%s_%s", levelName, missionName);

        TaggedStream lvlTS, msnTS;
        lvlTS.closeOnError(false);
        msnTS.closeOnError(false);

        Stream* base = lvlTS.open("anim/level", levelName, "crg");
        if (!base) return false;

        u16 tag;
        u32 size;
        while (lvlTS.readTag(&tag, &size)) {
            if (tag != 0x870) continue;
            u32 gc;
            if (!base->read(&gc, 4)) {
                lvlTS.close();
                delete pool;
                pool = nullptr;
                return false;
            }

            groupCount += gc;
        }

        Stream* msn = msnTS.open("anim/mission", missionCrgFilename, "crg");
        if (msn) {
            while (msnTS.readTag(&tag, &size)) {
                if (tag != 0x870) continue;
                u32 gc;
                if (!msn->read(&gc, 4)) {
                    lvlTS.close();
                    msnTS.close();
                    delete pool;
                    pool = nullptr;
                    return false;
                }

                groupCount += gc;
            }
        }

        if (groupCount == 0) {
            lvlTS.close();
            if (msn) msnTS.close();
            return true;
        }

        groups = new srCreatureGroup[groupCount];
        lvlTS.restart();
        msnTS.restart();

        u32 gidx = 0;
        while (lvlTS.readTag(&tag, &size)) {
            if (tag != 0x871) continue;
            bool loaded = groups[gidx].load(lvlTS);
            srType* type = loaded ? getType(&groups[gidx]) : nullptr;

            if (!loaded || !type) {
                lvlTS.close();
                if (msn) msnTS.close();
                return false;
            }

            groups[gidx].type = type;
            gidx++;
        }
        lvlTS.close();

        if (msn) {
            while (msnTS.readTag(&tag, &size)) {
                if (tag != 0x871) continue;
                bool loaded = groups[gidx].load(msnTS);
                srType* type = loaded ? getType(&groups[gidx]) : nullptr;

                if (!loaded || !type) {
                    lvlTS.close();
                    msnTS.close();
                    return false;
                }

                groups[gidx].type = type;
                gidx++;
            }
            msnTS.close();
        }

        return true;
    }
};