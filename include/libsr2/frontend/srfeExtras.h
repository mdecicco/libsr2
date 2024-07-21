#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/srui2TextMenu.h>
#include <libsr2/frontend/srfeUnknownScreen4.h>

namespace sr2 {
    class srui2TextMenu;
    class srfeExtras : public srfeUnknownScreen4 {
        public:
            srfeExtras(const char* name, const WidgetRef<ui2Master>& master);
            ~srfeExtras();

            virtual void initScreen();
            virtual void deinitScreen();
            virtual void method_0x100();
            virtual void method_0x110();
            virtual void method_0x128();

        protected:
            WidgetRef<srui2TextMenu> m_menu;
    };
};