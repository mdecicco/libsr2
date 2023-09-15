#pragma once
#include <libsr2/types.h>

namespace sr2 {
    enum phBoundType : u32 {
        BOUND_SPHERE = 0,
        BOUND_HOTDOG,
        BOUND_BOX,
        BOUND_GEOMETRY,
        BOUND_TERRAIN,
        BOUND_UNK0,
        BOUND_UNK1,
        BOUND_UNK2,
        BOUND_UNK3,
        BOUND_OTGRID,
        BOUND_ORTERRAINGRID,
        BOUND_FORCE_SPHERE,
        BOUND_UNK4,
        BOUND_COMPOSITE,
        INVALID_BOUND = 0xffffffff
    };
};