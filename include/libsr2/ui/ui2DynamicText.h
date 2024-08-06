#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Text.h>
#include <libsr2/i18n/txtStringTable.h>

namespace sr2 {
    class ui2DynamicText : public ui2Text {
        public:
            ui2DynamicText(const char* name, i32 x, i32 y, i32 p4, const char* p5, i32 p6, const WidgetRef<ui2Master>& master);
            ~ui2DynamicText();

            virtual void setFont(const char* fontName);
            virtual void method_0x148(const char* text);
            virtual void method_0x170(const char* fontName);

        protected:
            txtFontTex* m_font;
    };
};