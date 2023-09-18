#include <libsr2/managers/srActiveInstanceManager.h>
#include <libsr2/managers/srCreatureManager.h>
#include <libsr2/creature/srActivePool.h>
#include <libsr2/creature/srActive.h>
#include <libsr2/instances/srActiveInstance.h>
#include <libsr2/terrain/lvlLevel.h>

#include <stdlib.h>

namespace sr2 {
    srActiveInstanceMgr* srActiveInstanceMgr::instance = nullptr;

    srActiveInstanceMgr::srActiveInstanceMgr() {
        instanceCount = 0;
        creatureMgr = nullptr;
        instances = nullptr;
    }

    srActiveInstanceMgr::~srActiveInstanceMgr() {
        if (creatureMgr) {
            delete creatureMgr;
            creatureMgr = nullptr;
        }

        if (instances) {
            delete [] instances;
            instances = nullptr;
        }
    }

    srActiveInstanceMgr* srActiveInstanceMgr::get() {
        if (!srActiveInstanceMgr::instance) {
            srActiveInstanceMgr::instance = new srActiveInstanceMgr();
        }

        return srActiveInstanceMgr::instance;
    }

    void srActiveInstanceMgr::destroy() {
        if (!srActiveInstanceMgr::instance) return;

        delete srActiveInstanceMgr::instance;
        srActiveInstanceMgr::instance = nullptr;
    }

    void srActiveInstanceMgr::init(const char* levelName, const char* missionName) {
        creatureMgr = new srCreatureMgr();
        if (!creatureMgr->load(levelName, missionName)) abort();

        instanceCount = creatureMgr->pool->count;
        instances = new srActiveInstance[instanceCount];

        creatureMgr->FUN_001ac6b0();
        creatureMgr->FUN_001ac750();

        for (u32 i = 0;i < instanceCount;i++) {
            instances[i].setActive(&creatureMgr->pool->pool[i]);
            instances[i].FUN_001accc0();

            lvlLevel::get()->moveToRoom(&instances[i], 0);
        }
    }

    void srActiveInstanceMgr::reset() {

    }

    void srActiveInstanceMgr::update() {

    }
};