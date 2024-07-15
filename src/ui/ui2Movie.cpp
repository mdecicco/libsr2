#include <libsr2/ui/ui2Movie.h>

namespace sr2 {
    ui2Movie::ui2Movie(
        const char* name,
        const char* filename,
        undefined4 p3,
        i32 x,
        i32 y,
        i32 p6,
        i32* p7,
        const WidgetRef<ui2Master>& master
    ) : ui2Visual(name, x, y, master) {
    }

    ui2Movie::~ui2Movie() {
    }
};