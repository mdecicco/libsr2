#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>
#include <libsr2/ui/IntegerEventData.h>
#include <libsr2/ui/ASCIIStringEventData.h>

namespace sr2 {
    enum class VariableType : u32 {
        Unknown = 0,
        Integer = 1,
        String = 2
    };

    class ui2Variable : public ui2Widget {
        public:
            ui2Variable(const char* name, u32 sz, const WidgetRef<ui2Master>& master);
            virtual ~ui2Variable();

            void init();
            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);
            virtual const char* getType() const;

            void setIntegerData(i32 data);
            void setIntegerData(const WidgetRef<IntegerEventData>& data);
            void setStringData(const char* data);
            void setStringData(const WidgetRef<ASCIIStringEventData>& data);
            void maybeClearValue();
            VariableType getVariableType();
            i32 getIntegerData();
            const char* getStringData();
            WidgetRef<ui2EventData> getEvent();

        protected:
            i32 m_integerData;
            ui2String m_stringData;
            VariableType m_type;

            WidgetRef<IntegerEventData> m_intEventData;
            WidgetRef<ASCIIStringEventData> m_strEventData;
    };
};