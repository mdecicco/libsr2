#include <libsr2/frontend/srfeExtras.h>
#include <libsr2/frontend/srfeCredits.h>
#include <libsr2/ui/ui2Base.h>

namespace sr2 {
    srfeExtras::srfeExtras(const char* name, const WidgetRef<ui2Master>& master) : srfeBasicLayout1(name, master) {
    }

    srfeExtras::~srfeExtras() {
        deinitScreen();
    }

    void srfeExtras::initScreen() {
        srfeBasicLayout1::initScreen();
        m_menu = new srui2TextMenu("Extras_txtmenu", 4, "Extras", -10, 0x96, nullptr, 1, nullptr);
        m_menu->initMenuItem("Credits", 0, 0, 0);
        m_menu->initMenuItem("Theater", 1, 0, 0);
        m_menu->initMenuItem("ConceptArt", 2, 0, 0);
        m_menu->initMenuItem("BonusMissions", 3, 0, 0);

        FUN_001f0fb8(m_menu->getUserRepeater(0), WidgetEventType::UNK22, "Credits", 1);
        FUN_001f0fb8(m_menu->getUserRepeater(1), WidgetEventType::UNK22, "Theater", 1);
        FUN_001f0fb8(m_menu->getUserRepeater(2), WidgetEventType::UNK22, "ConceptArt", 1);
        FUN_001f0fb8(m_menu->getUserRepeater(3), WidgetEventType::UNK22, "BonusMissions", 1);

        m_input->addListener(m_menu, WidgetEventType::AnyInput);
        m_input->addListener(m_repeater0, WidgetEventType::BackPressed);

        FUN_001f0fb8(m_repeater0, WidgetEventType::BackPressed, "MainMenu", 0);
    }

    void srfeExtras::deinitScreen() {
        srfeBasicLayout1::deinitScreen();
        m_menu.release();
    }

    void srfeExtras::method_0x100() {
        srui2SRScreen::method_0x100();
        m_menu->FUN_001e5dc0(0);
    }

    void srfeExtras::method_0x110() {
        srfeBasicLayout1::method_0x110();
        m_menu->setMenuItemActive(3, true);
        m_menu->FUN_001e6000();

        WidgetRef<srfeCredits> credits = ui2Base::getGlobalMaster()->findWidget("Credits", "ui2Screen").cast<srfeCredits>();
        if (!credits) return;

        credits->FUN_0011c270();
    }

    void srfeExtras::method_0x128() {
        srfeBasicLayout1::method_0x128();
        m_menu->FUN_001e5e58();
    }
};