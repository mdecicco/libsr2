#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2DynamicText.h>

namespace sr2 {
    class srui2Clock : public ui2DynamicText {
        public:
            srui2Clock(const char* name, i32 x, i32 y, i32 p4, const WidgetRef<ui2Master>& master);
            ~srui2Clock();
    };
};