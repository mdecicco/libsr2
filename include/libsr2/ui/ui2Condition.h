#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>

namespace sr2 {
    class ui2Condition : public ui2Widget {
        public:
            struct Data {
                Data* next;
                Data* last;
                ui2String str;

                ui::BaseRef field_0x8;
                undefined4 field_0x10;
                ui::BaseRef field_0x14;
                undefined8 field_0x1c;

                void FUN_002018f0();
            };

            ui2Condition(
                const char* name,
                const char* p2,
                undefined4 p3,
                const WidgetRef<ui2Master>& master
            );
            virtual ~ui2Condition();

            virtual void method_0x30(const ui::BaseRef& p1, SOME_WIDGET_ENUM p2, const ui::BaseRef& p3);

            void FUN_00200c40(const char* p1);
            void FUN_00200c70(const ui::NamedRef& p1);
            void FUN_00200cb8(const ui::BaseRef& p1, const ui::NamedRef& p2, undefined4 p3, ui::BaseRef& p4, u64 p5);
            void FUN_00200d38(const ui::BaseRef& p1, const char* p2, undefined4 p3, ui::BaseRef& p4, u64 p5);
            void FUN_00200fe8(u64 p1, const ui::BaseRef& p2);
            void FUN_00201378(const ui::BaseRef& p1);
            void FUN_002013b0();

        protected:
            Data* m_someObject;

            ui2String field_0x78;
            undefined4 field_0x88;
    };
};