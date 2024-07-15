#include <libsr2/ui/ui2Screen.h>
#include <libsr2/ui/ui2Base.h>

#include <stdio.h>
#include <string.h>

namespace sr2 {
    ui2Screen::ui2Screen(const char* name, const WidgetRef<ui2Master>& master) : ui2Widget(name, master, true) {
        char* masterName = nullptr;
        
        if (name) {
            u32 masterNameLen = strlen(name);
            masterName = new char[masterNameLen + 8];
            snprintf(masterName, masterNameLen + 8, "%s-Master", name);
        }
        
        m_master = new ui2Master(masterName, nullptr, false);
        
        if (masterName) delete [] masterName;

        m_isInitialized = false;
        field_0x84 = 0;

        m_master->method_0xc0(false);
        addToMasterUnk0(0, 0x800090000);
        method_0xc0(true);

        field_0x48 = 1;
    }

    ui2Screen::~ui2Screen() {
    }

    void ui2Screen::reset() {
        deinit();
        m_master->reset();
        ui2Widget::reset();
    }

    void ui2Screen::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (event == WidgetEventType::UNK27) {
            if (m_isActive) method_0x100();
        } else if (event == WidgetEventType::UNK28) {
            method_0xa8(1);
            field_0x84 = 1;
        } else {
            ui2Widget::onEvent(source, event, data);
        }
    }

    void ui2Screen::method_0x48() {
        init();
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
        if (m_isActive) m_master->draw();
    }

    void ui2Screen::setActive(bool p1) {
        bool wasActive = m_isActive;
        ui2Widget::setActive(p1);

        if (wasActive != p1 && p1 && field_0x84 == 1) {
            method_0x100();
            field_0x84 = 0;
        }
    }

    void ui2Screen::method_0xe8(i32 p1) {
        ui2Widget::method_0xa8(p1);
        field_0x84 = 1;
    }

    void ui2Screen::method_0xc8(u32 p1) {
        m_master->method_0xc8(p1);
    }
    
    void ui2Screen::configureParser(datParser* parser) {
        m_master->configureParser(parser);
    }

    void ui2Screen::method_0x100() {
    }

    const char* ui2Screen::getType() const {
        return "ui2Screen";
    }

    bool ui2Screen::isA(const char* type) const {
        if (strcmp("ui2Screen", type) == 0) return true;
        return strcmp(getType(), type) == 0;
    }

    void ui2Screen::init() {
        WidgetRef<ui2Base> gm = ui2Base::getGlobalMaster();
        gm->pushMaster(m_master);

        initScreen();
        m_isInitialized = true;

        gm->popMaster();
    }

    void ui2Screen::deinit() {
        if (!m_isInitialized) return;
        m_isInitialized = false;
        deinitScreen();
    }
};