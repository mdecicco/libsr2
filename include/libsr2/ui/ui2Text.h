#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Visual.h>
#include <libsr2/ui/ASCIIStringEventData.h>
#include <libsr2/utilities/txtFontTex.h>

namespace sr2 {
    class ui2Text : public ui2Visual {
        public:
            ui2Text(const char* name, i32 x, i32 y, i32 p4, const WidgetRef<ui2Master>& master);
            virtual ~ui2Text();

            void init(i32 p1);

            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void draw();
            virtual void configureParser(datParser* parser);

            virtual void setBounds(i32 w, i32 h);
            virtual void setBoundsFromPos(i32 x, i32 y);
            virtual void setAlignment(i32 align);
            virtual void setAdvanceX(i32 advanceX);
            virtual void setTextName(const char* txtName);
            virtual void setTextName(const WidgetRef<ASCIIStringEventData>& event);
        
        protected:
            vec2i m_bounds;
            i32 m_alignment;
            TextDisplayData m_textData;
    };
};