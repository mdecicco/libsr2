#include <libsr2/ui/ui2Condition.h>
#include <libsr2/ui/ui2Base.h>
#include <libsr2/ui/ui2Variable.h>
#include <utils/Array.hpp>

namespace sr2 {
    ui2Condition::ui2Condition(
        const char* name,
        const char* variableName,
        CondComparator defaultComparator,
        const WidgetRef<ui2Master>& master
    ) : ui2Widget(name, master, true) {
        m_variableName.set(variableName);
        m_defaultComparator = defaultComparator;
    }

    ui2Condition::~ui2Condition() {
    }

    void ui2Condition::onEvent(const ui::NamedRef& source, WidgetEventType event, const WidgetRef<ui2EventData>& data) {
        if (!m_isActive || event != WidgetEventType::EvaluateCondition) {
            ui2Widget::onEvent(source, event, data);
            return;
        }

        evaluateEventData(nullptr);
    }
    
    void ui2Condition::setVariableName(const char* p1) {
        m_variableName.set(p1);
    }
    
    void ui2Condition::setVariable(const ui::NamedRef& p1) {
        m_variableName.set(p1->getName());
    }

    void ui2Condition::addTarget(
        const WidgetRef<ui2EventData>& compareTo,
        const ui::NamedRef& target,
        WidgetEventType eventType,
        const WidgetRef<ui2EventData>& eventData,
        WidgetEventCallback callback
    ) {
        addTarget(compareTo, target->getName(), eventType, eventData, callback);
    }

    void ui2Condition::addTarget(
        const WidgetRef<ui2EventData>& compareTo,
        const char* targetName,
        WidgetEventType eventType,
        const WidgetRef<ui2EventData>& eventData,
        WidgetEventCallback callback
    ) {
        m_targets.emplace(Data());
        Data& obj = m_targets.last();
        obj.compareTo = compareTo;
        obj.eventType = eventType;
        obj.eventData = eventData;
        obj.callback = callback;
        obj.targetName.set(targetName);
    }
    
    void ui2Condition::evaluateEventData(CondComparator comparator, const WidgetRef<ui2EventData>& eventData) {
        auto gm = ui2Base::getGlobalMaster();

        for (u32 i = 0;i < m_targets.size();i++) {
            Data& obj = m_targets[i];
            bool test = false;
            switch (comparator) {
                case CondComparator::LessThan: {
                    test = eventData->isLessThan(*obj.compareTo);
                    break;
                }
                case CondComparator::LessThanOrEqualTo: {
                    test = eventData->isLessThanOrEqualTo(*obj.compareTo);
                    break;
                }
                case CondComparator::GreaterThan: {
                    test = eventData->isGreaterThan(*obj.compareTo);
                    break;
                }
                case CondComparator::GreaterThanOrEqualTo: {
                    test = eventData->isGreaterThanOrEqualTo(*obj.compareTo);
                    break;
                }
                case CondComparator::IsEqualTo: {
                    test = eventData->isEqualTo(*obj.compareTo);
                    break;
                }
                case CondComparator::IsNotEqualTo: {
                    test = eventData->isNotEqualTo(*obj.compareTo);
                    break;
                }
                default: return;
            }

            if (!test) continue;

            ui::NamedRef w = gm->findWidget(obj.targetName.get()).cast<ui2Widget>();
            if (!w) continue;

            ((*w)->*obj.callback)(this, obj.eventType, obj.eventData);
        }
    }
    
    void ui2Condition::evaluateEventData(const WidgetRef<ui2EventData>& p1) {
        if (p1) {
            evaluateEventData(m_defaultComparator, p1);
            return;
        }

        evaluateEventData();
    }

    void ui2Condition::evaluateEventData() {
        WidgetRef<ui2Variable> var = ui2Base::getGlobalMaster()->findWidget(m_variableName.get()).cast<ui2Variable>();

        if (!var) {
            evaluateEventData(m_defaultComparator, nullptr);
            return;
        }

        evaluateEventData(m_defaultComparator, var->getEvent());
    }
};