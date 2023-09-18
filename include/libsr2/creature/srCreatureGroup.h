#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class TaggedStream;
    class srType;
    
    class srCreatureGroup {
        public:
            srCreatureGroup();
            ~srCreatureGroup();

            void reset();
            void traversePath();
            bool load(TaggedStream& ts);
            void initPoints(u32 pointCount);
            
            char name[32];
            undefined4 field1_0x20;
            vec3f currentPosition;
            u32 currentPointIdx;
            f32 pathInterpolationFactor;
            srType* type;
            u32 pointCount;
            vec3f* points;
            i32 unk0;
            f32 spawnRadius;
            f32 unk2;
    };
};