#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>

namespace sr2 {
    class ui2Repeater : public ui2Widget {
        public:
            ui2Repeater(char* name, const WidgetRef<ui2Master>& master);
            virtual ~ui2Repeater();

            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual const char* getType() const;
            virtual bool isA(const char* type) const;
        
        protected:
            undefined4 field_0x78;
            undefined4 field_0x7c;
    };
};