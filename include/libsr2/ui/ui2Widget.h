#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2EventData.h>
#include <libsr2/ui/WidgetRef.h>
#include <libsr2/ui/UnkWidgetBinTree1.h>
#include <libsr2/io/parFileIO.h>

namespace sr2 {
    class ui2String;

    enum class WidgetEventType : u32 {
        UNK12 = 0x80000001,
        UNK36 = 0x8000000a,
        UNK58 = 0x8000000b,
        UNK35 = 0x8000000c,
        UNK59 = 0x80000c1c,
        UNK0  = 0xa0000064,
        UNK1  = 0xa0000065,
        UNK2  = 0xa0000069,
        SetPosition  = 0xa00003e8,
        SetColor     = 0xa00003f2,
        UNK5  = 0xa00003f4,
        UNK6  = 0xa00003fc,
        UNK7  = 0xa00003fd,
        UNK8  = 0xa00007d0,
        StartTimer = 0xa0000fa0,
        SetTimerDuration = 0xa0000fb4,
        UNK60 = 0xa000139c,
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
        Activate = 0xc0000064,
        Deactivate = 0xc0000069,
        UNK11 = 0xc00003e8,
        UNK13 = 0xc00003f2,
        UNK14 = 0xc00003fc,
        UNK15 = 0xc00003fd,
        UNK57 = 0xc00007da,
        AcceptPressed = 0xc0000bb8,
        BackPressed = 0xc0000bb9,
        UpPressed = 0xc0000bba,
        DownPressed = 0xc0000bbb,
        LeftPressed = 0xc0000bbc,
        RightPressed = 0xc0000bbd,
        UNK52 = 0xc0000bbe,
        StartPressed = 0xc0000bcc,
        SelectPressed = 0xc0000bcd,
        TimerStarted = 0xc0000fa0,
        TimerFinished = 0xc0000faa,
        TimerDurationChanged = 0xc0000fb4,
        TextChanged = 0xc000139c,
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
        UNK32 = 0xc0002b0c,
        UNK55 = 0xc0002ee0,
        UNK56 = 0xc0002ef4
    };

    class ui2Widget : public ui2WidgetBase, public parFileIO {
        public:
            ui2Widget(const char* name, const WidgetRef<ui2Master>& master, bool doAssignMaster);
            virtual ~ui2Widget();

            virtual void releaseRef();
            virtual void reset();
            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void method_0x38(const ui::NamedRef& p1, WidgetEventType p2, const WidgetRef<ui2EventData>& p3);
            virtual void method_0x48();
            virtual void method_0x58();
            virtual void addListener(const ui::NamedRef& listener, WidgetEventType event, SomeWidgetCallback callback);
            virtual void addListener(const char* listenerName, WidgetEventType event, SomeWidgetCallback callback);
            virtual void removeListener(const ui::NamedRef& listener, WidgetEventType event);
            virtual void removeListener(const char* listenerName, WidgetEventType event);
            virtual void removeAllListeners(const ui::NamedRef& listener);
            virtual void removeAllListeners(const char* listenerName);
            virtual void dispatchEvent(WidgetEventType event, const WidgetRef<ui2EventData>& data = nullptr, const ui::NamedRef& source = nullptr);
            virtual void setActive(bool p1);
            virtual void method_0xa8(i32 p1);
            virtual void method_0xb0(WidgetEventType p1, WidgetEventType p2, const ui::BaseRef& p3);
            virtual void method_0xb8(WidgetEventType p1, WidgetEventType p2);
            virtual void method_0xc0(bool p1);
            virtual void method_0xc8(u32 p1);
            virtual void configureParser(datParser* parser);
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
            bool isActive();
            void addToMasterUnk0(i32 p1, u64 p2);
            void removeFromMasterUnk0();
            void addToMasterUnk0IfNecessary(i32 p1);
            void addToMasterUnk1();
            void removeFromMasterUnk1();
            void FUN_0020ac08(const ui::NamedRef& p1, u64 p2);
            bool shouldBeLoaded();

            bool field_0x18;                       
            bool m_isActive;
            undefined4 field_0x24;
            undefined4 field_0x28;
            u8 field_0x34;
            undefined4 field_0x38;
            i32 field_0x40;
            undefined4 field_0x44;
            undefined4 field_0x48;
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
            bool m_shouldBeLoaded;
    };
};