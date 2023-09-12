#include <libsr2/creature/crBone.h>
#include <libsr2/math/mat3x4f.h>

namespace sr2 {
    crBone::crBone() {
        math::identity(transform);
        transformPtr = nullptr;
    }

    crBone::~crBone() {

    }
};