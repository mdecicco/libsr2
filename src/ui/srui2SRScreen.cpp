#include <libsr2/ui/srui2SRScreen.h>

#include <string.h>

namespace sr2 {
    srui2SRScreen::srui2SRScreen(const char* name, const WidgetRef<ui2Master>& master) : ui2Screen(name, master) {
        setActive(false);
    }

    srui2SRScreen::~srui2SRScreen() {
        deinitScreen();
    }

    void srui2SRScreen::initScreen() {
        m_input = new ui2Input(nullptr, nullptr);
        m_repeater0 = new ui2Repeater(nullptr, nullptr);
        m_repeater1 = new ui2Repeater(nullptr, nullptr);
    }
    
    void srui2SRScreen::deinitScreen() {
        m_input = nullptr;
        m_repeater0 = nullptr;
        m_repeater1 = nullptr;
    }
    
    void srui2SRScreen::method_0x100() {
    }

    const char* srui2SRScreen::getType() const {
        return "srui2SRScreen";
    }

    bool srui2SRScreen::isA(const char* type) const {
        if (strcmp("srui2SRScreen", type) == 0 || strcmp("ui2Screen", type) == 0) return true;
        return strcmp(getType(), type) == 0;
    }
    
    void srui2SRScreen::FUN_001f0fb8(const WidgetRef<ui2Repeater>& p1, WidgetEventType p2, const char* p3, undefined4 p4) {
        if (!*p3) return;

        p1->addEventMapper(p2, WidgetEventType::Deactivate, nullptr);
        p1->addListener(this, WidgetEventType::Deactivate, &ui2Widget::acceptEvent);

        if (p4 == 0) {
            p1->addEventMapper(p2, WidgetEventType::UNK1, nullptr);
            p1->addListener(p3, WidgetEventType::UNK1, &ui2Widget::acceptEvent);
        } else {
            p1->addEventMapper(p2, WidgetEventType::UNK28, nullptr);
            p1->addListener(p3, WidgetEventType::UNK28, &ui2Widget::acceptEvent);
        }
    }
};