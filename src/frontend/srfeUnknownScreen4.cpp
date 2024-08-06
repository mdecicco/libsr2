#include <libsr2/frontend/srfeUnknownScreen4.h>
#include <libsr2/ui/ui2Base.h>

namespace sr2 {
    u32 srfeUnknownScreen4::ActiveCount = 0;
    u32 srfeUnknownScreen4::InitializedCount = 0;
    srfeUnknownScreen4::Widgets* srfeUnknownScreen4::WidgetsInstance = nullptr;
    srfeUnknownScreen4::srfeUnknownScreen4(const char* name, const WidgetRef<ui2Master>& master) : srfeUnknownScreen1(name, master) {
        m_wasUninitialized = false;

        if (m_isActive) ActiveCount++;
        if (WidgetsInstance) WidgetsInstance->master->setActive(true);
    }

    srfeUnknownScreen4::~srfeUnknownScreen4() {
        deinitScreen();
        if (m_isActive) ActiveCount--;
    }

    void srfeUnknownScreen4::setActive(bool p1) {
        bool wasActive = m_isActive;
        srfeUnknownScreen1::setActive(p1);

        if (wasActive != p1) {
            if (p1) {
                ActiveCount++;
                if (WidgetsInstance) {
                    if (ActiveCount == 1) WidgetsInstance->master->setActive(true);
                    method_0x108();
                }
            } else {
                ActiveCount--;
                if (WidgetsInstance && ActiveCount == 0) WidgetsInstance->master->setActive(false);
            }
        }
    }

    void srfeUnknownScreen4::initScreen() {
        srfeUnknownScreen1::initScreen();
        InitializedCount++;

        if (!WidgetsInstance) initWidgets();

        WidgetsInstance->show_ui_timer->addListener(this, WidgetEventType::TimerFinished, (WidgetEventCallback)&srfeUnknownScreen4::interceptEvent);
    }

    void srfeUnknownScreen4::deinitScreen() {
        srui2SRScreen::deinitScreen();

        if (m_wasUninitialized) return;
        m_wasUninitialized = true;

        if (WidgetsInstance) {
            WidgetsInstance->show_ui_timer->removeListener(m_widgetName->get(), WidgetEventType::TimerFinished);
        }

        InitializedCount--;

        if (InitializedCount == 0) {
            delete WidgetsInstance;
            WidgetsInstance = nullptr;
        }
    }

    void srfeUnknownScreen4::method_0x108() {
        srfeUnknownScreen1::method_0x108();
        
        m_input->setActive(false);
        method_0x128();

        Widgets* w = WidgetsInstance;
        w->bluebars_timer_01->startTimer();
        w->bluebars_timer_02->startTimer();
        w->bluebars_timer_03->startTimer();
        w->bluebars_timer_04->startTimer();
        w->bluebars_timer_05->startTimer();
        w->bluebars_timer_06->startTimer();
        w->bluebars_timer_07->startTimer();
        w->bluebars_timer_08->startTimer();
        w->bluebars_timer_09->startTimer();
        w->bluebars_timer_10->startTimer();
        w->bluebars_timer_11->startTimer();

        if (method_0x138()) {
            w->select_button_timer->startTimer();
        }

        if (method_0x130()) {
            w->back_button_timer->startTimer();
        }
     
        w->side_anim_begin_top_timer->startTimer();
        w->side_anim_begin_middle_timer->startTimer();
        w->side_anim_begin_bottom_timer->startTimer();
        w->clock_top_timer->startTimer();
        w->show_ui_timer->startTimer();
    }

    void srfeUnknownScreen4::method_0x110() {
        srfeUnknownScreen1::method_0x110();
        m_input->setActive(true);
    }
    
