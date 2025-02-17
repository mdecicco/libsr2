#pragma once
#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/ui2Master.h>
#include <libsr2/ui/ui2Menu.h>
#include <libsr2/ui/ui2Sound.h>

namespace sr2 {
    class ui2UnknownMenu0 : public ui2Menu {
        public:
            ui2UnknownMenu0(const char* name, i32 rowCount, i32 colCount, const WidgetRef<ui2Master>& master);
            virtual ~ui2UnknownMenu0();

            void init(const char* name);
            virtual void reset();

            void FUN_001f1a20();
            void FUN_001f1a50();
        
        protected:
            WidgetRef<ui2Sound> m_selectL;
            WidgetRef<ui2Sound> m_selectR;
    };
}