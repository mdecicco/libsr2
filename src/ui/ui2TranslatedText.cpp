#include <libsr2/ui/ui2TranslatedText.h>

namespace sr2 {
    ui2TranslatedText::ui2TranslatedText(const char* name, const char* p2, i32 x, i32 y, i32 p5, const WidgetRef<ui2Master>& master) : ui2Text(name, x, y, p5, master) {
    }

    ui2TranslatedText::~ui2TranslatedText() {
    }
};