    void srfeUnknownScreen4::method_0x128() {
        Widgets* w = WidgetsInstance;
        w->bluebars_01->setVisibility(0);
        w->bluebars_02->setVisibility(0);
        w->bluebars_03->setVisibility(0);
        w->bluebars_04->setVisibility(0);
        w->bluebars_05->setVisibility(0);
        w->bluebars_06->setVisibility(0);
        w->bluebars_07->setVisibility(0);
        w->bluebars_08->setVisibility(0);
        w->bluebars_09->setVisibility(0);
        w->bluebars_10->setVisibility(0);
        w->bluebars_11->setVisibility(0);

        w->side_anim_begin_top->FUN_001f5db0(3);
        w->side_anim_begin_middle->FUN_001f5db0(3);
        w->side_anim_begin_bottom->FUN_001f5db0(3);
        w->side_anim_loop_top->setVisibility(0);
        w->side_anim_loop_middle->setVisibility(0);
        w->side_anim_loop_bottom->setVisibility(0);
        w->clock_top->setVisibility(0);

        w->side_anim_begin_top->FUN_001f5db0(3);
        w->side_anim_begin_middle->FUN_001f5db0(3);
        w->side_anim_begin_bottom->FUN_001f5db0(3);
    }
    
    bool srfeUnknownScreen4::method_0x130() {
        return true;
    }
    
    bool srfeUnknownScreen4::method_0x138() {
        return true;
    }

