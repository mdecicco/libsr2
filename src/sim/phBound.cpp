#include <libsr2/sim/phBound.h>
#include <libsr2/sim/phBoundCollision.h>
#include <libsr2/sim/phBoundSphere.h>
#include <libsr2/sim/phBoundHotdog.h>
#include <libsr2/sim/phBoundBox.h>
#include <libsr2/sim/phBoundGeometry.h>
#include <libsr2/sim/phBoundOTGrid.h>
#include <libsr2/sim/phBoundComposite.h>
#include <libsr2/managers/datAssetManager.h>
#include <libsr2/io/datAsciiTokenizer.h>
#include <libsr2/io/datBinTokenizer.h>
#include <libsr2/io/stream.h>
#include <libsr2/math/vec3f.h>
#include <libsr2/math/mat3x4f.h>

#include <math.h>
#include <string.h>

namespace sr2 {
    phBound::phBound(phBoundType tp) {
        type = tp;
        materialCount = 0;
        centroidIsSet = false;
        centroid.x = 0.0f;
        centroid.z = 0.0f;
        centroid.y = 0.0f;
        centerOfGravityIsSet = false;
        centerOfGravity.x = 0.0f;
        centerOfGravity.z = 0.0f;
        centerOfGravity.y = 0.0f;
        gravityMultiplier = 1.0f;
        penetration = phBoundCollision::Penetration;
        penetrationBarelyMoved = phBoundCollision::PenetrationBarelyMoved;
        aabbRadius = 0.0f;
        longestExtentMag = 0.0f;
    }

    phBound::~phBound() {
    }

    void phBound::debugDraw(const mat3x4f& transform) {
        DebugDraw::box(aabbMin, aabbMax, transform);
        if (centerOfGravityIsSet) DebugDraw::sphere(0.1f, transform * centerOfGravity, vec4f(1.0f, 1.0f, 0.0f, 1.0f));
        if (centroidIsSet) DebugDraw::sphere(0.1f, transform * centroid, vec4f(1.0f, 0.0f, 1.0f, 1.0f));
    }

    void phBound::calculateBoundingBox() {
    }

    void phBound::calculateSphereFromBoundingBox() {
        centroid.x = aabbMin.x + (aabbMax.x - aabbMin.x) * 0.5;
        centroid.y = aabbMin.y + (aabbMax.y - aabbMin.y) * 0.5;
        centroid.z = aabbMin.z + (aabbMax.z - aabbMin.z) * 0.5;

        centroidIsSet = true;
        if (centroid.x == 0.0f && centroid.y == 0.0f && centroid.z == 0.0f) {
            centroidIsSet = false;
        }

        vec3f delta;
        math::sub(delta, centroid, aabbMax);
        aabbRadius = math::magnitude(delta);
        
        vec3f abs_min = {
            fabsf(aabbMin.x),
            fabsf(aabbMin.y),
            fabsf(aabbMin.z)
        };
        vec3f abs_max = {
            fabsf(aabbMax.x),
            fabsf(aabbMax.y),
            fabsf(aabbMax.z)
        };
        vec3f max_extent = {
            abs_max.x > abs_min.x ? abs_max.x : abs_min.x,
            abs_max.y > abs_min.y ? abs_max.y : abs_min.y,
            abs_max.z > abs_min.z ? abs_max.z : abs_min.z
        };

        longestExtentMag = math::magnitude(max_extent);
    }

    void phBound::setPenetration() {
        penetration = phBoundCollision::Penetration;
        penetrationBarelyMoved = phBoundCollision::PenetrationBarelyMoved;
    }
    
    void phBound::centerBound(const vec3f& center) {
        centroid = center;
        centroidIsSet = centroid.x != 0.0f || centroid.y != 0.0f || centroid.z != 0.0f;
    }
    
    void phBound::setCenterOfGravity(const vec3f& center) {
        centerOfGravity = center;
        centerOfGravityIsSet = center.x != 0.0f || center.y != 0.0f || center.z != 0.0f;
    }

    void phBound::setMaterial(phMaterial* material) {
    }
    
    bool phBound::parseText(datAsciiTokenizer& tok) {
        return false;
    }

    bool phBound::parseBinary(datBinTokenizer& tok) {
        return false;
    }

    bool phBound::parse(datAsciiTokenizer& tok, u32 unk) {
        if (unk == 0x6e) {
            parseText(tok);
            return true;
        }

        return false;
    }

