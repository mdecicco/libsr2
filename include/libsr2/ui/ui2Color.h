#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2WidgetBase.h>

namespace sr2 {
    class ui2Color : public ui2WidgetBase {
        public:
            ui2Color();
            virtual ~ui2Color();

            u32 color;
    };
};