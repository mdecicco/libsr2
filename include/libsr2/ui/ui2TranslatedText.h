#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Text.h>

namespace sr2 {
    class ui2TranslatedText : public ui2Text {
        public:
            ui2TranslatedText(const char* name, const char* p2, i32 x, i32 y, i32 p5, const WidgetRef<ui2Master>& master);
            virtual ~ui2TranslatedText();
    };
};