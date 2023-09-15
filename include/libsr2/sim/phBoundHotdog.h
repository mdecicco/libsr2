#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phBound.h>

namespace sr2 {
    class phMaterial;
    class phBoundHotdog : public phBound {
        public:
            phBoundHotdog();
            virtual ~phBoundHotdog();

            virtual bool parseText(datAsciiTokenizer& tok);
            virtual void calculateBoundingBox();

            void scaleBoundingBox(f32 u0, f32 u1);
            void setBoundingBox(f32 u0, f32 u1);
            void setDimensions(f32 radius, f32 length);

            f32 length;
            f32 radius;
            phMaterial* material;
    };
};