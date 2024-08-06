#include <libsr2/frontend/srfeUnknownScreen1.h>
#include <libsr2/ui/ui2Base.h>

#include <stdio.h>

namespace sr2 {
    u32 srfeUnknownScreen1::ActiveCount = 0;
    u32 srfeUnknownScreen1::InitializedCount = 0;
    srfeUnknownScreen1::Widgets* srfeUnknownScreen1::WidgetsInstance = nullptr;

    srfeUnknownScreen1::srfeUnknownScreen1(const char* name, const WidgetRef<ui2Master>& master) : srui2SRScreen(name, master) {
        m_wasUninitialized = false;

        if (m_isActive) ActiveCount++;
        if (WidgetsInstance) WidgetsInstance->master->setActive(true);
    }

    srfeUnknownScreen1::~srfeUnknownScreen1() {
        deinitScreen();
        if (m_isActive) ActiveCount--;
    }

    void srfeUnknownScreen1::setActive(bool p1) {
        bool wasActive = m_isActive;
        ui2Screen::setActive(p1);

        if (wasActive != p1) {
            if (p1) {
                ActiveCount++;
                if (WidgetsInstance && ActiveCount == 1) WidgetsInstance->master->setActive(true);
            } else {
                ActiveCount--;
                if (WidgetsInstance && ActiveCount == 0) WidgetsInstance->master->setActive(false);
            }
        }
    }

    void srfeUnknownScreen1::initScreen() {
        srui2SRScreen::initScreen();
        InitializedCount++;

        if (!WidgetsInstance) initWidgets();

        char textName[100] = { 0 };
        snprintf(textName, 100, "%s_Heading_txt", m_widgetName->get());
        m_headingText = new ui2TranslatedText(textName, "", 97, 80, -1, nullptr);
        m_headingText->setColorU32(0x80969696);
        m_input->addListener(WidgetsInstance->navok_l_snd, WidgetEventType::AcceptPressed);
        m_input->addListener(WidgetsInstance->navok_r_snd, WidgetEventType::AcceptPressed);
    }

    void srfeUnknownScreen1::deinitScreen() {
        srui2SRScreen::deinitScreen();

        if (m_wasUninitialized) return;
        m_wasUninitialized = true;

        InitializedCount--;

        if (InitializedCount == 0) {
            if (WidgetsInstance->bg_movie) WidgetsInstance->bg_movie->FUN_001f3210();
            delete WidgetsInstance;
            WidgetsInstance = nullptr;
            m_headingText = nullptr;
        }
    }

    void srfeUnknownScreen1::method_0x108() {
        m_headingText->setVisibility(1);
        WidgetsInstance->bluebar_l_snd->FUN_00207eb8();
        WidgetsInstance->bluebar_r_snd->FUN_00207eb8();
    }

    void srfeUnknownScreen1::method_0x110() {
        if (method_0x118()) {
            WidgetsInstance->showbuttons_l_snd->FUN_00207eb8();
            WidgetsInstance->showbuttons_r_snd->FUN_00207eb8();
        }

        if (method_0x120()) {
            WidgetsInstance->showtexts_l_snd->FUN_00207eb8();
            WidgetsInstance->showtexts_r_snd->FUN_00207eb8();
        }
    }

    bool srfeUnknownScreen1::method_0x118() {
        return false;
    }

    bool srfeUnknownScreen1::method_0x120() {
        return false;
    }

    void srfeUnknownScreen1::initWidgets() {
        WidgetsInstance = new Widgets();
        Widgets* w = WidgetsInstance;

        auto gm = ui2Base::getGlobalMaster();
        w->master = new ui2Master("background_Master", ui2Base::getMaster(), true);
        gm->pushMaster(w->master);

        w->master->setRenderPriority(5030);

        w->bg_movie = new ui2Movie("basicLayout1_bg_movie", "uipan.m2v", 0, 0, 0, 5, nullptr, nullptr);         
        w->bg_movie->FUN_001f3328(0);
        w->bg_movie->FUN_001f3350(0);

        w->movie_score_snd = new ui2Sound("background_movie_score_snd", 0x3d, nullptr);
        w->movie_score_snd->addEventMapper(WidgetEventType::UNK55, WidgetEventType::UNK33, nullptr);
        w->movie_score_snd->addEventMapper(WidgetEventType::UNK56, WidgetEventType::UNK33, nullptr);
        w->bg_movie->addListener(w->movie_score_snd, WidgetEventType::UNK55);
        w->bg_movie->addListener(w->movie_score_snd, WidgetEventType::UNK56);

        w->bluebar_l_snd = new ui2Sound("background_bluebar_l_snd", 0x33, nullptr);
        w->bluebar_l_snd->FUN_00208068(-1.0f);
        w->bluebar_r_snd = new ui2Sound("background_bluebar_r_snd", 0x34, nullptr);
        w->bluebar_r_snd->FUN_00208068(1.0f);

        w->navok_l_snd = new ui2Sound("background_navok_l_snd", 0x3b, nullptr);
        w->navok_l_snd->FUN_00208068(-1.0f);
        w->navok_l_snd->addEventMapper(WidgetEventType::AcceptPressed, WidgetEventType::UNK33, nullptr);

        w->navok_r_snd = new ui2Sound("background_navok_r_snd", 0x3c, nullptr);
        w->navok_r_snd->FUN_00208068(1.0f);
        w->navok_r_snd->addEventMapper(WidgetEventType::AcceptPressed, WidgetEventType::UNK33, nullptr);

        w->showbuttons_l_snd = new ui2Sound("background_showbuttons_l_snd", 0x2b, nullptr);
        w->showbuttons_l_snd->FUN_00208068(-1.0f);

        w->showbuttons_r_snd = new ui2Sound("background_showbuttons_r_snd", 0x2c, nullptr);
        w->showbuttons_r_snd->FUN_00208068(1.0f);

        w->showtexts_l_snd = new ui2Sound("background_showtexts_l_snd", 0x39, nullptr);
        w->showtexts_l_snd->FUN_00208068(-1.0f);

        w->showtexts_r_snd = new ui2Sound("background_showtexts_r_snd", 0x3a, nullptr);
        w->showtexts_r_snd->FUN_00208068(1.0f);

        gm->popMaster();

        if (ActiveCount == 0) w->master->setActive(false);
    }
};