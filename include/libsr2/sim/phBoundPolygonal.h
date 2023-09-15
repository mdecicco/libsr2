#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBound.h>

namespace sr2 {
    class phPolygon;
    class phBoundPolygonal : public phBound {
        public:
            struct Edge {
                u16 idx_a, idx_b;
            };

            phBoundPolygonal(phBoundType type);
            ~phBoundPolygonal();

            u32 vertexCount;
            u32 polyCount;
            u32 edgeCount;
            i32* unk0;
            i32 unk1, unk2;
            undefined4 unk3, unk4, unk5, unk6;
            vec3f* vertices;
            phPolygon* polygons;
            Edge* edges;
    };
};