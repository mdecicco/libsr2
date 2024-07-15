#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Visual.h>

namespace sr2 {
    class ui2Movie : public ui2Visual {
        public:
            ui2Movie(
                const char* name,
                const char* filename,
                undefined4 p3,
                i32 x,
                i32 y,
                i32 p6,
                i32* p7,
                const WidgetRef<ui2Master>& master
            );
            ~ui2Movie();
    };
};