#pragma once
#include <libsr2/sim/phBoundGeometry.h>

namespace sr2 {
    class phMaterial;

    class vehBound : public phBoundGeometry {
        public:
            vehBound();
            virtual ~vehBound();

            void init();
            void reset();
            void setFriction(f32 val);
            void setElasticity(f32 val);

            phMaterial* material;
            vec3f* clonedVertices;
            vec3f clonedAabbMin;
            vec3f clonedAabbMax;
            vec3f clonedCentroid;
    };
};