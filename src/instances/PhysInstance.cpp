#include <libsr2/instances/PhysInstance.h>

namespace sr2 {
    PhysInstance::PhysInstance() {
        unk0 = 1.0f;
        unk1 = 0;
    }
    
    PhysInstance::~PhysInstance() {
    }

    mat3x4f PhysInstance::getTransform() const {
        return transform;
    }

    void PhysInstance::setTransform(const mat3x4f& transform) {
        this->transform = transform;
    }

    f32 PhysInstance::phinst_method_0x30() {
        return 1.0f;
    }
};