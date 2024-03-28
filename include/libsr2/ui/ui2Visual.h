#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/ui2Color.h>
#include <libsr2/ui/ui2Position.h>

namespace sr2 {
    class ui2Visual : public ui2Widget {
        public:
            ui2Visual(const char* name, i32 x, i32 y, const WidgetRef<ui2Master>& master);
            virtual ~ui2Visual();

            void init();

            virtual void method_0x30(const ui::BaseRef& p1, SOME_WIDGET_ENUM p2, const ui::BaseRef& p3);
            virtual void method_0xe8(i32 x, i32 y);
            virtual void setPos(const WidgetRef<ui2Position>& p1);
            virtual void setColorU32(u32 color);
            virtual void setColorRGBA(u8 r, u8 g, u8 b, u8 a);
            virtual void setColor(const WidgetRef<ui2Color>& p1);
            virtual void method_0x110(undefined4 p1);
            virtual undefined4 method_0x118();
            virtual void prepParserAgain(datParser* parser);

        protected:
            vec2i m_pos;
            WidgetRef<ui2Color> m_unk0;
            WidgetRef<ui2Position> m_unk1;

            undefined4 field_0x78;
            undefined4 m_color;
    };
};