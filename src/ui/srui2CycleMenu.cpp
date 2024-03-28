#include <libsr2/ui/srui2CycleMenu.h>
#include <libsr2/ui/ui2Base.h>

#include <string.h>
#include <stdio.h>

namespace sr2 {
    srui2CycleMenu::srui2CycleMenu(
        const char *name,
        i32 rowCount,
        const char *p3,
        const char *p4,
        const char* p5,
        i32 x,
        i32 y,
        const char* p8,
        const WidgetRef<ui2Master>& master
    ) : ui2Widget(name, master, true) {
        m_rowCount = rowCount;
        field_0x7c = rowCount;

        m_pos = { x, y };
        m_innerWidth = 100;
        m_headingOffset = 70;
        field_0x124 = 1;
        field_0x120 = 1;

        init(p3, p4, p5, x, y, p8);
    }

    srui2CycleMenu::~srui2CycleMenu() {
    }

    void srui2CycleMenu::init(const char* p1, const char* p2, const char* p3, i32 x, i32 y, const char* p6) {
        ui::BaseRef nullRef;

        u32 bufLen = strlen(p1) + 20;
        char* nameBuf0 = new char[bufLen];
        char* nameBuf1 = new char[bufLen];

        field_0x100.set(p1);
        field_0x110.set(p2);
        field_0xe0.set(p6);

        field_0xf0 = new ui2Color();
        field_0xf0->color = 0x806cb4e1;
        
        field_0xf8 = new ui2Color();
        field_0xf8->color = 0x8011551d;

        auto parent = getMaster();

        snprintf(nameBuf0, bufLen, "%s_menu", p1);
        m_menu = new ui2Menu(nameBuf0, m_rowCount, 1, parent);

        if (p6) {
            m_menu->method_0x70(p6, SOME_WIDGET_ENUM::UNK37, 0x800080000);
            m_menu->method_0x70(p6, SOME_WIDGET_ENUM::UNK26, 0x800080000);

            auto var = ui2Base::getGlobalMaster()->findWidget(p6, "ui2Variable");
            // var->method_0x70(getName(), SOME_WIDGET_ENUM::UNK39, 0x1efb30ffff0000);
            // var->method_0x70(getName(), SOME_WIDGET_ENUM::UNK40, 0x1efb30ffff0000);
        }

        snprintf(nameBuf0, bufLen, "%s_Menu_rep", p1);
        m_menuRepeater = new ui2Repeater(nameBuf0, nullptr);
        m_menuRepeater->method_0x68(m_menu, SOME_WIDGET_ENUM::UNK12, 0x800080000);

        if (p3) {
            snprintf(nameBuf0, bufLen, "%s_Heading_txt", p1);
            m_headingText = new ui2TranslatedText(nameBuf0, p3, 0, 0, 0, nullptr);
            m_headingText->setColorU32(0x8034ab40); // ???
            m_headingText->field_0x48 = 0;
        }

        snprintf(nameBuf0, bufLen, "%s_left_Arrow_img", p1);
        m_leftArrowImg = new ui2Image(nameBuf0, "uisideasm_left", 0, 0, 0, nullptr);

        snprintf(nameBuf0, bufLen, "%s_left_Arrow_tmr", p1);
        m_leftArrowTimer = new ui2Timer(nameBuf0, 0.2f, 0, 0, nullptr);
        m_leftArrowTimer->method_0x68(m_leftArrowImg, SOME_WIDGET_ENUM::UNK34, 0x800080000);
        m_leftArrowImg->method_0xb0(SOME_WIDGET_ENUM::UNK34, SOME_WIDGET_ENUM::UNK4, field_0xf8);
        m_leftArrowImg->setColor(field_0xf8);
        

        snprintf(nameBuf0, bufLen, "%s_right_Arrow_img", p1);
        m_rightArrowImg = new ui2Image(nameBuf0, "uisideasm_rigt", 0, 0, 0, nullptr);

        snprintf(nameBuf0, bufLen, "%s_right_Arrow_tmr", p1);
        m_rightArrowTimer = new ui2Timer(nameBuf0, 0.2f, 0, 0, nullptr);
        m_rightArrowTimer->method_0x68(m_rightArrowImg, SOME_WIDGET_ENUM::UNK34, 0x800080000);
        m_rightArrowImg->method_0xb0(SOME_WIDGET_ENUM::UNK34, SOME_WIDGET_ENUM::UNK4, field_0xf8);
        m_rightArrowImg->setColor(field_0xf8);

        snprintf(nameBuf0, bufLen, "%s_Menu_Items_tbl", p1);
        m_menuItemsTbl = new ui2Table(nameBuf0, m_rowCount + 1, 1, 0, 0, nullptr);
        m_menuItemsTbl->setColSizes(0, -1, -2);

        parent = getMaster();
        snprintf(nameBuf0, bufLen, "%s_all_tbl", p1);
        m_allTbl = new ui2Table(nameBuf0, 5, 1, x, y, parent);
        m_allTbl->setColSizes(0, -1, -2);

        if (p3) m_allTbl->FUN_001fc6f8(m_headingText, 0, 0, 0x800080000);
        m_allTbl->FUN_001fc6f8(m_leftArrowImg, 1, 0, 0x800080000);
        m_allTbl->FUN_001fc6f8(m_menuItemsTbl, 2, 0, 0x800080000);
        m_allTbl->FUN_001fc6f8(m_rightArrowImg, 3, 0, 0x800080000);                                                                                    
        m_allTbl->setColSize(m_headingOffset, 0);
        m_allTbl->setColSize(36, 1);
        m_allTbl->setColSize(m_innerWidth, 2);
        m_allTbl->setColSize(36, 3);
        m_allTbl->setCellOffset(0, 2, 2, 0);
        m_allTbl->field_0x48 = 0;

        snprintf(nameBuf0, bufLen, "%s_NotSelectable_txt", p1);
        snprintf(nameBuf1, bufLen, "%s_Locked", p2);
        m_notSelectableText = new ui2TranslatedText(nameBuf0, nameBuf1, 0, 0, 0, nullptr);
        i32 width = m_allTbl->getRowSize(2);
        m_notSelectableText->setBounds(width, 900);
        m_notSelectableText->setAlignment(0);
        m_menuItemsTbl->FUN_001fc6f8(m_notSelectableText, m_rowCount, 0, 0x800080000);
        m_menu->method_0x68(m_notSelectableText, SOME_WIDGET_ENUM::UNK29, 0x800080000);
        m_menu->method_0x68(m_notSelectableText, SOME_WIDGET_ENUM::UNK38, 0x800080000);
        m_notSelectableText->method_0xb0(SOME_WIDGET_ENUM::UNK29, SOME_WIDGET_ENUM::UNK7, nullRef);
        m_notSelectableText->method_0xb0(SOME_WIDGET_ENUM::UNK38, SOME_WIDGET_ENUM::UNK6, nullRef);

        m_someWidgetArr0 = new ui::BaseRef[m_rowCount];
        m_someWidgetArr1 = new ui::BaseRef[m_rowCount];
        m_someWidgetArr2 = new ui::BaseRef[m_rowCount];
        m_someStringArr = new ui2String[m_rowCount];

        delete [] nameBuf0;
        delete [] nameBuf1;

        m_menu->method_0x70(getName(), SOME_WIDGET_ENUM::UNK12, 0x1efb30ffff0000);
        m_allTbl->method_0x70(getName(), SOME_WIDGET_ENUM::UNK30, 0x1efb30ffff0000);
    }
};