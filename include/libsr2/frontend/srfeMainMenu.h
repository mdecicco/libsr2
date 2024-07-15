#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeUnknownScreen4.h>
#include <libsr2/ui/srui2TextMenu.h>
#include <libsr2/ui/ui2Timer.h>

namespace sr2 {
    class srfeMainMenu : public srfeUnknownScreen4 {
        public:
            srfeMainMenu(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeMainMenu();

            virtual void initScreen();
            void FUN_00136070(const ui::NamedRef& p1, WidgetEventType p2, const WidgetRef<ui2EventData>& p3);
        
        protected:
            WidgetRef<ui2Timer> m_timer;
            WidgetRef<srui2TextMenu> m_textMenu;
    };
};