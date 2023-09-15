#include <libsr2/vehicle/vehBound.h>
#include <libsr2/sim/lvlMaterial.h>

namespace sr2 {
    vehBound::vehBound() : phBoundGeometry(BOUND_GEOMETRY) {
        material = new lvlMaterial();
        penetration = 0.0f;
        clonedVertices = nullptr;
    }

    vehBound::~vehBound() {
        delete material;
        material = nullptr;

        if (clonedVertices) delete [] clonedVertices;
        clonedVertices = nullptr;
    }

    void vehBound::init() {
        for (u32 i = 0;i < materialCount;i++) materials[i] = material;

        clonedVertices = new vec3f[vertexCount];
        for (u32 i = 0;i < vertexCount;i++) {
            clonedVertices[i] = vertices[i];
        }

        clonedAabbMin = aabbMin;
        clonedAabbMax = aabbMax;
        clonedCentroid = centroid;
    }

    void vehBound::reset() {
        if (clonedVertices) {
            for (u32 i = 0;i < vertexCount;i++) {
                clonedVertices[i] = vertices[i];
            }
        }

        clonedAabbMin = aabbMin;
        clonedAabbMax = aabbMax;
        clonedCentroid = centroid;
    }

    void vehBound::setFriction(f32 val) {
        material->friction = val;
    }

    void vehBound::setElasticity(f32 val) {
        material->elasticity = val;
    }
};