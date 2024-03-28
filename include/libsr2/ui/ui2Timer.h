#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>

namespace sr2 {
    class ui2Timer : public ui2Widget {
        public:
            ui2Timer(char* name, f32 time, i32 p3, i32 p4, const WidgetRef<ui2Master>& master);
            virtual ~ui2Timer();
    };
};