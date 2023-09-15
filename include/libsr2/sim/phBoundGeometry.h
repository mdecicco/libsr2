#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBoundPolygonal.h>

namespace sr2 {
    class phMaterial;
    class phBoundGeometry : public phBoundPolygonal {
        public:
            phBoundGeometry(phBoundType type);
            ~phBoundGeometry();

            bool load(const char* filename, vec3f* u0);
            bool loadBinary(const char* filename, vec3f* u0);
            void scaleSize(f32 x, f32 y, f32 z);

            phMaterial** materials;
            vec3f* edge_normal_related;
            f32* some_normal_dot_products;
    };
};