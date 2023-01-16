#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class SpatialPartitioner {
        public:
            static SpatialPartitioner* get();
            static SpatialPartitioner* create();
            static void destroy();

            void setWorldBounds(f32 cellSize, const vec3f& min, const vec3f& max);

        protected:
            SpatialPartitioner();
            ~SpatialPartitioner();
            static SpatialPartitioner* instance;

            vec3f m_world_min;
            vec3f m_world_max;
    };
};