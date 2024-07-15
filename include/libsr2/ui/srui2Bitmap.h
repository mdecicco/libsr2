#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Visual.h>

namespace sr2 {
    class srui2Bitmap : public ui2Visual {
        public:
            srui2Bitmap(const char* name, const char* filename, i32 x, i32 y, i32 p5, undefined4 p6, const WidgetRef<ui2Master>& master);
            ~srui2Bitmap();
    };
};