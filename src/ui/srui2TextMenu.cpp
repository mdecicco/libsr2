#include <libsr2/ui/srui2TextMenu.h>
#include <libsr2/ui/ui2Base.h>
#include <libsr2/ui/ui2Variable.h>
#include <libsr2/ui/ui2DynamicText.h>
#include <libsr2/io/datParser.h>

#include <string.h>
#include <stdio.h>
#include <assert.h>

namespace sr2 {
    srui2TextMenu::srui2TextMenu(
        const char* name,
        u32 colCount,
        const char* p3,
        i32 x,
        i32 y,
        const char* p6,
        undefined4 p7,
        const WidgetRef<ui2Master>& master
    ) : ui2Widget(name, master, true) {
        field_0x7c = colCount;
        m_itemCount = colCount;
        field_0xb8 = 0;

        m_pos = { x, y };
        field_0x14c = 1;
        m_textSize = { 200, 203 };
        m_textOffset = { 314, 0 };
        m_upArrowOffset = { 200, 0 };
        m_downArrowOffset = { 200, 100 };
        m_showTextBackdrop = false;
        m_someStrings = nullptr;

        init(p3, x, y, p6, p7);
    }

    srui2TextMenu::~srui2TextMenu() {
        delete [] m_textIsDynamic;
        delete [] m_lineSpacing;
        delete [] m_texts;
        delete [] m_descs;
        delete [] m_cursorImgs;
        delete [] m_mainRepeaters;
        delete [] m_userRepeaters;
    }

    void srui2TextMenu::init(const char* p1, i32 x, i32 y, const char* p4, undefined4 p5) {
        field_0x124.set(p1);
        field_0x134.set(p4);

        u32 bufLen = strlen(p1) + 20;
        char* nameBuf = new char[bufLen];

        field_0x48 = 1;
        field_0x144 = p5;

        snprintf(nameBuf, bufLen, "%s_menu", p1);
        m_menu = new ui2Menu(nameBuf, 1, m_itemCount, getMaster());

        snprintf(nameBuf, bufLen, "%s_Menu_rep", p1);
        m_menuRepeater = new ui2Repeater(nameBuf, nullptr);
        m_menuRepeater->addListener(*m_menu, WidgetEventType::MaybeAll, &ui2Widget::acceptEvent);

        snprintf(nameBuf, bufLen, "%s_Cursor_tmr", p1);
        m_cursorTimer = new ui2Timer(nameBuf, 1.4f, true, true, nullptr);
        m_cursorTimer->addEventMapper(WidgetEventType::UNK29, WidgetEventType::StartTimer, nullptr);
        m_menu->addListener(*m_cursorTimer, WidgetEventType::UNK29, &ui2Widget::acceptEvent);

        snprintf(nameBuf, bufLen, "%s_Cursor_l_snd", p1);
        m_cursorLSound = new ui2Sound(nameBuf, 0x31, nullptr);
        m_cursorLSound->FUN_00208068(-1.0f);
        m_cursorLSound->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK33, nullptr);
        m_cursorTimer->addListener(*m_cursorLSound, WidgetEventType::TimerFinished, &ui2Widget::acceptEvent);
        m_cursorLSound->FUN_00207fc8();
        m_cursorLSound->FUN_00207fe0(0.45f);