    void srfeUnknownScreen4::interceptEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive) return;
        method_0x110();
    }

    void srfeUnknownScreen4::initWidgets() {
        WidgetsInstance = new Widgets();
        Widgets* w = WidgetsInstance;

        auto gm = ui2Base::getGlobalMaster();
        w->master = new ui2Master("basicLayout1_Master", ui2Base::getMaster(), true);
        gm->pushMaster(w->master);

        w->master->setRenderPriority(5010);

        w->bluebars_01 = new ui2Image("basicLayout1_bluebars01", "uibluebars01", 0, 0, 0, nullptr);
        w->bluebars_02 = new ui2Image("basicLayout1_bluebars02", "uibluebars02", 0, 0, 0, nullptr);
        w->bluebars_03 = new ui2Image("basicLayout1_bluebars03", "uibluebars03", 0, 0, 0, nullptr);
        w->bluebars_04 = new ui2Image("basicLayout1_bluebars04", "uibluebars04", 0, 0, 0, nullptr);
        w->bluebars_05 = new ui2Image("basicLayout1_bluebars05", "uibluebars05", 0, 0, 0, nullptr);
        w->bluebars_06 = new ui2Image("basicLayout1_bluebars06", "uibluebars06", 0, 0, 0, nullptr);
        w->bluebars_07 = new ui2Image("basicLayout1_bluebars07", "uibluebars07", 0, 0, 0, nullptr);
        w->bluebars_08 = new ui2Image("basicLayout1_bluebars08", "uibluebars08", 0, 0, 0, nullptr);
        w->bluebars_09 = new ui2Image("basicLayout1_bluebars09", "uibluebars09", 0, 0, 0, nullptr);
        w->bluebars_10 = new ui2Image("basicLayout1_bluebars10", "uibluebars10", 0, 0, 0, nullptr);
        w->bluebars_11 = new ui2Image("basicLayout1_bluebars11", "uibluebars11", 0, 0, 0, nullptr);
        w->bluebars_01->field_0x48 = 0;
        w->bluebars_02->field_0x48 = 0;
        w->bluebars_03->field_0x48 = 0;
        w->bluebars_04->field_0x48 = 0;
        w->bluebars_05->field_0x48 = 0;
        w->bluebars_06->field_0x48 = 0;
        w->bluebars_07->field_0x48 = 0;
        w->bluebars_08->field_0x48 = 0;
        w->bluebars_09->field_0x48 = 0;
        w->bluebars_10->field_0x48 = 0;
        w->bluebars_11->field_0x48 = 0;

        w->bluebars_timer_01 = new ui2Timer("basicLayout1_bluebars_timer_01", 0.00f, 0, 1, nullptr);
        w->bluebars_timer_02 = new ui2Timer("basicLayout1_bluebars_timer_02", 0.05f, 0, 1, nullptr);
        w->bluebars_timer_03 = new ui2Timer("basicLayout1_bluebars_timer_03", 0.10f, 0, 1, nullptr);
        w->bluebars_timer_04 = new ui2Timer("basicLayout1_bluebars_timer_04", 0.15f, 0, 1, nullptr);
        w->bluebars_timer_05 = new ui2Timer("basicLayout1_bluebars_timer_05", 0.20f, 0, 1, nullptr);
        w->bluebars_timer_06 = new ui2Timer("basicLayout1_bluebars_timer_06", 0.25f, 0, 1, nullptr);
        w->bluebars_timer_07 = new ui2Timer("basicLayout1_bluebars_timer_07", 0.30f, 0, 1, nullptr);
        w->bluebars_timer_08 = new ui2Timer("basicLayout1_bluebars_timer_08", 0.35f, 0, 1, nullptr);
        w->bluebars_timer_09 = new ui2Timer("basicLayout1_bluebars_timer_09", 0.40f, 0, 1, nullptr);
        w->bluebars_timer_10 = new ui2Timer("basicLayout1_bluebars_timer_10", 0.45f, 0, 1, nullptr);
        w->bluebars_timer_11 = new ui2Timer("basicLayout1_bluebars_timer_11", 0.50f, 0, 1, nullptr);

        w->bluebars_timer_01->addListener(w->bluebars_01, WidgetEventType::TimerFinished);
        w->bluebars_timer_02->addListener(w->bluebars_02, WidgetEventType::TimerFinished);
        w->bluebars_timer_03->addListener(w->bluebars_03, WidgetEventType::TimerFinished);
        w->bluebars_timer_04->addListener(w->bluebars_04, WidgetEventType::TimerFinished);
        w->bluebars_timer_05->addListener(w->bluebars_05, WidgetEventType::TimerFinished);
        w->bluebars_timer_06->addListener(w->bluebars_06, WidgetEventType::TimerFinished);
        w->bluebars_timer_07->addListener(w->bluebars_07, WidgetEventType::TimerFinished);
        w->bluebars_timer_08->addListener(w->bluebars_08, WidgetEventType::TimerFinished);
        w->bluebars_timer_09->addListener(w->bluebars_09, WidgetEventType::TimerFinished);
        w->bluebars_timer_10->addListener(w->bluebars_10, WidgetEventType::TimerFinished);
        w->bluebars_timer_11->addListener(w->bluebars_11, WidgetEventType::TimerFinished);
        w->bluebars_01->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::Show, nullptr);

        w->bluebars_02->FUN_0020ac08(w->bluebars_01, 2);
        w->bluebars_03->FUN_0020ac08(w->bluebars_01, 2);
        w->bluebars_04->FUN_0020ac08(w->bluebars_01, 2);
        w->bluebars_05->FUN_0020ac08(w->bluebars_01, 2);
        w->bluebars_06->FUN_0020ac08(w->bluebars_01, 2);
        w->bluebars_07->FUN_0020ac08(w->bluebars_01, 2);
        w->bluebars_08->FUN_0020ac08(w->bluebars_01, 2);
        w->bluebars_09->FUN_0020ac08(w->bluebars_01, 2);
        w->bluebars_10->FUN_0020ac08(w->bluebars_01, 2);
        w->bluebars_11->FUN_0020ac08(w->bluebars_01, 2);

        w->bluebars_table = new ui2Table("basicLayout1_bluebars_table", 5, 3, 0, 50, nullptr);
        w->bluebars_table->setColSizes(128, -1, -2);
        w->bluebars_table->setRowSizes(128, -1, -2);
        w->bluebars_table->FUN_001fc9f0(0, 0, { *w->bluebars_01, *w->bluebars_02, *w->bluebars_03, *w->bluebars_04, *w->bluebars_05, nullptr });
        w->bluebars_table->FUN_001fcae0(4, 1, { *w->bluebars_06, *w->bluebars_07, nullptr });
        w->bluebars_table->FUN_001fc9f0(0, 2, { *w->bluebars_11, *w->bluebars_10, *w->bluebars_09, *w->bluebars_08, nullptr });

        w->select_button = new ui2Image("basicLayout1_select_button", "uibuttonx", 53, 379, -1, nullptr);
        w->back_button = new ui2Image("basicLayout1_back_button", "uibuttontriangle", 556, 379, -1, nullptr);
        w->select_text = new ui2TranslatedText("basicLayout1_BasicLayout_Select_txt", "BasicLayout_Select", 86, 395, -1, nullptr);
        w->back_text = new ui2TranslatedText("basicLayout1_BasicLayout_Back_txt", "BasicLayout_Back", 405, 395, -1, nullptr);

        w->back_text->setBounds(150, 50);
        w->back_text->setAlignment(2);
        w->select_text->setColorU32(0x80969696);
        w->back_text->setColorU32(0x80969696);

        w->select_button_timer = new ui2Timer("basicLayout1_select_button_timer", 0.6f, 0, 0, nullptr);
        w->back_button_timer = new ui2Timer("basicLayout1_back_button_timer", 0.45f, 0, 0, nullptr);

        w->select_button_timer->addListener(w->select_button, WidgetEventType::TimerFinished);
        w->select_button_timer->addListener(w->select_text, WidgetEventType::TimerFinished);

        w->back_button_timer->addListener(w->back_button, WidgetEventType::TimerFinished);
        w->back_button_timer->addListener(w->back_text, WidgetEventType::TimerFinished);

        w->select_button->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::Show, nullptr);
        w->back_button->FUN_0020ac08(w->select_button, 2);
        w->select_text->FUN_0020ac08(w->select_button, 2);
        w->back_text->FUN_0020ac08(w->select_button, 2);

        w->side_anim_begin_top = new ui2Image("basicLayout1_side_anim_begin_top", "uibuggyzoom", 0x223, 0x72, 10, nullptr);
        w->side_anim_begin_middle = new ui2Image("basicLayout1_side_anim_begin_middle", "uibuggyzoom", 0x223, 0xab, 10, nullptr);
        w->side_anim_begin_bottom = new ui2Image("basicLayout1_side_anim_begin_bottom", "uibuggyzoom", 0x223, 0xe4, 10, nullptr);
        
        w->side_anim_begin_top_timer = new ui2Timer("basicLayout1_side_anim_begin_top_timer", 0.55, 0, 1, nullptr);
        w->side_anim_begin_middle_timer = new ui2Timer("basicLayout1_side_anim_begin_middle_timer", 1.05, 0, 1, nullptr);
        w->side_anim_begin_bottom_timer = new ui2Timer("basicLayout1_side_anim_begin_bottom_timer", 1.55, 0, 1, nullptr);

        w->side_anim_begin_top->setFrameRate(15.0f);
        w->side_anim_begin_middle->setFrameRate(15.0f);
        w->side_anim_begin_bottom->setFrameRate(15.0f);

        w->side_anim_loop_top = new ui2Image("basicLayout1_side_anim_loop_top", "uibuggyturn", 0x223, 0x72, 10, nullptr);
        w->side_anim_loop_middle = new ui2Image("basicLayout1_side_anim_loop_middle", "uibuggyturn", 0x223, 0xab, 10, nullptr);
        w->side_anim_loop_bottom = new ui2Image("basicLayout1_side_anim_loop_bottom", "uibuggyturn", 0x223, 0xe4, 10, nullptr);

        w->side_anim_loop_top->setFrameRate(15.0f);
        w->side_anim_loop_middle->setFrameRate(15.0f);
        w->side_anim_loop_bottom->setFrameRate(15.0f);

        // show and start the loop animation when this one ends
        w->side_anim_begin_top->addListener(w->side_anim_loop_top, WidgetEventType::AnimationEnded);
        w->side_anim_loop_top->addEventMapper(WidgetEventType::AnimationEnded, WidgetEventType::UNK8, nullptr);
        w->side_anim_loop_top->addEventMapper(WidgetEventType::AnimationEnded, WidgetEventType::Show, nullptr);

        // hide this animation when it ends
        w->side_anim_begin_top->addListener(w->side_anim_begin_top, WidgetEventType::AnimationEnded);
        w->side_anim_begin_top->addEventMapper(WidgetEventType::AnimationEnded, WidgetEventType::Hide, nullptr);


        // show and start the loop animation when this one ends
        w->side_anim_begin_middle->addListener(w->side_anim_loop_middle, WidgetEventType::AnimationEnded);
        w->side_anim_loop_middle->addEventMapper(WidgetEventType::AnimationEnded, WidgetEventType::UNK8, nullptr);
        w->side_anim_loop_middle->addEventMapper(WidgetEventType::AnimationEnded, WidgetEventType::Show, nullptr);

        // hide this animation when it ends
        w->side_anim_begin_middle->addListener(w->side_anim_begin_middle, WidgetEventType::AnimationEnded);
        w->side_anim_begin_middle->addEventMapper(WidgetEventType::AnimationEnded, WidgetEventType::Hide, nullptr);


        // show and start the loop animation when this one ends
        w->side_anim_begin_bottom->addListener(w->side_anim_loop_bottom, WidgetEventType::AnimationEnded);
        w->side_anim_loop_bottom->addEventMapper(WidgetEventType::AnimationEnded, WidgetEventType::UNK8, nullptr);
        w->side_anim_loop_bottom->addEventMapper(WidgetEventType::AnimationEnded, WidgetEventType::Show, nullptr);
        
        // hide this animation when it ends
        w->side_anim_begin_bottom->addListener(w->side_anim_begin_bottom, WidgetEventType::AnimationEnded);
        w->side_anim_begin_bottom->addEventMapper(WidgetEventType::AnimationEnded, WidgetEventType::Hide, nullptr);



        w->side_anim_begin_top_timer->addListener(w->side_anim_begin_top, WidgetEventType::TimerFinished);
        w->side_anim_begin_top->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::Show, nullptr);
        w->side_anim_begin_top->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK8, nullptr);

        w->side_anim_begin_middle_timer->addListener(w->side_anim_begin_middle, WidgetEventType::TimerFinished);
        w->side_anim_begin_middle->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::Show, nullptr);
        w->side_anim_begin_middle->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK8, nullptr);

        w->side_anim_begin_bottom_timer->addListener(w->side_anim_begin_bottom, WidgetEventType::TimerFinished);
        w->side_anim_begin_bottom->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::Show, nullptr);
        w->side_anim_begin_bottom->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK8, nullptr);

        w->side_anim_l_snd = new ui2Sound("basicLayout1_side_anim_l_snd", 0x2d, nullptr);
        w->side_anim_l_snd->FUN_00208068(-1.0f);
        w->side_anim_l_snd->FUN_002080b8(2);
        w->side_anim_l_snd->FUN_00207fe0(0.5f);
        w->side_anim_l_snd->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK33, nullptr);
        w->side_anim_begin_top_timer->addListener(w->side_anim_l_snd, WidgetEventType::TimerFinished);
        w->side_anim_begin_middle_timer->addListener(w->side_anim_l_snd, WidgetEventType::TimerFinished);
        w->side_anim_begin_bottom_timer->addListener(w->side_anim_l_snd, WidgetEventType::TimerFinished);

        w->side_anim_r_snd = new ui2Sound("basicLayout1_side_anim_r_snd", 0x2e, nullptr);
        w->side_anim_r_snd->FUN_00208068(1.0f);
        w->side_anim_r_snd->FUN_002080b8(2);
        w->side_anim_r_snd->FUN_00207fe0(0.5f);
        w->side_anim_r_snd->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK33, nullptr);
        w->side_anim_begin_top_timer->addListener(w->side_anim_r_snd, WidgetEventType::TimerFinished);
        w->side_anim_begin_middle_timer->addListener(w->side_anim_r_snd, WidgetEventType::TimerFinished);
        w->side_anim_begin_bottom_timer->addListener(w->side_anim_r_snd, WidgetEventType::TimerFinished);

        w->clock_top = new srui2Clock("basicLayout1_clock_top", 0x1db, 0x5c, 0, nullptr);
        // todo:
        // w->clock_top->method_0x170();
        w->clock_top->setColorU32(0x80969696);
        // w->clock_top->method_0x140();
        // if (w->clock_top->FUN_001e26c8()) w->clock_top->FUN_001e2630(0xd, 0x39, 0, 0);

        w->clock_top_timer = new ui2Timer("basicLayout1_clock_top_timer", 0.15, 0, 1, nullptr);
        w->clock_top_timer->addListener(w->clock_top, WidgetEventType::TimerFinished);
        w->clock_top->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::Show, nullptr);

        w->show_ui_timer = new ui2Timer("basicLayout1_show_ui_timer", 0.75, 0, 1, nullptr);

        gm->popMaster();

        if (ActiveCount == 0) w->master->setActive(false);

        // that was a nightmare
    }
};