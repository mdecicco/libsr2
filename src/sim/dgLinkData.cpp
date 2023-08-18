#include <libsr2/sim/dgLinkData.h>
#include <libsr2/math/quat.h>

namespace sr2 {
    dgLinkData::dgLinkData() {

    }

    dgLinkData::~dgLinkData() {

    }
    
    void dgLinkData::getXAxis(vec3f& out) {
        math::rotate(
            unk0->orient,
            { 1.0f, 0.0f, 0.0f },
            out
        );
    }
};