#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBackground.h>
#include <libsr2/ui/ui2TranslatedText.h>
#include <libsr2/ui/ui2Image.h>
#include <libsr2/ui/ui2Timer.h>
#include <libsr2/ui/ui2Table.h>
#include <libsr2/ui/srui2Clock.h>

namespace sr2 {
    class srfeBasicLayout2 : public srfeBackground {
        public:
            srfeBasicLayout2(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeBasicLayout2();

            virtual void setActive(bool active);
            virtual void initScreen();
            virtual void deinitScreen();
            virtual void method_0x108();
            virtual void method_0x110();
            virtual void method_0x128();
            virtual bool method_0x130();
            virtual bool method_0x138();

            void showUiListener(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);

        protected:
            struct Widgets {
                WidgetRef<ui2Master> master;
                WidgetRef<ui2Image> bluebars01;
                WidgetRef<ui2Image> bluebars02;
                WidgetRef<ui2Image> bluebars03;
                WidgetRef<ui2Image> bluebars04;
                WidgetRef<ui2Image> bluebars05;
                WidgetRef<ui2Image> bluebars06;
                WidgetRef<ui2Image> bluebars07;
                WidgetRef<ui2Image> bluebars08;
                WidgetRef<ui2Image> bluebars09;
                WidgetRef<ui2Image> bluebars10;
                WidgetRef<ui2Image> bluebars11;
                WidgetRef<ui2Table> bluebarsTable;
                WidgetRef<ui2Timer> bluebarsTimer01;
                WidgetRef<ui2Timer> bluebarsTimer02;
                WidgetRef<ui2Timer> bluebarsTimer03;
                WidgetRef<ui2Timer> bluebarsTimer04;
                WidgetRef<ui2Timer> bluebarsTimer05;
                WidgetRef<ui2Timer> bluebarsTimer06;
                WidgetRef<ui2Timer> bluebarsTimer07;
                WidgetRef<ui2Timer> bluebarsTimer08;
                WidgetRef<ui2Timer> bluebarsTimer09;
                WidgetRef<ui2Timer> bluebarsTimer10;
                WidgetRef<ui2Timer> bluebarsTimer11;
                WidgetRef<ui2Image> selectBtn;
                WidgetRef<ui2Image> backBtn;
                WidgetRef<ui2TranslatedText> selectText;
                WidgetRef<ui2TranslatedText> backText;
                WidgetRef<ui2Timer> selectBtnTimer;
                WidgetRef<ui2Timer> backBtnTimer;
                WidgetRef<srui2Clock> clockTop;
                WidgetRef<ui2Timer> clockTopTimer;
                WidgetRef<ui2Timer> showUITimer;
            };

            static u32 ActiveCount;
            static u32 InitializedCount;
            static Widgets* WidgetsInstance;
            static void initWidgets();

            bool m_wasUninitialized;
    };
};