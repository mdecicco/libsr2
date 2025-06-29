#pragma once
#include <libsr2/types.h>
#include <libsr2/managers/dgPhysManagerBase.h>

namespace sr2 {
    class dgPhysManager : public dgPhysManagerBase {
        public:
            dgPhysManager();
            virtual ~dgPhysManager();

            virtual void method_0x18(phImpact* impact);
            virtual void method_0x20();
    };
};