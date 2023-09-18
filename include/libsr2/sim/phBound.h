#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBoundType.h>
#include <libsr2/utilities/DebugDrawer.h>

namespace sr2 {
    class datAsciiTokenizer;
    class datBinTokenizer;
    class phMaterial;

    class phBound : public IDebugDrawable {
        public:
            phBound(phBoundType param_1);
            virtual ~phBound();

            virtual void debugDraw(const mat3x4f& transform = mat3x4f::identity);
            virtual void calculateBoundingBox();
            void calculateSphereFromBoundingBox();
            void setPenetration();

            virtual void centerBound(const vec3f& center);
            virtual void setCenterOfGravity(const vec3f& center);
            virtual void setMaterial(phMaterial* material);
            virtual bool parseText(datAsciiTokenizer& tok);
            virtual bool parseBinary(datBinTokenizer& tok);
            bool parse(datAsciiTokenizer& tok, u32 unk);
            bool parse(datBinTokenizer& tok, u32 unk);
            void FUN_0028ce20(vec3f& outPosition, const mat3x4f& tform);

            static phBound* load(const char* filename);
            static phBound* parse(datAsciiTokenizer& tok);

            phBoundType type;
            u32 materialCount;
            vec3f aabbMin;
            vec3f aabbMax;
            bool centroidIsSet;
            vec3f centroid;
            bool centerOfGravityIsSet;
            vec3f centerOfGravity;
            f32 aabbRadius;
            f32 longestExtentMag;
            f32 penetration;
            f32 penetrationBarelyMoved;
            f32 gravityMultiplier;
    };
};