#pragma once
#include <libsr2/types.h>
#include <libsr2/instances/lvlInstance.h>
#include <libsr2/sim/phInst.h>

namespace sr2 {
    class PhysInstance : public UnkInstance0, public phInst {
        public:
            PhysInstance();
            ~PhysInstance();

            virtual f32 phinst_method_0x30();

            f32 unk0, unk2;
            undefined unk1;
    };
};