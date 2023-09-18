#pragma once
#include <libsr2/types.h>
#include <libsr2/instances/unkInstance0.h>

namespace sr2 {
    class srActive;

    class srActiveInstance : public unkInstance0 {
        public:
            srActiveInstance();
            ~srActiveInstance();

            void setActive(srActive* active);
            void FUN_001accc0();
    };
};