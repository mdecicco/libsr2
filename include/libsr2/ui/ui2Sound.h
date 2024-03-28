#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>

namespace sr2 {
    class SomeAudioThing6;

    class ui2Sound : public ui2Widget {
        public:
            ui2Sound(char* name, i32 p2, const WidgetRef<ui2Master>& master);
            virtual ~ui2Sound();

            virtual void method_0x30(const ui::BaseRef& p1, SOME_WIDGET_ENUM p2, const ui::BaseRef& p3);
            virtual void draw();
            virtual const char* getType() const;
            virtual bool isA(const char* type) const;

            void FUN_00207da0(i32 p1);
            void FUN_00207e20(const char* search);
            void FUN_00207eb8();
            void FUN_00207f78();
            void FUN_00207fc8();
            void FUN_00207fd8();
            void FUN_00207fe0(f32 p1);
            void FUN_00207fe8(f32 p1);
            void FUN_00208068(f32 p1);
            void FUN_002080b8(undefined4 p1);
            bool FUN_00208108();

            static i32 DAT_00362ea0;
            static i32 DAT_00362eac;
            static i32 DAT_00362eb4;
            static i32 DAT_00362eb8[4];
            static i32 DAT_00362ec8[4];
            static i32 DAT_00362ed8[4];
        
        protected:
            SomeAudioThing6* field_0x78;
            i32 field_0x7c;
            undefined4 field_0x80;
            f32 field_0x84;
            undefined4 field_0x88;
    };
};