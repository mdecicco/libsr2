#include <libsr2/ui/srui2ButtonMenu.h>
#include <libsr2/ui/ui2Base.h>
#include <libsr2/ui/IntegerEventData.h>
#include <libsr2/io/datParser.h>

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

    void srui2ButtonMenu::reset() {
        delete [] m_labels;
        delete [] m_buttonImages;
        delete [] m_crossImages;
        delete [] m_mainRepeaters;
        delete [] m_userRepeaters;
        delete [] m_values;
        ui2Widget::reset();
    }

    void srui2ButtonMenu::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive) {
            if (event == WidgetEventType::Activate) {
                ui2Widget::onEvent(source, event, data);
                if (!m_isActive) return;
            } else {
                ui2Widget::onEvent(source, event, data);
                return;
            }
        }

        switch (event) {
            case WidgetEventType::LeftPressed: {
                if (field_0x168 != 1) break;

                m_leftArrowImg->setColor(field_0x120);
                m_leftArrowTimer->startTimer();
                m_menuRepeater->acceptEvent(source, event, data);
                break;
            }
            case WidgetEventType::RightPressed: {
                if (field_0x168 != 1) break;

                m_rightArrowImg->setColor(field_0x120);
                m_rightArrowTimer->startTimer();
                m_menuRepeater->acceptEvent(source, event, data);
                break;
            }
            case WidgetEventType::UNK53: {
                FUN_001eb7d8(1);
                break;
            }
            case WidgetEventType::UNK54: {
                FUN_001eb7d8(0);
                break;
            }
            case WidgetEventType::SetPosition: {
                m_allTbl->acceptEvent(source, event, data);
                break;
            }
            default: {
                m_menuRepeater->acceptEvent(source, event, data);
                break;
            }
        }

        if (event != WidgetEventType::Activate) ui2Widget::onEvent(source, event, data);
    }

    void srui2ButtonMenu::method_0x58() {
        FUN_001ebd18();
    }

    void srui2ButtonMenu::prepParser(datParser* parser) {
        char buf[160];
        snprintf(buf, 160, "%s.PosX", getName());
        parser->add(PARSE_TYPE::INT32, buf, &m_pos.x, 1, nullptr);
        snprintf(buf, 160, "%s.PosY", getName());
        parser->add(PARSE_TYPE::INT32, buf, &m_pos.y, 1, nullptr);
        snprintf(buf, 160, "%s.LineSpacing", getName());
        parser->add(PARSE_TYPE::INT32, buf, &m_lineSpacing, 1, nullptr);
    }

    void srui2ButtonMenu::init(const char* p1, const char* p2, i32 x, i32 y, const char* p5) {
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
            m_menu->addListener(p5, WidgetEventType::SetValueInteger);
            m_menu->addListener(p5, WidgetEventType::SetValueString);
            auto var = ui2Base::getGlobalMaster()->findWidget(p5, "ui2Variable").cast<ui2Widget>();
            var->addListener(getName(), WidgetEventType::ValueChangedInteger, (WidgetEventCallback)&srui2ButtonMenu::interceptEvent);
            var->addListener(getName(), WidgetEventType::ValueChangedString, (WidgetEventCallback)&srui2ButtonMenu::interceptEvent);
        }
        
        snprintf(nameBuf0, bufLen, "%s_Menu_rep", p1);
        m_menuRepeater = new ui2Repeater(nameBuf0, nullptr);
        m_menuRepeater->addListener(m_menu, WidgetEventType::MaybeAll);

        if (field_0x168) {
            snprintf(nameBuf0, bufLen, "%s_Cursor_img", p1);
            m_cursorImg = new ui2Image(nameBuf0, "uichevron", 0, 0, 0, nullptr);
            m_cursorImg->FUN_001f5db0(2);
            m_cursorImg->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK8, nullptr);
            
            snprintf(nameBuf0, bufLen, "%s_Cursor_tmr", p1);
            m_cursorTimer = new ui2Timer(nameBuf0, 1.4f, 1, 1, nullptr);
            m_cursorTimer->addListener(m_menu, WidgetEventType::TimerFinished);

            snprintf(nameBuf0, bufLen, "%s_Cursor_l_snd", p1);
            m_cursorLSound = new ui2Sound(nameBuf0, 49, nullptr);
            m_cursorLSound->FUN_00208068(-1.0f);
            m_cursorLSound->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK33, nullptr);
            m_cursorTimer->addListener(m_cursorLSound, WidgetEventType::TimerFinished);
            m_cursorLSound->FUN_00207fc8();
            m_cursorLSound->FUN_00207fe0(0.45f);

            snprintf(nameBuf0, bufLen, "%s_Cursor_r_snd", p1);
            m_cursorRSound = new ui2Sound(nameBuf0, 50, nullptr);
            m_cursorRSound->FUN_00208068(1.0f);
            m_cursorRSound->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::UNK33, nullptr);
            m_cursorTimer->addListener(m_cursorRSound, WidgetEventType::TimerFinished);
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
        m_leftArrowTimer->addListener(m_leftArrowImg, WidgetEventType::TimerFinished);
        m_leftArrowImg->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::SetColor, field_0x130);
        m_leftArrowImg->setColor(field_0x130);
        m_leftArrowImg->field_0x48 = 0;

        snprintf(nameBuf0, bufLen, "%s_right_Arrow_img", p1);
        m_rightArrowImg = new ui2Image(nameBuf0, "uisidearrowrigt", 0, 0, 0, nullptr);

        snprintf(nameBuf0, bufLen, "%s_right_Arrow_tmr", p1);
        m_rightArrowTimer = new ui2Timer(nameBuf0, 0.2f, 0, 0, nullptr);
        m_rightArrowTimer->addListener(m_rightArrowImg, WidgetEventType::TimerFinished);
        m_rightArrowImg->addEventMapper(WidgetEventType::TimerFinished, WidgetEventType::SetColor, field_0x130);
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
        m_menu->addListener(m_menuItemsTbl, WidgetEventType::UNK29);

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
            m_allTbl->FUN_001fc6f8(m_cursorImg, 0, 0, &ui2Widget::acceptEvent);
            m_allTbl->FUN_001fc6f8(m_headingText, 1, 0, &ui2Widget::acceptEvent);
        }

        m_allTbl->FUN_001fc6f8(m_menuItemsTbl, 0, 1, &ui2Widget::acceptEvent);
        m_allTbl->FUN_001fc6f8(m_leftArrowImg, 1, 1, &ui2Widget::acceptEvent);
        m_allTbl->FUN_001fc6f8(m_rightArrowImg, 2, 1, &ui2Widget::acceptEvent);
        m_allTbl->setCellOffset(-30, 2, 0, 0);
        m_allTbl->setCellOffset(40, 0, 1, 0);

        m_labels = new WidgetRef<ui2TranslatedText>[m_buttonCount];
        m_buttonImages = new WidgetRef<ui2Image>[m_buttonCount];
        m_crossImages = new WidgetRef<ui2Image>[m_buttonCount];
        m_mainRepeaters = new WidgetRef<ui2Repeater>[m_buttonCount];
        m_userRepeaters = new WidgetRef<ui2Repeater>[m_buttonCount];
        m_values = new ui2String[m_buttonCount];

        delete [] nameBuf0;
        delete [] nameBuf1;

        m_menuItemsTbl->addListener(getName(), WidgetEventType::UNK30);
        m_menu->addListener(getName(), WidgetEventType::MaybeAll);
    }

    void srui2ButtonMenu::interceptEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        switch (event) {
            case WidgetEventType::ValueChangedInteger:
            case WidgetEventType::ValueChangedString: {
                if (data) m_menu->FUN_001fa298(data);
                break;
            }
            case WidgetEventType::UNK29: {
                if (field_0x164 || field_0x168) {
                    m_selectLSound->FUN_00207eb8();
                    m_selectRSound->FUN_00207eb8();
                }

                break;
            }
            case WidgetEventType::UNK30: {
                if (!m_menu || !m_crossImages) break;

                for (u32 i = 0;i < m_buttonCount;i++) {
                    if (!m_crossImages[i]) continue;
                    m_crossImages[i]->setVisibility(false);
                }

                break;
            }
            default: {
                if (source != m_menu) abort();
                break;
            }
        }

        if (source == m_menu) dispatchEvent(event, data);
    }
    
    void srui2ButtonMenu::addOption(const char* name, u32 index, const char* value) {
        const char* textA = field_0x138.get();
        const char* textB = field_0x148.get();

        u32 textALen = strlen(textA);
        u32 textBLen = strlen(textB);
        u32 bufLen = textALen + textBLen + 50;

        char* buf0 = new char[bufLen];
        char* buf1 = new char[bufLen];

        snprintf(buf0, bufLen, "%s_%s_txt", textA, name);
        snprintf(buf1, bufLen, "%s_%s", textB, name);

        WidgetRef<ui2Master> master = getMaster();

        m_labels[index] = new ui2TranslatedText(buf0, buf1, 0, 0, 0, master);
        u32 colSz = m_menuItemsTbl->getColSize(index);
        m_labels[index]->setBounds(colSz, 900);
        m_labels[index]->setAlignment(0);
        m_labels[index]->field_0x48 = 0;

        snprintf(buf0, bufLen, "%s_%s_Button_img", textA, name);
        m_buttonImages[index] = new ui2Image(buf0, field_0x15c, 0, 0, 0, master);
        m_buttonImages[index]->FUN_001f5db0(2);
        m_buttonImages[index]->field_0x48 = 0;

        snprintf(buf0, bufLen, "%s_%s_Cross_img", textA, name);
        m_crossImages[index] = new ui2Image(buf0, "uinoselect", 0, 0, -5, master);
        m_crossImages[index]->setColorU32(0x8011551d);
        m_crossImages[index]->setVisibility(false);
        m_crossImages[index]->field_0x48 = 0;

        snprintf(buf0, bufLen, "%s_%s_Main_rep", textA, name);
        m_mainRepeaters[index] = new ui2Repeater(buf0, master);

        snprintf(buf0, bufLen, "%s_%s_User_rep", textA, name);
        m_userRepeaters[index] = new ui2Repeater(buf0, master);

        m_labels[index]->addEventMapper(WidgetEventType::UNK53, WidgetEventType::SetColor, field_0x120);
        m_labels[index]->addEventMapper(WidgetEventType::UNK54, WidgetEventType::SetColor, field_0x130);
        m_labels[index]->addEventMapper(WidgetEventType::UNK31, WidgetEventType::Hide, nullptr);
        m_labels[index]->addEventMapper(WidgetEventType::UNK32, WidgetEventType::Show, nullptr);
        
        m_buttonImages[index]->addEventMapper(WidgetEventType::UNK53, WidgetEventType::SetColor, field_0x120);
        m_buttonImages[index]->addEventMapper(WidgetEventType::UNK54, WidgetEventType::SetColor, field_0x130);
        m_buttonImages[index]->addEventMapper(WidgetEventType::UNK31, WidgetEventType::Hide, nullptr);
        m_buttonImages[index]->addEventMapper(WidgetEventType::UNK32, WidgetEventType::Show, nullptr);

        m_crossImages[index]->addEventMapper(WidgetEventType::UNK24, WidgetEventType::Hide, nullptr);
        m_crossImages[index]->addEventMapper(WidgetEventType::UNK25, WidgetEventType::Show, nullptr);
        m_crossImages[index]->addEventMapper(WidgetEventType::UNK31, WidgetEventType::Hide, nullptr);
        m_crossImages[index]->addEventMapper(WidgetEventType::UNK32, WidgetEventType::Show, nullptr);
        m_crossImages[index]->addEventMapper(WidgetEventType::SetColor, WidgetEventType::UNK66, nullptr);

        m_mainRepeaters[index]->addListener(m_labels[index], WidgetEventType::MaybeAll);
        m_mainRepeaters[index]->addListener(m_buttonImages[index], WidgetEventType::MaybeAll);
        m_mainRepeaters[index]->addListener(m_crossImages[index], WidgetEventType::MaybeAll);
        m_mainRepeaters[index]->addListener(m_userRepeaters[index], WidgetEventType::MaybeAll);

        m_menu->FUN_001f8218(m_mainRepeaters[index], index, 0, 1, &ui2Widget::acceptEvent);

        if (value) {
            m_values[index].set(value);
            WidgetRef<ASCIIStringEventData> eventData = new ASCIIStringEventData();
            eventData->data = m_values[index].get();
            m_menu->FUN_001f86c8(index, 0, WidgetEventType::SetValueString, eventData);
        }

        m_menuItemsTbl->FUN_001fc6f8(m_buttonImages[index], index, 0, &ui2Widget::acceptEvent);
        m_menuItemsTbl->FUN_001fc6f8(m_crossImages[index], index, 1, &ui2Widget::acceptEvent);
        m_menuItemsTbl->FUN_001fc6f8(m_labels[index], index, 2, &ui2Widget::acceptEvent);

        if (field_0x158 == 0) {
            m_menuItemsTbl->setCellOffset(  0,   3, index, 0);
            m_menuItemsTbl->setCellOffset(-32, -29, index, 1);
            m_menuItemsTbl->setCellOffset( -6,   0, index, 2);
        } else if (field_0x158 == 1) {
            m_menuItemsTbl->setCellOffset(  0, -29, index, 0);
            m_menuItemsTbl->setCellOffset(  0, -30, index, 1);
            m_menuItemsTbl->setCellOffset( -6,   0, index, 2);
        }

        m_menu->FUN_001fa518();
        m_allTbl->forceRecalculate();

        delete [] buf0;
        delete [] buf1;
    }

    void srui2ButtonMenu::FUN_001eadd8(u32 idx, u32 p2) {
        m_values[idx].set("");

        WidgetRef<IntegerEventData> eventData = new IntegerEventData();
        eventData->data = p2;

        m_menu->FUN_001f86c8(idx, 0, WidgetEventType::SetValueInteger, eventData);
    }

    void srui2ButtonMenu::FUN_001eaf58(u32 p1, u32 p2) {
        m_menu->FUN_001f8b38(p1, 0, p2 != 0);
    }

    void srui2ButtonMenu::FUN_001eaf98(u32 p1) {
        m_menu->FUN_001f9990(p1, 0);
    }

    void srui2ButtonMenu::FUN_001eb090(i32 idx) {
        if (idx >= m_buttonCount) idx = m_buttonCount;
        field_0x7c = idx;

        i32 colSz = m_menuItemsTbl->getColSize(0);
        m_menuItemsTbl->setBounds(field_0x7c * colSz, 200);

        if (field_0x7c < m_buttonCount) {
            m_leftArrowImg->setVisibility(true);
            m_rightArrowImg->setVisibility(true);

            if (field_0x158 == 0 || field_0x158 == 1) {
                m_allTbl->setCellOffset(0, 19, 1, 1);
                m_allTbl->setCellOffset(field_0x7c * m_menuItemsTbl->getColSize(0) + 27, 19, 2, 1);
            }

            m_allTbl->setCellOffset(36, 0, 0, 1);
            return;
        }

        m_leftArrowImg->setVisibility(false);
        m_rightArrowImg->setVisibility(false);
        m_allTbl->setCellOffset(0, 0, 0, 1);
    }

    void srui2ButtonMenu::FUN_001eb260() {
        if (field_0x160) {
            m_cursorImg->setVisibility(false);
            m_headingText->setVisibility(false);
            m_cursorLSound->FUN_00207fc8();
            m_cursorRSound->FUN_00207fc8();
        }

        m_leftArrowImg->setVisibility(false);
        m_rightArrowImg->setVisibility(false);

        for (u32 i = 0;i < m_buttonCount;i++) {
            if (!m_mainRepeaters[i]) continue;

            ui::NamedRef w = m_mainRepeaters[i].cast<ui2Widget>();
            w->dispatchEvent(WidgetEventType::Hide);
            w->dispatchEvent(WidgetEventType::Deactivate);
        }

        m_selectLSound->FUN_00207fc8();
        m_selectRSound->FUN_00207fc8();

        field_0x164 = 0;
    }

    void srui2ButtonMenu::FUN_001eb440() {
        if (field_0x160) {
            m_headingText->setVisibility(true);
            m_cursorImg->setVisibility(true);
            m_cursorImg->FUN_001f5df0(-10);
            m_cursorTimer->startTimer();
            m_cursorLSound->FUN_00207fd8();
            m_cursorRSound->FUN_00207fd8();
        }

        if (field_0x7c < m_buttonCount) {
            m_leftArrowImg->setVisibility(true);
            m_rightArrowImg->setVisibility(true);
            m_leftArrowImg->setColor(field_0x130);
            m_rightArrowImg->setColor(field_0x130);
        }

        for (u32 i = 0;i < m_buttonCount;i++) {
            if (m_mainRepeaters[i]) {
                ui::NamedRef w = m_mainRepeaters[i].cast<ui2Widget>();
                w->dispatchEvent(WidgetEventType::Activate);
            }

            if (m_labels[i]) {
                m_labels[i]->setVisibility(true);
            }

            if (m_crossImages[i]) {
                m_menu->FUN_001f8ee0(i, 0);
                m_crossImages[i]->setVisibility(true);
            }

            if (m_buttonImages[i]) {
                m_buttonImages[i]->setVisibility(true);
                m_buttonImages[i]->FUN_001f5df0(-10);
            }
        }

        bool wasActive = m_menu->isActive();
        m_menu->setActive(true);
        m_menu->FUN_001fa518();
        m_allTbl->forceRecalculate();
        m_menu->setActive(wasActive);

        m_selectLSound->FUN_00207fd8();
        m_selectRSound->FUN_00207fd8();
        field_0x164 = 1;
    }

    void srui2ButtonMenu::FUN_001eb7d8(i32 p1) {
        field_0x168 = p1;

        if (!p1) {
            i32 outRow, outCol;
            ui::BaseRef outWidget;

            if (m_menu->FUN_001fa3d8(&outRow, &outCol, outWidget)) {
                outWidget.cast<ui2Widget>()->dispatchEvent(WidgetEventType::SetColor, field_0x130);
            }

            m_menuRepeater->setActive(false);
            if (field_0x160) {
                m_cursorImg->setVisibility(false);
                m_cursorLSound->FUN_00207fc8();
                m_cursorRSound->FUN_00207fc8();
                m_headingText->setColorU32(0x8011551d);
            }

            m_selectLSound->FUN_00207fc8();
            m_selectRSound->FUN_00207fc8();

            return;
        }

        m_menuRepeater->setActive(true);

        i32 outRow, outCol;
        ui::BaseRef outWidget;
        if (m_menu->FUN_001fa3d8(&outRow, &outCol, outWidget)) {
            outWidget.cast<ui2Widget>()->dispatchEvent(WidgetEventType::SetColor, field_0x120);
        }

        m_menu->FUN_001fa518();
        if (field_0x160) {
            if (field_0x164) {
                m_cursorImg->setVisibility(true);
                m_cursorLSound->FUN_00207fd8();
                m_cursorRSound->FUN_00207fd8();
            }

            m_cursorImg->FUN_001f5df0(-10);
            m_cursorTimer->startTimer();
            m_headingText->setColorU32(0x806cb4e1);
        }

        m_selectLSound->FUN_00207fd8();
        m_selectRSound->FUN_00207fd8();
    }

    bool srui2ButtonMenu::FUN_001ebd18() {
        m_allTbl->setRowSize(m_lineSpacing, 0);
        m_allTbl->FUN_001fc540(m_pos.x, m_pos.y);
        m_allTbl->forceRecalculate();
        return m_menuItemsTbl->forceRecalculate();
    }

    i32 srui2ButtonMenu::getSelectedIndex() {
        i32 outRow, outCol;
        ui::BaseRef outWidget;
        m_menu->FUN_001fa3d8(&outRow, &outCol, outWidget);
        return outRow;
    }
};
