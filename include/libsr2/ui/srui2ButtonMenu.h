#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/ui2Menu.h>
#include <libsr2/ui/ui2Repeater.h>
#include <libsr2/ui/ui2Image.h>
#include <libsr2/ui/ui2Timer.h>
#include <libsr2/ui/ui2TranslatedText.h>
#include <libsr2/ui/ui2Sound.h>
#include <libsr2/ui/ui2Table.h>
#include <libsr2/ui/ui2Color.h>

namespace sr2 {
    class srui2ButtonMenu : public ui2Widget {
        public:
            srui2ButtonMenu(
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
            );
            virtual ~srui2ButtonMenu();

            virtual void reset();
            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void method_0x58();
            virtual void prepParser(datParser* parser);

            void init(const char* p1, const char* p2, i32 x, i32 y, const char* p5);
            void interceptEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            void addOption(const char* name, u32 index, const char* value);
            void FUN_001eadd8(u32 idx, u32 p2);
            void FUN_001eaf58(u32 p1, u32 p2);
            void FUN_001eaf98(u32 p1);
            void FUN_001eb090(i32 idx);
            void FUN_001eb260();
            void FUN_001eb440();
            void FUN_001eb7d8(i32 p1);
            bool FUN_001ebd18();
            i32 getSelectedIndex();
        
        protected:
            vec2i m_pos;
            i32 m_lineSpacing;
            i32 m_buttonCount;
            WidgetRef<ui2Menu> m_menu;
            WidgetRef<ui2Repeater> m_menuRepeater;
            WidgetRef<ui2Image> m_cursorImg;
            WidgetRef<ui2Timer> m_cursorTimer;
            WidgetRef<ui2Sound> m_cursorLSound;
            WidgetRef<ui2Sound> m_cursorRSound;
            WidgetRef<ui2TranslatedText> m_headingText;
            WidgetRef<ui2Image> m_leftArrowImg;
            WidgetRef<ui2Timer> m_leftArrowTimer;
            WidgetRef<ui2Image> m_rightArrowImg;
            WidgetRef<ui2Timer> m_rightArrowTimer;
            WidgetRef<ui2Sound> m_selectLSound;
            WidgetRef<ui2Sound> m_selectRSound;
            WidgetRef<ui2Table> m_menuItemsTbl;
            WidgetRef<ui2Table> m_allTbl;

            WidgetRef<ui2TranslatedText>* m_labels;
            WidgetRef<ui2Image>* m_buttonImages;
            WidgetRef<ui2Image>* m_crossImages;
            WidgetRef<ui2Repeater>* m_mainRepeaters;
            WidgetRef<ui2Repeater>* m_userRepeaters;
            ui2String* m_values;

            i32 field_0x7c;
            ui2String field_0x110;
            WidgetRef<ui2Color> field_0x120;
            WidgetRef<ui2Color> field_0x128;
            WidgetRef<ui2Color> field_0x130;
            ui2String field_0x138;
            ui2String field_0x148;
            i32 field_0x158;
            const char* field_0x15c;
            undefined4 field_0x160;
            undefined4 field_0x168;
            undefined4 field_0x164;
    };
};