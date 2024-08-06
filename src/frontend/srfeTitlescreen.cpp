#include <libsr2/frontend/srfeTitlescreen.h>
#include <libsr2/frontend/srfeHiScoresBase.h>
#include <libsr2/managers/audManager.h>
#include <libsr2/ui/ui2Base.h>

#include <string.h>

namespace sr2 {
    undefined4 srfeTitlescreen::DAT_0035ef70 = 0;

    srfeTitlescreen::srfeTitlescreen(const char* name, const WidgetRef<ui2Master>& master) : srui2SRScreen(name, master) {
        field_0xa0 = 0;
        field_0xa4 = 4;
        field_0xa8 = 0;
        field_0xac = 0;
    }

    srfeTitlescreen::~srfeTitlescreen() {
    }

    void srfeTitlescreen::initScreen() {
        srui2SRScreen::initScreen();

        m_backgroundImg = new srui2Bitmap("Titlescreen_Image", "titlescreen.tga", 0, 0, 10, 1, nullptr);
        m_inactivityTimer = new ui2Timer("Titlescreen_InactivityTimer", 30.0f, false, true, nullptr);
        m_inactivityTimer->FUN_002005f0(1);
        m_input->addListener(m_inactivityTimer, WidgetEventType::AnyInput);
        m_inactivityTimer->addListener(this, WidgetEventType::TimerFinished, (WidgetEventCallback)&srfeTitlescreen::interceptEvent);

        m_cursorImg = new ui2Image("Titlescreen_cursor", "uichevron", 200, 400, 0, nullptr);
        m_cursorImg->FUN_001f5db0(2);
        m_cursorImg->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK8, nullptr);

        m_cursorTimer = new ui2Timer("Titlescreen_cursor_timer", 1.4f, true, true, nullptr);
        m_cursorTimer->addListener(m_cursorImg, WidgetEventType::TimerFinished);

