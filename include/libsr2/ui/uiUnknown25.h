#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2EventData.h>

namespace sr2 {
    class uiUnknown25 : public ui2EventData {
        public:
            uiUnknown25();
            ~uiUnknown25();

            i32 row;
            i32 col;
    };
};