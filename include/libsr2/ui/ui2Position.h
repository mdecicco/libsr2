#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2WidgetBase.h>

namespace sr2 {
    class ui2Position : public ui2WidgetBase {
        public:
            ui2Position();
            ~ui2Position();

            vec2i pos;
    };
};