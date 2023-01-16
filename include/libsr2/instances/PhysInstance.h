#pragma once
#include <libsr2/types.h>
#include <libsr2/instances/lvlInstance.h>
#include <libsr2/sim/phInst.h>

namespace sr2 {
    class phInst;
    class PhysInstance : public UnkInstance0 {
        public:
            PhysInstance();
            ~PhysInstance();

            f32 unk0, unk2;
            undefined unk1;
            phInst phys;
    };
};