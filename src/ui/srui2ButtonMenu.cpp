#include <libsr2/ui/srui2ButtonMenu.h>
#include <libsr2/ui/ui2Base.h>

#include <string.h>
#include <stdio.h>

namespace sr2 {
    srui2ButtonMenu::srui2ButtonMenu(
        const char *name,
        i32 buttonCount,
        const char *p3,
        const char *p4,
        i32 p5,
        i32 x,
        i32 y,
        const char *p8,
        i32 p9,
        const WidgetRef<ui2Master>& master
    ) : ui2Widget(name, master, true) {
        field_0x7c = buttonCount;
        m_buttonCount = buttonCount;
        field_0x158 = p5;
        m_pos = { x, y };
        m_lineSpacing = 0;
        field_0x15c = nullptr;
        field_0x164 = 0;
        field_0x160 = p9;
        field_0x168 = 1;

        init(p3, p4, x, y, p8);
    }

    srui2ButtonMenu::~srui2ButtonMenu() {
    }

    void srui2ButtonMenu::init(const char* p1, const char* p2, i32 x, i32 y, const char* p5) {
        ui::BaseRef nullRef;

        u32 bufLen = strlen(p1) + 20;
        char* nameBuf0 = new char[bufLen];
        char* nameBuf1 = new char[bufLen];

        field_0x138.set(p1);
        field_0x148.set(p2);
        field_0x110.set(p5);
        
        field_0x120 = new ui2Color();
        field_0x120->color = 0x806cb4e1;

        field_0x128 = new ui2Color();
        field_0x128->color = 0x80969696;

        field_0x130 = new ui2Color();
        field_0x130->color = 0x8011551d;

        WidgetRef<ui2Master> master = getMaster();
        snprintf(nameBuf0, bufLen, "%s_menu", p1);
        m_menu = new ui2Menu(nameBuf0, m_buttonCount, 1, master);

        if (p5) {
            m_menu->addListener(p5, WidgetEventType::UNK29, &ui2Widget::method_0x38);
            m_menu->addListener(p5, WidgetEventType::UNK26, &ui2Widget::method_0x38);
            auto var = ui2Base::getGlobalMaster()->findWidget(p5, "ui2Variable").cast<ui2Widget>();
            var->addListener(getName(), WidgetEventType::UNK39, (SomeWidgetCallback)&srui2ButtonMenu::FUN_001ebac8);
            var->addListener(getName(), WidgetEventType::UNK40, (SomeWidgetCallback)&srui2ButtonMenu::FUN_001ebac8);
        }
        
        snprintf(nameBuf0, bufLen, "%s_Menu_rep", p1);
        m_menuRepeater = new ui2Repeater(nameBuf0, nullptr);
        m_menuRepeater->addListener(m_menu, WidgetEventType::UNK12, &ui2Widget::method_0x38);

        if (field_0x168) {
            snprintf(nameBuf0, bufLen, "%s_Cursor_img", p1);
            m_cursorImg = new ui2Image(nameBuf0, "uichevron", 0, 0, 0, nullptr);
            m_cursorImg->FUN_001f5db0(2);
            m_cursorImg->method_0xb0(WidgetEventType::TimerFinished, WidgetEventType::UNK8, nullRef);
            
            snprintf(nameBuf0, bufLen, "%s_Cursor_tmr", p1);
            m_cursorTimer = new ui2Timer(nameBuf0, 1.4f, 1, 1, nullptr);
            m_cursorTimer->addListener(m_menu, WidgetEventType::TimerFinished, &ui2Widget::method_0x38);

            snprintf(nameBuf0, bufLen, "%s_Cursor_l_snd", p1);
            m_cursorLSound = new ui2Sound(nameBuf0, 49, nullptr);
            m_cursorLSound->FUN_00208068(-1.0f);
            m_cursorLSound->method_0xb0(WidgetEventType::TimerFinished, WidgetEventType::UNK33, nullRef);
            m_cursorTimer->addListener(m_cursorLSound, WidgetEventType::TimerFinished, &ui2Widget::method_0x38);
            m_cursorLSound->FUN_00207fc8();
            m_cursorLSound->FUN_00207fe0(0.45f);

            snprintf(nameBuf0, bufLen, "%s_Cursor_r_snd", p1);
            m_cursorRSound = new ui2Sound(nameBuf0, 50, nullptr);
            m_cursorRSound->FUN_00208068(1.0f);
            m_cursorRSound->method_0xb0(WidgetEventType::TimerFinished, WidgetEventType::UNK33, nullRef);
            m_cursorTimer->addListener(m_cursorRSound, WidgetEventType::TimerFinished, &ui2Widget::method_0x38);
            m_cursorRSound->FUN_00207fc8();
            m_cursorRSound->FUN_00207fe0(0.45f);

            snprintf(nameBuf0, bufLen, "%s_Heading_txt", p1);
            snprintf(nameBuf1, bufLen, "%s_Heading", p2);
            m_headingText = new ui2TranslatedText(nameBuf0, nameBuf1, 0, 0, 0, nullptr);
            m_headingText->setColorU32(0x806cb4e1);
            m_headingText->field_0x48 = 0;
        }

        snprintf(nameBuf0, bufLen, "%s_left_Arrow_img", p1);
        m_leftArrowImg = new ui2Image(nameBuf0, "uisidearrowsleft", 0, 0, 0, nullptr);

        snprintf(nameBuf0, bufLen, "%s_left_Arrow_tmr", p1);
        m_leftArrowTimer = new ui2Timer(nameBuf0, 0.2f, 0, 0, nullptr);
        m_leftArrowTimer->addListener(m_leftArrowImg, WidgetEventType::TimerFinished, &ui2Widget::method_0x38);
        m_leftArrowImg->method_0xb0(WidgetEventType::TimerFinished, WidgetEventType::SetColor, field_0x130);
        m_leftArrowImg->setColor(field_0x130);
        m_leftArrowImg->field_0x48 = 0;

        snprintf(nameBuf0, bufLen, "%s_right_Arrow_img", p1);
        m_rightArrowImg = new ui2Image(nameBuf0, "uisidearrowrigt", 0, 0, 0, nullptr);

        snprintf(nameBuf0, bufLen, "%s_right_Arrow_tmr", p1);
        m_rightArrowTimer = new ui2Timer(nameBuf0, 0.2f, 0, 0, nullptr);
        m_rightArrowTimer->addListener(m_rightArrowImg, WidgetEventType::TimerFinished, &ui2Widget::method_0x38);
        m_rightArrowImg->method_0xb0(WidgetEventType::TimerFinished, WidgetEventType::SetColor, field_0x130);
        m_rightArrowImg->setColor(field_0x130);
        m_rightArrowImg->field_0x48 = 0;

        snprintf(nameBuf0, bufLen, "%s_sel_l_snd", p1);
        m_selectLSound = new ui2Sound(nameBuf0, 62, nullptr);
        m_selectLSound->FUN_00208068(-1.0f);
        m_selectLSound->FUN_00207fc8();

        snprintf(nameBuf0, bufLen, "%s_sel_r_snd", p1);
        m_selectRSound = new ui2Sound(nameBuf0, 63, nullptr);
        m_selectRSound->FUN_00208068(1.0f);
        m_selectRSound->FUN_00207fc8();
        
        snprintf(nameBuf0, bufLen, "%s_Menu_Items_tbl", p1);
        m_menuItemsTbl = new ui2Table(nameBuf0, m_buttonCount, 3, 0, 0, getMaster());
        m_menuItemsTbl->FUN_001fd0a8(0, 0, 0, 0);
        m_menuItemsTbl->setRowSize(0, 0);
        m_menuItemsTbl->setRowSize(29, 1);
        m_menuItemsTbl->setRowSize(0, 2);
        m_menu->addListener(m_menuItemsTbl, WidgetEventType::UNK29, &ui2Widget::method_0x38);

        if (field_0x158 == 0) {
            m_menuItemsTbl->setColSizes(80, -1, -2);
            field_0x15c = "uitextbutton";
            m_menuItemsTbl->setCellOffset(-9, 0, 1, 0);
        } else if (field_0x158 == 1) {
            m_menuItemsTbl->setColSizes(140, -1, -2);
            field_0x15c = "uitextlarge";
            m_menuItemsTbl->setCellOffset(-69, 0, 1, 0);
        }

        snprintf(nameBuf0, bufLen, "%s_all_tbl", p1);
        m_allTbl = new ui2Table(nameBuf0, 3, 2, x, y, getMaster());
        m_allTbl->setColSizes(0, -1, -2);
        m_allTbl->setRowSize(m_lineSpacing, 0);

        if (field_0x160) {
            m_allTbl->FUN_001fc6f8(m_cursorImg, 0, 0, &ui2Widget::method_0x38);
            m_allTbl->FUN_001fc6f8(m_headingText, 1, 0, &ui2Widget::method_0x38);
        }

        m_allTbl->FUN_001fc6f8(m_menuItemsTbl, 0, 1, &ui2Widget::method_0x38);
        m_allTbl->FUN_001fc6f8(m_leftArrowImg, 1, 1, &ui2Widget::method_0x38);
        m_allTbl->FUN_001fc6f8(m_rightArrowImg, 2, 1, &ui2Widget::method_0x38);
        m_allTbl->setCellOffset(-30, 2, 0, 0);
        m_allTbl->setCellOffset(40, 0, 1, 0);

        m_someWidgetArr0 = new ui::BaseRef[m_buttonCount];
        m_someWidgetArr1 = new ui::BaseRef[m_buttonCount];
        m_someWidgetArr2 = new ui::BaseRef[m_buttonCount];
        m_someWidgetArr3 = new ui::BaseRef[m_buttonCount];
        m_someWidgetArr4 = new ui::BaseRef[m_buttonCount];
        m_someStringArr = new ui2String[m_buttonCount];

        delete [] nameBuf0;
        delete [] nameBuf1;

        m_menuItemsTbl->addListener(getName(), WidgetEventType::UNK30, (SomeWidgetCallback)&srui2ButtonMenu::FUN_001ebac8);
        m_menu->addListener(getName(), WidgetEventType::UNK12, (SomeWidgetCallback)&srui2ButtonMenu::FUN_001ebac8);
    }

    void srui2ButtonMenu::FUN_001ebac8(const ui::NamedRef& p1, WidgetEventType p2, const WidgetRef<ui2EventData>& p3) {
    }
};