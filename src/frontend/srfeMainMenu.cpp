#include <libsr2/frontend/srfeMainMenu.h>

#include <assert.h>

namespace sr2 {
    srfeMainMenu::srfeMainMenu(const char* name, const WidgetRef<ui2Master>& master) : srfeUnknownScreen4(name, master) {
    }

    srfeMainMenu::~srfeMainMenu() {
    }

    void srfeMainMenu::initScreen() {
        srfeUnknownScreen4::initScreen();

        m_headingText->setTextName("MainMenu_Heading");
        
        m_textMenu = new srui2TextMenu("MainMenu_txtmenu", 7, "MainMenu", -10, 150, nullptr, 1, nullptr);
        m_textMenu->initMenuItem("SmugglerMission", 0, 0, 0);
        m_textMenu->initMenuItem("TurfWars", 1, 0, 0);
        m_textMenu->initMenuItem("CheckpointRace", 2, 0, 0);
        m_textMenu->initMenuItem("JoyRiding", 3, 0, 0);
        m_textMenu->initMenuItem("ViewHighScores", 4, 0, 0);
        m_textMenu->initMenuItem("Options", 5, 0, 0);
        m_textMenu->initMenuItem("Extras", 6, 0, 0);

        FUN_001f0fb8(m_textMenu->getUserRepeater(0), WidgetEventType::UNK22, "LoadOrNew", 1);
        FUN_001f0fb8(m_textMenu->getUserRepeater(1), WidgetEventType::UNK22, "PlayerSelTurfWars", 1);
        FUN_001f0fb8(m_textMenu->getUserRepeater(2), WidgetEventType::UNK22, "PlayerSelCPR", 1);
        FUN_001f0fb8(m_textMenu->getUserRepeater(3), WidgetEventType::UNK22, "PlayerSelJoyriding", 1);
        FUN_001f0fb8(m_textMenu->getUserRepeater(4), WidgetEventType::UNK22, "HiScoresMain", 1);
        FUN_001f0fb8(m_textMenu->getUserRepeater(5), WidgetEventType::UNK22, "Options", 1);
        FUN_001f0fb8(m_textMenu->getUserRepeater(6), WidgetEventType::UNK22, "Extras", 1);

        m_textMenu->FUN_001e5dc0(0);

        m_timer = new ui2Timer("MainMenu_InactivityTimer", 30.0f, false, true, nullptr);
        m_timer->FUN_002005f0(1);

        m_input->addListener(m_timer, WidgetEventType::UNK59, &ui2Widget::acceptEvent);
        m_input->addListener(m_textMenu, WidgetEventType::UNK59, &ui2Widget::acceptEvent);
        m_timer->addListener(this, WidgetEventType::TimerFinished, (SomeWidgetCallback)&srfeMainMenu::FUN_00136070);
    }

    void srfeMainMenu::FUN_00136070(const ui::NamedRef& p1, WidgetEventType p2, const WidgetRef<ui2EventData>& p3) {
        assert(p2 == WidgetEventType::TimerFinished);

        setActive(false);
        // MaybeActivateTitleScreen();
    }
};