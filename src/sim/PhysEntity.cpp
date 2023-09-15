#include <libsr2/sim/PhysEntity.h>

namespace sr2 {
    PhysEntity::PhysEntity() {
        bound = nullptr;
        unk1 = nullptr;
        unk2 = { 1.0f, 1.0f, 1.0f };
        unk0 = 1.0f;
        mass = 1.0f;
        invMass = 1.0f;
        angInertia = { 1.0f, 1.0f, 1.0f };
        invAngInertia = { 1.0f, 1.0f, 1.0f };
        flags = 0x10;
        otherFlags = 0x1f;
    }

    PhysEntity::~PhysEntity() {
    }

    void PhysEntity::setFlag(u32 flag, bool state) {
        if (state) {
            flags |= flag;
            return;
        }

        flags &= ~flag;
    }

    void PhysEntity::setOtherFlag(u32 flag, bool state) {
        if (state) {
            otherFlags |= flag;
            return;
        }

        otherFlags &= ~flag;
    }
};