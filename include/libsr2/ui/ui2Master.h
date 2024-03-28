#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>

namespace sr2 {
    class ui2Master : public ui2Widget {
        public:
            struct Unk0 {
                Unk0* next;
                Unk0* last;
                i32 field_0x8;
                undefined8 field_0xc;
                ui::NamedRef field_0x14;

                static void destroy(Unk0** listPtr);
            };

            struct Unk1 {
                Unk1* next;
                Unk1* last;
                ui::NamedRef field_0x8;

                static void destroy(Unk1** listPtr);
                static void removeAll(Unk1** listPtr, const ui::NamedRef& widget);
            };

            ui2Master(const char* name, const WidgetRef<ui2Master>& master, bool doAssignMaster);
            virtual ~ui2Master();

            virtual void reset();
            virtual void draw();
            virtual void method_0x48();
            virtual bool loadWidget();
            virtual void method_0x58();
            virtual void method_0xc8();
            virtual void prepParserAgain(datParser* parser);
            virtual const char* getType() const;
            virtual bool isA(const char* type) const;
            virtual void method_0xe8();
            virtual void method_0xf0();

            void insertToUnk0(const ui::NamedRef& p1, i32 p2, u64 p3);
            void deleteFromUnk0(const ui::NamedRef& p1);
            undefined4 insertToUnk1(const ui::NamedRef& p1);
            i32 deleteFromUnk1(const ui::NamedRef& p1);
        
        protected:
            Unk0* m_unk0;
            Unk1* m_unk1;
    };
};