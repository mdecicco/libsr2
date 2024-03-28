#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2WidgetBase.h>
#include <libsr2/ui/WidgetRef.h>
#include <libsr2/ui/UnkWidgetBinTree1.h>
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class ui2String;

    enum SOME_WIDGET_ENUM : u32 {
        UNK12 = 0x80000001,
        UNK36 = 0x8000000a,
        UNK35 = 0x8000000c,
        UNK0  = 0xa0000064,
        UNK1  = 0xa0000065,
        UNK2  = 0xa0000069,
        UNK3  = 0xa00003e8,
        UNK4  = 0xa00003f2,
        UNK5  = 0xa00003f4,
        UNK6  = 0xa00003fc,
        UNK7  = 0xa00003fd,
        UNK8  = 0xa00007d0,
        UNK41 = 0xa0000fa0,
        UNK46 = 0xa0001770,
        UNK27 = 0xa0001f4a,
        UNK28 = 0xa0001f54,
        UNK37 = 0xa0002328,
        UNK26 = 0xa0002332,
        UNK43 = 0xa0002333,
        UNK44 = 0xa00023f0,
        UNK42 = 0xa0002710,
        UNK48 = 0xa00032c8,
        UNK33 = 0xa00032c9,
        UNK49 = 0xa00032ca,
        UNK50 = 0xa00032cb,
        UNK51 = 0xa00032cc,
        UNK9  = 0xc0000064,
        UNK10 = 0xc0000069,
        UNK11 = 0xc00003e8,
        UNK13 = 0xc00003f2,
        UNK14 = 0xc00003fc,
        UNK15 = 0xc00003fd,
        UNK16 = 0xc0000bb8,
        UNK17 = 0xc0000bb9,
        UNK18 = 0xc0000bba,
        UNK19 = 0xc0000bbb,
        UNK20 = 0xc0000bbc,
        UNK21 = 0xc0000bbd,
        UNK52 = 0xc0000bbe,
        UNK34 = 0xc0000faa,
        UNK47 = 0xc0001770,
        UNK29 = 0xc0001b58,
        UNK53 = 0xc0001b62,
        UNK54 = 0xc0001b6c,
        UNK22 = 0xc0001b76,
        UNK23 = 0xc0001b80,
        UNK24 = 0xc0001b8a,
        UNK25 = 0xc0001b8b,
        UNK38 = 0xc0001bbc,
        UNK39 = 0xc0002328,
        UNK40 = 0xc0002332,
        UNK45 = 0xc00023f0,
        UNK30 = 0xc0002af8,
        UNK31 = 0xc0002b02,
        UNK32 = 0xc0002b0c
    };

    class ui2Widget : public ui2WidgetBase, public parFileIO {
        public:
            ui2Widget(const char* name, const WidgetRef<ui2Master>& master, bool doAssignMaster);
            virtual ~ui2Widget();

            virtual void releaseRef();
            virtual void reset();
            virtual void method_0x30(const ui::BaseRef& p1, SOME_WIDGET_ENUM p2, const ui::BaseRef& p3);
            virtual void method_0x38(const ui::BaseRef& p1, SOME_WIDGET_ENUM p2, const ui::BaseRef& p3);
            virtual void method_0x48();
            virtual void method_0x58();
            virtual void method_0x68(const ui::NamedRef& ref, SOME_WIDGET_ENUM p2, u64 p3);
            virtual void method_0x70(const char* p1, SOME_WIDGET_ENUM p2, u64 p3);
            virtual void method_0x78(const ui::NamedRef& ref, i32 p2, u64 p3);
            virtual void method_0x80(const char* p1, i32 p2, i32 p3);
            virtual void method_0x88(const ui::NamedRef& ref);
            virtual void method_0x90(const char* p1);
            virtual void method_0x98(SOME_WIDGET_ENUM p1, const ui::BaseRef& p2, ui::BaseRef& p3);
            virtual void method_0xa0(bool p1);
            virtual void method_0xa8(i32 p1);
            virtual void method_0xb0(SOME_WIDGET_ENUM p1, SOME_WIDGET_ENUM p2, const ui::BaseRef& p3);
            virtual void method_0xb8(SOME_WIDGET_ENUM p1, SOME_WIDGET_ENUM p2);
            virtual void method_0xc0(bool p1);
            virtual void method_0xc8();
            virtual void prepParserAgain(datParser* parser);
            virtual void draw();
            virtual bool loadWidget();
            virtual const char* getName() const;
            virtual const char* getType() const;
            virtual bool isA(const char* type) const;

            // parFileIO
            virtual const char* getFileType() const;
            virtual const char* getDirectory() const;
            virtual datParserNode* prepParser(datParser* parser);
            virtual void afterLoad();

            ui2String* generateName();
            WidgetRef<ui2Master> getMaster();
            bool FUN_0020a070();
            void addToMasterUnk0(i32 p1, u64 p2);
            void removeFromMasterUnk0();
            void FUN_0020aa80(i32 p1, u64 p2);
            void addToMasterUnk1();
            void removeFromMasterUnk1();
            void FUN_0020ac08(i32* p1, u64 p2);

            bool field_0x18;                       
            bool field_0x1c;
            undefined4 field_0x24;
            undefined4 field_0x28;
            u8 field_0x34;
            undefined4 field_0x38;
            i32 field_0x40;
            undefined4 field_0x44;
            undefined4 field_0x48;
            undefined4 field_0x4c;
            ui2Widget* field_0x5c;
            u64 field_0x68;
            undefined4 field_0x74;
        
        protected:
            ui2String* m_widgetName;
            WidgetRef<ui2Master> m_master;
            UnkWidgetBinTree0 m_someBinTree0;
            UnkWidgetBinTree1 m_someBinTree1;
            bool m_addedToMasterUnk0;
            bool m_addedToMasterUnk1;
    };
};