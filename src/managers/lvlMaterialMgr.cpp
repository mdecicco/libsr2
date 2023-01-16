#include <libsr2/managers/lvlMaterialMgr.h>

namespace sr2 {
    lvlMaterialMgr* lvlMaterialMgr::create(u32 materialCapacity) {
        if (instance) return (lvlMaterialMgr*)instance;
        instance = new lvlMaterialMgr(materialCapacity);
        return (lvlMaterialMgr*)instance;
    }
    lvlMaterialMgr* lvlMaterialMgr::get() {
        return (lvlMaterialMgr*)instance;
    }

    lvlMaterialMgr::lvlMaterialMgr(u32 materialCapacity) : phMaterialMgr(materialCapacity) {

    }

    lvlMaterialMgr::~lvlMaterialMgr() {

    }
};