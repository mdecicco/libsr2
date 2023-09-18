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

    void phBoundHotdog::debugDraw(const mat3x4f& transform) {
        mat3x4f t = transform;
        math::negate(t.y);
        DebugDraw::capsule(radius, length * 0.5f, Axis::Y_AXIS, t);
        if (centerOfGravityIsSet) DebugDraw::sphere(0.01f, t * centerOfGravity, vec4f(1.0f, 1.0f, 0.0f, 1.0f));
        if (centroidIsSet) DebugDraw::sphere(0.01f, t * centroid, vec4f(1.0f, 0.0f, 1.0f, 1.0f));
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