        snprintf(nameBuf, bufLen, "%s_Cursor_r_snd", p1);
        m_cursorRSound = new ui2Sound(nameBuf, 0x32, nullptr);
        m_cursorRSound->FUN_00208068(1.0f);
        m_cursorRSound->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK33, nullptr);
        m_cursorTimer->addListener(*m_cursorRSound, WidgetEventType::TimerFinished, &ui2Widget::acceptEvent);
        m_cursorLSound->FUN_00207fc8();
        m_cursorLSound->FUN_00207fe0(0.45f);

        if (p4) {
            m_menu->addListener(p4, WidgetEventType::UNK26, &ui2Widget::acceptEvent);
            auto var = ui2Base::getGlobalMaster()->findWidget(p4, "ui2Variable").cast<ui2Variable>();
            var->addListener(var->getName(), WidgetEventType::UNK40, (SomeWidgetCallback)&srui2TextMenu::FUN_001e6318);
        }

        snprintf(nameBuf, bufLen, "%s_Menu_Items_tbl", p1);
        m_menuItemsTbl = new ui2Table(nameBuf, 2, m_itemCount, 0, 0, getMaster());
        m_menuItemsTbl->field_0x48 = 0;
        m_menuItemsTbl->FUN_001fd0a8(0, 0, 0, 0);
        m_menuItemsTbl->setColSizes(30, -1, -2);
        m_menuItemsTbl->setColSize(64, 0);
        m_menuItemsTbl->setColSize(120, 1);
        m_menu->addListener(*m_menuItemsTbl, WidgetEventType::UNK29, &ui2Widget::acceptEvent);

        if (field_0x144) {
            snprintf(nameBuf, bufLen, "%s_Desc_rep", p1);
            m_descRepeater = new ui2Repeater(nameBuf, getMaster());

            snprintf(nameBuf, bufLen, "%s_Desc_img", p1);
            m_descImg = new ui2Image(nameBuf, "uisquarered", 0, 0, 5, nullptr);
            m_descImg->setSize(210, 213);
            m_descImg->field_0x48 = 0;
        }

        m_activeColor = new ui2Color();
        m_activeColor->color = 0x806cb4e1;

        m_inactiveColor = new ui2Color();
        m_inactiveColor->color = 0x8011551d;

        snprintf(nameBuf, bufLen, "%s_up_Arrow_img", p1);
        m_upArrowImg = new ui2Image(nameBuf, "uiUpArrow", 0, 0, 0, nullptr);

        snprintf(nameBuf, bufLen, "%s_up_Arrow_tmr", p1);
        m_upArrowTimer = new ui2Timer(nameBuf, 0.2f, false, false, nullptr);
        m_upArrowTimer->addListener(*m_upArrowImg, WidgetEventType::TimerFinished, &ui2Widget::acceptEvent);
        m_upArrowImg->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::SetColor, m_inactiveColor);
        m_upArrowImg->setColor(m_inactiveColor);
        m_upArrowImg->field_0x48 = 0;
        m_upArrowImg->setVisibility(0);

        snprintf(nameBuf, bufLen, "%s_down_Arrow_img", p1);
        m_downArrowImg = new ui2Image(nameBuf, "uiDownArrow", 0, 0, 0, nullptr);

        snprintf(nameBuf, bufLen, "%s_down_Arrow_tmr", p1);
        m_downArrowTimer = new ui2Timer(nameBuf, 0.2f, false, false, nullptr);
        m_downArrowTimer->addListener(*m_downArrowImg, WidgetEventType::TimerFinished, &ui2Widget::acceptEvent);
        m_downArrowImg->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::SetColor, m_inactiveColor);
        m_downArrowImg->setColor(m_inactiveColor);
        m_downArrowImg->field_0x48 = 0;
        m_downArrowImg->setVisibility(0);

        snprintf(nameBuf, bufLen, "%s_Select_l_snd", p1);
        m_selectLSound = new ui2Sound(nameBuf, 62, nullptr);
        m_selectLSound->FUN_00208068(-1.0f);
        m_selectLSound->addEventMapper(WidgetEventType::UNK29, WidgetEventType::UNK33, nullptr);
        m_menu->addListener(m_selectLSound, WidgetEventType::UNK29, &ui2Widget::acceptEvent);
        m_selectLSound->FUN_00207fc8();

        snprintf(nameBuf, bufLen, "%s_Select_r_snd", p1);
        m_selectRSound = new ui2Sound(nameBuf, 63, nullptr);
        m_selectRSound->FUN_00208068(1.0f);
        m_selectRSound->addEventMapper(WidgetEventType::UNK29, WidgetEventType::UNK33, nullptr);
        m_menu->addListener(m_selectRSound, WidgetEventType::UNK29, &ui2Widget::acceptEvent);
        m_selectRSound->FUN_00207fc8();

        snprintf(nameBuf, bufLen, "%s_all_tbl", p1);
        m_allTbl = new ui2Table(nameBuf, 7, 1, x, y, getMaster());
        m_allTbl->setColSizes(0, -1, -2);
        m_allTbl->setColSizes(0, -1, -2);
        m_allTbl->FUN_001fc6f8(m_menuItemsTbl, 0, 0, &ui2Widget::acceptEvent);
        m_allTbl->FUN_001fc6f8(m_upArrowImg, 1, 0, &ui2Widget::acceptEvent);
        m_allTbl->FUN_001fc6f8(m_downArrowImg, 2, 0, &ui2Widget::acceptEvent);
        m_allTbl->setCellOffset(200, 100, 2, 0);

        if (field_0x144) {
            m_allTbl->FUN_001fc6f8(m_descRepeater, 3, 0, &ui2Widget::acceptEvent);
            m_allTbl->setCellOffset(314, 0, 3, 0);

            m_allTbl->FUN_001fc6f8(m_descImg, 4, 0, &ui2Widget::acceptEvent);
            m_allTbl->setCellOffset(309, -5, 4, 0);
        }

        m_textIsDynamic = new bool[m_itemCount];
        m_lineSpacing = new u32[m_itemCount];

        m_texts = new WidgetRef<ui2Text>[m_itemCount];
        m_cursorImgs = new WidgetRef<ui2Image>[m_itemCount];
        m_mainRepeaters = new WidgetRef<ui2Repeater>[m_itemCount];
        m_userRepeaters = new WidgetRef<ui2Repeater>[m_itemCount];

        for (u32 i = 0;i < m_itemCount;i++) m_lineSpacing[i] = 0x1e;

        if (field_0x144) m_descs = new WidgetRef<ui2TranslatedText>[m_itemCount];

        delete [] nameBuf;

        addListener(getName(), WidgetEventType::MaybeAll, (SomeWidgetCallback)&srui2TextMenu::FUN_001e6318);
    }

    void srui2TextMenu::reset() {
        ui2Widget::reset();
    }
    
    void srui2TextMenu::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        bool alreadyPropagated = false;
        if (!m_isActive) {
            ui2Widget::onEvent(source, event, data);
            alreadyPropagated = true;

            if (!m_isActive) return;
        }

        switch (event) {
            case WidgetEventType::UpPressed: {
                m_upArrowImg->setColor(m_activeColor);
                m_upArrowTimer->startTimer();
                break;
            }
            case WidgetEventType::DownPressed: {
                m_downArrowImg->setColor(m_activeColor);
                m_downArrowTimer->startTimer();
                break;
            }
            default: break;
        }
        
        m_menuRepeater->acceptEvent(source, event, data);
        if (!alreadyPropagated) ui2Widget::onEvent(source, event, data);
    }
    
    void srui2TextMenu::method_0x58() {
        FUN_001e6530();
    }

    void srui2TextMenu::configureParser(datParser* parser) {
        const char* name = m_widgetName->get();
        char buf[160] = { 0 };
        snprintf(buf, 160, "%s.PosX", name);
        parser->add(PARSE_TYPE::INT32, buf, &m_pos.x, 1, nullptr);
        
        snprintf(buf, 160, "%s.PosY", name);
        parser->add(PARSE_TYPE::INT32, buf, &m_pos.y, 1, nullptr);
        
        snprintf(buf, 160, "%s.UpArrowOffsetX", name);
        parser->add(PARSE_TYPE::INT32, buf, &m_upArrowOffset.x, 1, nullptr);
        
        snprintf(buf, 160, "%s.UpArrowOffsetY", name);
        parser->add(PARSE_TYPE::INT32, buf, &m_upArrowOffset.y, 1, nullptr);
        
        snprintf(buf, 160, "%s.DownArrowOffsetX", name);
        parser->add(PARSE_TYPE::INT32, buf, &m_downArrowOffset.x, 1, nullptr);
        
        snprintf(buf, 160, "%s.DownArrowOffsetY", name);
        parser->add(PARSE_TYPE::INT32, buf, &m_downArrowOffset.y, 1, nullptr);
        
        snprintf(buf, 160, "%s.TextOffsetX", name);
        parser->add(PARSE_TYPE::INT32, buf, &m_textOffset.x, 1, nullptr);
        
        snprintf(buf, 160, "%s.TextOffsetY", name);
        parser->add(PARSE_TYPE::INT32, buf, &m_textOffset.y, 1, nullptr);
        
        snprintf(buf, 160, "%s.TextWidth", name);
        parser->add(PARSE_TYPE::INT32, buf, &m_textSize.x, 1, nullptr);
        
        snprintf(buf, 160, "%s.TextHeight", name);
        parser->add(PARSE_TYPE::INT32, buf, &m_textSize.y, 1, nullptr);
        
        snprintf(buf, 160, "%s.ShowTextBackdrop", name);
        parser->add(PARSE_TYPE::BOOLEAN, buf, &m_showTextBackdrop, 1, nullptr);
        
        for (u32 i = 0;i < m_itemCount;i++) {
            snprintf(buf, 160, "%s.LineSpacing:%d", name, i);
            parser->add(PARSE_TYPE::INT32, buf, &m_lineSpacing[i], 1, nullptr);
        }
    }

    void srui2TextMenu::initMenuItem(const char* name, i32 index, const char* p3, i32 p4) {
        u32 strLen = strlen(field_0x124.get());
        u32 nameLen = strlen(name);
        u32 unkStrLen = strLen + nameLen + 50;
        char* str0 = new char[unkStrLen];
        char* str1 = new char[unkStrLen];

        if (p4 < 1) {
            snprintf(str0, unkStrLen, "%s_%s_txt", field_0x124.get(), name);
            snprintf(str1, unkStrLen, "%s_%s", field_0x124.get(), name);

            ui2TranslatedText* txt = new ui2TranslatedText(str0, str1, 0, 0, 0, getMaster());
            m_texts[index] = txt;
            m_textIsDynamic[index] = false;
        } else {
            snprintf(str0, unkStrLen, "%s_%s_txt", field_0x124.get(), name);

            ui2DynamicText* txt = new ui2DynamicText(str0, 0, 0, 0, "", p4, getMaster());
            m_texts[index] = txt;
            m_textIsDynamic[index] = true;

            txt->setFont("uimedH");
        }

        m_texts[index]->field_0x48 = 0;

        if (field_0x144) {
            snprintf(str0, unkStrLen, "%s_%s_Desc_txt", field_0x124.get(), name);
            snprintf(str1, unkStrLen, "%s_%s_Desc", field_0x124.get(), name);

            ui2TranslatedText* txt = new ui2TranslatedText(str0, str1, 0, 0, 0, getMaster());
            m_descs[index] = txt;
            
            txt->setBounds(200, 203);
            txt->setColorU32(0x806cb4e1);
        }

        snprintf(str0, unkStrLen, "%s_%s_Cursor_img", field_0x124.get(), name);
        m_cursorImgs[index] = new ui2Image(str0, "uichevron", 0, 0, 0, getMaster());
        m_cursorImgs[index]->FUN_001f5db0(2);
        m_cursorImgs[index]->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK8, nullptr);
        m_cursorImgs[index]->field_0x48 = 0;
        m_cursorTimer->addListener(m_cursorImgs[index], WidgetEventType::TimerFinished, &ui2Widget::acceptEvent);

        snprintf(str0, unkStrLen, "%s_%s_Main_rep", field_0x124.get(), name);
        m_mainRepeaters[index] = new ui2Repeater(str0, getMaster());

        snprintf(str0, unkStrLen, "%s_%s_User_rep", field_0x124.get(), name);
        m_userRepeaters[index] = new ui2Repeater(str0, getMaster());

        m_menuItemsTbl->FUN_001fc6f8(m_cursorImgs[index], 0, index, &ui2Widget::acceptEvent);
        m_menuItemsTbl->FUN_001fc6f8(m_texts[index], 1, index, &ui2Widget::acceptEvent);
        m_menuItemsTbl->setCellOffset(0, 2, 0, index);

        m_cursorImgs[index]->addEventMapper(WidgetEventType::UNK53, WidgetEventType::Show, nullptr);
        m_cursorImgs[index]->addEventMapper(WidgetEventType::UNK54, WidgetEventType::Hide, nullptr);
        m_cursorImgs[index]->addEventMapper(WidgetEventType::UNK53, WidgetEventType::UNK8, nullptr);

        m_texts[index]->addEventMapper(WidgetEventType::UNK53, WidgetEventType::SetColor, m_activeColor);
        m_texts[index]->addEventMapper(WidgetEventType::UNK54, WidgetEventType::SetColor, m_inactiveColor);
        m_texts[index]->addEventMapper(WidgetEventType::UNK31, WidgetEventType::Hide, nullptr);
        m_texts[index]->addEventMapper(WidgetEventType::UNK32, WidgetEventType::Show, nullptr);

        if (field_0x144) {
            m_descs[index]->addEventMapper(WidgetEventType::UNK53, WidgetEventType::Show, nullptr);
            m_descs[index]->addEventMapper(WidgetEventType::UNK54, WidgetEventType::Hide, nullptr);
            m_descRepeater->addListener(m_descs[index], WidgetEventType::MaybeAll, &ui2Widget::acceptEvent);
        }

        m_mainRepeaters[index]->addListener(m_cursorImgs[index], WidgetEventType::MaybeAll, &ui2Widget::acceptEvent);
        m_mainRepeaters[index]->addListener(m_texts[index], WidgetEventType::MaybeAll, &ui2Widget::acceptEvent);
        m_mainRepeaters[index]->addListener(m_userRepeaters[index], WidgetEventType::MaybeAll, &ui2Widget::acceptEvent);

        if (field_0x144) {
            m_mainRepeaters[index]->addListener(m_descs[index], WidgetEventType::MaybeAll, &ui2Widget::acceptEvent);
        }

        m_menu->FUN_001f8218(m_mainRepeaters[index], 0, index, 1, &ui2Widget::acceptEvent);

        if (p3) {
            m_someStrings[index].set(p3);
            WidgetRef<ASCIIStringEventData> event = new ASCIIStringEventData();
            event->data = m_someStrings[index].get();

            m_menu->FUN_001f86c8(index, 0, WidgetEventType::UNK26, event);
        }

        m_allTbl->FUN_001fdd88();
        m_menuItemsTbl->FUN_001fdd88();
        m_menu->FUN_001fa518();

        delete [] str0;
        delete [] str1;
    }

    void srui2TextMenu::setMenuItemText(u32 index, const char* text) {
        m_texts[index]->setTextName(text);
    }

    void srui2TextMenu::setMenuItemTextWide(u32 index, const wchar_t* text) {
        // todo
        // m_texts[index]->setTextName(text);
    }
    
    void srui2TextMenu::setMenuItemActive(i32 index, bool active) {
        m_menu->FUN_001f8b38(0, index, active);

        if (field_0x14c) {
            m_cursorImgs[index]->setActive(active);
            m_texts[index]->setActive(active);
        }
    }
   
    WidgetRef<ui2Repeater> srui2TextMenu::getUserRepeater(u32 idx) {
        return m_userRepeaters[idx];
    }
    
    void srui2TextMenu::FUN_001e5cc8(u32 index) {
        if (index >= m_itemCount) index = m_itemCount;
        field_0x7c = index;

        u32 rowSz = m_menuItemsTbl->getRowSize(0);
        m_menuItemsTbl->setBounds(640, index * rowSz);

        if (index < m_itemCount && field_0x14c == 1) {
            m_upArrowImg->setVisibility(1);
            m_downArrowImg->setVisibility(1);
        } else {
            m_upArrowImg->setVisibility(0);
            m_downArrowImg->setVisibility(0);
        }
    }
    
    void srui2TextMenu::FUN_001e5dc0(u32 index) {
        m_menu->FUN_001f9990(0, index);
    }

    i32 srui2TextMenu::FUN_001e5de8() {
        i32 outRow, outCol;
        ui::BaseRef w;
        m_menu->FUN_001fa3d8(&outRow, &outCol, w);

        return outCol;
    }

    void srui2TextMenu::FUN_001e5e58() {
        for (u32 i = 0;i < m_itemCount;i++) {
            m_mainRepeaters[i]->dispatchEvent(WidgetEventType::Hide);
            m_texts[i]->setActive(false);
            m_cursorImgs[i]->setActive(false);
        }

        if (field_0x144) m_descImg->setVisibility(0);
        m_upArrowImg->setVisibility(0);
        m_downArrowImg->setVisibility(0);

        m_cursorLSound->FUN_00207fc8();
        m_cursorRSound->FUN_00207fc8();
        m_selectLSound->FUN_00207fc8();
        m_selectRSound->FUN_00207fc8();

        field_0x14c = 0;
    }

    void srui2TextMenu::FUN_001e6000() {
        for (u32 i = 0;i < m_itemCount;i++) {
            if (m_menu->FUN_001f8ee0(0, i) == 1) {
                m_cursorImgs[i]->setActive(true);
                m_texts[i]->setActive(true);
            }

            m_texts[i]->setVisibility(1);
        }

        m_menu->FUN_001fa518();

        if (field_0x7c < m_itemCount) {
            m_upArrowImg->setVisibility(1);
            m_downArrowImg->setVisibility(1);
            
            m_upArrowImg->setColor(m_inactiveColor);
            m_downArrowImg->setColor(m_inactiveColor);
        }

        m_cursorLSound->FUN_00207fd8();
        m_cursorRSound->FUN_00207fd8();
        m_selectLSound->FUN_00207fd8();
        m_selectRSound->FUN_00207fd8();

        if (field_0x144 && m_showTextBackdrop) m_descImg->setVisibility(1);
        field_0x14c = 1;
    }
    
    void srui2TextMenu::FUN_001e6298(i32 p1) {
        field_0x150 = p1;
        if (p1 == 0) {
            m_activeColor->color = 0x806cb4e1;
            m_inactiveColor->color = 0x8011551d;
        } else if (p1 == 1) {
            m_activeColor->color = 0x80ffffff;
            m_inactiveColor->color = 0x80969696;
        }
    }

    void srui2TextMenu::FUN_001e6318(const ui::NamedRef& p1, WidgetEventType p2, const WidgetRef<ui2EventData>& p3) {
        if (p2 != WidgetEventType::UNK39 && p2 != WidgetEventType::UNK40) {
            assert((p1 == m_menu));
            dispatchEvent(p2, p3);
            return;
        }

        if (p3) m_menu->FUN_001fa298(p3);
        dispatchEvent(p2, p3);
    }
    
    void srui2TextMenu::FUN_001e6440(i32 offsetX, i32 offsetY, i32 width, i32 height) {
        if (!field_0x144) return;

        m_allTbl->setCellOffset(offsetX, offsetY, 3, 0);
        m_allTbl->setCellOffset(offsetX - 5, offsetY - 5, 4, 0);
        m_descImg->setSize(width + 10, height + 10);

        for (u32 i = 0;i < m_itemCount;i++) {
            m_descs[i]->setBounds(width, height);
        }
    }

    void srui2TextMenu::FUN_001e6530() {
        m_allTbl->FUN_001fc540(m_pos.x, m_pos.y);
        FUN_001e6440(m_textOffset.x, m_textOffset.y, m_textSize.x, m_textSize.y);

        if (field_0x14c && field_0x144) {
            m_descImg->setVisibility(m_showTextBackdrop);
        }

        m_allTbl->setCellOffset(m_upArrowOffset.x, m_upArrowOffset.y, 1, 0);
        m_allTbl->setCellOffset(m_downArrowOffset.x, m_downArrowOffset.y, 2, 0);

        for (u32 i = 0;i < m_itemCount;i++) {
            m_menuItemsTbl->setRowSize(m_lineSpacing[i], i);
        }

        m_allTbl->FUN_001fdd88();
        m_menuItemsTbl->FUN_001fdd88();
    }

    void srui2TextMenu::FUN_001e6948() {
        m_cursorLSound->FUN_00207fc8();
        m_cursorRSound->FUN_00207fc8();
        m_selectLSound->FUN_00207fc8();
        m_selectRSound->FUN_00207fc8();
    }

    void srui2TextMenu::FUN_001e6988() {
        m_cursorLSound->FUN_00207fd8();
        m_cursorRSound->FUN_00207fd8();
        m_selectLSound->FUN_00207fd8();
        m_selectRSound->FUN_00207fd8();
    }
};