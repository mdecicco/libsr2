#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2EventData.h>
#include <libsr2/ui/WidgetRef.h>
#include <libsr2/ui/UnkWidgetBinTree1.h>
#include <libsr2/io/parFileIO.h>

#include <utils/String.h>

namespace sr2 {
    class ui2String;

    enum class WidgetEventType : u32 {
        MaybeAll = 0x80000001,
        UNK36 = 0x8000000a,
        UNK58 = 0x8000000b,
        UNK35 = 0x8000000c,
        AnyInput = 0x80000c1c,
        Activate  = 0xa0000064,
        UNK1  = 0xa0000065,
        Deactivate  = 0xa0000069,
        SetPosition  = 0xa00003e8,
        SetColor     = 0xa00003f2,
        UNK5  = 0xa00003f4,
        Show  = 0xa00003fc,
        Hide  = 0xa00003fd,
        UNK8  = 0xa00007d0,
        StartTimer = 0xa0000fa0,
        SetTimerDuration = 0xa0000fb4,
        UNK60 = 0xa000139c,
        UNK46 = 0xa0001770,
        UNK27 = 0xa0001f4a,
        UNK28 = 0xa0001f54,
        SetValueInteger = 0xa0002328,
        SetValueString = 0xa0002332,
        UNK43 = 0xa0002333,
        ClearValue = 0xa00023f0,
        UNK42 = 0xa0002710,
        UNK65 = 0xa0002ee0,
        UNK48 = 0xa00032c8,
        UNK33 = 0xa00032c9,
        UNK49 = 0xa00032ca,
        UNK50 = 0xa00032cb,
        UNK51 = 0xa00032cc,
        Activated = 0xc0000064,
        Deactivated = 0xc0000069,
        UNK11 = 0xc00003e8,
        UNK13 = 0xc00003f2,
        UNK14 = 0xc00003fc,
        UNK15 = 0xc00003fd,
        AnimationStarted = 0xc00007d0,
        AnimationEnded = 0xc00007da,
        AnimationFrame = 0xc00007e4,
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
        UNK62 = 0xc0001bda,
        UNK61 = 0xc0001be4,
        ValueChangedInteger = 0xc0002328,
        ValueChangedString = 0xc0002332,
        ValueCleared = 0xc00023f0,
        UNK30 = 0xc0002af8,
        UNK31 = 0xc0002b02,
        UNK32 = 0xc0002b0c,
        UNK55 = 0xc0002ee0,
        UNK63 = 0xc0002eea,
        UNK56 = 0xc0002ef4,
        UNK64 = 0xc0002efe
    };

    class ui2Widget : public ui2WidgetBase, public parFileIO {
        public:
            ui2Widget(const char* name, const WidgetRef<ui2Master>& master, bool doAssignMaster);
            virtual ~ui2Widget();

            virtual void releaseRef();
            virtual void reset();
            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void acceptEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void method_0x48();
            virtual void method_0x58();
            virtual void addListener(const ui::NamedRef& listener, WidgetEventType event, WidgetEventCallback acceptOverride = nullptr);
            virtual void addListener(const char* listenerName, WidgetEventType event, WidgetEventCallback acceptOverride = nullptr);
            virtual void removeListener(const ui::NamedRef& listener, WidgetEventType event);
            virtual void removeListener(const char* listenerName, WidgetEventType event);
            virtual void removeAllListeners(const ui::NamedRef& listener);
            virtual void removeAllListeners(const char* listenerName);
            virtual void addEventMapper(WidgetEventType incoming, WidgetEventType outgoing, const WidgetRef<ui2EventData>& event);
            virtual void removeEventMapper(WidgetEventType incoming, WidgetEventType outgoing);
            virtual void dispatchEvent(WidgetEventType event, const WidgetRef<ui2EventData>& data = nullptr, const ui::NamedRef& source = nullptr);
            virtual void setActive(bool p1);
            virtual void method_0xa8(i32 p1);
            virtual void setShouldLoad(bool p1);
            virtual void method_0xc8(u32 p1);
            virtual void configureParser(datParser* parser);
            virtual void draw();
            virtual bool loadWidget();
            virtual const char* getName() const;
            virtual const char* getType() const;
            virtual bool isA(const char* type) const;

            // parFileIO
            virtual const char* getFileType();
            virtual const char* getDirectory();
            virtual void prepParser(datParser* parser);
            virtual void afterLoad();

            ui2String* generateName();
            WidgetRef<ui2Master> getMaster();
            bool isActive();
            void makeRenderable(i32 priority);
            void stopRendering();
            void setRenderPriority(i32 priority);
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
            undefined4 field_0x74;
        
        protected:
            struct EventListener {
                WidgetEventType type;
                utils::String widgetName;
                WidgetEventCallback acceptCallback;
            };
            struct EventMapper {
                WidgetEventType incoming;
                WidgetEventType outgoing;
                WidgetRef<ui2EventData> event;
            };

            ui2String* m_widgetName;
            WidgetRef<ui2Master> m_master;
            bool m_isRenderable;
            bool m_addedToMasterUnk1;
            bool m_shouldBeLoaded;
            utils::Array<EventListener> m_listeners;
            utils::Array<EventMapper> m_mappers;
    };
};