#include <libsr2/ui/ui2Visual.h>
#include <libsr2/io/datParser.h>

#include <libsr2/ui/ui2TranslatedText.h>

#include <stdio.h>

namespace sr2 {
    ui2Visual::ui2Visual(const char* name, i32 x, i32 y, const WidgetRef<ui2Master>& master) : ui2Widget(name, master, true) {
        m_isVisible = true;
        m_pos = { x, y };
        m_color = 0x80ffffff;

        init();
    }

    ui2Visual::~ui2Visual() {
    }

    void ui2Visual::init() {
        m_unk0 = new ui2Color();
        m_unk1 = new ui2Position();
        field_0x48 = 1;
    }

    void ui2Visual::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive) {
            ui2Widget::onEvent(source, event, data);
            return;
        }

        switch (event) {
            case WidgetEventType::SetPosition: {
                if (!data) return;
                setPos(data.cast<ui2Position>());
                return;
            }
            case WidgetEventType::SetColor: {
                if (!data) return;
                setColor(data.cast<ui2Color>());
                return;
            }
            case WidgetEventType::Show: {
                setVisibility(true);
                return;
            }
            case WidgetEventType::Hide: {
                setVisibility(false);
                return;
            }
            default: {
                if (event > WidgetEventType::SetColor) {
                    ui2Widget::onEvent(source, event, data);
                }
            }
        }
    }
    
    void ui2Visual::method_0xe8(i32 x, i32 y) {
        m_unk1->pos = { x, y };

        setPos(m_unk1);
    }

    void ui2Visual::setPos(const WidgetRef<ui2Position>& p1) {
        vec2i pos = p1->pos;

        if (pos.x == m_pos.x && pos.y == m_pos.y) return;
        m_pos = pos;

        dispatchEvent(WidgetEventType::UNK11, p1);
    }

    void ui2Visual::setColorU32(u32 color) {
        m_unk0->color = color;
        setColor(m_unk0);
    }

    void ui2Visual::setColorRGBA(u8 r, u8 g, u8 b, u8 a) {
        setColorU32(r | (g << 0x8) | (b << 0x10) | ((a + 1) >> 1) << 0x18);
    }

    void ui2Visual::setColor(const WidgetRef<ui2Color>& p1) {
        u32 color = p1->color;
        
        if (m_color == color) return;
        m_color = color;
        
        dispatchEvent(WidgetEventType::UNK13, p1);
    }

    void ui2Visual::setVisibility(bool p1) {
        if (m_isVisible == p1) return;
        m_isVisible = p1;

        if (p1) dispatchEvent(WidgetEventType::UNK15, nullptr);
        else dispatchEvent(WidgetEventType::UNK14, nullptr);
    }
    
    undefined4 ui2Visual::method_0x118() {
        return m_isVisible;
    }
    
    void ui2Visual::configureParser(datParser* parser) {
        char buf[160] = { 0 };
        snprintf(buf, 160, "%s.PosX", m_widgetName->get());
        parser->add(PARSE_TYPE::INT32, buf, &m_pos.x, 1, nullptr);

        snprintf(buf, 160, "%s.PosX", m_widgetName->get());
        parser->add(PARSE_TYPE::INT32, buf, &m_pos.y, 1, nullptr);
    }
};