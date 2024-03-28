#include <libsr2/ui/ui2Visual.h>
#include <libsr2/io/datParser.h>

#include <stdio.h>

namespace sr2 {
    ui2Visual::ui2Visual(const char* name, i32 x, i32 y, const WidgetRef<ui2Master>& master) : ui2Widget(name, master, true) {
        field_0x78 = 1;
        m_pos = { x, y };
        m_color = 0x80ffffff;
    }

    ui2Visual::~ui2Visual() {
    }

    void ui2Visual::init() {
        m_unk0 = new ui2Color();
        m_unk1 = new ui2Position();
        field_0x48 = 1;
    }

    void ui2Visual::onEvent(const ui::BaseRef& p1, WidgetEventType p2, const ui::BaseRef& p3) {
        if (!field_0x1c) {
            ui2Widget::onEvent(p1, p2, p3);
            return;
        }

        switch (p2) {
            case WidgetEventType::SetPosition: {
                if (!p3) return;
                setPos(p3.cast<ui2Position>());
                return;
            }
            case WidgetEventType::SetColor: {
                if (!p3) return;
                setColor(p3.cast<ui2Color>());
                return;
            }
            case WidgetEventType::UNK6: {
                method_0x110(true);
                return;
            }
            case WidgetEventType::UNK7: {
                method_0x110(false);
                return;
            }
            default: {
                if (p2 > WidgetEventType::SetColor) {
                    ui2Widget::onEvent(p1, p2, p3);
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

        ui::BaseRef w;
        method_0x98(WidgetEventType::UNK11, p1, w);
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
        
        ui::BaseRef w;
        method_0x98(WidgetEventType::UNK13, p1, w);
    }

    void ui2Visual::method_0x110(undefined4 p1) {
        if (field_0x78 == p1) return;
        field_0x78 = p1;
        ui::BaseRef w;

        if (p1) method_0x98(WidgetEventType::UNK15, nullptr, w);
        else method_0x98(WidgetEventType::UNK14, nullptr, w);
    }
    
    undefined4 ui2Visual::method_0x118() {
        return field_0x78;
    }
    
    void ui2Visual::prepParserAgain(datParser* parser) {
        char buf[160] = { 0 };
        snprintf(buf, 160, "%s.PosX", m_widgetName->get());
        parser->add(PARSE_TYPE::INT32, buf, &m_pos.x, 1, nullptr);

        snprintf(buf, 160, "%s.PosX", m_widgetName->get());
        parser->add(PARSE_TYPE::INT32, buf, &m_pos.y, 1, nullptr);
    }
};