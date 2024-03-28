#include <libsr2/ui/ui2Image.h>

namespace sr2 {
    ui2Image::ui2Image(char* name, const char* p2, i32 x, i32 y, i32 p5, const WidgetRef<ui2Master>& master) : ui2Visual(name, x, y, master) {
    }

    ui2Image::ui2Image(char* name, i32 x, i32 y, i32 p4, const WidgetRef<ui2Master>& master) : ui2Visual(name, x, y, master) {
    }

    ui2Image::~ui2Image() {
    }

    void ui2Image::FUN_001f5e18(i32 p1, i32 p2) {
    }
};