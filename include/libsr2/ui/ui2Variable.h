#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/IntegerEventData.h>
#include <libsr2/ui/ASCIIStringEventData.h>

namespace sr2 {
    class ui2Variable : public ui2Widget {
        public:
            ui2Variable(const char* name, u32 sz, const WidgetRef<ui2Master>& master);
            virtual ~ui2Variable();

            void init();
            virtual void onEvent(const ui::BaseRef& p1, WidgetEventType p2, const ui::BaseRef& p3);

            void FUN_002021f0(undefined4 p1);
            void FUN_00202310(const WidgetRef<IntegerEventData>& p1);
            void FUN_002023a0(const char* p1);
            void FUN_002024c0(const WidgetRef<ASCIIStringEventData>& p1);
            void FUN_00202580();
            undefined4 FUN_00202610();
            undefined4 FUN_00202618();
            const char* FUN_00202620();
            void FUN_00202628(ui2Variable* p1);

        protected:
            undefined4 field_0x78;
            ui2String field_0x7c;
            undefined4 field_0x8c;

            WidgetRef<IntegerEventData> m_intEventData;
            WidgetRef<ASCIIStringEventData> m_strEventData;
    };
};