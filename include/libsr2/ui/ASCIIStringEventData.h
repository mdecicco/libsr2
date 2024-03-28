#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2WidgetBase.h>

namespace sr2 {
    class ASCIIStringEventData : public ui2WidgetBase {
        public:
            ASCIIStringEventData();
            ~ASCIIStringEventData();

            const char* data;
    };
};