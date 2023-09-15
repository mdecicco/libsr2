#include <libsr2/sim/phBoundHotdog.h>
#include <libsr2/io/datAsciiTokenizer.h>
#include <libsr2/managers/phMaterialMgr.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/math/math.h>

namespace sr2 {
    phBoundHotdog::phBoundHotdog() : phBound(BOUND_HOTDOG) {
    }

    phBoundHotdog::~phBoundHotdog() {
    }

    bool phBoundHotdog::parseText(datAsciiTokenizer& tok) {
        tok.matchToken("length:");
        f32 _length = tok.readFloat();
        tok.matchToken("radius:");
        f32 _radius = tok.readFloat();
        setDimensions(_radius, _length);

        if (tok.checkToken("centroid:", true)) {
            centerBound(tok.readVec3());
        }

        if (tok.checkToken("cg:", true)) tok.readVec3();

        if (tok.checkToken("materials:", true)) {
            u32 materialCount = tok.readInt32();
            if (materialCount == 1) {
                phMaterialMgr* m = phMaterialMgr::get();
                if (!m) m = phMaterialMgr::create(100);

                material = m->addMaterial(tok);
            }
        }

        return true;
    }
    
    void phBoundHotdog::calculateBoundingBox() {
        aabbMax = {
            radius,
            radius,
            length * 0.5f + radius
        };

        aabbMin = aabbMax;
        math::negate(aabbMin);

        math::add(aabbMax, centroid);
        math::add(aabbMin, centroid);

        calculateSphereFromBoundingBox();
        setPenetration();
    }

    void phBoundHotdog::scaleBoundingBox(f32 u0, f32 u1) {
        length = math::max(u1 * length + (radius * radius) * (u1 - u0), 0.0f);
        radius *= u0;
        calculateBoundingBox();
    }

    void phBoundHotdog::setBoundingBox(f32 u0, f32 u1) {
        radius = u0 * 0.5f;
        length = math::max(u1 - u0, 0.0f);
        calculateBoundingBox();
    }

    void phBoundHotdog::setDimensions(f32 _radius, f32 _length) {
        radius = _radius;
        length = _length;
        aabbRadius = length * 0.5f + radius;
        centroid = { 0.0f, 0.0f, 0.0f };
        calculateBoundingBox();
    }
};