        m_cursorLeftSound = new ui2Sound("Titlescreen_cursorleft_snd", 0x31, nullptr);
        m_cursorLeftSound->FUN_00208068(-1.0f);
        m_cursorLeftSound->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK33, nullptr);
        m_cursorTimer->addListener(m_cursorLeftSound, WidgetEventType::TimerFinished);
        m_cursorLeftSound->FUN_00207fe0(0.45f);

        m_cursorRightSound = new ui2Sound("Titlescreen_cursorright_snd", 0x32, nullptr);
        m_cursorRightSound->FUN_00208068(1.0f);
        m_cursorRightSound->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK33, nullptr);
        m_cursorTimer->addListener(m_cursorRightSound, WidgetEventType::TimerFinished);
        m_cursorRightSound->FUN_00207fe0(0.45f);

        m_input->addListener(this, WidgetEventType::AcceptPressed, (WidgetEventCallback)&srfeTitlescreen::interceptEvent);
        m_input->addListener(this, WidgetEventType::StartPressed, (WidgetEventCallback)&srfeTitlescreen::interceptEvent);
    }

    void srfeTitlescreen::deinitScreen() {
        srui2SRScreen::deinitScreen();
        m_backgroundImg.release();
        m_inactivityTimer.release();
        m_cursorImg.release();
        m_cursorTimer.release();
        m_cursorLeftSound.release();
        m_cursorRightSound.release();
    }

    const char* srfeTitlescreen::getType() const {
        return "srfeTitlescreen";
    }

    bool srfeTitlescreen::isA(const char* type) const {
        if (strcmp("srfeTitlescreen", type) == 0) return true;
        return srui2SRScreen::isA(type);
    }

    void srfeTitlescreen::setActive(bool isActive) {
        bool wasActive = m_isActive;
        ui2Screen::setActive(isActive);

        if (wasActive == isActive || !isActive) return;

        if (field_0xac) {
            if (!srfeTitlescreen::DAT_0035ef70) {
                FUN_00117ad8();
                field_0xac = 0;
            }
        }

        if (!field_0xa8 || field_0xac) {
            m_inactivityTimer->setDuration(30.0f);
        } else {
            m_inactivityTimer->setDuration(20.0f);
        }

        m_inactivityTimer->startTimer();
        m_input->FUN_001fee50();
        field_0xac = 0;
    }

    void srfeTitlescreen::interceptEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        switch (event) {
            case WidgetEventType::StartPressed: {
                srfeTitlescreen::FUN_00117ad8();
                break;
            }
            case WidgetEventType::AcceptPressed: {
                srfeTitlescreen::FUN_00117ad8();
                break;
            }
            case WidgetEventType::TimerFinished: {
                setActive(false);
                srfeTitlescreen::Activate();
                break;
            }
            default: {
                abort();
                break;
            }
        }
    }

    void srfeTitlescreen::FUN_00117ad8() {
        WidgetRef<ui2Screen> screen;
        if (srfeTitlescreen::DAT_0035ef70) {
            screen = ui2Base::getGlobalMaster()->findWidget("MovieUIIntro", "ui2Screen").cast<ui2Screen>();
        } else {
            screen = ui2Base::getGlobalMaster()->findWidget("MainMenu", "ui2Screen").cast<ui2Screen>();
        }

        audManager::playSound(0x3b);
        audManager::playSound(0x3c);

        screen->method_0xa8(1);
        setActive(false);
        
        field_0xa8 = 0;
        
        // UI::GameLoadingScreen::FUN_001c54e8(&UI::GameLoadingScreen::InstanceMem,9);
        // UI::GameLoadingScreen::InstanceMem.progress_frac = 1.0;
        // UI::GameLoadingScreen::InstanceMem.progress_frac_again_for_some_reason = 1.0;
        // UI::GameLoadingScreen::FUN_001c54f0(&UI::GameLoadingScreen::InstanceMem,0.25);

        srfeTitlescreen::FUN_001175c8();
        srfeTitlescreen::DAT_0035ef70 = 0;
    }

    void srfeTitlescreen::FUN_00117560() {
        srfeTitlescreen::FUN_00117718("HiScoresSmugglerM", 1);
        srfeTitlescreen::FUN_00117718("HiScoresCNS", 1);
        srfeTitlescreen::FUN_00117718("HiScoresLootGrab", 1);
        srfeTitlescreen::FUN_00117718("HiScoresBombTag", 1);
        srfeTitlescreen::FUN_00117718("HiScoresCheckpointRace", 1);
    }

    void srfeTitlescreen::FUN_001175c8() {
        srfeTitlescreen::FUN_00117718("HiScoresSmugglerM", 0);
        srfeTitlescreen::FUN_00117718("HiScoresCNS", 0);
        srfeTitlescreen::FUN_00117718("HiScoresLootGrab", 0);
        srfeTitlescreen::FUN_00117718("HiScoresBombTag", 0);
        srfeTitlescreen::FUN_00117718("HiScoresCheckpointRace", 0);
    }

    void srfeTitlescreen::FUN_00117718(const char* p1, undefined4 p2) {
        WidgetRef<srfeHiScoresBase> hs = ui2Base::getGlobalMaster()->findWidget(p1, "srfeHiScoresBase").cast<srfeHiScoresBase>();
        if (!hs) return;

        // hs->FUN_00122a08(p2);
    }

    void srfeTitlescreen::FUN_00117800(undefined4 p1) {
        WidgetRef<srfeTitlescreen> ts = ui2Base::getGlobalMaster()->findWidget("Titlescreen", "srfeTitlescreen").cast<srfeTitlescreen>();
        if (!ts) return;

        ts->field_0xac = p1;
    }

    void srfeTitlescreen::Activate() {
        WidgetRef<srfeTitlescreen> ts = ui2Base::getGlobalMaster()->findWidget("Titlescreen", "srfeTitlescreen").cast<srfeTitlescreen>();
        if (!ts) return;

        ts->field_0xa4++;
        if (ts->field_0xa4 > 4) ts->field_0xa4 = 0;

        WidgetRef<ui2Screen> movie = ui2Base::getGlobalMaster()->findWidget("MovieIntro", "ui2Screen").cast<ui2Screen>();
        // UI::GameLoadingScreen::FUN_001c54e8(&UI::GameLoadingScreen::InstanceMem,9);
        // UI::GameLoadingScreen::InstanceMem.progress_frac = 1.0;
        // UI::GameLoadingScreen::InstanceMem.progress_frac_again_for_some_reason = 1.0;
        // UI::GameLoadingScreen::FUN_001c54f0(&UI::GameLoadingScreen::InstanceMem,0.25);

        srfeTitlescreen::FUN_00117560();
        ts->field_0xa8 = 1;
        ts->field_0xac = 0;

        movie->method_0xa8(1);
    }
};