    bool phBound::parse(datBinTokenizer& tok, u32 unk) {
        if (unk == 0x6e) {
            parseBinary(tok);
            return true;
        }
        
        return false;
    }

    void phBound::FUN_0028ce20(vec3f& outPosition, const mat3x4f& tform) {
        if (centroidIsSet) {
            math::copy(outPosition, tform.w);
            return;
        }
        
        math::mult(outPosition, tform, centroid);
        math::add(outPosition, tform.w);
    }

    phBound* phBound::parse(datAsciiTokenizer& tok) {
        char version[32];
        tok.matchToken("version:");
        tok.getToken(version, 32);

        if (strcmp(version, "1.01") == 0) return nullptr;
        else if (strcmp(version, "1.1") != 0 && strcmp(version, "1.10") != 0) return nullptr;

        char type[32];
        tok.matchToken("type:");
        tok.getToken(type, 32);

        phBound* bnd = nullptr;
        
        if (stricmp(type, "BOX") == 0) bnd = new phBoundBox();
        else if (stricmp(type, "SPHERE") == 0) bnd = new phBoundSphere(1.0f);
        else if (stricmp(type, "GEOMETRY") == 0) bnd = new phBoundGeometry(BOUND_GEOMETRY);
        else if (stricmp(type, "HOTDOG") == 0) bnd = new phBoundHotdog();
        else if (stricmp(type, "OTGRID") == 0) bnd = new phBoundOTGrid();

        if (!bnd->parse(tok, 0x6e)) {
            delete bnd;
            return nullptr;
        }

        return bnd;
    }

    phBound* phBound::load(const char* filename) {
        Stream* fp = datAssetManager::open("bound", filename, "bbnd", 1, true);
        if (!fp) {
            fp = datAssetManager::open("bound", filename, "bnd", 1, true);
            if (!fp) return nullptr;

            datAsciiTokenizer tok;
            tok.init(filename, fp);

            char version[32];
            tok.matchToken("version:");
            tok.getToken(version, 32);

            if (strcmp(version, "1.01") == 0) {
                fp->close();
                phBoundGeometry* bnd = new phBoundGeometry(BOUND_GEOMETRY);

                if (bnd->load(filename, nullptr)) return bnd;
                delete bnd;

                return nullptr;
            } else if (strcmp(version, "1.1") != 0 && strcmp(version, "1.10") != 0) {
                fp->close();
                return nullptr;
            }

            char type[32];
            tok.matchToken("type:");
            tok.getToken(type, 32);

            phBound* bnd = nullptr;

            if (stricmp(type, "SPHERE") == 0) bnd = new phBoundSphere(1.0f);
            else if (stricmp(type, "HOTDOG") == 0) bnd = new phBoundHotdog();
            else if (stricmp(type, "BOX") == 0) bnd = new phBoundBox();
            else if (stricmp(type, "GEOMETRY") == 0) bnd = new phBoundGeometry(BOUND_GEOMETRY);
            else if (stricmp(type, "OTGRID") == 0) bnd = new phBoundOTGrid();
            else if (stricmp(type, "COMPOSITE") == 0) bnd = new phBoundComposite();

            if (!bnd->parse(tok, 0x6e)) {
                delete bnd;
                return nullptr;
            }

            fp->close();
            return bnd;
        }

        datBinTokenizer tok;
        tok.init(filename, fp);
        i32 ver = tok.readInt32();
        if (ver != 0x6e) {
            fp->close();
            phBoundGeometry* bnd = new phBoundGeometry(BOUND_GEOMETRY);

            if (bnd->loadBinary(filename, nullptr)) return bnd;
            delete bnd;

            return nullptr;
        }

        phBoundType tp = (phBoundType)tok.readInt32();
        phBound* bnd = nullptr;
        switch (tp) {
            case BOUND_HOTDOG: { bnd = new phBoundHotdog(); break; }
            case BOUND_SPHERE: { bnd = new phBoundSphere(1.0f); break; }
            case BOUND_BOX: { bnd = new phBoundBox(); break; }
            case BOUND_GEOMETRY: { bnd = new phBoundGeometry(BOUND_GEOMETRY); break; }
            default: {
                fp->close();
                return nullptr;
            }
        }

        if (!bnd->parse(tok, 0x6e)) {
            delete bnd;
            return nullptr;
        }

        fp->close();
        return bnd;
    }
};