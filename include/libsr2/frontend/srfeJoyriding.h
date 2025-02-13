#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBasicLayout2.h>
#include <libsr2/ui/srui2ButtonMenu.h>
// #include <libsr2/ui/ui2UnknownMenu0.h>
#include <libsr2/ui/ui2Table.h>
#include <libsr2/ui/ui2Condition.h>

namespace sr2 {
    class srfeJoyriding : public srfeBasicLayout2 {
        public:
            srfeJoyriding(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeJoyriding();

            virtual void initScreen();
            virtual void deinitScreen();
            virtual void method_0x100();
            virtual void method_0x110();
            virtual bool method_0x118();
            virtual void method_0x128();

        protected:
            WidgetRef<srui2ButtonMenu> m_levelBtnMenu;
            WidgetRef<srui2ButtonMenu> m_timeOfDayBtnMenu;
            WidgetRef<srui2ButtonMenu> m_policeBtnMenu;
            // WidgetRef<ui2UnknownMenu0> m_menuMenu;
            WidgetRef<ui2Table> m_menuTable;
            WidgetRef<ui2Condition> m_nextScreenP1Cond;
    };
};