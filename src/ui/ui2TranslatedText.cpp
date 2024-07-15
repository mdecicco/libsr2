#include <libsr2/ui/ui2TranslatedText.h>
#include <libsr2/ui/ui2Base.h>
#include <libsr2/utilities/txtFontTex.h>

#include <string.h>

namespace sr2 {
    ui2TranslatedText::ui2TranslatedText(const char* name, const char* text, i32 x, i32 y, i32 p5, const WidgetRef<ui2Master>& master) : ui2Text(name, x, y, p5, master) {
        init(text);
    }

    ui2TranslatedText::~ui2TranslatedText() {
    }

    void ui2TranslatedText::init(const char* text) {
        m_event = new ASCIIStringEventData();
        m_event->data = "";

        m_rawText[0] = 0;

        if (text) setTextName(text);
    }

    void ui2TranslatedText::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive) {
            ui2Text::onEvent(source, event, data);
            return;
        }

        switch (event) {
            case WidgetEventType::UNK60: {
                if (!data) return;
                setTextName(data.cast<ASCIIStringEventData>());
                break;
            }
            case WidgetEventType::UNK46: {
                m_string = ui2Base::getGameplayText().findString(m_rawText);
                break;
            }
            default: {
                ui2Text::onEvent(source, event, data);
            }
        }
    }

    void ui2TranslatedText::draw() {
        ui2Text::draw();
        if (!m_isActive || !field_0x78 || !m_rawText[0]) return;

        m_string->font->draw(
            m_string->field_0xc.x,
            m_string->field_0xc.y,
            m_string->field_0x14,
            m_string->field_0x15,
            m_string->text,
            m_textData,
            0,
            nullptr
        );
    }
    
    void ui2TranslatedText::method_0xc8(u32 p1) {
        if (p1 & 1) m_string = ui2Base::getGameplayText().findString(m_rawText);
    }

    void ui2TranslatedText::setTextName(const char* txtName) {
        m_event->data = txtName;
        setTextName(m_event);
    }

    void ui2TranslatedText::setTextName(const WidgetRef<ASCIIStringEventData>& event) {
        if (strncmp(m_rawText, event->data, 64) == 0) return;
        strncpy(m_rawText, event->data, 64);
        m_string = ui2Base::getGameplayText().findString(m_rawText);
        dispatchEvent(WidgetEventType::TextChanged, event);
    }
};