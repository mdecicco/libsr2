#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBound.h>

namespace sr2 {
    class phBoundComposite : public phBound {
        public:
            phBoundComposite();
            phBoundComposite(u32 boneCount);
            virtual ~phBoundComposite();

            virtual void debugDraw(const mat3x4f& transform = mat3x4f::identity);
            virtual bool parseText(datAsciiTokenizer& tok);
            virtual void calculateBoundingBox();

            void init(u32 boundCount);
            void setBound(u32 idx, phBound* bound, i32 someIdx);
            void FUN_002afba0();
            void FUN_002afbd8();
            f32 FUN_002aff20(mat3x4f* transforms);

            u32 boundCount;
            phBound** bounds;
            u32 indexCount;
            i32* someIndices;
            mat3x4f* boundTransforms;
            mat3x4f* field6_0x6c;
            mat3x4f* field7_0x70;
    };
};