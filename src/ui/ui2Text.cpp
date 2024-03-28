#include <libsr2/ui/ui2Text.h>

namespace sr2 {
    ui2Text::ui2Text(const char* name, i32 x, i32 y, i32 p4, const WidgetRef<ui2Master>& master) : ui2Visual(name, x, y, master) {
    }

    ui2Text::~ui2Text() {
    }
    
    void ui2Text::setBounds(i32 w, i32 h) {
    }
    
    void ui2Text::setBoundsFromPos(i32 x, i32 y) {
    }
    
    void ui2Text::setAlignment(i32 align) {
    }
};