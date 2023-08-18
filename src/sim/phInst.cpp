#include <libsr2/sim/phInst.h>

namespace sr2 {
    phInst::phInst() {
    }

    phInst::~phInst() {
    }
    
    mat3x4f* phInst::method_0x10() {
        return &transform;
    }

    void phInst::getLocalVelocity(vec3f& p1, vec3f& p2) {
        p2 = { 0.0f, 0.0f, 0.0f };
    }

    i32 phInst::method_0x20() {
        return 0;
    }

    i32 phInst::method_0x28() {
        return 0;
    }
};