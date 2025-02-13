#include <libsr2/frontend/srfeBasicLayout2.h>
#include <libsr2/ui/ui2Base.h>

namespace sr2 {
    u32 srfeBasicLayout2::ActiveCount = 0;
    u32 srfeBasicLayout2::InitializedCount = 0;
    srfeBasicLayout2::Widgets* srfeBasicLayout2::WidgetsInstance = nullptr;

    srfeBasicLayout2::srfeBasicLayout2(const char* name, const WidgetRef<ui2Master>& master) : srfeBackground(name, master) {
        m_wasUninitialized = false;

        if (m_isActive) ActiveCount++;
        if (WidgetsInstance) WidgetsInstance->master->setActive(true);
    }

    srfeBasicLayout2::~srfeBasicLayout2() {
        deinitScreen();
        ActiveCount--;
    }

    void srfeBasicLayout2::setActive(bool active) {
        bool wasActive = m_isActive;
        srfeBackground::setActive(active);

        if (wasActive == active) return;

        if (active) {
            ActiveCount++;
            if (ActiveCount == 1 && WidgetsInstance) WidgetsInstance->master->setActive(true);
        } else {
            ActiveCount--;
            if (ActiveCount == 0 && WidgetsInstance) WidgetsInstance->master->setActive(false);
        }
    }

    void srfeBasicLayout2::initScreen() {
        srfeBackground::initScreen();
        InitializedCount++;

        if (!WidgetsInstance) initWidgets();

        WidgetsInstance->showUITimer->addListener(this, WidgetEventType::TimerFinished, (WidgetEventCallback)&srfeBasicLayout2::showUiListener);
    }

    void srfeBasicLayout2::deinitScreen() {
        srfeBackground::deinitScreen();

        if (m_wasUninitialized) return;
        m_wasUninitialized = true;

        InitializedCount--;

        if (InitializedCount == 0) {
            delete WidgetsInstance;
            WidgetsInstance = nullptr;
        }

        WidgetsInstance->showUITimer->removeListener(m_widgetName->get(), WidgetEventType::TimerFinished);
    }

    void srfeBasicLayout2::method_0x108() {
        srfeBackground::method_0x108();

        m_input->setActive(false);
        method_0x128();

        Widgets* w = WidgetsInstance;
        w->bluebarsTimer01->startTimer();
        w->bluebarsTimer02->startTimer();
        w->bluebarsTimer03->startTimer();
        w->bluebarsTimer04->startTimer();
        w->bluebarsTimer05->startTimer();
        w->bluebarsTimer06->startTimer();
        w->bluebarsTimer07->startTimer();
        w->bluebarsTimer08->startTimer();
        w->bluebarsTimer09->startTimer();
        w->bluebarsTimer10->startTimer();
        w->bluebarsTimer11->startTimer();

        if (method_0x138()) {
            w->selectBtnTimer->startTimer();
        }

        if (method_0x130()) {
            w->backBtnTimer->startTimer();
        }

        w->clockTopTimer->startTimer();
        w->showUITimer->startTimer();
    }

    void srfeBasicLayout2::method_0x110() {
        srfeBackground::method_0x110();
        m_input->setActive(true);
    }

    void srfeBasicLayout2::method_0x128() {
        Widgets* w = WidgetsInstance;

        w->bluebars01->setVisibility(false);
        w->bluebars02->setVisibility(false);
        w->bluebars03->setVisibility(false);
        w->bluebars04->setVisibility(false);
        w->bluebars05->setVisibility(false);
        w->bluebars06->setVisibility(false);
        w->bluebars07->setVisibility(false);
        w->bluebars08->setVisibility(false);
        w->bluebars09->setVisibility(false);
        w->bluebars10->setVisibility(false);
        w->bluebars11->setVisibility(false);

        w->selectBtn->setVisibility(false);
        w->backBtn->setVisibility(false);
        w->selectText->setVisibility(false);
        w->backText->setVisibility(false);
        w->clockTop->setVisibility(false);
    }

    bool srfeBasicLayout2::method_0x130() {
        return true;
    }

    bool srfeBasicLayout2::method_0x138() {
        return true;
    }

    void srfeBasicLayout2::showUiListener(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive) return;

