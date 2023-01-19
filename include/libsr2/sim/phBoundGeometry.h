#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBoundPolygonal.h>

namespace sr2 {
    class phMaterial;
    class phBoundGeometry : public phBoundPolygonal {
        public:
            phBoundGeometry(BOUND_TYPE type);
            ~phBoundGeometry();

            phMaterial** materials;
            vec3f* edge_normal_related;
            f32* some_normal_dot_products;
    };
};