#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class phInst;
    class phColliderBase;

    struct phIntersectionPoint {
        phInst* instance;
        u32 some_cell_idx;
        vec3f contact_point;

        vec3f field3_0x14;
        f32 field4_0x20;
        phColliderBase* field9_0x2c;
        i32 field10_0x30;
        
        // more to come...
    };
};