#include <libsr2/sim/phBoundGeometry.h>
#include <libsr2/sim/phPolygon.h>

namespace sr2 {
    phBoundGeometry::phBoundGeometry(phBoundType type) : phBoundPolygonal(type) {
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

    bool phBoundGeometry::load(const char* filename, vec3f* u0) {
        return false;
    }

    bool phBoundGeometry::loadBinary(const char* filename, vec3f* u0) {
        return false;
    }
    
    void phBoundGeometry::scaleSize(f32 x, f32 y, f32 z) {
        for (u32 i = 0;i < vertexCount;i++) {
            if (x > 0.0f) vertices[i].x *= x;
            if (y > 0.0f) vertices[i].y *= y;
            if (z > 0.0f) vertices[i].z *= z;
        }

        for (u32 i = 0;i < polyCount;i++) {
            polygons[i].calculateNormal(vertices);
        }

        calculateBoundingBox();
    }
};