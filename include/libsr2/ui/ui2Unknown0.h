#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2WidgetBase.h>

namespace sr2 {
    class ui2Unknown0 : public ui2WidgetBase {
        public:
            ui2Unknown0();
            virtual ~ui2Unknown0();

            char field_0x10[32];
            u32 stringFlags;
            LANGUAGE language;
    };
};