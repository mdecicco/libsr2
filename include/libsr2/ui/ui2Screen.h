#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/ui2Master.h>

namespace sr2 {
    class ui2Screen : public ui2Widget {
        public:
            ui2Screen(const char* name, const WidgetRef<ui2Master>& master);
            virtual ~ui2Screen();

            virtual void reset();
            virtual void method_0x30(const ui::BaseRef& p1, SOME_WIDGET_ENUM p2, const ui::BaseRef& p3);
            virtual void method_0x48();
            virtual bool loadWidget();
            virtual void method_0x58();
            virtual void draw();
            virtual void method_0xa0(bool p1);
            virtual void method_0xc8();
            virtual void prepParserAgain(datParser* parser);
            virtual void method_0xe8(i32 p1);
            virtual void method_0xf0() = 0;
            virtual void method_0xf8() = 0;
            virtual void method_0x100();

            void FUN_001fbab0();
            void FUN_001fbbe0();

        public:
            WidgetRef<ui2Master> m_master;

            undefined4 field_0x80;
            undefined4 field_0x84;
    };
};