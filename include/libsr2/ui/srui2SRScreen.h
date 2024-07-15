#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Screen.h>
#include <libsr2/ui/ui2Input.h>
#include <libsr2/ui/ui2Repeater.h>

namespace sr2 {
    class srui2SRScreen : public ui2Screen {
        public:
            srui2SRScreen(const char* name, const WidgetRef<ui2Master>& master);
            ~srui2SRScreen();

            virtual void initScreen();
            virtual void deinitScreen();
            virtual void method_0x100();
            virtual const char* getType() const;
            virtual bool isA(const char* type) const;

            void FUN_001f0fb8(const WidgetRef<ui2Repeater>& p1, WidgetEventType p2, const char* p3, undefined4 p4);
        
        protected:
            WidgetRef<ui2Input> m_input;
            WidgetRef<ui2Repeater> m_repeater0;
            WidgetRef<ui2Repeater> m_repeater1;
    };
};