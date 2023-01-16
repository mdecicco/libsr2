#include <libsr2/sim/SpatialPartitioner.h>

namespace sr2 {
    SpatialPartitioner* SpatialPartitioner::instance = nullptr;
    SpatialPartitioner* SpatialPartitioner::get() {
        return instance;
    }

    SpatialPartitioner* SpatialPartitioner::create() {
        if (instance) return instance;
        instance = new SpatialPartitioner();
        return instance;
    }

    void SpatialPartitioner::destroy() {
        if (!instance) return;
        delete instance;
        instance = nullptr;
    }

    void SpatialPartitioner::setWorldBounds(f32 cellSize, const vec3f& min, const vec3f& max) {
    }


    SpatialPartitioner::SpatialPartitioner() {
    }

    SpatialPartitioner::~SpatialPartitioner() {
    }
};