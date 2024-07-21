#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/ui2Menu.h>
#include <libsr2/ui/ui2Repeater.h>
#include <libsr2/ui/ui2Timer.h>
#include <libsr2/ui/ui2Sound.h>
#include <libsr2/ui/ui2Table.h>
#include <libsr2/ui/ui2Image.h>
#include <libsr2/ui/ui2Color.h>
#include <libsr2/ui/ui2String.h>
#include <libsr2/ui/ui2TranslatedText.h>

namespace sr2 {
    class ui2Menu;
    class ui2Repeater;
    class ui2Timer;
    class ui2Sound;
    class ui2Table;
    class ui2Image;

    class srui2TextMenu : public ui2Widget {
        public:
            srui2TextMenu(
                const char* name,
                u32 colCount,
                const char* p3,
                i32 x,
                i32 y,
                const char* p6,
                undefined4 p7,
                const WidgetRef<ui2Master>& master
            );
            virtual ~srui2TextMenu();

            void init(const char* p1, i32 x, i32 y, const char* p4, undefined4 p5);
            virtual void reset();
            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void method_0x58();
            virtual void configureParser(datParser* parser);

            void initMenuItem(const char* name, i32 index, const char* p3, i32 p4);
            void setMenuItemText(u32 index, const char* text);
            void setMenuItemTextWide(u32 index, const wchar_t* text);
            void setMenuItemActive(i32 index, bool active);
            WidgetRef<ui2Repeater> getUserRepeater(u32 index);
            void FUN_001e5cc8(u32 index);
            void FUN_001e5dc0(u32 index);
            i32 FUN_001e5de8();
            void FUN_001e5e58();
            void FUN_001e6000();
            void FUN_001e6298(i32 p1);
            void FUN_001e6318(const ui::NamedRef& p1, WidgetEventType p2, const WidgetRef<ui2EventData>& p3);
            void FUN_001e6440(i32 offsetX, i32 offsetY, i32 width, i32 height);
            void FUN_001e6530();
            void FUN_001e6948();
            void FUN_001e6988();

        protected:
            u32 m_itemCount;
            vec2i m_pos;
            vec2i m_textSize;
            vec2i m_textOffset;
            vec2i m_upArrowOffset;
            vec2i m_downArrowOffset;
            bool m_showTextBackdrop;
            bool* m_textIsDynamic;
            u32* m_lineSpacing;
            ui2String* m_someStrings;
            WidgetRef<ui2Text>* m_texts;
            WidgetRef<ui2TranslatedText>* m_descs;
            WidgetRef<ui2Image>* m_cursorImgs;
            WidgetRef<ui2Repeater>* m_mainRepeaters;
            WidgetRef<ui2Repeater>* m_userRepeaters;

            WidgetRef<ui2Menu> m_menu;
            WidgetRef<ui2Repeater> m_menuRepeater;
            WidgetRef<ui2Repeater> m_descRepeater;
            WidgetRef<ui2Timer> m_cursorTimer;
            WidgetRef<ui2Timer> m_upArrowTimer;
            WidgetRef<ui2Timer> m_downArrowTimer;
            WidgetRef<ui2Sound> m_cursorLSound;
            WidgetRef<ui2Sound> m_cursorRSound;
            WidgetRef<ui2Sound> m_selectLSound;
            WidgetRef<ui2Sound> m_selectRSound;
            WidgetRef<ui2Table> m_menuItemsTbl;
            WidgetRef<ui2Table> m_allTbl;
            WidgetRef<ui2Image> m_descImg;
            WidgetRef<ui2Image> m_upArrowImg;
            WidgetRef<ui2Image> m_downArrowImg;
            WidgetRef<ui2Color> m_activeColor;
            WidgetRef<ui2Color> m_inactiveColor;

            ui::NamedRef field_0xa8;
            ui::NamedRef field_0xb0;
            
            undefined4 field_0x7c;
            undefined4 field_0xb8;
            ui2String field_0x124;
            ui2String field_0x134;
            undefined4 field_0x144;
            undefined4 field_0x14c;
            i32 field_0x150;
    };
};