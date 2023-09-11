#include <libsr2/instances/PhysInstance.h>

namespace sr2 {
    PhysInstance::PhysInstance() {
        unk0 = 1.0f;
        unk1 = 0;
    }
    
    PhysInstance::~PhysInstance() {
    }

    f32 PhysInstance::phinst_method_0x30() {
        return 1.0f;
    }
};