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
            "Joyriding_TimeOfDay_btnmenu", 4, "Joyriding_TimeOfDay", "Joyriding_TimeOfDay", 1, 0, 0, "timeOfDay_var", 1, nullptr
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
            "Joyriding_Police_btnmenu", 2, "Joyriding_Police", "Joyriding_Police", 1, 0, 0, "police_var", 1, nullptr
        );
        m_policeBtnMenu->addOption("Off", 0, nullptr);
        m_policeBtnMenu->FUN_001eadd8(0, 0);
        m_policeBtnMenu->addOption("On", 1, nullptr);
        m_policeBtnMenu->FUN_001eadd8(1, 1);
        m_input->addListener(m_policeBtnMenu, WidgetEventType::AnyInput);

        auto gm = ui2Base::getGlobalMaster();
        WidgetRef<ui2Variable> policeVar = gm->findWidget("police_var", "ui2Variable").cast<ui2Variable>();
        policeVar->maybeClearValue();

        m_menuMenu = new ui2UnknownMenu0("Joyriding_menuMenu_menu", 1, 3, nullptr);
        m_menuMenu->FUN_001f8218(m_levelBtnMenu, 0, 0, 1, &ui2Widget::acceptEvent);
        m_menuMenu->FUN_001f8218(m_timeOfDayBtnMenu, 0, 1, 1, &ui2Widget::acceptEvent);
        m_menuMenu->FUN_001f8218(m_policeBtnMenu, 0, 2, 1, &ui2Widget::acceptEvent);
        m_menuMenu->FUN_001fa518();

        m_input->addListener(m_menuMenu, WidgetEventType::AnyInput);
        m_input->addListener(m_repeater0, WidgetEventType::BackPressed);

        FUN_001f0fb8(m_repeater0, WidgetEventType::BackPressed, "PlayerSelJoyriding", 0);

        m_repeater0->addEventMapper(WidgetEventType::BackPressed, WidgetEventType::BackPressed, nullptr);
        m_repeater0->addEventMapper(WidgetEventType::BackPressed, WidgetEventType::ClearValue, nullptr);
        m_repeater0->addListener("police_var", WidgetEventType::ClearValue);

        m_nextScreenP1Cond = new ui2Condition("Joyriding_nextScreenP1_cond", "humans_var", CondComparator::IsEqualTo, nullptr);

        WidgetRef<ASCIIStringEventData> eventData = new ASCIIStringEventData();
        eventData->data = "1";
        m_nextScreenP1Cond->addTarget(eventData, "VehicleSelJoyriding", WidgetEventType::UNK28, nullptr, &ui2Widget::acceptEvent);

        eventData = new ASCIIStringEventData();
        eventData->data = "2";
        m_nextScreenP1Cond->addTarget(eventData, "VehicleSel2PlJoyriding", WidgetEventType::UNK28, nullptr, &ui2Widget::acceptEvent);

        m_nextScreenP1Cond->addEventMapper(WidgetEventType::AcceptPressed, WidgetEventType::EvaluateCondition, nullptr);

        m_input->addListener(m_repeater1, WidgetEventType::AcceptPressed);
        m_input->addListener(m_nextScreenP1Cond, WidgetEventType::AcceptPressed);
        m_repeater1->addEventMapper(WidgetEventType::AcceptPressed, WidgetEventType::Deactivate, nullptr);
        m_repeater1->addListener(this, WidgetEventType::Deactivate);
        m_input->addListener(this, WidgetEventType::AcceptPressed, (WidgetEventCallback)&srfeJoyriding::interceptEvent);
        m_input->addListener(this, WidgetEventType::BackPressed, (WidgetEventCallback)&srfeJoyriding::interceptEvent);

        m_menuTable = new ui2Table("Joyriding_menuTable_tbl", 1, 3, 30, 130, nullptr);
        m_menuTable->setRowSizes(60, -1, -2);
        m_menuTable->FUN_001fc6f8(m_levelBtnMenu, 0, 0, &ui2Widget::acceptEvent);
        m_menuTable->FUN_001fc6f8(m_timeOfDayBtnMenu, 0, 1, &ui2Widget::acceptEvent);
        m_menuTable->FUN_001fc6f8(m_policeBtnMenu, 0, 2, &ui2Widget::acceptEvent);

        m_menuTable->forceRecalculate();
    }

    void srfeJoyriding::deinitScreen() {
        srfeBasicLayout2::deinitScreen();
        
        m_levelBtnMenu = nullptr;
        m_timeOfDayBtnMenu = nullptr;
        m_policeBtnMenu = nullptr;
        m_menuMenu = nullptr;
        m_menuTable = nullptr;
        m_nextScreenP1Cond = nullptr;
    }

    void srfeJoyriding::method_0x100() {
        srui2SRScreen::method_0x100();
        m_levelBtnMenu->FUN_001eaf98(0);
        m_timeOfDayBtnMenu->FUN_001eaf98(0);
        m_policeBtnMenu->FUN_001eaf98(0);

        m_menuMenu->FUN_001f9990(0, 0);

        // I think this is checking if each of the levels are unlocked
        u32 level0 = 1; // FUN_001b0808(0, 0);
        u32 level1 = 1; // FUN_001b0808(1, 0);
        u32 level2 = 1; // FUN_001b0808(2, 0);

        m_levelBtnMenu->FUN_001eaf58(0, level0);
        m_levelBtnMenu->FUN_001eaf58(1, level1);
        m_levelBtnMenu->FUN_001eaf58(2, level2);
    }

    void srfeJoyriding::method_0x110() {
        srfeBasicLayout2::method_0x110();

        m_levelBtnMenu->FUN_001eb440();
        m_timeOfDayBtnMenu->FUN_001eb440();
        m_policeBtnMenu->FUN_001eb440();

        m_menuMenu->FUN_001fa518();
        m_menuMenu->FUN_001f1a50();

        auto gm = ui2Base::getGlobalMaster();

        WidgetRef<ui2Variable> var;
        
        var = gm->findWidget("mission_var", "ui2Variable").cast<ui2Variable>();
        if (var) var->setStringData("Cruise.mp");

        var = gm->findWidget("team_game_var", "ui2Variable").cast<ui2Variable>();
        if (var) var->setIntegerData(0);

        // INT_0035f2e0 = -1;

        var = gm->findWidget("regularStartupScreen_var", "ui2Variable").cast<ui2Variable>();
        if (var) var->setStringData(getName());
    }

    bool srfeJoyriding::method_0x118() {
        return true;
    }

    void srfeJoyriding::method_0x128() {
        srfeBasicLayout2::method_0x128();

        m_levelBtnMenu->FUN_001eb260();
        m_timeOfDayBtnMenu->FUN_001eb260();
        m_policeBtnMenu->FUN_001eb260();

        m_menuMenu->FUN_001f1a20();
    }

    void srfeJoyriding::interceptEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        switch (event) {
            case WidgetEventType::BackPressed: {
                method_0x128();
                break;
            }
            case WidgetEventType::AcceptPressed: {
                MAP_SELECTION selection = (MAP_SELECTION)m_levelBtnMenu->getSelectedIndex();
                switch (selection) {
                    case MAP_SELECTION::Afghan:
                    case MAP_SELECTION::Vietnam:
                    case MAP_SELECTION::AfghanSnow: {
                        // Game::InMenuState::SelectedLevel = selection;
                        break;
                    }
                }

                break;
            }
        }
    }
};