#include <libsr2/ui/ui2Text.h>
#include <libsr2/io/datParser.h>

#include <stdio.h>

namespace sr2 {
    ui2Text::ui2Text(const char* name, i32 x, i32 y, i32 p4, const WidgetRef<ui2Master>& master) : ui2Visual(name, x, y, master) {
        m_bounds = { 9999, 9999 };
        m_alignment = 1; 
 
        m_textData.flags = 0x102;
        m_textData.field_0x20 = 9999;
        m_textData.color = 0xffffffff;
        m_textData.pos.x = 0;
        m_textData.pos.y = 0;
        m_textData.top = 0;
        m_textData.bottom = 9999;
        m_textData.left = 0;
        m_textData.right = 9999;
        m_textData.field_0x1c = 0;
        m_textData.field_0x24 = 0;
        m_textData.field_0x2c = 0;
        m_textData.field_0x28 = 0;

        init(p4);
    }

    ui2Text::~ui2Text() {
    }

    void ui2Text::init(i32 p1) {
        addToMasterUnk0(p1, 0x800090000);
        m_textData.flags = m_textData.flags | 8;
    }

    void ui2Text::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        ui2Visual::onEvent(source, event, data);
    }

    void ui2Text::draw() {
        ui2Widget::draw();
        if (!m_isActive || !field_0x78) return;
        
        m_textData.pos = m_pos;
        m_textData.top = m_pos.y;
        m_textData.left = m_pos.x;
        m_textData.right = m_pos.x + m_bounds.x;
        m_textData.bottom = m_pos.y + m_bounds.y;

        if (m_alignment == 1) m_textData.flags = m_textData.flags & 0xfffffffa | 2;
        else if (m_alignment == 0) m_textData.flags = m_textData.flags & 0xfffffff9 | 1;
        else if (m_alignment == 2) m_textData.flags = m_textData.flags & 0xfffffffc | 4;
    }
    
    void ui2Text::configureParser(datParser* parser) {
        ui2Visual::configureParser(parser);

        char buf[160] = { 0 };
        const char* name = m_widgetName->get();

        snprintf(buf, 160, "%s.BBoxWidth", name);
        parser->add(PARSE_TYPE::INT32, buf, &m_bounds.x, 1, nullptr);
        snprintf(buf, 160, "%s.BBoxHeight", name);
        parser->add(PARSE_TYPE::INT32, buf, &m_bounds.y, 1, nullptr);
        snprintf(buf, 160, "%s.Alignment", name);
        parser->add(PARSE_TYPE::INT32, buf, &m_alignment, 1, nullptr);
    }
    
    void ui2Text::setBounds(i32 w, i32 h) {
        m_bounds = { w, h };
    }
    
    void ui2Text::setBoundsFromPos(i32 x, i32 y) {
        m_bounds = {
            (x - m_pos.x) + 1,
            (y - m_pos.y) + 1
        };
    }
    
    void ui2Text::setAlignment(i32 align) {
        m_alignment = align;
    }

    void ui2Text::setTextName(const char* txtName) {
    }
    
    void ui2Text::setTextName(const WidgetRef<ASCIIStringEventData>& event) {
    }
};