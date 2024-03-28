#pragma once
#include <libsr2/types.h>

namespace sr2 {
    class vehCarSimBase;
    class datParser;
    class datParserNode;

    class vehFluid {
        public:
            vehFluid();
            ~vehFluid();

            void init(vehCarSimBase* veh, const vec3f& min, const vec3f& max);
            void reset();
            void update();
            datParserNode* prepParser(datParser* parser);
            u32 FUN_0025fa58();
            void FUN_0025fa68();
            void FUN_0025fa78();
            void FUN_0025fa90(const vec3f& unk);

            u32 flags;
            f32 current;
            vehCarSimBase* vehicle;
            f32 buoyancy;
            f32 damping;
            vec3f grid[4][4][4];

            f32 field1_0x4;
            vec3f field3_0xc;
    };
};