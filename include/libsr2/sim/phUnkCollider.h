#pragma once
#include <libsr2/types.h>
#include <libsr2/sim/phColliderBase.h>

namespace sr2 {
    // vtable for this is located at 0x003a9828
    class phUnkCollider : public phColliderBase {
        public:
            phUnkCollider();
            virtual ~phUnkCollider();
    };
};