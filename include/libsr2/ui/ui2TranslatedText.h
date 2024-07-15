#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Text.h>
#include <libsr2/ui/ASCIIStringEventData.h>

namespace sr2 {
    struct txtStringData;

    class ui2TranslatedText : public ui2Text {
        public:
            ui2TranslatedText(const char* name, const char* text, i32 x, i32 y, i32 p5, const WidgetRef<ui2Master>& master);
            virtual ~ui2TranslatedText();

            void init(const char* text);

            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void draw();
            virtual void method_0xc8(u32 p1);

            virtual void setTextName(const char* txtName);
            virtual void setTextName(const WidgetRef<ASCIIStringEventData>& event);
        
        protected:
            WidgetRef<ASCIIStringEventData> m_event;
            char m_rawText[64];
            txtStringData* m_string;
    };
};