#include <libsr2/ui/ui2Screen.h>
#include <libsr2/ui/ui2Base.h>

#include <stdio.h>
#include <string.h>

namespace sr2 {
    ui2Screen::ui2Screen(const char* name, const WidgetRef<ui2Master>& master) : ui2Widget(name, master, true) {
        u32 masterNameLen = strlen(name);
        char* masterName = new char[masterNameLen + 8];
        snprintf(masterName, masterNameLen, "%s-Master", masterName);
        m_master = new ui2Master(masterName, nullptr, false);
        delete [] masterName;

        field_0x80 = 0;
        field_0x84 = 0;

        m_master->method_0xc0(false);
        addToMasterUnk0(0, 0x800090000);
        method_0xc0(true);

        field_0x48 = 1;
    }

    ui2Screen::~ui2Screen() {
    }

    void ui2Screen::reset() {
        FUN_001fbbe0();
        m_master->reset();
        ui2Widget::reset();
    }

    void ui2Screen::method_0x30(const ui::BaseRef& p1, SOME_WIDGET_ENUM p2, const ui::BaseRef& p3) {
        if (p2 == SOME_WIDGET_ENUM::UNK27) {
            if (field_0x1c) method_0x100();
        } else if (p2 == SOME_WIDGET_ENUM::UNK28) {
            method_0xa8(1);
            field_0x84 = 1;
        } else {
            ui2Widget::method_0x30(p1, p2, p3);
        }
    }

    void ui2Screen::method_0x48() {
        FUN_001fbab0();
        m_master->method_0x48();
        method_0x100();
    }

    bool ui2Screen::loadWidget() {
        ui2Widget::loadWidget();
        return m_master->loadWidget();
    }

    void ui2Screen::method_0x58() {
        m_master->method_0x58();
    }

    void ui2Screen::draw() {
        ui2Widget::draw();
        if (field_0x1c) m_master->draw();
    }

    void ui2Screen::method_0xa0(bool p1) {
        bool u = field_0x1c;
        ui2Widget::method_0xa0(p1);

        if (u != p1 && p1 && field_0x84 == 1) {
            method_0x100();
            field_0x84 = 0;
        }
    }

    void ui2Screen::method_0xc8() {
        m_master->method_0xc8();
    }
    
    void ui2Screen::prepParserAgain(datParser* parser) {
        m_master->prepParserAgain(parser);
    }

    void ui2Screen::method_0xe8(i32 p1) {
        ui2Widget::method_0xa8(p1);
        field_0x84 = 1;
    }

    void ui2Screen::method_0x100() {
    }

    void ui2Screen::FUN_001fbab0() {
        ui2Base::getGlobalMaster()->pushMaster(m_master);
        method_0xf0();

        field_0x80 = 1;
    }

    void ui2Screen::FUN_001fbbe0() {
        if (field_0x80) {
            field_0x80 = 0;
            method_0xf8();
        }
    }
};