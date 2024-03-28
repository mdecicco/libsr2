#include <libsr2/ui/srui2TextMenu.h>
#include <libsr2/ui/ui2Base.h>
#include <libsr2/ui/ui2Variable.h>

#include <string.h>
#include <stdio.h>

namespace sr2 {
    srui2TextMenu::srui2TextMenu(
        char* name,
        u32 colCount,
        char* p3,
        i32 x,
        i32 y,
        char* p6,
        undefined4 p7,
        const WidgetRef<ui2Master>& master
    ) : ui2Widget(name, master, true) {
        field_0x7c = colCount;
        m_colCount = colCount;
        field_0xb8 = 0;
        field_0xbc = 0;

        m_pos = { x, y };
        field_0x14c = 1;
        m_textSize = { 200, 203 };
        m_textOffset = { 314, 0 };
        m_upArrowOffset = { 200, 0 };
        m_downArrowOffset = { 200, 100 };
        m_showTextBackdrop = false;

        init(p3, x, y, p6, p7);
    }

    srui2TextMenu::~srui2TextMenu() {
    }

    void srui2TextMenu::init(char* p1, i32 x, i32 y, char* p4, undefined4 p5) {
        field_0x124.set(p1);
        field_0x134.set(p4);

        u32 bufLen = strlen(p1) + 20;
        char* nameBuf = new char[bufLen];

        field_0x48 = 1;
        field_0x144 = p5;

        ui::BaseRef nullRef;

        snprintf(nameBuf, bufLen, "%s_menu", p1);
        m_menu = new ui2Menu(nameBuf, 1, m_colCount, getMaster());

        snprintf(nameBuf, bufLen, "%s_Menu_rep", p1);
        m_menuRepeater = new ui2Repeater(nameBuf, nullptr);
        m_menuRepeater->method_0x68(*m_menu, SOME_WIDGET_ENUM::UNK12, 0x800080000);

        snprintf(nameBuf, bufLen, "%s_Cursor_tmr", p1);
        m_cursorTimer = new ui2Timer(nameBuf, 1.4f, 1, 1, nullptr);
        m_cursorTimer->method_0xb0(SOME_WIDGET_ENUM::UNK29, SOME_WIDGET_ENUM::UNK41, nullRef);
        m_menu->method_0x68(*m_cursorTimer, SOME_WIDGET_ENUM::UNK29, 0x800080000);

        snprintf(nameBuf, bufLen, "%s_Cursor_l_snd", p1);
        m_cursorLSound = new ui2Sound(nameBuf, 0x31, nullptr);
        m_cursorLSound->FUN_00208068(-1.0f);
        m_cursorLSound->method_0xb0(SOME_WIDGET_ENUM::UNK34, SOME_WIDGET_ENUM::UNK33, nullRef);
        m_cursorTimer->method_0x68(*m_cursorLSound, SOME_WIDGET_ENUM::UNK34, 0x800080000);
        m_cursorLSound->FUN_00207fc8();
        m_cursorLSound->FUN_00207fe0(0.45f);

        snprintf(nameBuf, bufLen, "%s_Cursor_r_snd", p1);
        m_cursorRSound = new ui2Sound(nameBuf, 0x32, nullptr);
        m_cursorRSound->FUN_00208068(1.0f);
        m_cursorRSound->method_0xb0(SOME_WIDGET_ENUM::UNK34, SOME_WIDGET_ENUM::UNK33, nullRef);
        m_cursorTimer->method_0x68(*m_cursorRSound, SOME_WIDGET_ENUM::UNK34, 0x800080000);
        m_cursorLSound->FUN_00207fc8();
        m_cursorLSound->FUN_00207fe0(0.45f);

        if (p4) {
            m_menu->method_0x70(p4, SOME_WIDGET_ENUM::UNK26, 0x800080000);
            auto var = ui2Base::getGlobalMaster()->findWidget(p4, "ui2Variable").cast<ui2Variable>();
            var->method_0x70(var->getName(), SOME_WIDGET_ENUM::UNK40, 0x1e6318ffff0000);
        }

        snprintf(nameBuf, bufLen, "%s_Menu_Items_tbl", p1);
        m_menuItemsTbl = new ui2Table(nameBuf, 2, m_colCount, 0, 0, getMaster());
        m_menuItemsTbl->field_0x48 = 0;
        m_menuItemsTbl->FUN_001fd0a8(0, 0, 0, 0);
        m_menuItemsTbl->setColSizes(30, -1, -2);
        m_menuItemsTbl->setColSize(64, 0);
        m_menuItemsTbl->setColSize(120, 1);
        m_menu->method_0x68(*m_menuItemsTbl, SOME_WIDGET_ENUM::UNK29, 0x800080000);

        if (field_0x144) {
            snprintf(nameBuf, bufLen, "%s_Desc_rep", p1);
            m_descRepeater = new ui2Repeater(nameBuf, getMaster());

            snprintf(nameBuf, bufLen, "%s_Desc_img", p1);
            m_descImg = new ui2Image(nameBuf, "uisquarered", 0, 0, 5, nullptr);
            m_descImg->FUN_001f5e18(210, 213);
            m_descImg->field_0x48 = 0;
        }

        field_0xe4 = new ui2Color();
        field_0xe4->color = 0x806cb4e1;

        field_0xec = new ui2Color();
        field_0xec->color = 0x8011551d;

        snprintf(nameBuf, bufLen, "%s_up_Arrow_img", p1);
        m_upArrowImg = new ui2Image(nameBuf, "uiUpArrow", 0, 0, 0, nullptr);

        snprintf(nameBuf, bufLen, "%s_up_Arrow_tmr", p1);
        m_upArrowTimer = new ui2Timer(nameBuf, 0.2f, 0, 0, nullptr);
        m_upArrowTimer->method_0x68(*m_upArrowImg, SOME_WIDGET_ENUM::UNK34, 0x800080000);
        m_upArrowImg->method_0xb0(SOME_WIDGET_ENUM::UNK34, SOME_WIDGET_ENUM::UNK4, field_0xec);
        m_upArrowImg->setColor(field_0xec);
        m_upArrowImg->field_0x48 = 0;
        m_upArrowImg->method_0x110(0);

        snprintf(nameBuf, bufLen, "%s_down_Arrow_img", p1);
        m_downArrowImg = new ui2Image(nameBuf, "uiDownArrow", 0, 0, 0, nullptr);

        snprintf(nameBuf, bufLen, "%s_down_Arrow_tmr", p1);
        m_downArrowTimer = new ui2Timer(nameBuf, 0.2f, 0, 0, nullptr);
        m_downArrowTimer->method_0x68(*m_downArrowImg, SOME_WIDGET_ENUM::UNK34, 0x800080000);
        m_downArrowImg->method_0xb0(SOME_WIDGET_ENUM::UNK34, SOME_WIDGET_ENUM::UNK4, field_0xec);
        m_downArrowImg->setColor(field_0xec);
        m_downArrowImg->field_0x48 = 0;
        m_downArrowImg->method_0x110(0);

        snprintf(nameBuf, bufLen, "%s_Select_l_snd", p1);
        m_selectLSound = new ui2Sound(nameBuf, 62, nullptr);
        m_selectLSound->FUN_00208068(-1.0f);
        m_selectLSound->method_0xb0(SOME_WIDGET_ENUM::UNK29, SOME_WIDGET_ENUM::UNK33, nullRef);
        m_selectLSound->method_0x68(m_selectLSound, SOME_WIDGET_ENUM::UNK29, SOME_WIDGET_ENUM::UNK33);
        m_selectLSound->FUN_00207fc8();

        snprintf(nameBuf, bufLen, "%s_Select_r_snd", p1);
        m_selectRSound = new ui2Sound(nameBuf, 63, nullptr);
        m_selectRSound->FUN_00208068(1.0f);
        m_selectRSound->method_0xb0(SOME_WIDGET_ENUM::UNK29, SOME_WIDGET_ENUM::UNK33, nullRef);
        m_selectRSound->method_0x68(m_selectRSound, SOME_WIDGET_ENUM::UNK29, SOME_WIDGET_ENUM::UNK33);
        m_selectRSound->FUN_00207fc8();

        snprintf(nameBuf, bufLen, "%s_all_tbl", p1);
        m_allTbl = new ui2Table(nameBuf, 7, 1, x, y, getMaster());
        m_allTbl->setColSizes(0, -1, -2);
        m_allTbl->setColSizes(0, -1, -2);
        m_allTbl->FUN_001fc6f8(m_menuItemsTbl, 0, 0, 0x800080000);
        m_allTbl->FUN_001fc6f8(m_upArrowImg, 1, 0, 0x800080000);
        m_allTbl->FUN_001fc6f8(m_downArrowImg, 2, 0, 0x800080000);
        m_allTbl->setCellOffset(200, 100, 2, 0);

        if (field_0x144) {
            m_allTbl->FUN_001fc6f8(m_descRepeater, 3, 0, 0x800080000);
            m_allTbl->setCellOffset(314, 0, 3, 0);

            m_allTbl->FUN_001fc6f8(m_descImg, 4, 0, 0x800080000);
            m_allTbl->setCellOffset(309, -5, 4, 0);
        }

        m_someArr = new undefined4[m_colCount];
        m_columns = new undefined4[m_colCount];

        m_someWidgetArr0 = new ui::NamedRef[m_colCount];
        m_someWidgetArr2 = new ui::NamedRef[m_colCount];
        m_someWidgetArr3 = new ui::NamedRef[m_colCount];
        m_someWidgetArr4 = new ui::NamedRef[m_colCount];

        for (u32 i = 0;i < m_colCount;i++) m_columns[i] = 0x1e;

        if (field_0x144) m_someWidgetArr1 = new ui::NamedRef[m_colCount];

        delete [] nameBuf;

        method_0x70(getName(), SOME_WIDGET_ENUM::UNK12, 0x1e6318ffff0000);
    }
};