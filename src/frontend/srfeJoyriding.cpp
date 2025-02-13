#include <libsr2/frontend/srfeJoyriding.h>
#include <libsr2/ui/ui2Base.h>
#include <libsr2/ui/ui2Variable.h>

namespace sr2 {
    srfeJoyriding::srfeJoyriding(const char* name, const WidgetRef<ui2Master>& master) : srfeBasicLayout2(name, master) {
    }

    srfeJoyriding::~srfeJoyriding() {
        deinitScreen();
    }

    void srfeJoyriding::initScreen() {
        srfeBasicLayout2::initScreen();

        m_headingText->setTextName("Joyriding_Heading");
        m_levelBtnMenu = new srui2ButtonMenu(
            "Joyriding_Level_btnmenu", 3, "Joyriding_Level", "Joyriding_Level", 1, 0, 0, "level_var", 1, nullptr
        );
        m_levelBtnMenu->addOption("Afghanistan", 0, "Afghan");
        m_levelBtnMenu->addOption("Vietnam", 1, "Vietnam");
        m_levelBtnMenu->addOption("AfghanWinter", 2, "afghan_snow");
        m_input->addListener(m_levelBtnMenu, WidgetEventType::AnyInput);

        m_timeOfDayBtnMenu = new srui2ButtonMenu(
            "Joyriding_TimeOfDay_btnmenu", 3, "Joyriding_TimeOfDay", "Joyriding_TimeOfDay", 1, 0, 0, "timeOfDay_var", 1, nullptr
        );
        m_timeOfDayBtnMenu->addOption("Morning", 0, nullptr);
        m_timeOfDayBtnMenu->FUN_001eadd8(0, 0);
        m_timeOfDayBtnMenu->addOption("Daytime", 1, nullptr);
        m_timeOfDayBtnMenu->FUN_001eadd8(1, 1);
        m_timeOfDayBtnMenu->addOption("Evening", 2, nullptr);
        m_timeOfDayBtnMenu->FUN_001eadd8(2, 2);
        m_timeOfDayBtnMenu->addOption("Stormy", 3, nullptr);
        m_timeOfDayBtnMenu->FUN_001eadd8(3, 3);
        m_input->addListener(m_timeOfDayBtnMenu, WidgetEventType::AnyInput);

        m_policeBtnMenu = new srui2ButtonMenu(
            "Joyriding_Police_btnmenu", 3, "Joyriding_Police", "Joyriding_Police", 1, 0, 0, "police_var", 1, nullptr
        );
        m_policeBtnMenu->addOption("Off", 0, nullptr);
        m_policeBtnMenu->FUN_001eadd8(0, 0);
        m_policeBtnMenu->addOption("On", 1, nullptr);
        m_policeBtnMenu->FUN_001eadd8(1, 1);
        m_input->addListener(m_policeBtnMenu, WidgetEventType::AnyInput);

        auto gm = ui2Base::getGlobalMaster();
        WidgetRef<ui2Variable> policeVar = gm->findWidget("police_var", "ui2Variable").cast<ui2Variable>();

        policeVar->maybeClearValue();
    }

    void srfeJoyriding::deinitScreen() {
        srfeBasicLayout2::deinitScreen();
    }

    void srfeJoyriding::method_0x100() {
        srui2SRScreen::method_0x100();
    }

    void srfeJoyriding::method_0x110() {
        srfeBasicLayout2::method_0x110();
    }

    bool srfeJoyriding::method_0x118() {
        return true;
    }

    void srfeJoyriding::method_0x128() {
        srfeBasicLayout2::method_0x128();
    }
};