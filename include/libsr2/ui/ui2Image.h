#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Visual.h>

namespace sr2 {
    class ui2Image : public ui2Visual {
        public:
            ui2Image(char* name, const char* p2, i32 x, i32 y, i32 p5, const WidgetRef<ui2Master>& master);
            ui2Image(char* name, i32 x, i32 y, i32 p4, const WidgetRef<ui2Master>& master);
            virtual ~ui2Image();

            void FUN_001f5e18(i32 p1, i32 p2);
    };
};