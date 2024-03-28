#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2WidgetBase.h>

namespace sr2 {
    class IntegerEventData : public ui2WidgetBase {
        public:
            IntegerEventData();
            ~IntegerEventData();

            i32 data;
    };
};