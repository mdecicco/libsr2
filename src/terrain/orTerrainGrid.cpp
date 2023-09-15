#include <libsr2/terrain/orTerrainGrid.h>
#include <libsr2/sim/phBound.h>

namespace sr2 {
    f32 orTerrainGrid::LODDist[8] = { 0.0f };
    f32 orTerrainGrid::LODDistSq[8] = { 0.0f };
    f32 orTerrainGrid::LODDistRelated = 0.0f;

    orTerrainGrid::orTerrainGrid() : phBound(BOUND_ORTERRAINGRID) {
        m_did_read_info = false;
        m_did_load_cullflags = false;
        m_did_load_palette = false;
        m_did_load_8199 = false;
    }

    orTerrainGrid::~orTerrainGrid() {
    }

    void orTerrainGrid::calculateBoundingBox() {
        aabbMin = { 0.0f, -10.0f, 0.0f };
        aabbMax = {
            f32(scale * width),
            500.0f,
            f32(scale * height)
        };

        setPenetration();
        calculateSphereFromBoundingBox();
    }

    void orTerrainGrid::squareLODDists() {
        for (u32 i = 0;i < 8;i++) {
            if (LODDist[i] <= 0.0f) LODDistSq[i] = -1.0f;
            else LODDistSq[i] = LODDist[i] * LODDist[i];
        }
    }
};