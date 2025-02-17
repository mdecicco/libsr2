#pragma once
#include <libsr2/types.h>
#include <libsr2/frontend/srfeBasicLayout2.h>
#include <libsr2/ui/srui2ButtonMenu.h>

namespace sr2 {
    class srfePlayerSel : public srfeBasicLayout2 {
        public:
            srfePlayerSel(
                const char* name,
                const char* prevScreenName,
                const char* nextScreenName,
                bool allow1on1,
                const WidgetRef<ui2Master>& master
            );
            ~srfePlayerSel();

            virtual void initScreen();
            virtual void deinitScreen();
            virtual void method_0x100();
            virtual void method_0x110();
            virtual void method_0x128();
        
        protected:
            const char* m_nextScreenName;
            const char* m_prevScreenName;
            WidgetRef<srui2ButtonMenu> m_selection;
            bool m_allow1on1;
    };
};