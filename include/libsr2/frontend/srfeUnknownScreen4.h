#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen1.h>
#include <libsr2/ui/ui2Image.h>
#include <libsr2/ui/ui2Timer.h>
#include <libsr2/ui/ui2Text.h>
#include <libsr2/ui/ui2Table.h>
#include <libsr2/ui/srui2Clock.h>

namespace sr2 {
    class srfeUnknownScreen4 : public srfeUnknownScreen1 {
        public:
            struct Widgets {
                WidgetRef<ui2Master> master;
                WidgetRef<ui2Image> bluebars_01;
                WidgetRef<ui2Image> bluebars_02;
                WidgetRef<ui2Image> bluebars_03;
                WidgetRef<ui2Image> bluebars_04;
                WidgetRef<ui2Image> bluebars_05;
                WidgetRef<ui2Image> bluebars_06;
                WidgetRef<ui2Image> bluebars_07;
                WidgetRef<ui2Image> bluebars_08;
                WidgetRef<ui2Image> bluebars_09;
                WidgetRef<ui2Image> bluebars_10;
                WidgetRef<ui2Image> bluebars_11;
                WidgetRef<ui2Table> bluebars_table;
                WidgetRef<ui2Timer> bluebars_timer_01;
                WidgetRef<ui2Timer> bluebars_timer_02;
                WidgetRef<ui2Timer> bluebars_timer_03;
                WidgetRef<ui2Timer> bluebars_timer_04;
                WidgetRef<ui2Timer> bluebars_timer_05;
                WidgetRef<ui2Timer> bluebars_timer_06;
                WidgetRef<ui2Timer> bluebars_timer_07;
                WidgetRef<ui2Timer> bluebars_timer_08;
                WidgetRef<ui2Timer> bluebars_timer_09;
                WidgetRef<ui2Timer> bluebars_timer_10;
                WidgetRef<ui2Timer> bluebars_timer_11;
                WidgetRef<ui2Image> select_button;
                WidgetRef<ui2Image> back_button;
                WidgetRef<ui2Text> select_text;
                WidgetRef<ui2Text> back_text;
                WidgetRef<ui2Timer> select_button_timer;
                WidgetRef<ui2Timer> back_button_timer;
                WidgetRef<ui2Image> side_anim_begin_top;
                WidgetRef<ui2Image> side_anim_begin_middle;
                WidgetRef<ui2Image> side_anim_begin_bottom;
                WidgetRef<ui2Timer> side_anim_begin_top_timer;
                WidgetRef<ui2Timer> side_anim_begin_middle_timer;
                WidgetRef<ui2Timer> side_anim_begin_bottom_timer;
                WidgetRef<ui2Image> side_anim_loop_top;
                WidgetRef<ui2Image> side_anim_loop_middle;
                WidgetRef<ui2Image> side_anim_loop_bottom;
                WidgetRef<ui2Sound> side_anim_l_snd;
                WidgetRef<ui2Sound> side_anim_r_snd;
                WidgetRef<srui2Clock> clock_top;
                WidgetRef<ui2Timer> clock_top_timer;
                WidgetRef<ui2Timer> show_ui_timer;
            };

            srfeUnknownScreen4(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeUnknownScreen4();

            virtual void setActive(bool p1);
            virtual void initScreen();
            virtual void deinitScreen();
            virtual void method_0x108();
            virtual void method_0x110();
            virtual void method_0x128();
            virtual bool method_0x130();
            virtual bool method_0x138();

            void interceptEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
        
        protected:
            static u32 ActiveCount;
            static u32 InitializedCount;
            static Widgets* WidgetsInstance;
            static void initWidgets();
    };
};