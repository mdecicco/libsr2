#pragma once
#include <libsr2/types.h>
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class ambientData : public parFileIO {
        public:
            ambientData();
            virtual ~ambientData();

            vec3f size;
            vec3f wheelVelocities[4];
            vec3f ptxOffset;
            f32 ptxRate;
            f32 mass;
            f32 friction;
            f32 elasticity;
            f32 tireDisplacementLimit;
            f32 tireDampingCoefficient;
            f32 suspensionLimit;
            f32 suspensionDampingCoefficient;
            f32 maxDamage;
            f32 ptxThreshold;
            f32 contraThreshold;
            f32 impulseThreshold;

            vec3f field_0x18;
            f32  field_0x70;
            undefined2 field_0x94;
            undefined2 field_0x96;
    };
};