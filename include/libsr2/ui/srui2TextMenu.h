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
                char* name,
                u32 colCount,
                char* p3,
                i32 x,
                i32 y,
                char* p6,
                undefined4 p7,
                const WidgetRef<ui2Master>& master
            );
            virtual ~srui2TextMenu();

            void init(char* p1, i32 x, i32 y, char* p4, undefined4 p5);
        
        protected:
            u32 m_colCount;
            vec2i m_pos;
            vec2i m_textSize;
            vec2i m_textOffset;
            vec2i m_upArrowOffset;
            vec2i m_downArrowOffset;
            bool m_showTextBackdrop;
            undefined4* m_someArr;
            undefined4* m_columns;
            ui::NamedRef* m_someWidgetArr0;
            ui::NamedRef* m_someWidgetArr1;
            ui::NamedRef* m_someWidgetArr2;
            ui::NamedRef* m_someWidgetArr3;
            ui::NamedRef* m_someWidgetArr4;

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

            ui::NamedRef field_0xa8;
            ui::NamedRef field_0xb0;
            WidgetRef<ui2Color> field_0xe4;
            WidgetRef<ui2Color> field_0xec;
            
            undefined4 field_0x48;
            undefined4 field_0x7c;
            undefined4 field_0xb8;
            undefined4 field_0xbc;
            ui2String field_0x124;
            ui2String field_0x134;
            undefined4 field_0x144;
            undefined4 field_0x14c;
    };
};