#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBoundGeometry.h>

namespace sr2 {
    class lvlMaterial;
    class vehBound : public phBoundGeometry {
        public:
            vehBound();
            ~vehBound();

            lvlMaterial* material;
            vec3f* field_0x9c;
    };
};

