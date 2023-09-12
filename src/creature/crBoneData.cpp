#include <libsr2/creature/crBoneData.h>
#include <libsr2/creature/crBone.h>
#include <libsr2/math/mat3x4f.h>

namespace sr2 {
    crBoneData::crBoneData() {
    }

    crBoneData::~crBoneData() {
    }

    void crBoneData::transform(mat3x4f* transform, crBone* bones) {
        if (!transform) *bones[index].transformPtr = bones[index].transform;
        else math::mult(*bones[index].transformPtr, bones[index].transform, *transform);
        
        if (children) children->transform(bones[index].transformPtr, bones);
        if (next) next->transform(transform, bones);
    }
};