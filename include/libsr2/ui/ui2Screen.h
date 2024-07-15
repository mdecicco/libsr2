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
            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual void method_0x48();
            virtual bool loadWidget();
            virtual void method_0x58();
            virtual void draw();
            virtual void setActive(bool p1);
            virtual void method_0xe8(i32 p1);
            virtual void method_0xc8(u32 p1);
            virtual void configureParser(datParser* parser);
            virtual void initScreen() = 0;
            virtual void deinitScreen() = 0;
            virtual void method_0x100();
            virtual const char* getType() const;
            virtual bool isA(const char* type) const;

            void init();
            void deinit();

        public:
            WidgetRef<ui2Master> m_master;
            bool m_isInitialized;

            undefined4 field_0x84;
    };
};