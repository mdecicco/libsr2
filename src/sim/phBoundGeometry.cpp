#include <libsr2/sim/phBoundGeometry.h>

namespace sr2 {
    phBoundGeometry::phBoundGeometry(BOUND_TYPE type) : phBoundPolygonal(type) {
        materials = nullptr;
        vertexCount = 0;
        materialCount = 0;
        edgeCount = 0;
        polyCount = 0;
        edge_normal_related = nullptr;
        some_normal_dot_products = nullptr;
    }

    phBoundGeometry::~phBoundGeometry() {
        if (polygons != nullptr) delete [] polygons;
        if (vertices != nullptr) delete [] vertices;
        if (edges != nullptr) delete [] edges;
        if (materials == nullptr) delete [] materials;
        if (edge_normal_related == nullptr) delete [] edge_normal_related;
        if (some_normal_dot_products == nullptr) delete [] some_normal_dot_products;
    }
};