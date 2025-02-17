#pragma once
#include <libsr2/types.h>
#include <libsr2/ui/ui2Widget.h>

namespace sr2 {
    enum class CondComparator {
        LessThan,
        LessThanOrEqualTo,
        GreaterThan,
        GreaterThanOrEqualTo,
        IsEqualTo,
        IsNotEqualTo
    };

    class ui2Condition : public ui2Widget {
        public:
            struct Data {
                ui2String targetName;
                WidgetRef<ui2EventData> compareTo;
                WidgetEventType eventType;
                WidgetRef<ui2EventData> eventData;
                WidgetEventCallback callback;
            };

            ui2Condition(
                const char* name,
                const char* variableName,
                CondComparator defaultComparator,
                const WidgetRef<ui2Master>& master
            );
            virtual ~ui2Condition();

            virtual void onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data);

            void setVariableName(const char* p1);
            void setVariable(const ui::NamedRef& p1);
            void addTarget(
                const WidgetRef<ui2EventData>& compareTo,
                const ui::NamedRef& target,
                WidgetEventType eventType,
                const WidgetRef<ui2EventData>& eventData,
                WidgetEventCallback callback
            );
            void addTarget(
                const WidgetRef<ui2EventData>& compareTo,
                const char* targetName,
                WidgetEventType eventType,
                const WidgetRef<ui2EventData>& eventData,
                WidgetEventCallback callback
            );
            void evaluateEventData(CondComparator comparator, const WidgetRef<ui2EventData>& eventData);
            void evaluateEventData(const WidgetRef<ui2EventData>& p1);
            void evaluateEventData();

        protected:
            utils::Array<Data> m_targets;
            ui2String m_variableName;
            CondComparator m_defaultComparator;
    };
};