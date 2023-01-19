#include <libsr2/terrain/orTerrainGrid.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/managers/phMaterialMgr.h>
#include <libsr2/sim/phBound.h>
#include <libsr2/utilities/utils.h>
#include <libsr2/io/datAsciiTokenizer.h>
#include <libsr2/io/stream.h>
#include <libsr2/io/TaggedStream.h>
#include <libsr2/gfx/gfxTexture.h>

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
        aabb_min = { 0.0f, -10.0f, 0.0f };
        aabb_max = {
            scale * width,
            500.0f,
            scale * height
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