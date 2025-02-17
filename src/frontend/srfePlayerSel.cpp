#include <libsr2/frontend/srfePlayerSel.h>

namespace sr2 {
    srfePlayerSel::srfePlayerSel(
        const char* name,
        const char* nextScreenName,
        const char* prevScrenName,
        bool allow1on1,
        const WidgetRef<ui2Master>& master
    ) : srfeBasicLayout2(name, master) {
        m_nextScreenName = nextScreenName;
        m_prevScreenName = prevScrenName;
        m_allow1on1 = allow1on1;
    }

    srfePlayerSel::~srfePlayerSel() {
        deinitScreen();
    }

    void srfePlayerSel::initScreen() {
        srfeBasicLayout2::initScreen();

        char buf0[100];
        char buf1[100];

        snprintf(buf0, 100, "%s_Player_btnmenu", getName());
        snprintf(buf1, 100, "%s_Selection", getName());

        m_headingText->setTextName("PlayerSel_Heading");

        m_selection = new srui2ButtonMenu(buf0, 3, buf1, "PlayerSel_Selection", 1, 150, 190, "humans_var", 1, nullptr);
        m_selection->addOption("1Player", 0, "1");
        m_selection->addOption("2Players", 1, "2");
        if (m_allow1on1) m_selection->addOption("1On1", 2, "2-1ON1");

        m_input->addListener(m_selection, WidgetEventType::AnyInput);

        m_input->addListener(m_repeater0, WidgetEventType::BackPressed);
        FUN_001f0fb8(m_repeater0, WidgetEventType::BackPressed, m_prevScreenName, 0);

        m_input->addListener(m_repeater1, WidgetEventType::AcceptPressed);
        FUN_001f0fb8(m_repeater1, WidgetEventType::AcceptPressed, m_nextScreenName, 1);
    }

    void srfePlayerSel::deinitScreen() {
        srfeBasicLayout2::deinitScreen();
    }

    void srfePlayerSel::method_0x100() {
        srui2SRScreen::method_0x100();
        m_selection->FUN_001eaf98(0);
    }

    void srfePlayerSel::method_0x110() {
        srfeBasicLayout2::method_0x110();
        m_selection->FUN_001eb440();
    }

    void srfePlayerSel::method_0x128() {
        srfeBasicLayout2::method_0x128();
        m_selection->FUN_001eb260();
    }
};