#include <libsr2/ui/ui2DynamicText.h>

namespace sr2 {
    ui2DynamicText::ui2DynamicText(const char* name, i32 x, i32 y, i32 p4, const char* p5, i32 p6, const WidgetRef<ui2Master>& master) : ui2Text(name, x, y, p4, master) {
    }

    ui2DynamicText::~ui2DynamicText() {
    }

    void ui2DynamicText::setFont(const char* fontName) {
    }
};