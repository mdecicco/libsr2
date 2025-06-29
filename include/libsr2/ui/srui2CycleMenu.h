#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/ui2Menu.h>
#include <libsr2/ui/ui2Repeater.h>
#include <libsr2/ui/ui2TranslatedText.h>
#include <libsr2/ui/ui2Image.h>
#include <libsr2/ui/ui2Timer.h>
#include <libsr2/ui/ui2Table.h>
#include <libsr2/ui/ui2Color.h>

namespace sr2 {
    class srui2CycleMenu : public ui2Widget {
        public:
            srui2CycleMenu(
                const char *name,
                i32 rowCount,
                const char *p3,
                const char *p4,
                const char* p5,
                i32 x,
                i32 y,
                const char* p8,
                const WidgetRef<ui2Master>& master
            );
            virtual ~srui2CycleMenu();

            void init(const char* p1, const char* p2, const char* p3, i32 x, i32 y, const char* p6);
            void interceptEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            void addOption(const char* name, u32 index, const char* value);
        
        protected:
            vec2i m_pos;
            u32 m_innerWidth;
            i32 m_headingOffset;
            i32 m_itemCount;

            WidgetRef<ui2Menu> m_menu;
            WidgetRef<ui2Repeater> m_menuRepeater;
            WidgetRef<ui2TranslatedText> m_headingText;
            WidgetRef<ui2TranslatedText> m_notSelectableText;
            WidgetRef<ui2Image> m_leftArrowImg;
            WidgetRef<ui2Image> m_rightArrowImg;
            WidgetRef<ui2Timer> m_leftArrowTimer;
            WidgetRef<ui2Timer> m_rightArrowTimer;
            WidgetRef<ui2Table> m_menuItemsTbl;
            WidgetRef<ui2Table> m_allTbl;

            ui::BaseRef* m_someWidgetArr0;
            ui::BaseRef* m_someWidgetArr1;
            ui::BaseRef* m_someWidgetArr2;
            ui2String* m_someStringArr;

            WidgetRef<ui2Color> field_0xf0;
            WidgetRef<ui2Color> field_0xf8;

            i32 field_0x7c;
            ui2String field_0xe0;
            ui2String field_0x100;
            ui2String field_0x110;
            undefined4 field_0x120;
            undefined4 field_0x124;

    };
};