#include <libsr2/ui/srui2Bitmap.h>

namespace sr2 {
    srui2Bitmap::srui2Bitmap(
        const char* name,
        const char* filename,
        i32 x,
        i32 y,
        i32 p5,
        undefined4 p6,
        const WidgetRef<ui2Master>& master
    ) : ui2Visual(name, x, y, master) {
    }

    srui2Bitmap::~srui2Bitmap() {
    }
};