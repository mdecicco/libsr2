#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBoundPolygonal.h>
#include <libsr2/sim/phPolygon.h>

namespace sr2 {
    class phMaterial;
    class phBoundBox : public phBoundPolygonal {
        public:
            phBoundBox();
            ~phBoundBox();

            void setSize(const vec3f& size);

            vec3f dimensions;
            vec3f verts[8];
            phPolygon sides[6];

            phMaterial* material;
        
        private:
            static vec3f unitCorners[8];
            static vec3f faceNormals[6];
            static vec3f edgeNormals[12];
            static vec3f cornerNormals[8];
            static u16 boxEdges[24];
    };
};