        method_0x110();
    }

    void srfeBasicLayout2::initWidgets() {
        Widgets* w = new Widgets();
        WidgetsInstance = w;
        
        auto gm = ui2Base::getGlobalMaster();

        w->master = new ui2Master("basicLayout2_Master", ui2Base::getMaster(), true);
        gm->pushMaster(w->master);

        w->master->setRenderPriority(0x1392);

        w->bluebars01 = new ui2Image("basicLayout2_bluebars01", "uibluebarsb01", 0, 0, 0, nullptr);
        w->bluebars02 = new ui2Image("basicLayout2_bluebars02", "uibluebarsb02", 0, 0, 0, nullptr);
        w->bluebars03 = new ui2Image("basicLayout2_bluebars03", "uibluebarsb03", 0, 0, 0, nullptr);
        w->bluebars04 = new ui2Image("basicLayout2_bluebars04", "uibluebarsb04", 0, 0, 0, nullptr);
        w->bluebars05 = new ui2Image("basicLayout2_bluebars05", "uibluebarsb05", 0, 0, 0, nullptr);
        w->bluebars06 = new ui2Image("basicLayout2_bluebars06", "uibluebarsb06", 0, 0, 0, nullptr);
        w->bluebars07 = new ui2Image("basicLayout2_bluebars07", "uibluebarsb07", 0, 0, 0, nullptr);
        w->bluebars08 = new ui2Image("basicLayout2_bluebars08", "uibluebarsb08", 0, 0, 0, nullptr);
        w->bluebars09 = new ui2Image("basicLayout2_bluebars09", "uibluebarsb09", 0, 0, 0, nullptr);
        w->bluebars10 = new ui2Image("basicLayout2_bluebars10", "uibluebarsb10", 0, 0, 0, nullptr);
        w->bluebars11 = new ui2Image("basicLayout2_bluebars11", "uibluebarsb11", 0, 0, 0, nullptr);

        w->bluebars01->field_0x48 = 0;
        w->bluebars02->field_0x48 = 0;
        w->bluebars03->field_0x48 = 0;
        w->bluebars04->field_0x48 = 0;
        w->bluebars05->field_0x48 = 0;
        w->bluebars06->field_0x48 = 0;
        w->bluebars07->field_0x48 = 0;
        w->bluebars08->field_0x48 = 0;
        w->bluebars09->field_0x48 = 0;
        w->bluebars10->field_0x48 = 0;
        w->bluebars11->field_0x48 = 0;

        w->bluebarsTimer01 = new ui2Timer("basicLayout2_bluebars_timer_01", 0.00f, false, true, nullptr);
        w->bluebarsTimer02 = new ui2Timer("basicLayout2_bluebars_timer_02", 0.05f, false, true, nullptr);
        w->bluebarsTimer03 = new ui2Timer("basicLayout2_bluebars_timer_03", 0.10f, false, true, nullptr);
        w->bluebarsTimer04 = new ui2Timer("basicLayout2_bluebars_timer_04", 0.15f, false, true, nullptr);
        w->bluebarsTimer05 = new ui2Timer("basicLayout2_bluebars_timer_05", 0.20f, false, true, nullptr);
        w->bluebarsTimer06 = new ui2Timer("basicLayout2_bluebars_timer_06", 0.25f, false, true, nullptr);
        w->bluebarsTimer07 = new ui2Timer("basicLayout2_bluebars_timer_07", 0.30f, false, true, nullptr);
        w->bluebarsTimer08 = new ui2Timer("basicLayout2_bluebars_timer_08", 0.35f, false, true, nullptr);
        w->bluebarsTimer09 = new ui2Timer("basicLayout2_bluebars_timer_09", 0.40f, false, true, nullptr);
        w->bluebarsTimer10 = new ui2Timer("basicLayout2_bluebars_timer_10", 0.45f, false, true, nullptr);
        w->bluebarsTimer11 = new ui2Timer("basicLayout2_bluebars_timer_11", 0.50f, false, true, nullptr);

        w->bluebarsTimer01->addListener(w->bluebars01, WidgetEventType::TimerFinished);
        w->bluebarsTimer02->addListener(w->bluebars02, WidgetEventType::TimerFinished);
        w->bluebarsTimer03->addListener(w->bluebars03, WidgetEventType::TimerFinished);
        w->bluebarsTimer04->addListener(w->bluebars04, WidgetEventType::TimerFinished);
        w->bluebarsTimer05->addListener(w->bluebars05, WidgetEventType::TimerFinished);
        w->bluebarsTimer06->addListener(w->bluebars06, WidgetEventType::TimerFinished);
        w->bluebarsTimer07->addListener(w->bluebars07, WidgetEventType::TimerFinished);
        w->bluebarsTimer08->addListener(w->bluebars08, WidgetEventType::TimerFinished);
        w->bluebarsTimer09->addListener(w->bluebars09, WidgetEventType::TimerFinished);
        w->bluebarsTimer10->addListener(w->bluebars10, WidgetEventType::TimerFinished);
        w->bluebarsTimer11->addListener(w->bluebars11, WidgetEventType::TimerFinished);

        w->bluebars01->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::Show, nullptr);
        w->bluebars02->copyEventConfig(w->bluebars01, EventConfig::CopyMappers);
        w->bluebars03->copyEventConfig(w->bluebars01, EventConfig::CopyMappers);
        w->bluebars04->copyEventConfig(w->bluebars01, EventConfig::CopyMappers);
        w->bluebars05->copyEventConfig(w->bluebars01, EventConfig::CopyMappers);
        w->bluebars06->copyEventConfig(w->bluebars01, EventConfig::CopyMappers);
        w->bluebars07->copyEventConfig(w->bluebars01, EventConfig::CopyMappers);
        w->bluebars08->copyEventConfig(w->bluebars01, EventConfig::CopyMappers);
        w->bluebars09->copyEventConfig(w->bluebars01, EventConfig::CopyMappers);
        w->bluebars10->copyEventConfig(w->bluebars01, EventConfig::CopyMappers);
        w->bluebars11->copyEventConfig(w->bluebars01, EventConfig::CopyMappers);

        w->bluebarsTable = new ui2Table("basicLayout2_bluebars_table", 5, 3, 0, 50, nullptr);
        w->bluebarsTable->setColSizes(128, -1, -2);
        w->bluebarsTable->setRowSizes(128, -1, -2);
        w->bluebarsTable->FUN_001fc9f0(0, 0, { *w->bluebars01, *w->bluebars02, *w->bluebars03, *w->bluebars04, *w->bluebars05, nullptr });
        w->bluebarsTable->FUN_001fcae0(4, 1, { *w->bluebars06, *w->bluebars07, nullptr });
        w->bluebarsTable->FUN_001fc9f0(0, 2, { *w->bluebars11, *w->bluebars10, *w->bluebars09, *w->bluebars08, nullptr });

        w->selectBtn = new ui2Image("basicLayout2_select_button", "uibuttonx", 53, 379, -1, nullptr);
        w->backBtn = new ui2Image("basicLayout2_back_button", "uibuttontriangle", 556, 379, -1, nullptr);
        w->selectText = new ui2TranslatedText("basicLayout2_BasicLayout_Select_txt", "BasicLayout_Select", 86, 395, -1, nullptr);
        w->backText = new ui2TranslatedText("basicLayout2_BasicLayout_Back_txt", "BasicLayout_Back", 405, 395, -1, nullptr);

        w->backText->setBounds(150, 50);
        w->backText->setAlignment(2);
        w->selectText->setColorU32(0x80969696);
        w->backText->setColorU32(0x80969696);

        w->selectBtnTimer = new ui2Timer("basicLayout2_select_button_timer", 0.6f, false, false, nullptr);
        w->backBtnTimer = new ui2Timer("basicLayout2_back_button_timer", 0.45f, false, false, nullptr);

        w->selectBtnTimer->addListener(w->selectBtn, WidgetEventType::TimerFinished);
        w->selectBtnTimer->addListener(w->selectText, WidgetEventType::TimerFinished);
        w->backBtnTimer->addListener(w->backBtn, WidgetEventType::TimerFinished);
        w->backBtnTimer->addListener(w->backText, WidgetEventType::TimerFinished);

        w->selectBtn->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::Show, nullptr);
        w->backBtn->copyEventConfig(w->selectBtn, EventConfig::CopyMappers);
        w->selectText->copyEventConfig(w->selectBtn, EventConfig::CopyMappers);
        w->backText->copyEventConfig(w->selectBtn, EventConfig::CopyMappers);

        w->clockTop = new srui2Clock("basicLayout2_clock_top", 475, 92, 0, nullptr);
        // w->clockTop->text->method_0x170("uiclock");
        // w->clockTop->text->setColorU32(0x80969696);
        // w->clockTop->text->setAdvanceX(10);

        if (w->clockTop->updateTime()) w->clockTop->setTime(13, 57, 0, 0);

        w->clockTopTimer = new ui2Timer("basicLayout2_clock_top_timer", 0.15f, false, true, nullptr);
        w->clockTopTimer->addListener(w->clockTop, WidgetEventType::TimerFinished);
        w->clockTop->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::Show, nullptr);

        w->showUITimer = new ui2Timer("basicLayout2_show_ui_timer", 0.75f, false, true, nullptr);
        gm->popMaster();
        
        if (ActiveCount == 0) w->master->setActive(false);
    }
};