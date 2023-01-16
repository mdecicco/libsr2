#include <libsr2/instances/lvlInstance.h>

namespace sr2 {
    lvlInstance::lvlInstance() {
        geomIdxPlus1 = 0;
    }
    lvlInstance::~lvlInstance() {
    }
    
    UnkInstance0::UnkInstance0() {
        room_id = 0xffff;
        flags = 0;
        prev = next = nullptr;
    }

    UnkInstance0::~UnkInstance0() {
    }
};