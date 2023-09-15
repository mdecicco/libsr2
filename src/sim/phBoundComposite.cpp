#include <libsr2/sim/phBoundComposite.h>
#include <libsr2/io/datAsciiTokenizer.h>
#include <libsr2/managers/phMaterialMgr.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/math/mat3x4f.h>
#include <libsr2/math/math.h>

#include <math.h>

namespace sr2 {
    phBoundComposite::phBoundComposite() : phBound(BOUND_COMPOSITE) {
    }
    
    phBoundComposite::phBoundComposite(u32 boneCount) : phBound(BOUND_COMPOSITE) {
    }

    phBoundComposite::~phBoundComposite() {
    }

    bool phBoundComposite::parseText(datAsciiTokenizer& tok) {
        phMaterial* mtrl = nullptr;

        tok.matchToken("numBounds:");
        init(tok.readInt32());

        if (tok.checkToken("centroid:", true)) centerBound(tok.readVec3());
        if (tok.checkToken("cg:", true)) setCenterOfGravity(tok.readVec3());
        if (tok.checkToken("materials:", true)) {
            i32 materialCount = tok.readInt32();
            if (materialCount == 1) {
                phMaterialMgr* m = phMaterialMgr::get();
                if (!m) m = phMaterialMgr::create(100);

                mtrl = m->addMaterial(tok);
            }
        }

        for (u32 i = 0;i < boundCount;i++) {
            tok.matchToken("bound:");
            i32 idx = tok.readInt32();
            phBound* bnd = phBound::parse(tok);
            setBound(idx, bnd, -1);
            if (mtrl) bnd->setMaterial(mtrl);
        }

        calculateBoundingBox();
        return true;
    }

    void phBoundComposite::calculateBoundingBox() {
        FUN_002afba0();
    }

    void phBoundComposite::init(u32 _boundCount) {
        boundCount = _boundCount;
        indexCount = _boundCount;
        bounds = new phBound*[boundCount];
        someIndices = new i32[boundCount];
        boundTransforms = new mat3x4f[boundCount];
        field6_0x6c = boundTransforms;
        field7_0x70 = boundTransforms;

        for (u32 i = 0;i < boundCount;i++) {
            bounds[i] = nullptr;
            someIndices[i] = -1;
            math::identity(boundTransforms[i]);
        }
    }
    
    void phBoundComposite::setBound(u32 idx, phBound* bound, i32 someIdx) {
        bounds[idx] = bound;
        someIndices[idx] = someIdx;
    }

    void phBoundComposite::FUN_002afba0() {
        FUN_002afbd8();
        aabbRadius = FUN_002aff20(field6_0x6c);
    }

    void phBoundComposite::FUN_002afbd8() {
        aabbMin = { FLT_MAX, FLT_MAX, FLT_MAX };
        aabbMax = { FLT_MIN, FLT_MIN, FLT_MIN };

        for (u32 i = 0;i < boundCount;i++) {
            phBound* bnd = bounds[i];
            if (!bnd) continue;

            mat3x4f& tform = field6_0x6c[i];

            f32 t0 = (
                math::min(tform.x.x * bnd->aabbMin.x, tform.x.x * bnd->aabbMax.x) +
                math::min(tform.y.x * bnd->aabbMin.y, tform.y.x * bnd->aabbMax.y) +
                math::min(tform.z.x * bnd->aabbMin.z, tform.z.x * bnd->aabbMax.z) +
                tform.w.x
            );
            f32 t1 = (
                math::min(tform.x.y * bnd->aabbMin.x, tform.x.y * bnd->aabbMax.x) +
                math::min(tform.y.y * bnd->aabbMin.y, tform.y.y * bnd->aabbMax.y) +
                math::min(tform.z.y * bnd->aabbMin.z, tform.z.y * bnd->aabbMax.z) +
                tform.w.y
            );
            f32 t2 = (
                math::min(tform.x.z * bnd->aabbMin.x, tform.x.z * bnd->aabbMax.x) +
                math::min(tform.y.z * bnd->aabbMin.y, tform.y.z * bnd->aabbMax.y) +
                math::min(tform.z.z * bnd->aabbMin.z, tform.z.z * bnd->aabbMax.z) +
                tform.w.z
            );
            aabbMin.x = math::min(aabbMin.x, t0);
            aabbMin.y = math::min(aabbMin.y, t1);
            aabbMin.z = math::min(aabbMin.z, t2);

            f32 t3 = (
                math::max(tform.x.x * bnd->aabbMin.x, tform.x.x * bnd->aabbMax.x) +
                math::max(tform.y.x * bnd->aabbMin.y, tform.y.x * bnd->aabbMax.y) +
                math::max(tform.z.x * bnd->aabbMin.z, tform.z.x * bnd->aabbMax.z) +
                tform.w.x
            );
            f32 t4 = (
                math::max(tform.x.y * bnd->aabbMin.x, tform.x.y * bnd->aabbMax.x) +
                math::max(tform.y.y * bnd->aabbMin.y, tform.y.y * bnd->aabbMax.y) +
                math::max(tform.z.y * bnd->aabbMin.z, tform.z.y * bnd->aabbMax.z) +
                tform.w.y
            );
            f32 t5 = (
                math::max(tform.x.z * bnd->aabbMin.x, tform.x.z * bnd->aabbMax.x) +
                math::max(tform.y.z * bnd->aabbMin.y, tform.y.z * bnd->aabbMax.y) +
                math::max(tform.z.z * bnd->aabbMin.z, tform.z.z * bnd->aabbMax.z) +
                tform.w.z
            );
            aabbMax.x = math::max(aabbMax.x, t3);
            aabbMax.y = math::max(aabbMax.y, t4);
            aabbMax.z = math::max(aabbMax.z, t5);
        }

        f32 minMagSq = math::magnitudeSq(aabbMin);
        f32 maxMagSq = math::magnitudeSq(aabbMax);
        longestExtentMag = sqrtf(math::max(minMagSq, maxMagSq));
        setPenetration();
    }

    f32 phBoundComposite::FUN_002aff20(mat3x4f* transforms) {
        f32 out = 0.0f;

        for (u32 i = 0;i < boundCount;i++) {
            vec3f c;
            bounds[i]->FUN_0028ce20(c, transforms[i]);
            out = math::max(math::magnitude(c) + bounds[i]->aabbRadius, out);
        }

        return out;
    }
};